/*++

Copyright (c) 1989-1992  Microsoft Corporation

Module Name:

    miscc.c

Abstract:

    This module implements machine independent miscellaneous kernel functions.

Author:

    David N. Cutler (davec) 13-May-1989

Environment:

    Kernel mode only.

Revision History:

--*/

#include "ki.h"

#pragma alloc_text(PAGE, KeAddSystemServiceTable)
#pragma alloc_text(PAGE, KeRemoveSystemServiceTable)
#pragma alloc_text(PAGE, KeSetTimeUpdateNotifyRoutine)
#pragma alloc_text(PAGE, KeQueryActiveProcessors)
#pragma alloc_text(PAGELK, KiCalibrateTimeAdjustment)

#undef KeEnterCriticalRegion

VOID
KeEnterCriticalRegion (
   VOID
   )

/*++

Routine Description:

   This function disables kernel APC's.

   N.B. The following code does not require any interlocks. There are
        two cases of interest: 1) On an MP system, the thread cannot
        be running on two processors as once, and 2) if the thread is
        is interrupted to deliver a kernel mode APC which also calls
        this routine, the values read and stored will stack and unstack
        properly.

Arguments:

   None.

Return Value:

   None.

--*/

{
    //
    // Simply directly disable kernel APCs.
    //

    KeGetCurrentThread()->KernelApcDisable -= 1;
    return;
}

#undef KeLeaveCriticalRegion

VOID
KeLeaveCriticalRegion (
    VOID
    )

/*++

Routine Description:

    This function enables kernel APC's and requests an APC interrupt if
    appropriate.

Arguments:

    None.

Return Value:

    None.

--*/

{

    //
    // Increment the kernel APC disable count. If the resultant count is
    // zero and the thread's kernel APC List is not empty, then request an
    // APC interrupt.
    //
    // For multiprocessor performance, the following code utilizes the fact
    // that queuing an APC is done by first queuing the APC, then checking
    // the AST disable count. The following code increments the disable
    // count first, checks to determine if it is zero, and then checks the
    // kernel AST queue.
    //
    // See also KiInsertQueueApc().
    //

    KiLeaveCriticalRegion();
    return;
}

ULONGLONG
KeQueryInterruptTime (
    VOID
    )

/*++

Routine Description:

    This function returns the current interrupt time by determining when the
    time is stable and then returning its value.

Arguments:

    CurrentTime - Supplies a pointer to a variable that will receive the
        current system time.

Return Value:

    None.

--*/

{

    LARGE_INTEGER CurrentTime;

    KiQueryInterruptTime(&CurrentTime);
    return CurrentTime.QuadPart;
}

VOID
KeQuerySystemTime (
    OUT PLARGE_INTEGER CurrentTime
    )

/*++

Routine Description:

    This function returns the current system time by determining when the
    time is stable and then returning its value.

Arguments:

    CurrentTime - Supplies a pointer to a variable that will receive the
        current system time.

Return Value:

    None.

--*/

{

    KiQuerySystemTime(CurrentTime);
    return;
}

VOID
KeQueryTickCount (
    OUT PLARGE_INTEGER CurrentCount
    )

/*++

Routine Description:

    This function returns the current tick count by determining when the
    count is stable and then returning its value.

Arguments:

    CurrentCount - Supplies a pointer to a variable that will receive the
        current tick count.

Return Value:

    None.

--*/

{

    KiQueryTickCount(CurrentCount);
    return;
}

ULONG
KeQueryTimeIncrement (
    VOID
    )

/*++

Routine Description:

    This function returns the time increment value in 100ns units. This
    is the value that is added to the system time at each interval clock
    interrupt.

Arguments:

    None.

Return Value:

    The time increment value is returned as the function value.

--*/

{

    return KeMaximumIncrement;
}

VOID
KeEnableInterrupts (
    IN BOOLEAN Enable
    )

/*++

Routine Description:

    This function enables interrupts based on the specified enable state.

Arguments:

    Enable - Supplies a boolean value that determines whether interrupts
        are to be enabled.

Return Value:

    None.

--*/

{

    if (Enable != FALSE) {
        _enable();
    }

    return;
}

VOID
KeSetDmaIoCoherency (
    IN ULONG Attributes
    )

/*++

Routine Description:

    This function sets (enables/disables) DMA I/O coherency with data
    caches.

Arguments:

    Attributes - Supplies the set of DMA I/O coherency attributes for
        the host system.

Return Value:

    None.

--*/

{

    KiDmaIoCoherency = Attributes;
}

#if defined(_AMD64_) || defined(_X86_)

#pragma alloc_text(INIT, KeSetProfileIrql)

VOID
KeSetProfileIrql (
    IN KIRQL ProfileIrql
    )

/*++

Routine Description:

    This function sets the profile IRQL.

    N.B. There are only two valid values for the profile IRQL which are
        PROFILE_LEVEL and HIGH_LEVEL.

Arguments:

    Irql - Supplies the synchronization IRQL value.

Return Value:

    None.

--*/

{

    ASSERT((ProfileIrql == PROFILE_LEVEL) || (ProfileIrql == HIGH_LEVEL));

    KiProfileIrql = ProfileIrql;
}

#endif

VOID
KeSetSystemTime (
    IN PLARGE_INTEGER NewTime,
    OUT PLARGE_INTEGER OldTime,
    IN BOOLEAN AdjustInterruptTime,
    IN PLARGE_INTEGER HalTimeToSet OPTIONAL
    )

/*++

Routine Description:

    This function sets the system time to the specified value and updates
    timer queue entries to reflect the difference between the old system
    time and the new system time.

Arguments:

    NewTime - Supplies a pointer to a variable that specifies the new system
        time.

    OldTime - Supplies a pointer to a variable that will receive the previous
        system time.

    AdjustInterruptTime - If TRUE the amount of time being adjusted is
        also applied to InterruptTime and TickCount.

    HalTimeToSet - Supplies an optional time that if specified is to be used
        to set the time in the realtime clock.

Return Value:

    None.

--*/

{

    LIST_ENTRY AbsoluteListHead;
    LIST_ENTRY ExpiredListHead;
    ULONG Index;
    PLIST_ENTRY ListHead;
    PLIST_ENTRY NextEntry;
    KIRQL OldIrql1;
    KIRQL OldIrql2;
    LARGE_INTEGER TimeDelta;
    TIME_FIELDS TimeFields;
    PKTIMER Timer;

    ASSERT((NewTime->HighPart & 0xf0000000) == 0);

    ASSERT(KeGetCurrentIrql() < DISPATCH_LEVEL);

    //
    // If a realtime clock value is specified, then convert the time value
    // to time fields.
    //

    if (ARGUMENT_PRESENT(HalTimeToSet)) {
        RtlTimeToTimeFields(HalTimeToSet, &TimeFields);
    }

    //
    // Set affinity to the processor that keeps the system time, raise IRQL
    // to dispatcher level and lock the dispatcher database, then raise IRQL
    // to HIGH_LEVEL to synchronize with the clock interrupt routine.
    //

    KeSetSystemAffinityThread((KAFFINITY)1);
    KiLockDispatcherDatabase(&OldIrql1);
    KeRaiseIrql(HIGH_LEVEL, &OldIrql2);

    //
    // Save the previous system time, set the new system time, and set
    // the realtime clock, if a time value is specified.
    //

    KiQuerySystemTime(OldTime);
    SharedUserData->SystemTime.High2Time = NewTime->HighPart;
    SharedUserData->SystemTime.LowPart   = NewTime->LowPart;
    SharedUserData->SystemTime.High1Time = NewTime->HighPart;

    if (ARGUMENT_PRESENT(HalTimeToSet)) {
        ExCmosClockIsSane = HalSetRealTimeClock(&TimeFields);
    }

    //
    // Compute the difference between the previous system time and the new
    // system time.
    //

    TimeDelta.QuadPart = NewTime->QuadPart - OldTime->QuadPart;

    //
    // Update the boot time to reflect the delta. This keeps time based
    // on boot time constant
    //

    KeBootTime.QuadPart = KeBootTime.QuadPart + TimeDelta.QuadPart;

    //
    // Track the overall bias applied to the boot time.
    //

    KeBootTimeBias = KeBootTimeBias + TimeDelta.QuadPart;

    //
    // Lower IRQL to dispatch level and if needed adjust the physical
    // system interrupt time.
    //

    KeLowerIrql(OldIrql2);
    if (AdjustInterruptTime) {

        //
        // Adjust the physical time of the system
        //

        AdjustInterruptTime = KiAdjustInterruptTime (TimeDelta.QuadPart);
    }

    //
    // If the physical interrupt time of the system was not adjusted,
    // recompute any absolute timers in the system for the new
    // system time.
    //

    if (!AdjustInterruptTime) {

        //
        // Remove all absolute timers from the timer queue so their due time
        // can be recomputed.
        //

        InitializeListHead(&AbsoluteListHead);
        for (Index = 0; Index < TIMER_TABLE_SIZE; Index += 1) {
            ListHead = &KiTimerTableListHead[Index];
            NextEntry = ListHead->Flink;
            while (NextEntry != ListHead) {
                Timer = CONTAINING_RECORD(NextEntry, KTIMER, TimerListEntry);
                NextEntry = NextEntry->Flink;
                if (Timer->Header.Absolute != FALSE) {
                    RemoveEntryList(&Timer->TimerListEntry);
                    InsertTailList(&AbsoluteListHead, &Timer->TimerListEntry);
                }
            }
        }

        //
        // Recompute the due time and reinsert all absolute timers in the timer
        // tree. If a timer has already expired, then insert the timer in the
        // expired timer list.
        //

        InitializeListHead(&ExpiredListHead);
        while (AbsoluteListHead.Flink != &AbsoluteListHead) {
            Timer = CONTAINING_RECORD(AbsoluteListHead.Flink, KTIMER, TimerListEntry);
            KiRemoveTreeTimer(Timer);
            Timer->DueTime.QuadPart -= TimeDelta.QuadPart;
            if (KiReinsertTreeTimer(Timer, Timer->DueTime) == FALSE) {
                Timer->Header.Inserted = TRUE;
                InsertTailList(&ExpiredListHead, &Timer->TimerListEntry);
            }
        }

        //
        // If any of the attempts to reinsert a timer failed, then timers have
        // already expired and must be processed.
        //
        // N.B. The following function returns with the dispatcher database
        //      unlocked.
        //

        KiTimerListExpire(&ExpiredListHead, OldIrql1);

    } else {

        KiUnlockDispatcherDatabase(OldIrql1);

    }


    //
    // Set affinity back to its original value.
    //

    KeRevertToUserAffinityThread();

    return;
}

BOOLEAN
KiAdjustInterruptTime (
    IN LONGLONG TimeDelta
    )

/*++

Routine Description:

    This function moves the physical interrupt time of the system foreward by
    the specified time delta after a system wake has occurred.

Arguments:

    TimeDelta - Supplies the time delta to be added to the interrupt time, tick
        count and the perforamnce counter in 100ns units.

Return Value:

    None.

--*/

{

    ADJUST_INTERRUPT_TIME_CONTEXT Adjust;

    //
    // Time can only be moved forward.
    //

    if (TimeDelta < 0) {
        return FALSE;

    } else {
        Adjust.KiNumber = KeNumberProcessors;
        Adjust.HalNumber = KeNumberProcessors;
        Adjust.Adjustment = (ULONGLONG) TimeDelta;
        Adjust.Barrier = 1;
        KiIpiGenericCall((PKIPI_BROADCAST_WORKER)KiCalibrateTimeAdjustment,
                         (ULONG_PTR)(&Adjust));

        return TRUE;
    }
}

VOID
KiCalibrateTimeAdjustment (
    PADJUST_INTERRUPT_TIME_CONTEXT  Adjust
    )

/*++

Routine Description:

    Worker function to calibrate the adjustment of time on all processors.

Arguments:

    Adjust - Supplies the operation context.

Return Value:

    None.

--*/

{

    BOOLEAN             Enable;
    LARGE_INTEGER       InterruptTime;
    LARGE_INTEGER       SetTime;
    LARGE_INTEGER       PerfFreq;
    ULARGE_INTEGER      li;
    LARGE_INTEGER       NewTickCount;
    ULONG               NewTickOffset;
    ULONG               cl, divisor;
    LARGE_INTEGER       PerfCount;

    //
    // As each processor arrives, subtract one off the remaining processor
    // count.  If this is the last processor to arrive compute the time
    // change, and signal all processor when to apply the performance
    // counter change.
    //

    if (InterlockedDecrement((PLONG)&Adjust->KiNumber)) {
        Enable = KeDisableInterrupts();

        //
        // It is possible to deadlock here if one or more of the
        // other processors gets and processes a freeze request
        // while this processor has interrupts disabled.  Poll
        // for IPI_FREEZE requests until all processors are known
        // to be in this code and hence wont be requesting a
        // freeze.
        //

        do {
            KiPollFreezeExecution();
        } while (Adjust->KiNumber != (ULONG)-1);

        //
        // Wait to perform the time set
        //

        while (Adjust->Barrier) ;

    } else {

        //
        // Set timer expiration dpc to scan the timer queues once for any
        // expired timers.
        //

        KeRemoveQueueDpc (&KiTimerExpireDpc);
        KeInsertQueueDpc (&KiTimerExpireDpc, (PVOID) TIMER_TABLE_SIZE, NULL);

        //
        // Disable interrupts and indicate that this processor is now
        // in final portion of this code.
        //

        Enable = KeDisableInterrupts();
        InterlockedDecrement((PLONG) &Adjust->KiNumber);

        //
        // Adjust Interrupt Time.
        //

        InterruptTime.QuadPart = KeQueryInterruptTime() + Adjust->Adjustment;
        SetTime.QuadPart = Adjust->Adjustment;
        
        //
        // Get the current times
        //

        PerfCount = KeQueryPerformanceCounter (&PerfFreq);

        //
        // Compute performance counter for current SetTime
        //

        //
        // Multiply SetTime * PerfCount and obtain 96bit result
        // in cl, li.LowPart, li.HighPart.  Then divide the 96bit
        // result by 10,000,000 to get new performance counter value.
        //

        li.QuadPart = RtlEnlargedUnsignedMultiply (
                            (ULONG) SetTime.LowPart,
                            (ULONG) PerfFreq.LowPart
                            ).QuadPart;

        cl = li.LowPart;
        li.QuadPart = li.HighPart +
                      RtlEnlargedUnsignedMultiply (
                            (ULONG) SetTime.LowPart,
                            (ULONG) PerfFreq.HighPart
                            ).QuadPart;

        li.QuadPart = li.QuadPart +
                      RtlEnlargedUnsignedMultiply (
                            (ULONG) SetTime.HighPart,
                            (ULONG) PerfFreq.LowPart
                            ).QuadPart;

        li.HighPart = li.HighPart + SetTime.HighPart * PerfFreq.HighPart;

        divisor = 10000000;
        Adjust->NewCount.HighPart =
            RtlEnlargedUnsignedDivide (
                li,
                divisor,
                &li.HighPart
                );

        li.LowPart = cl;
        Adjust->NewCount.LowPart =
            RtlEnlargedUnsignedDivide (
                li,
                divisor,
                NULL
                );

        Adjust->NewCount.QuadPart += PerfCount.QuadPart;

        //
        // Compute tick count and tick offset for current InterruptTime
        //

        NewTickCount = RtlExtendedLargeIntegerDivide(
                            InterruptTime,
                            KeMaximumIncrement,
                            &NewTickOffset
                            );

        //
        // Apply changes to InterruptTime, TickCount, TickOffset, and the
        // performance counter
        //

        KiTickOffset = KeMaximumIncrement - NewTickOffset;
        KeInterruptTimeBias += Adjust->Adjustment;
        SharedUserData->TickCountLow = NewTickCount.LowPart;

#if defined(_AMD64_) || defined(_IA64_)

        KeTickCount = NewTickCount;

#else

        KeTickCount.High2Time = NewTickCount.HighPart;
        KeTickCount.LowPart   = NewTickCount.LowPart;
        KeTickCount.High1Time = NewTickCount.HighPart;

#endif

        SharedUserData->InterruptTime.High2Time = InterruptTime.HighPart;
        SharedUserData->InterruptTime.LowPart   = InterruptTime.LowPart;
        SharedUserData->InterruptTime.High1Time = InterruptTime.HighPart;

        //
        // Apply the performance counter change
        //

        Adjust->Barrier = 0;
    }

    HalCalibratePerformanceCounter (
        (LONG volatile *) &Adjust->HalNumber,
        (ULONGLONG) Adjust->NewCount.QuadPart
        );

    KeEnableInterrupts(Enable);
}

VOID
KeSetTimeIncrement (
    IN ULONG MaximumIncrement,
    IN ULONG MinimumIncrement
    )

/*++

Routine Description:

    This function sets the time increment value in 100ns units. This
    value is added to the system time at each interval clock interrupt.

Arguments:

    MaximumIncrement - Supplies the maximum time between clock interrupts
        in 100ns units supported by the host HAL.

    MinimumIncrement - Supplies the minimum time between clock interrupts
        in 100ns units supported by the host HAL.

Return Value:

    None.

--*/

{

    KeMaximumIncrement = MaximumIncrement;
    KeMinimumIncrement = max(MinimumIncrement, 10 * 1000);
    KeTimeAdjustment = MaximumIncrement;
    KeTimeIncrement = MaximumIncrement;
    KiTickOffset = MaximumIncrement;
}

BOOLEAN
KeAddSystemServiceTable(
    IN PULONG_PTR Base,
    IN PULONG Count OPTIONAL,
    IN ULONG Limit,
    IN PUCHAR Number,
    IN ULONG Index
    )

/*++

Routine Description:

    This function allows the caller to add a system service table
    to the system

Arguments:

    Base - Supplies the address of the system service table dispatch
        table.

    Count - Supplies an optional pointer to a table of per system service
        counters.

    Limit - Supplies the limit of the service table. Services greater
        than or equal to this limit will fail.

    Arguments - Supplies the address of the argument count table.

    Index - Supplies index of the service table.

Return Value:

    TRUE - The operation was successful.

    FALSE - the operation failed. A service table is already bound to
        the specified location, or the specified index is larger than
        the maximum allowed index.

--*/

{

    PAGED_CODE();

    //
    // If a system service table is already defined for the specified
    // index, then return FALSE. Otherwise, establish the new system
    // service table.
    //

    if ((Index > NUMBER_SERVICE_TABLES - 1) ||
        (KeServiceDescriptorTable[Index].Base != NULL) ||
        (KeServiceDescriptorTableShadow[Index].Base != NULL)) {
        return FALSE;

    } else {

        //
        // If the service table index is equal to the Win32 table, then
        // only update the shadow system service table. Otherwise, both
        // the shadow and static system service tables are updated.
        //

        KeServiceDescriptorTableShadow[Index].Base = Base;
        KeServiceDescriptorTableShadow[Index].Count = Count;
        KeServiceDescriptorTableShadow[Index].Limit = Limit;

        //
        // The global pointer associated with the table base is
        // placed just before the service table.
        //

#if defined(_IA64_)

        KeServiceDescriptorTableShadow[Index].TableBaseGpOffset =
                                        (LONG)(*(Base-1) - (ULONG_PTR)Base);

#endif

        KeServiceDescriptorTableShadow[Index].Number = Number;
        if (Index != 1) {
            KeServiceDescriptorTable[Index].Base = Base;
            KeServiceDescriptorTable[Index].Count = Count;
            KeServiceDescriptorTable[Index].Limit = Limit;

#if defined(_IA64_)

            KeServiceDescriptorTable[Index].TableBaseGpOffset =
                                        (LONG)(*(Base-1) - (ULONG_PTR)Base);

#endif

            KeServiceDescriptorTable[Index].Number = Number;
        }

        return TRUE;
    }
}

BOOLEAN
KeRemoveSystemServiceTable(
    IN ULONG Index
    )

/*++

Routine Description:

    This function allows the caller to remove a system service table
    from the system. This can only be called at system shutdown.

Arguments:

    Index - Supplies index of the service table.

Return Value:

    TRUE - The operation was successful.

    FALSE - the operation failed. A service table is is not bound or is illegal to remove

--*/

{

    PAGED_CODE();

    if ((Index > NUMBER_SERVICE_TABLES - 1) ||
        ((KeServiceDescriptorTable[Index].Base == NULL) &&
         (KeServiceDescriptorTableShadow[Index].Base == NULL))) {

        return FALSE;

    } else {
        KeServiceDescriptorTableShadow[Index].Base = NULL;
        KeServiceDescriptorTableShadow[Index].Count = 0;
        KeServiceDescriptorTableShadow[Index].Limit = 0;

#if defined(_IA64_)

        KeServiceDescriptorTableShadow[Index].TableBaseGpOffset = 0;

#endif

        KeServiceDescriptorTableShadow[Index].Number = 0;
        if (Index != 1) {
            KeServiceDescriptorTable[Index].Base = NULL;
            KeServiceDescriptorTable[Index].Count = 0;
            KeServiceDescriptorTable[Index].Limit = 0;

#if defined(_IA64_)

            KeServiceDescriptorTable[Index].TableBaseGpOffset = 0;

#endif

            KeServiceDescriptorTable[Index].Number = 0;
        }

        return TRUE;
    }
}

VOID
FASTCALL
KeSetTimeUpdateNotifyRoutine(
    IN PTIME_UPDATE_NOTIFY_ROUTINE NotifyRoutine
    )

/*++

Routine Description:

    This function sets the address of a callout routine which will be called
    each time the runtime for a thread is updated.

Arguments:

    RoutineNotify - Supplies the address of the time update notify callout
        routine.

Return Value:

    None.

--*/

{

    PAGED_CODE();

    KiTimeUpdateNotifyRoutine = NotifyRoutine;
    return;
}

KAFFINITY
KeQueryActiveProcessors(
    VOID
    )

/*++

Routine Description:

    This function returns the current set of active processors
    in the system.

Arguments:

    None.

Return Value:

    KAFFINITY bitmask representing the set of active processors

--*/

{
    PAGED_CODE();

    return(KeActiveProcessors);
}

#undef KeIsAttachedProcess

BOOLEAN
KeIsAttachedProcess(
    VOID
    )

/*++

Routine Description:

    This function determines if the current thread is attached to a process.

Arguments:

    None.

Return Value:

    TRUE is returned if the current thread is attached to a process. Otherwise,
    FALSE is returned.

--*/

{
    return KiIsAttachedProcess();
}

#undef KeAreApcsDisabled

BOOLEAN
KeAreApcsDisabled(
    VOID
    )

/*++

Routine Description:

    This function determines if APCs are disabled for the current thread.

Arguments:

    None.

Return Value:

    TRUE is returned if APCs are disabled for the current thread. Otherwise,
    FALE is returned.

--*/

{
    return KeGetCurrentThread()->KernelApcDisable != 0;
}

ULONG
KeGetRecommendedSharedDataAlignment (
    VOID
    )

/*++

Routine Description:

    This function returns the size of the largest cache line in the system.
    This value should be used as a recommended alignment / granularity for
    shared data.

Arguments:

    None.

Return Value:

    The size of the largest cache line in the system is returned as the
    function value.

--*/

{
    return KeLargestCacheLine;
}

PKPRCB
KeGetPrcb(
    ULONG ProcessorNumber
    )

/*++

Routine Description:

    This function returns the address of the Processor Control Block (PRCB)
    for the specified processor.

Arguments:

    ProcessorNumber - Supplies the number of the processor the PRCB 
    is to be returned for.

Return Value:

    Returns the address of the requested PRCB or NULL if ProcessorNumber
    is not valid.

--*/

{

    ASSERT(ProcessorNumber < MAXIMUM_PROCESSORS);

    if (ProcessorNumber < (ULONG)KeNumberProcessors) {
        return KiProcessorBlock[ProcessorNumber];
    }

    return NULL;
}

NTSTATUS
KeCopySafe(
    VOID UNALIGNED *Destination,
    CONST VOID UNALIGNED *Source,
    SIZE_T Length
    )

/*++

Routine Description:
    This function attempts to safely copy a block of memory.  If an excpetion occurs the
    exception status is returned.  
    
Arguments:

    Destination - Supplies a pointer to the destination memory.
    
    Source - Supplies a pointer to the source memory.
    
    Length - Supplies the size of memory in bytes to be copied.

Return Value:

    Return the status of the copy.

--*/

{
    NTSTATUS Status = STATUS_SUCCESS;

    try {
        RtlCopyMemory(Destination, Source, Length);
    } __except(EXCEPTION_EXECUTE_HANDLER) {

          Status = _exception_code();
    }

    return Status;
}
