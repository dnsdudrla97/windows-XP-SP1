/*++

Copyright (c) 1989  Microsoft Corporation

Module Name:

    kernlini.c

Abstract:

    This module contains the code to initialize the kernel data structures
    and to initialize the idle thread, its process, and the processor control
    block.

    For the i386, it also contains code to initialize the PCR.

Author:

    David N. Cutler (davec) 21-Apr-1989

Environment:

    Kernel mode only.

Revision History:

    24-Jan-1990  shielin

                 Changed for NT386

    20-Mar-1990     bryanwi

                Added KiInitializePcr

--*/

#include "ki.h"
#include "fastsys.inc"

#define TRAP332_GATE 0xEF00

VOID
KiSetProcessorType(
    VOID
    );

VOID
KiSetCR0Bits(
    VOID
    );

BOOLEAN
KiIsNpxPresent(
    VOID
    );

VOID
KiI386PentiumLockErrataFixup (
    VOID
    );

VOID
KiInitializeDblFaultTSS(
    IN PKTSS Tss,
    IN ULONG Stack,
    IN PKGDTENTRY TssDescriptor
    );

VOID
KiInitializeTSS2 (
    IN PKTSS Tss,
    IN PKGDTENTRY TssDescriptor
    );

VOID
KiSwapIDT (
    VOID
    );

VOID
KeSetup80387OrEmulate (
    IN PVOID *R3EmulatorTable
    );

VOID
KiGetCacheInformation(
    VOID
    );

ULONG
KiGetCpuVendor(
    VOID
    );

ULONG
KiGetFeatureBits (
    VOID
    );

NTSTATUS
KiMoveRegTree(
    HANDLE  Source,
    HANDLE  Dest
    );

VOID
Ki386EnableDE (
    IN volatile PLONG Number
    );

VOID
Ki386EnableFxsr (
    IN volatile PLONG Number
    );


VOID
Ki386EnableXMMIExceptions (
    IN volatile PLONG Number
    );


VOID
Ki386EnableGlobalPage (
    IN volatile PLONG Number
    );

VOID
Ki386UseSynchronousTbFlush (
    IN volatile PLONG Number
    );

BOOLEAN
KiInitMachineDependent (
    VOID
    );

VOID
KiInitializeMTRR (
    IN BOOLEAN LastProcessor
    );

VOID
KiInitializePAT (
    VOID
    );

VOID
KiAmdK6InitializeMTRR(
    VOID
    );

VOID
KiRestoreFastSyscallReturnState(
    VOID
    );

VOID
KiLogMcaErrors (
    VOID
    );

#ifdef ALLOC_PRAGMA
#pragma alloc_text(INIT,KiInitializeKernel)
#pragma alloc_text(INIT,KiInitializePcr)
#pragma alloc_text(INIT,KiInitializeDblFaultTSS)
#pragma alloc_text(INIT,KiInitializeTSS2)
#pragma alloc_text(INIT,KiSwapIDT)
#pragma alloc_text(INIT,KeSetup80387OrEmulate)
#pragma alloc_text(INIT,KiGetFeatureBits)
#pragma alloc_text(INIT,KiGetCacheInformation)
#pragma alloc_text(INIT,KiGetCpuVendor)
#pragma alloc_text(INIT,KiMoveRegTree)
#pragma alloc_text(INIT,KiInitMachineDependent)
#pragma alloc_text(INIT,KiI386PentiumLockErrataFixup)
#pragma alloc_text(INIT,KiLogMcaErrors)
#endif

BOOLEAN KiI386PentiumLockErrataPresent = FALSE;
BOOLEAN KiIgnoreUnexpectedTrap07 = FALSE;

ULONG KiFastSystemCallDisable;

extern PVOID Ki387RoundModeTable;
extern PVOID Ki386IopmSaveArea;
extern ULONG KeI386ForceNpxEmulation;
extern WCHAR CmDisabledFloatingPointProcessor[];
extern UCHAR CmpCyrixID[];
extern UCHAR CmpIntelID[];
extern UCHAR CmpAmdID[];
extern UCHAR CmpTransmetaID[];
extern UCHAR CmpCentaurID[];
extern BOOLEAN KiFastSystemCallIsIA32;

#ifndef NT_UP
extern PVOID ScPatchFxb;
extern PVOID ScPatchFxe;
#endif

typedef enum {
    CPU_NONE,
    CPU_INTEL,
    CPU_AMD,
    CPU_CYRIX,
    CPU_TRANSMETA,
    CPU_CENTAUR,
    CPU_UNKNOWN
} CPU_VENDORS;


//
// If this processor does XMMI, take advantage of it.  Default is
// no XMMI.
//

BOOLEAN KeI386XMMIPresent;

//
// x86 statically provides the idle process and idle thread for
// processor 0.
//

EPROCESS    KiIdleProcess;
ETHREAD     KiIdleThread0;

//
// Define prototypes and static initialization for the fast zero
// page routines.
//

VOID
FASTCALL
KiZeroPage (
    PVOID PageBase
    );

VOID
FASTCALL
KiXMMIZeroPage (
    PVOID PageBase
    );

VOID
FASTCALL
KiXMMIZeroPageNoSave (
    PVOID PageBase
    );

KE_ZERO_PAGE_ROUTINE KeZeroPage = KiZeroPage;
KE_ZERO_PAGE_ROUTINE KeZeroPageFromIdleThread = KiZeroPage;

//
// Line size of the d-cache closest to the processor.   Used by machine
// dependent prefetch routines.  Default to 32.
//

ULONG KePrefetchNTAGranularity = 32;

VOID
FASTCALL
RtlPrefetchMemoryNonTemporal(
    PVOID   Memory,
    SIZE_T  Length
    );


//
// The following spinlock is for compatiblity with 486 systems that don't
// have a cmpxchg8b instruction and therefore need to synchronize using a
// spinlock.  NOTE: This spinlock should be initialized on x86 systems.
//

ULONG Ki486CompatibilityLock;

//
// Profile vars
//

extern  KIDTENTRY IDT[];

VOID
KiInitializeKernel (
    IN PKPROCESS Process,
    IN PKTHREAD Thread,
    IN PVOID IdleStack,
    IN PKPRCB Prcb,
    IN CCHAR Number,
    PLOADER_PARAMETER_BLOCK LoaderBlock
    )

/*++

Routine Description:

    This function gains control after the system has been bootstrapped and
    before the system has been initialized. Its function is to initialize
    the kernel data structures, initialize the idle thread and process objects,
    initialize the processor control block, call the executive initialization
    routine, and then return to the system startup routine. This routine is
    also called to initialize the processor specific structures when a new
    processor is brought on line.

Arguments:

    Process - Supplies a pointer to a control object of type process for
        the specified processor.

    Thread - Supplies a pointer to a dispatcher object of type thread for
        the specified processor.

    IdleStack - Supplies a pointer the base of the real kernel stack for
        idle thread on the specified processor.

    Prcb - Supplies a pointer to a processor control block for the specified
        processor.

    Number - Supplies the number of the processor that is being
        initialized.

    LoaderBlock - Supplies a pointer to the loader parameter block.

Return Value:

    None.

--*/

{
    LONG  Index;
    ULONG DirectoryTableBase[2];
    KIRQL OldIrql;
    PKPCR Pcr;
    BOOLEAN NpxFlag;
    BOOLEAN FxsrPresent;
    BOOLEAN XMMIPresent;
    ULONG FeatureBits;

    KiSetProcessorType();
    KiSetCR0Bits();
    NpxFlag = KiIsNpxPresent();

    Pcr = KeGetPcr();

    //
    // Initialize DPC listhead and lock.
    //

    InitializeListHead(&Prcb->DpcListHead);
    KeInitializeSpinLock(&Prcb->DpcLock);
    Prcb->DpcRoutineActive = 0;
    Prcb->DpcQueueDepth = 0;
    Prcb->MaximumDpcQueueDepth = KiMaximumDpcQueueDepth;
    Prcb->MinimumDpcRate = KiMinimumDpcRate;
    Prcb->AdjustDpcThreshold = KiAdjustDpcThreshold;

    //
    // Initialize processor's PowerState
    //

    PoInitializePrcb (Prcb);

    //
    // Check for unsupported processor revision
    //

    if (Prcb->CpuType == 3) {
        KeBugCheckEx(UNSUPPORTED_PROCESSOR,0x386,0,0,0);
    }

    //
    // Get the processor FeatureBits for this processor.
    //

    FeatureBits = KiGetFeatureBits();
    Prcb->FeatureBits = FeatureBits;

    //
    // Do one time initialization of the ProcesorControlSpace in the PRCB
    // so local kernel debugger can get things like the GDT
    //

    KiSaveProcessorControlState(&Prcb->ProcessorState);

    //
    // Get processor Cache Size information.
    //

    KiGetCacheInformation();

    //
    // initialize the per processor lock queue entry for implemented locks.
    //

    KiInitQueuedSpinLocks(Prcb, Number);

    //
    // If the initial processor is being initialized, then initialize the
    // per system data structures.
    //

    if (Number == 0) {

        //
        // Set default node.  Used in non-multinode systems and in
        // multinode systems until the node topology is available.
        //

        KeNodeBlock[0] = &KiNode0;

#if defined(KE_MULTINODE)

        for (Index = 1; Index < MAXIMUM_CCNUMA_NODES; Index++) {

            //
            // Set temporary node.
            //

            KeNodeBlock[Index] = &KiNodeInit[Index];
        }

#endif

        Prcb->ParentNode = KeNodeBlock[0];
        KeNodeBlock[0]->ProcessorMask = Prcb->SetMember;

        //
        // Initial setting for global Cpu & Stepping levels
        //

        KeI386NpxPresent = NpxFlag;
        KeI386CpuType = Prcb->CpuType;
        KeI386CpuStep = Prcb->CpuStep;

        KeProcessorArchitecture = PROCESSOR_ARCHITECTURE_INTEL;
        KeProcessorLevel = (USHORT)Prcb->CpuType;
        if (Prcb->CpuID == 0) {
            KeProcessorRevision = 0xFF00 |
                                  (((Prcb->CpuStep >> 4) + 0xa0 ) & 0x0F0) |
                                  (Prcb->CpuStep & 0xf);
        } else {
            KeProcessorRevision = Prcb->CpuStep;
        }

        KeFeatureBits = FeatureBits;

        KeI386FxsrPresent = ((KeFeatureBits & KF_FXSR) ? TRUE:FALSE);

        KeI386XMMIPresent = ((KeFeatureBits & KF_XMMI) ? TRUE:FALSE);

        //
        // As of Whistler, cmpxchg8b is a required instruction.
        //

        if ((KeFeatureBits & KF_CMPXCHG8B) == 0) {

            ULONG Vendor[3];

            //
            // Argument 1:
            //   bits 31-24: Unique value for missing feature.
            //   bits 23-0 : Family/Model/Stepping (this could compress).
            // Arguments 2 thru 4:
            //   Vendor Id string.
            //

            RtlCopyMemory(Vendor, Prcb->VendorString, sizeof(Vendor));
            KeBugCheckEx(UNSUPPORTED_PROCESSOR,
                         (1 << 24 )     // rev this for other required features
                          | (Prcb->CpuType << 16) | Prcb->CpuStep,
                         Vendor[0],
                         Vendor[1],
                         Vendor[2]
                         );
        }

        //
        // Lower IRQL to APC level.
        //

        KeLowerIrql(APC_LEVEL);


        //
        // Initialize kernel internal spinlocks
        //

        KeInitializeSpinLock(&KiFreezeExecutionLock);

        //
        // Initialize 486 compatibility lock
        //

        KeInitializeSpinLock(&Ki486CompatibilityLock);

#if !defined(NT_UP)

        //
        // Set this processor as the master (ie first found) processor
        // in this SMT set (whether or not it is actually SMT).
        //

        Prcb->MultiThreadSetMaster = Prcb;

        //
        // During Text Mode setup, it is possible the system is
        // running with an MP kernel and a UP HAL.  On X86 systems,
        // spinlocks are implemented in both the kernel and the HAL
        // with the verisons that alter IRQL in the HAL.   If the
        // HAL is UP, it will not actually acquire/release locks
        // while the MP kernel will which will cause the system to
        // hang (or crash).   As this can only occur during text
        // mode setup, we will detect the situation and disable
        // the kernel only versions of queued spinlocks if the HAL
        // is UP (and the kernel MP).
        //
        // We need to patch 3 routines, two of them are void and
        // the other returns a boolean (must be true (and ZF must be
        // clear) in a UP case).
        //
        // Determine if the HAL us UP by acquiring the dispatcher
        // lock and examining it to see if the HAL actually did
        // anything to it.
        //

        OldIrql = KfAcquireSpinLock(&Ki486CompatibilityLock);
        if (Ki486CompatibilityLock == 0) {

            //
            // KfAcquireSpinLock is in the HAL and it did not
            // change the value of the lock.  This is a UP HAL.
            //

            extern UCHAR KeTryToAcquireQueuedSpinLockAtRaisedIrqlUP;
            PUCHAR PatchTarget, PatchSource;
            UCHAR Byte;

            #define RET 0xc3

            *(PUCHAR)(KeAcquireQueuedSpinLockAtDpcLevel) = RET;
            *(PUCHAR)(KeReleaseQueuedSpinLockFromDpcLevel) = RET;

            //
            // Copy the UP version of KeTryToAcquireQueuedSpinLockAtRaisedIrql
            // over the top of the MP versin.
            //

            PatchSource = &(KeTryToAcquireQueuedSpinLockAtRaisedIrqlUP);
            PatchTarget = (PUCHAR)(KeTryToAcquireQueuedSpinLockAtRaisedIrql);

            do {
                Byte = *PatchSource++;
                *PatchTarget++ = Byte;
            } while (Byte != RET);

            #undef RET
        }
        KeReleaseSpinLock(&Ki486CompatibilityLock, OldIrql);

#endif

        //
        // Performance architecture independent initialization.
        //

        KiInitSystem();

        //
        // Initialize idle thread process object and then set:
        //
        //      1. all the quantum values to the maximum possible.
        //      2. the process in the balance set.
        //      3. the active processor mask to the specified process.
        //

        DirectoryTableBase[0] = 0;
        DirectoryTableBase[1] = 0;
        KeInitializeProcess(Process,
                            (KPRIORITY)0,
                            (KAFFINITY)(0xffffffff),
                            &DirectoryTableBase[0],
                            FALSE);

        Process->ThreadQuantum = MAXCHAR;

#if !defined(NT_UP)

    } else {

        //
        // Adjust global cpu setting to represent lowest of all processors
        //

        FxsrPresent = ((FeatureBits & KF_FXSR) ? TRUE:FALSE);
        if (FxsrPresent != KeI386FxsrPresent) {
            //
            // FXSR support must be available on all processors or on none
            //
            KeBugCheckEx (MULTIPROCESSOR_CONFIGURATION_NOT_SUPPORTED, KF_FXSR, 0, 0, 0);
        }

        XMMIPresent = ((FeatureBits & KF_XMMI) ? TRUE:FALSE);
        if (XMMIPresent != KeI386XMMIPresent) {
            //
            // XMMI support must be available on all processors or on none
            //
            KeBugCheckEx (MULTIPROCESSOR_CONFIGURATION_NOT_SUPPORTED, KF_XMMI, 0, 0, 0);
        }

        if (NpxFlag != KeI386NpxPresent) {
            //
            // NPX support must be available on all processors or on none
            //

            KeBugCheckEx (MULTIPROCESSOR_CONFIGURATION_NOT_SUPPORTED, 0x387, 0, 0, 0);
        }

        if ((ULONG)(Prcb->CpuType) != KeI386CpuType) {

            if ((ULONG)(Prcb->CpuType) < KeI386CpuType) {

                //
                // What is the lowest CPU type
                //

                KeI386CpuType = (ULONG)Prcb->CpuType;
                KeProcessorLevel = (USHORT)Prcb->CpuType;
            }
        }

        if ((KiBootFeatureBits & KF_CMPXCHG8B)  &&  !(FeatureBits & KF_CMPXCHG8B)) {
            //
            // cmpxchg8b must be available on all processors, if installed at boot
            //

            KeBugCheckEx (MULTIPROCESSOR_CONFIGURATION_NOT_SUPPORTED, KF_CMPXCHG8B, 0, 0, 0);
        }

        if ((KeFeatureBits & KF_GLOBAL_PAGE)  &&  !(FeatureBits & KF_GLOBAL_PAGE)) {
            //
            // Global page support must be available on all processors, if on boot processor
            //

            KeBugCheckEx (MULTIPROCESSOR_CONFIGURATION_NOT_SUPPORTED, KF_GLOBAL_PAGE, 0, 0, 0);
        }

        if ((KeFeatureBits & KF_PAT)  &&  !(FeatureBits & KF_PAT)) {
            //
            // PAT must be available on all processors, if on boot processor
            //

            KeBugCheckEx (MULTIPROCESSOR_CONFIGURATION_NOT_SUPPORTED, KF_PAT, 0, 0, 0);
        }

        if ((KeFeatureBits & KF_MTRR)  &&  !(FeatureBits & KF_MTRR)) {
            //
            // MTRR must be available on all processors, if on boot processor
            //

            KeBugCheckEx (MULTIPROCESSOR_CONFIGURATION_NOT_SUPPORTED, KF_MTRR, 0, 0, 0);
        }

        if ((KeFeatureBits & KF_FAST_SYSCALL) != (FeatureBits & KF_FAST_SYSCALL)) {
            //
            // If this feature is not available on all processors
            // don't use it at all.
            //

            KiFastSystemCallDisable = 1;
        }

        if ((KeFeatureBits & KF_XMMI64) != (FeatureBits & KF_XMMI64)) {

            //
            // If not all processors support Streaming SIMD Extensions
            // 64bit FP don't use it at all.
            //

            KeFeatureBits &= ~KF_XMMI64;
        }

        //
        // Use lowest stepping value
        //

        if (Prcb->CpuStep < KeI386CpuStep) {
            KeI386CpuStep = Prcb->CpuStep;
            if (Prcb->CpuID == 0) {
                KeProcessorRevision = 0xFF00 |
                                      ((Prcb->CpuStep >> 8) + 'A') |
                                      (Prcb->CpuStep & 0xf);
            } else {
                KeProcessorRevision = Prcb->CpuStep;
            }
        }

        //
        // Use subset of all NT feature bits available on each processor
        //

        KeFeatureBits &= FeatureBits;

        //
        // Lower IRQL to DISPATCH level.
        //

        KeLowerIrql(DISPATCH_LEVEL);

#endif

    }

    //
    // Update processor features
    //

    SharedUserData->ProcessorFeatures[PF_MMX_INSTRUCTIONS_AVAILABLE] =
        (KeFeatureBits & KF_MMX) ? TRUE : FALSE;

    SharedUserData->ProcessorFeatures[PF_COMPARE_EXCHANGE_DOUBLE] =
        (KeFeatureBits & KF_CMPXCHG8B) ? TRUE : FALSE;

    SharedUserData->ProcessorFeatures[PF_XMMI_INSTRUCTIONS_AVAILABLE] =
        ((KeFeatureBits & KF_FXSR) && (KeFeatureBits & KF_XMMI)) ? TRUE : FALSE;

    SharedUserData->ProcessorFeatures[PF_XMMI64_INSTRUCTIONS_AVAILABLE] =
        ((KeFeatureBits & KF_FXSR) && (KeFeatureBits & KF_XMMI64)) ? TRUE : FALSE;

    SharedUserData->ProcessorFeatures[PF_3DNOW_INSTRUCTIONS_AVAILABLE] =
        (KeFeatureBits & KF_3DNOW) ? TRUE : FALSE;

    SharedUserData->ProcessorFeatures[PF_RDTSC_INSTRUCTION_AVAILABLE] =
        (KeFeatureBits & KF_RDTSC) ? TRUE : FALSE;

    //
    // Initialize idle thread object and then set:
    //
    //      1. the initial kernel stack to the specified idle stack.
    //      2. the next processor number to the specified processor.
    //      3. the thread priority to the highest possible value.
    //      4. the state of the thread to running.
    //      5. the thread affinity to the specified processor.
    //      6. the specified processor member in the process active processors
    //          set.
    //

    KeInitializeThread(Thread, (PVOID)((ULONG)IdleStack),
                       (PKSYSTEM_ROUTINE)NULL, (PKSTART_ROUTINE)NULL,
                       (PVOID)NULL, (PCONTEXT)NULL, (PVOID)NULL, Process);
    Thread->NextProcessor = Number;
    Thread->Priority = HIGH_PRIORITY;
    Thread->State = Running;
    Thread->Affinity = (KAFFINITY)(1<<Number);
    Thread->WaitIrql = DISPATCH_LEVEL;
    SetMember(Number, Process->ActiveProcessors);

    //
    // Initialize the processor block. (Note that some fields have been
    // initialized at KiInitializePcr().
    //

    Prcb->CurrentThread = Thread;
    Prcb->NextThread = (PKTHREAD)NULL;
    Prcb->IdleThread = Thread;
    Pcr->NtTib.StackBase = Thread->InitialStack;

    //
    // call the executive initialization routine.
    //

    try {
        ExpInitializeExecutive(Number, LoaderBlock);

    } except(KeBugCheckEx(PHASE0_EXCEPTION,
                          (ULONG)GetExceptionCode(),
                          (ULONG_PTR)GetExceptionInformation(),
                          0,0), EXCEPTION_EXECUTE_HANDLER) {
        ; // should never get here
    }

    //
    // If the initial processor is being initialized, then compute the
    // timer table reciprocal value and reset the PRCB values for the
    // controllable DPC behavior in order to reflect any registry
    // overrides.
    //

    if (Number == 0) {
        KiTimeIncrementReciprocal = KiComputeReciprocal((LONG)KeMaximumIncrement,
                                                        &KiTimeIncrementShiftCount);

        Prcb->MaximumDpcQueueDepth = KiMaximumDpcQueueDepth;
        Prcb->MinimumDpcRate = KiMinimumDpcRate;
        Prcb->AdjustDpcThreshold = KiAdjustDpcThreshold;
    }

    if (Number == 0) {

        //
        // Processor 0's DPC stack was temporarily allocated on
        // the Double Fault Stack, switch to a proper kernel
        // stack now.
        //

        PVOID DpcStack;

        DpcStack = MmCreateKernelStack(FALSE, 0);

        if (DpcStack == NULL) {
            KeBugCheckEx(NO_PAGES_AVAILABLE, 1, 0, 0, 0);
        }
        Prcb->DpcStack = DpcStack;

        //
        // Allocate 8k IOPM bit map saved area to allow BiosCall swap
        // bit maps.
        //

        Ki386IopmSaveArea = ExAllocatePoolWithTag(PagedPool,
                                                  PAGE_SIZE * 2,
                                                  '  eK');
        if (Ki386IopmSaveArea == NULL) {
            KeBugCheckEx(NO_PAGES_AVAILABLE, 2, PAGE_SIZE * 2, 0, 0);
        }
    }

    //
    // Set the priority of the specified idle thread to zero, set appropriate
    // member in KiIdleSummary and return to the system start up routine.
    //

    KeRaiseIrql(DISPATCH_LEVEL, &OldIrql);
    KeSetPriorityThread(Thread, (KPRIORITY)0);

    //
    // if a thread has not been selected to run on the current processors,
    // check to see if there are any ready threads; otherwise add this
    // processors to the IdleSummary
    //

    KeAcquireQueuedSpinLockAtDpcLevel(KeQueuedSpinLockContext(LockQueueDispatcherLock));
    if (Prcb->NextThread == (PKTHREAD)NULL) {
        SetMember(Number, KiIdleSummary);
    }

    KeReleaseQueuedSpinLockFromDpcLevel(KeQueuedSpinLockContext(LockQueueDispatcherLock));

    KeRaiseIrql(HIGH_LEVEL, &OldIrql);

    //
    // This processor has initialized
    //

    LoaderBlock->Prcb = (ULONG)NULL;

    return;
}

VOID
KiInitializePcr (
    IN ULONG Processor,
    IN PKPCR    Pcr,
    IN PKIDTENTRY Idt,
    IN PKGDTENTRY Gdt,
    IN PKTSS Tss,
    IN PKTHREAD Thread,
    IN PVOID DpcStack
    )

/*++

Routine Description:

    This function is called to initialize the PCR for a processor.  It
    simply stuffs values into the PCR.  (The PCR is not inited statically
    because the number varies with the number of processors.)

    Note that each processor has its own IDT, GDT, and TSS as well as PCR!

Arguments:

    Processor - Processor whose PCR to initialize.

    Pcr - Linear address of PCR.

    Idt - Linear address of i386 IDT.

    Gdt - Linear address of i386 GDT.

    Tss - Linear address (NOT SELECTOR!) of the i386 TSS.

    Thread - Dummy thread object to use very early on.

Return Value:

    None.

--*/
{
    // set version values

    Pcr->MajorVersion = PCR_MAJOR_VERSION;
    Pcr->MinorVersion = PCR_MINOR_VERSION;

    Pcr->PrcbData.MajorVersion = PRCB_MAJOR_VERSION;
    Pcr->PrcbData.MinorVersion = PRCB_MINOR_VERSION;

    Pcr->PrcbData.BuildType = 0;

#if DBG
    Pcr->PrcbData.BuildType |= PRCB_BUILD_DEBUG;
#endif

#ifdef NT_UP
    Pcr->PrcbData.BuildType |= PRCB_BUILD_UNIPROCESSOR;
#endif

#if defined (_X86PAE_)
    if (Processor == 0) {
        //
        //  PAE feature must be initialized prior to the first HAL call.
        //

        SharedUserData->ProcessorFeatures[PF_PAE_ENABLED] = TRUE;
    }
#endif

    //  Basic addressing fields

    Pcr->SelfPcr = Pcr;
    Pcr->Prcb = &(Pcr->PrcbData);

    //  Thread control fields

    Pcr->NtTib.ExceptionList = EXCEPTION_CHAIN_END;
    Pcr->NtTib.StackBase = 0;
    Pcr->NtTib.StackLimit = 0;
    Pcr->NtTib.Self = 0;

    Pcr->PrcbData.CurrentThread = Thread;

    //
    // Init Prcb.Number and ProcessorBlock such that Ipi will work
    // as early as possible.
    //

    Pcr->PrcbData.Number = (UCHAR)Processor;
    Pcr->PrcbData.SetMember = 1 << Processor;
    KiProcessorBlock[Processor] = Pcr->Prcb;

    Pcr->Irql = 0;

    //  Machine structure addresses

    Pcr->GDT = Gdt;
    Pcr->IDT = Idt;
    Pcr->TSS = Tss;
    Pcr->PrcbData.DpcStack = DpcStack;

    //
    // Initially, set this processor as only member of SMT set.
    //

    Pcr->PrcbData.MultiThreadProcessorSet = Pcr->PrcbData.SetMember;

    return;
}

#if 0
VOID
KiInitializeDblFaultTSS(
    IN PKTSS Tss,
    IN ULONG Stack,
    IN PKGDTENTRY TssDescriptor
    )

/*++

Routine Description:

    This function is called to initialize the double-fault TSS for a
    processor.  It will set the static fields of the TSS to point to
    the double-fault handler and the appropriate double-fault stack.

    Note that the IOPM for the double-fault TSS grants access to all
    ports.  This is so the standard HAL's V86-mode callback to reset
    the display to text mode will work.

Arguments:

    Tss - Supplies a pointer to the double-fault TSS

    Stack - Supplies a pointer to the double-fault stack.

    TssDescriptor - Linear address of the descriptor for the TSS.

Return Value:

    None.

--*/

{
    PUCHAR  p;
    ULONG   i;
    ULONG   j;

    //
    // Set limit for TSS
    //

    if (TssDescriptor != NULL) {
        TssDescriptor->LimitLow = sizeof(KTSS) - 1;
        TssDescriptor->HighWord.Bits.LimitHi = 0;
    }

    //
    // Initialize IOPMs
    //

    for (i = 0; i < IOPM_COUNT; i++) {
            p = (PUCHAR)(Tss->IoMaps[i]);

        for (j = 0; j < PIOPM_SIZE; j++) {
            p[j] = 0;
        }
    }

    //  Set IO Map base address to indicate no IO map present.

    // N.B. -1 does not seem to be a valid value for the map base.  If this
    //      value is used, byte immediate in's and out's will actually go
    //      the hardware when executed in V86 mode.

    Tss->IoMapBase = KiComputeIopmOffset(IO_ACCESS_MAP_NONE);

    //  Set flags to 0, which in particular disables traps on task switches.

    Tss->Flags = 0;


    //  Set LDT and Ss0 to constants used by NT.

    Tss->LDT  = 0;
    Tss->Ss0  = KGDT_R0_DATA;
    Tss->Esp0 = Stack;
    Tss->Eip  = (ULONG)KiTrap08;
    Tss->Cs   = KGDT_R0_CODE || RPL_MASK;
    Tss->Ds   = KGDT_R0_DATA;
    Tss->Es   = KGDT_R0_DATA;
    Tss->Fs   = KGDT_R0_DATA;


    return;

}
#endif


VOID
KiInitializeTSS (
    IN PKTSS Tss
    )

/*++

Routine Description:

    This function is called to initialize the TSS for a processor.
    It will set the static fields of the TSS.  (ie Those fields that
    the part reads, and for which NT uses constant values.)

    The dynamic fields (Esp0 and CR3) are set in the context swap
    code.

Arguments:

    Tss - Linear address of the Task State Segment.

Return Value:

    None.

--*/
{

    //  Set IO Map base address to indicate no IO map present.

    // N.B. -1 does not seem to be a valid value for the map base.  If this
    //      value is used, byte immediate in's and out's will actually go
    //      the hardware when executed in V86 mode.

    Tss->IoMapBase = KiComputeIopmOffset(IO_ACCESS_MAP_NONE);

    //  Set flags to 0, which in particular disables traps on task switches.

    Tss->Flags = 0;


    //  Set LDT and Ss0 to constants used by NT.

    Tss->LDT = 0;
    Tss->Ss0 = KGDT_R0_DATA;

    return;
}

VOID
KiInitializeTSS2 (
    IN PKTSS Tss,
    IN PKGDTENTRY TssDescriptor
    )

/*++

Routine Description:

    Do part of TSS init we do only once.

Arguments:

    Tss - Linear address of the Task State Segment.

    TssDescriptor - Linear address of the descriptor for the TSS.

Return Value:

    None.

--*/
{
    PUCHAR  p;
    ULONG   i;
    ULONG   j;

    //
    // Set limit for TSS
    //

    if (TssDescriptor != NULL) {
        TssDescriptor->LimitLow = sizeof(KTSS) - 1;
        TssDescriptor->HighWord.Bits.LimitHi = 0;
    }

    //
    // Initialize IOPMs
    //

    for (i = 0; i < IOPM_COUNT; i++) {
        p = (PUCHAR)(Tss->IoMaps[i].IoMap);

        for (j = 0; j < PIOPM_SIZE; j++) {
            p[j] = (UCHAR)-1;
        }
    }

    //
    // Initialize Software Interrupt Direction Maps
    //

    for (i = 0; i < IOPM_COUNT; i++) {
        p = (PUCHAR)(Tss->IoMaps[i].DirectionMap);
        for (j = 0; j < INT_DIRECTION_MAP_SIZE; j++) {
            p[j] = 0;
        }
        // dpmi requires special case for int 2, 1b, 1c, 23, 24
        p[0] = 4;
        p[3] = 0x18;
        p[4] = 0x18;
    }

    //
    // Initialize the map for IO_ACCESS_MAP_NONE
    //
    p = (PUCHAR)(Tss->IntDirectionMap);
    for (j = 0; j < INT_DIRECTION_MAP_SIZE; j++) {
        p[j] = 0;
    }

    // dpmi requires special case for int 2, 1b, 1c, 23, 24
    p[0] = 4;
    p[3] = 0x18;
    p[4] = 0x18;

    return;
}

VOID
KiSwapIDT (
    )

/*++

Routine Description:

    This function is called to edit the IDT.  It swaps words of the address
    and access fields around into the format the part actually needs.
    This allows for easy static init of the IDT.

    Note that this procedure edits the current IDT.

Arguments:

    None.

Return Value:

    None.

--*/
{
    LONG    Index;
    USHORT Temp;

    //
    // Rearrange the entries of IDT to match i386 interrupt gate structure
    //

    for (Index = 0; Index <= MAXIMUM_IDTVECTOR; Index += 1) {
        Temp = IDT[Index].Selector;
        IDT[Index].Selector = IDT[Index].ExtendedOffset;
        IDT[Index].ExtendedOffset = Temp;
    }
}

ULONG
KiGetCpuVendor(
    VOID
    )

/*++

Routine Description:

    (Try to) Determine the manufacturer of this processor based on
    data returned by the CPUID instruction (if present).

Arguments:

    None.

Return Value:

    One of the members of the enumeration CPU_VENDORS (defined above).

--*/
{
    PKPRCB Prcb;
    ULONG  Junk;
    ULONG  Buffer[4];

    Prcb = KeGetCurrentPrcb();
    Prcb->VendorString[0] = 0;

    if (!Prcb->CpuID) {
        return CPU_NONE;
    }

    CPUID(0, &Junk, Buffer+0, Buffer+2, Buffer+1);
    Buffer[3] = 0;

    //
    // Copy vendor string to Prcb for debugging (ensure it's NULL
    // terminated).
    //

    RtlCopyMemory(
        Prcb->VendorString,
        Buffer,
        sizeof(Prcb->VendorString) - 1
        );

    Prcb->VendorString[sizeof(Prcb->VendorString) - 1] = '\0';

    if (strcmp((PVOID)Buffer, CmpIntelID) == 0) {
        return CPU_INTEL;
    } else if (strcmp((PVOID)Buffer, CmpAmdID) == 0) {
        return CPU_AMD;
    } else if (strcmp((PVOID)Buffer, CmpCyrixID) == 0) {
        return CPU_CYRIX;
    } else if (strcmp((PVOID)Buffer, CmpTransmetaID) == 0) {
        return CPU_TRANSMETA;
    } else if (strcmp((PVOID)Buffer, CmpCentaurID) == 0) {
        return CPU_CENTAUR;
    }
    return CPU_UNKNOWN;
}

ULONG
KiGetFeatureBits (
    VOID
    )

/*++

Routine Description:

    Examine the processor specific feature bits to determine the
    Windows 2000 supported features supported by this processor.

Arguments:

    None.

Return Value:

    Returns a Windows 2000 normalized set of processor features.

--*/

{
    ULONG           Junk;
    ULONG           Temp;
    ULONG           ProcessorFeatures;
    ULONG           NtBits;
    ULONG           ExtendedProcessorFeatures;
    ULONG           ProcessorSignature;
    ULONG           CpuVendor;
    PKPRCB          Prcb;
    BOOLEAN         ExtendedCPUIDSupport = TRUE;

    Prcb = KeGetCurrentPrcb();

    NtBits = KF_WORKING_PTE;

    //
    // Determine the processor type
    //

    CpuVendor = KiGetCpuVendor();

    //
    // If this processor does not support the CPUID instruction,
    // don't try to use it.
    //

    if (CpuVendor == CPU_NONE) {
        return NtBits;
    }

    //
    // Determine which NT compatible features are present
    //

    CPUID (1, &ProcessorSignature, &Temp, &Junk, &ProcessorFeatures);

    //
    // CPUID(1) now returns information in EBX.  On the grounds that
    // the low functions are supposed to be standard, we record the
    // information regardless of processor vendor even though it may
    // be 0 or undefined on older implementations.
    //

    Prcb->InitialApicId = (UCHAR)(Temp >> 24);

    //
    // AMD specific stuff
    //

    if (CpuVendor == CPU_AMD) {

        //
        // Check for K5 and above.
        //

        if ((ProcessorSignature & 0x0F00) >= 0x0500) {

            if ((ProcessorSignature & 0x0F00) == 0x0500) {

                switch (ProcessorSignature & 0x00F0) {

                case 0x0010: // K5 Model 1

                    //
                    // for K5 Model 1 stepping 0 or 1 don't set global page
                    //

                    if ((ProcessorSignature & 0x000F) > 0x03) {

                        //
                        // K5 Model 1 stepping 2 or greater
                        //

                        break;
                    }

                    //
                    // K5 Model 1 stepping 0 or 1, FALL THRU.
                    //

                case 0x0000:        // K5 Model 0

                    //
                    // for K5 Model 0 or model unknown don't set global page
                    //

                    ProcessorFeatures &= ~0x2000;
                    break;

                case 0x0080:        // K6 Model 8 (K6-2)

                    //
                    // All steppings >= 8 support MTRRs.
                    //

                    if ((ProcessorSignature & 0x000F) >= 0x8) {
                        NtBits |= KF_AMDK6MTRR;
                    }
                    break;

                case 0x0090:        // K6 Model 9 (K6-3)

                    NtBits |= KF_AMDK6MTRR;
                    break;

                default:            // anything else, nothing to do.

                    break;
                }
            }

        } else {

            //
            // Less than family 5, don't set GLOBAL PAGE, LARGE
            // PAGE or CMOV.  (greater than family 5 will have the
            // bits set correctly).
            //

            ProcessorFeatures &= ~(0x08 | 0x2000 | 0x8000);

            //
            // We don't know what this processor returns if we
            // probe for extended CPUID support.
            //

            ExtendedCPUIDSupport = FALSE;
        }
    }

    //
    // Intel specific stuff
    //

    if (CpuVendor == CPU_INTEL) {
        if (Prcb->CpuType >= 6) {
            WRMSR (0x8B, 0);
            CPUID (1, &Junk, &Junk, &Junk, &ProcessorFeatures);
            Prcb->UpdateSignature.QuadPart = RDMSR (0x8B);
        }

        else if (Prcb->CpuType == 5) {
            KiI386PentiumLockErrataPresent = TRUE;
        }

        if ( ((ProcessorSignature & 0x0FF0) == 0x0610 &&
              (ProcessorSignature & 0x000F) <= 0x9) ||

             ((ProcessorSignature & 0x0FF0) == 0x0630 &&
              (ProcessorSignature & 0x000F) <= 0x4)) {

            NtBits &= ~KF_WORKING_PTE;
        }

        //
        // Don't support prior attempts at implementing syscall/sysexit
        // instructions.
        //

        if ((Prcb->CpuType < 6) ||
            ((Prcb->CpuType == 6) && (Prcb->CpuStep < 0x0303))) {

            ProcessorFeatures &= ~KI_FAST_SYSCALL_SUPPORTED;
        }
    }

    //
    // Cyrix specific stuff
    //

    if (CpuVendor == CPU_CYRIX) {

        //
        // Workaround bug 324467 which is caused by INTR being
        // held high too long during an FP instruction and causing
        // random Trap07 with no exception bits.
        //

        extern BOOLEAN KiIgnoreUnexpectedTrap07;

        KiIgnoreUnexpectedTrap07 = TRUE;

        //
        // Workaround CMPXCHG bug to Cyrix processors where
        // Family = 6, Model = 0, Stepping <= 1.  Note that
        // Prcb->CpuStep contains both model and stepping.
        //
        // Disable Locking in one of processor specific registers
        // (accessible via i/o space index/data pair).
        //

        if ((Prcb->CpuType == 6) &&
            (Prcb->CpuStep <= 1)) {

            #define CRC_NDX (PUCHAR)0x22
            #define CRC_DAT (CRC_NDX + 1)
            #define CCR1    0xc1

            UCHAR ValueCCR1;

            //
            // Get current setting.
            //

            WRITE_PORT_UCHAR(CRC_NDX, CCR1);

            ValueCCR1 = READ_PORT_UCHAR(CRC_DAT);

            //
            // Set the NO_LOCK bit and write it back.
            //

            ValueCCR1 |= 0x10;

            WRITE_PORT_UCHAR(CRC_NDX, CCR1);
            WRITE_PORT_UCHAR(CRC_DAT, ValueCCR1);

            #undef CCR1
            #undef CRC_DAT
            #undef CRC_NDX
        }
    }

    //
    // Check the standard CPUID feature bits.
    //
    // The following bits are known to work on Intel, AMD and Cyrix.
    // We hope (and assume) the clone makers will follow suit.
    //

    if (ProcessorFeatures & 0x00000002) {
        NtBits |= KF_V86_VIS | KF_CR4;
    }

    if (ProcessorFeatures & 0x00000008) {
        NtBits |= KF_LARGE_PAGE | KF_CR4;
    }

    if (ProcessorFeatures & 0x00000010) {
        NtBits |= KF_RDTSC;
    }

    //
    // N.B. CMPXCHG8B MUST be done in a generic manner or clone processors
    // will not be able to boot if they set this feature bit.
    //
    // This was incorrect in NT4 and resulted processor vendors claiming
    // not to support cmpxchg8b even if they did.   Whistler requires
    // cmpxchg8b, work around this problems for the cases we know about.
    //
    // Because cmpxchg8b is a requirement for whistler, winnt32 needs to
    // be modified if new processors are added to the following list.
    // Also, setupldr.   Both executables were modified so as to warn
    // the user rather than installing an unbootable system.
    //

    if ((ProcessorFeatures & 0x00000100) == 0) {

        ULONGLONG MsrValue;

        if ((CpuVendor == CPU_TRANSMETA) &&
            (Prcb->CpuType >= 5)         &&
            (Prcb->CpuStep >= 0x402)) {

            //
            // Transmeta processors have a cpuid feature bit 'mask' in
            // msr 80860004.   Unmask the cmpxchg8b bit.
            //

            MsrValue = RDMSR(0x80860004);
            MsrValue |= 0x100;
            WRMSR(0x80860004, MsrValue);

            ProcessorFeatures |= 0x100;

        } else if ((CpuVendor == CPU_CENTAUR) &&
                   (Prcb->CpuType >= 5)) {

            //
            // Centaur/IDT processors turn on the cmpxchg8b
            // feature bit by setting bit 1 in MSR 107.
            //

            ULONG CentaurFeatureControlMSR = 0x107;

            if (Prcb->CpuType >= 6) {

                //
                // Centaur processors (Cyrix III) turn on the cmpxchg8b
                // feature bit by setting bit 1 in MSR 1107.
                //
            
                CentaurFeatureControlMSR = 0x1107;
            }

            MsrValue = RDMSR(CentaurFeatureControlMSR);
            MsrValue |= 2;
            WRMSR(CentaurFeatureControlMSR, MsrValue);

            ProcessorFeatures |= 0x100;
        }
    }

    if (ProcessorFeatures & 0x00000100) {
        NtBits |= KF_CMPXCHG8B;
    }

    if (ProcessorFeatures & KI_FAST_SYSCALL_SUPPORTED) {
        NtBits |= KF_FAST_SYSCALL;
        KiFastSystemCallIsIA32 = TRUE;
    }

    if (ProcessorFeatures & 0x00001000) {
        NtBits |= KF_MTRR;
    }

    if (ProcessorFeatures & 0x00002000) {
        NtBits |= KF_GLOBAL_PAGE | KF_CR4;
    }

    if (ProcessorFeatures & 0x00008000) {
        NtBits |= KF_CMOV;
    }

    if (ProcessorFeatures & 0x00010000) {
        NtBits |= KF_PAT;
    }

    if (ProcessorFeatures & 0x00200000) {
        NtBits |= KF_DTS;
    }

    if (ProcessorFeatures & 0x00800000) {
        NtBits |= KF_MMX;
    }

    if (ProcessorFeatures & 0x01000000) {
        NtBits |= KF_FXSR;
    }

    if (ProcessorFeatures & 0x02000000) {
        NtBits |= KF_XMMI;
    }

    if (ProcessorFeatures & 0x04000000) {
        NtBits |= KF_XMMI64;
    }

    if (ProcessorFeatures & 0x10000000) {
        NtBits |= KF_SMT;
        Prcb->LogicalProcessorsPerPhysicalProcessor = (UCHAR)(Temp >> 16);
    } else {
        Prcb->LogicalProcessorsPerPhysicalProcessor = 1;
    }

    //
    // Check extended functions.   First, check for existance,
    // then check extended function 0x80000001 (Extended Processor
    // Features) if present.
    //
    // Note: Intel guarantees that no processor that doesn't support
    // extended CPUID functions will ever return a value with the
    // most significant bit set.   Microsoft asks all CPU vendors
    // to make the same guarantee.
    //

    if (ExtendedCPUIDSupport != FALSE) {

        CPUID(0x80000000, &Temp, &Junk, &Junk, &Junk);

        //
        // Sanity check the result, assuming there are no more
        // than 256 extended feature functions (should be valid
        // for a little while).
        //

        if ((Temp & 0xffffff00) == 0x80000000) {

            //
            // Check extended processor features.  These, by definition,
            // can vary on a processor by processor basis.
            //

            if (Temp >= 0x80000001) {

                CPUID(0x80000001, &Temp, &Junk, &Junk, &ExtendedProcessorFeatures);

                //
                // With these, we can only do what we're told.
                //

                switch (CpuVendor) {
                case CPU_AMD:

                    if (ExtendedProcessorFeatures & 0x80000000) {
                        NtBits |= KF_3DNOW;
                    }

#if 0

                    //
                    // There is a security hole with this implementation
                    // of fast system call such that it is possible to
                    // end up in the trap01 handler running on the user
                    // stack (ie not kernel stack).   Unfortunately this
                    // prohibits use of this instruction pair.
                    //

                    if (ExtendedProcessorFeatures & 0x00000800) {

                        //
                        // This processor supports AMD's implementation
                        // of SYSENTER/SYSEXIT (SYSCALL/SYSRET).  Use this
                        // unless it also supports the IA32 version.
                        //

                        if ((NtBits & KF_FAST_SYSCALL) == 0) {
                            NtBits |= KF_FAST_SYSCALL;
                        }
                    }

#endif

#if 0

                    if (ExtendedProcessorFeatures & 0x00100000) {
                        Temp = (ULONG)RDMSR(0xc0000080);
                        Temp |= 0x800;
                        WRMSR(0xc0000080, (ULONGLONG)Temp);
                        KeErrorMask = 0x9;
                        MmPaeErrMask = 0x8;
                        MmPaeMask = 0x8000000000000000UI64;
                    }
#endif

                    break;
                }
            }
        }
    }

    return NtBits;
}

VOID
KiGetCacheInformation(
    VOID
    )
{
#define CPUID_REG_COUNT 4
    ULONG CpuidData[CPUID_REG_COUNT];
    ULONG Line = 64;
    ULONG Size = 0;
    ULONG AdjustedSize = 0;
    UCHAR Assoc = 0;
    ULONG CpuVendor;
    PKPCR Pcr;

    //
    // Set default.
    //

    Pcr = KeGetPcr();

    Pcr->SecondLevelCacheSize = 0;

    //
    // Determine the processor manufacturer
    //

    CpuVendor = KiGetCpuVendor();

    if (CpuVendor == CPU_NONE) {
        return;
    }

    //
    // Obtain Cache size information for those processors on which
    // we know how.
    //

    switch (CpuVendor) {
    case CPU_INTEL:

        CPUID(0, CpuidData, CpuidData+1, CpuidData+2, CpuidData+3);

        //
        // Check this processor supports CPUID function 2 which is the
        // one that returns cache size info.
        //

        if (CpuidData[0] >= 2) {

            //
            // The above returns a series of bytes.    (In EAX, EBX, ECX
            // and EDX).   The least significant byte (of EAX) gives the
            // number of times CPUID(2 ...) should be issued to return
            // the complete set of data.   The bytes are self describing
            // data.
            //
            // In particular, the bytes describing the L2 cache size
            // will be in the following set (and meaning)
            //
            // 0x40       0  bytes
            // 0x41     128K bytes
            // 0x42     256K bytes
            // 0x43     512K bytes
            // 0x44    1024K bytes
            // 0x45    2048K bytes
            // 0x46    4096K bytes
            //
            // I am extrapolating the above as anything in the range
            // 0x41 thru 0x4f can be computed as
            //
            //   128KB << (descriptor - 0x41)
            //
            // The Intel folks say keep it to a reasonable upper bound,
            // eg 49.
            //
            // N.B. the range 0x80 .. 0x86 indicates the same cache
            // sizes but 8 way associative.
            //
            // Also, the most significant bit of each register indicates
            // whether not the register contains valid information.
            // 0 == Valid, 1 == InValid.
            //

            ULONG CpuidIterations;
            ULONG i;
            ULONG CpuidReg;

            BOOLEAN FirstPass = TRUE;

            do {
                CPUID(2, CpuidData, CpuidData+1, CpuidData+2, CpuidData+3);

                if (FirstPass) {

                    //
                    // Get the iteration count from the first byte
                    // of the returned data then replace that byte
                    // with 0 (a null descriptor).
                    //

                    CpuidIterations = CpuidData[0] & 0xff;
                    CpuidData[0] &= 0xffffff00;

                    FirstPass = FALSE;
                }

                for (i = 0; i < CPUID_REG_COUNT; i++) {

                    CpuidReg = CpuidData[i];

                    if (CpuidReg & 0x80000000) {

                        //
                        // Register doesn't contain valid data,
                        // skip it.
                        //

                        continue;
                    }

                    while (CpuidReg) {

                        //
                        // Get LS Byte from this DWORD and remove the
                        // byte.
                        //

                        UCHAR Descriptor = (UCHAR)(CpuidReg & 0xff);
                        CpuidReg >>= 8;

                        if (Descriptor == 0) {

                            //
                            // NULL descriptor
                            //

                            continue;
                        }

                        if (((Descriptor > 0x40) && (Descriptor <= 0x47)) ||
                            ((Descriptor > 0x78) && (Descriptor <= 0x7c)) ||
                            ((Descriptor > 0x80) && (Descriptor <= 0x87))) {

                            //
                            // L2 descriptor.
                            //
                            // To date, for all the descriptors we know
                            // about those above 0x78 are 8 way and those
                            // below are 4 way.
                            //

                            Assoc = Descriptor >= 0x79 ? 8 : 4;

                            if (((Descriptor & 0xf8) == 0x78) &&
                                (Line < 128)) {
                                Line = 128;
                            }
                            Descriptor &= 0x07;

                            //
                            // Assert the descriptor is in the range we
                            // officially know about.   If this hits on
                            // a checked build, check with Intel about
                            // the interpretation.
                            //

                            ASSERT(Descriptor <= 0x6);

                            Size = 0x10000 << Descriptor;
                            if ((Size / Assoc) > AdjustedSize) {
                                AdjustedSize = Size / Assoc;
                                Pcr->SecondLevelCacheSize = Size;
                                Pcr->SecondLevelCacheAssociativity = Assoc;
                            }

                        } else if ((Descriptor > 0x21) && (Descriptor <= 0x29)) {
                            if (Line < 128) {
                                Line = 128;
                            }
                            Assoc = 8;
                            switch (Descriptor) {
                            case 0x22:
                                Size = 512 * 1024;
                                Assoc = 4;
                                break;
                            case 0x23:
                                Size = 1024 * 1024;
                                break;
                            case 0x25:
                                Size = 2048 * 1024;
                                break;
                            case 0x29:
                                Size = 4096 * 1024;
                                break;
                            default:
                                Size = 0;
                                break;
                            }
                            if ((Size / Assoc) > AdjustedSize) {
                                AdjustedSize = Size / Assoc;
                                Pcr->SecondLevelCacheSize = Size;
                                Pcr->SecondLevelCacheAssociativity = Assoc;
                            }
                        } else if ((Descriptor > 0x65) && (Descriptor < 0x69)) {

                            //
                            // L1 Descriptor with line size of 64 bytes.
                            // N.B. These are the only ones we know about,
                            // we will get it wrong on future processors
                            // unless a less brain dead way of determining
                            // line size is provided.
                            //

                            KePrefetchNTAGranularity = 64;
                        }

                        //
                        // else if (do other descriptors)
                        //

                    } // while more bytes in this register

                } // for each register

                //
                // Note: Always run thru all iterations indicated by
                // the first to ensure a subsequent call won't start
                // part way thru.
                //

            } while (--CpuidIterations);
        }
        break;
    case CPU_AMD:

        //
        // Get L1 Cache Data.
        //

        CPUID(0x80000000, CpuidData, CpuidData+1, CpuidData+2, CpuidData+3);
        if (CpuidData[0] < 0x80000005) {

            //
            // This processor doesn't support L1 cache details.
            //

            break;
        }
        CPUID(0x80000005, CpuidData, CpuidData+1, CpuidData+2, CpuidData+3);
        KePrefetchNTAGranularity = CpuidData[2] & 0xff;

        //
        // Get L2 data.
        //

        CPUID(0x80000000, CpuidData, CpuidData+1, CpuidData+2, CpuidData+3);
        if (CpuidData[0] < 0x80000006) {

            //
            // This processor doesn't support L2 cache details.
            //

            break;
        }
        CPUID(0x80000006, CpuidData, CpuidData+1, CpuidData+2, CpuidData+3);
        Line = CpuidData[2] & 0xff;
        switch ((CpuidData[2] >> 12) & 0xf) {
        case 0x2:   Assoc = 2;  break;
        case 0x4:   Assoc = 4;  break;
        case 0x6:   Assoc = 8;  break;
        case 0x8:   Assoc = 16; break;

        //
        // ff is really fully associative, just represent as 16 way.
        //
        case 0xff:  Assoc = 16; break;
        default:    Assoc = 1;  break;
        }
        Size = (CpuidData[2] >> 16) << 10;
        if ((Pcr->PrcbData.CpuType == 0x6) &&
            (Pcr->PrcbData.CpuStep == 0x300)) {

            //
            // Model 6,3,0 uses a different algorithm to report cache
            // size.
            //

            Size = 64 * 1024;
        }

        Pcr->SecondLevelCacheAssociativity = Assoc;
        Pcr->SecondLevelCacheSize = Size;
        break;
    }

    if (Line > KeLargestCacheLine) {
        KeLargestCacheLine = Line;
    }

#undef CPUID_REG_COUNT
}

#define MAX_ATTEMPTS    10

BOOLEAN
KiInitMachineDependent (
    VOID
    )
{
    KAFFINITY       ActiveProcessors, CurrentAffinity;
    ULONG           NumberProcessors;
    IDENTITY_MAP    IdentityMap;
    ULONG           Index;
    ULONG           Average;
    ULONG           Junk;
    struct {
        LARGE_INTEGER   PerfStart;
        LARGE_INTEGER   PerfEnd;
        LONGLONG        PerfDelta;
        LARGE_INTEGER   PerfFreq;
        LONGLONG        TSCStart;
        LONGLONG        TSCEnd;
        LONGLONG        TSCDelta;
        ULONG           MHz;
    } Samples[MAX_ATTEMPTS], *pSamp;
    PUCHAR          PatchLocation;

    //
    // If PDE large page is supported, enable it.
    //
    // We enable large pages before global pages to make TLB invalidation
    // easier while turning on large pages.
    //

    if (KeFeatureBits & KF_LARGE_PAGE) {
        if (Ki386CreateIdentityMap(&IdentityMap,
                                   &Ki386EnableCurrentLargePage,
                                   &Ki386EnableCurrentLargePageEnd )) {

            KiIpiGenericCall (
                (PKIPI_BROADCAST_WORKER) Ki386EnableTargetLargePage,
                (ULONG)(&IdentityMap)
            );
        }

        //
        // Always call Ki386ClearIdentityMap() to free any memory allocated
        //

        Ki386ClearIdentityMap(&IdentityMap);
    }

    //
    // If PDE/PTE global page is supported, enable it
    //

    if (KeFeatureBits & KF_GLOBAL_PAGE) {
        NumberProcessors = KeNumberProcessors;
        KiIpiGenericCall (
            (PKIPI_BROADCAST_WORKER) Ki386EnableGlobalPage,
            (ULONG)(&NumberProcessors)
        );
    }

#if !defined(NT_UP)

    //
    // If some processor doesn't have proper MP PTE implementation,
    // then use a synchronous TB shoot down handler
    //

    if (!(KeFeatureBits & KF_WORKING_PTE)) {
        NumberProcessors = KeNumberProcessors;
        KiIpiGenericCall (
            (PKIPI_BROADCAST_WORKER) Ki386UseSynchronousTbFlush,
            (ULONG)(&NumberProcessors)
        );
    }

#endif

    //
    // If PAT or MTRR supported but the HAL indicates it shouldn't
    // be used (eg on a Shared Memory Cluster), drop the feature.
    //

    if (KeFeatureBits & (KF_PAT | KF_MTRR)) {

        NTSTATUS Status;
        BOOLEAN  UseFrameBufferCaching;
        ULONG    Size;

        Status = HalQuerySystemInformation(
                     HalFrameBufferCachingInformation,
                     sizeof(UseFrameBufferCaching),
                     &UseFrameBufferCaching,
                     &Size
                     );

        if (NT_SUCCESS(Status) &&
            (UseFrameBufferCaching == FALSE)) {

            //
            // Hal says don't use.
            //

            KeFeatureBits &= ~(KF_PAT | KF_MTRR);
        }
    }


    //
    // If PAT is supported then initialize it.
    //

    if (KeFeatureBits & KF_PAT) {
        KiInitializePAT();
    }


    //
    // Check to see if the floating point emulator should be used.
    //

    SharedUserData->ProcessorFeatures[PF_FLOATING_POINT_PRECISION_ERRATA] =
            FALSE;

    switch (KeI386ForceNpxEmulation) {
    case 0:

        //
        // Use the emulator based on the value in KeI386NpxPresent
        //

        break;

    case 1:

        //
        // Only use the emulator if any processor has the known
        // Pentium floating point division problem.
        //

        if (KeI386NpxPresent) {

            //
            // A coprocessor is present, check to see if the precision
            // errata exists.
            //

            double  Dividend, Divisor;
            BOOLEAN PrecisionErrata = FALSE;

            ActiveProcessors = KeActiveProcessors;
            for (CurrentAffinity = 1; ActiveProcessors; CurrentAffinity <<= 1) {

                if (ActiveProcessors & CurrentAffinity) {
                    ActiveProcessors &= ~CurrentAffinity;

                    //
                    // Run calculation on each processor.
                    //

                    KeSetSystemAffinityThread(CurrentAffinity);
                    _asm {

                        ;
                        ; This is going to destroy the state in the coprocesssor,
                        ; but we know that there's no state currently in it.
                        ;

                        cli
                        mov     eax, cr0
                        mov     ecx, eax    ; hold original cr0 value
                        and     eax, not (CR0_TS+CR0_MP+CR0_EM)
                        mov     cr0, eax

                        fninit              ; to known state
                    }

                    Dividend = 4195835.0;
                    Divisor  = 3145727.0;

                    _asm {
                        fld     Dividend
                        fdiv    Divisor     ; test known faulty divison
                        fmul    Divisor     ; Multiple quotient by divisor
                        fcomp   Dividend    ; Compare product and dividend
                        fstsw   ax          ; Move float conditions to ax
                        sahf                ; move to eflags

                        mov     cr0, ecx    ; restore cr0
                        sti

                        jc      short em10
                        jz      short em20
em10:                   mov     PrecisionErrata, TRUE
em20:
                    }
                    if (PrecisionErrata) {
                        KeI386NpxPresent = FALSE;
                        SharedUserData->ProcessorFeatures[PF_FLOATING_POINT_PRECISION_ERRATA] = TRUE;
                        break;
                    }
                }
            }

        }
        break;

    default:

        //
        // Unknown setting - use the emulator
        //

        KeI386NpxPresent = FALSE;
        break;
    }

    //
    // Setup processor features, and install emulator if needed
    //

    SharedUserData->ProcessorFeatures[PF_FLOATING_POINT_EMULATED] =
            !KeI386NpxPresent;

    if (!KeI386NpxPresent) {

        //
        // MMx, fast save/restore, streaming SIMD not available when
        // emulator is used.  (Nor FP errata).
        //

        KeFeatureBits &= ~(KF_MMX | KF_FXSR | KF_XMMI | KF_XMMI64);
        KeI386XMMIPresent = FALSE;
        KeI386FxsrPresent = FALSE;

        SharedUserData->ProcessorFeatures[PF_MMX_INSTRUCTIONS_AVAILABLE]      =
        SharedUserData->ProcessorFeatures[PF_XMMI_INSTRUCTIONS_AVAILABLE]     =
        SharedUserData->ProcessorFeatures[PF_3DNOW_INSTRUCTIONS_AVAILABLE]    =
        SharedUserData->ProcessorFeatures[PF_XMMI64_INSTRUCTIONS_AVAILABLE]   =
        SharedUserData->ProcessorFeatures[PF_FLOATING_POINT_PRECISION_ERRATA] =
            FALSE;
    }

    //
    // If CR4 exists, enable DE extensions for IO breakpoints
    //

    if (KeFeatureBits & KF_CR4) {
        NumberProcessors = KeNumberProcessors;

        KiIpiGenericCall (
            (PKIPI_BROADCAST_WORKER) Ki386EnableDE,
            (ULONG)(&NumberProcessors)
        );
    }

    //
    // If FXSR feature is supported, set OSFXSR (bit 9) in CR4
    //

    if (KeFeatureBits & KF_FXSR) {
        NumberProcessors = KeNumberProcessors;

        KiIpiGenericCall (
            (PKIPI_BROADCAST_WORKER) Ki386EnableFxsr,
            (ULONG)(&NumberProcessors)
        );


        //
        // If XMMI feature is supported,
        //    a. Hook int 19 handler
        //    b. Set OSXMMEXCPT (bit 10) in CR4
        //    c. Enable use of fast XMMI based zero page routines.
        //    d. Remove return instruction at start of prefetch routine.
        //

        if (KeFeatureBits & KF_XMMI) {
            KiIpiGenericCall (
                (PKIPI_BROADCAST_WORKER) Ki386EnableXMMIExceptions,
                (ULONG)(&NumberProcessors)
            );

            KeZeroPage = KiXMMIZeroPage;
            KeZeroPageFromIdleThread = KiXMMIZeroPageNoSave;

            *(PUCHAR)&RtlPrefetchMemoryNonTemporal = 0x90;
        }


    } else {
#ifndef NT_UP
        //
        // Patch the fxsave instruction in SwapContext to use
        // "fnsave {dd, 31}, fwait {9b}"
        //
        ASSERT( ((ULONG)&ScPatchFxe-(ULONG)&ScPatchFxb) >= 3);

        PatchLocation = (PUCHAR)&ScPatchFxb;

        *PatchLocation++ = 0xdd;
        *PatchLocation++ = 0x31;
        *PatchLocation++ = 0x9b;

        while (PatchLocation < (PUCHAR)&ScPatchFxe) {
            //
            // Put nop's in the remaining bytes
            //
            *PatchLocation++ = 0x90;
        }
#endif
    }

    //
    // If the system (ie all processors) supports fast system
    // call/return, initialize the machine specific registers
    // required to support it.
    //

    KiRestoreFastSyscallReturnState();

    ActiveProcessors = KeActiveProcessors;
    for (CurrentAffinity=1; ActiveProcessors; CurrentAffinity <<= 1) {

        if (ActiveProcessors & CurrentAffinity) {

            //
            // Switch to that processor, and remove it from the
            // remaining set of processors
            //

            ActiveProcessors &= ~CurrentAffinity;
            KeSetSystemAffinityThread(CurrentAffinity);

            //
            // Determine the MHz for the processor
            //

            KeGetCurrentPrcb()->MHz = 0;

            if (KeFeatureBits & KF_RDTSC) {

                Index = 0;
                pSamp = Samples;

                for (; ;) {

                    //
                    // Collect a new sample
                    // Delay the thread a "long" amount and time it with
                    // a time source and RDTSC.
                    //

                    CPUID (0, &Junk, &Junk, &Junk, &Junk);
                    pSamp->PerfStart = KeQueryPerformanceCounter (NULL);
                    pSamp->TSCStart = RDTSC();
                    pSamp->PerfFreq.QuadPart = -50000;

                    KeDelayExecutionThread (KernelMode, FALSE, &pSamp->PerfFreq);

                    CPUID (0, &Junk, &Junk, &Junk, &Junk);
                    pSamp->PerfEnd = KeQueryPerformanceCounter (&pSamp->PerfFreq);
                    pSamp->TSCEnd = RDTSC();

                    //
                    // Calculate processors MHz
                    //

                    pSamp->PerfDelta = pSamp->PerfEnd.QuadPart - pSamp->PerfStart.QuadPart;
                    pSamp->TSCDelta = pSamp->TSCEnd - pSamp->TSCStart;

                    pSamp->MHz = (ULONG) ((pSamp->TSCDelta * pSamp->PerfFreq.QuadPart + 500000L) /
                                          (pSamp->PerfDelta * 1000000L));


                    //
                    // If last 2 samples matched within a MHz, done
                    //

                    if (Index) {
                        if (pSamp->MHz == pSamp[-1].MHz ||
                            pSamp->MHz == pSamp[-1].MHz + 1 ||
                            pSamp->MHz == pSamp[-1].MHz - 1) {
                                break;
                        }
                    }

                    //
                    // Advance to next sample
                    //

                    pSamp += 1;
                    Index += 1;

                    //
                    // If too many samples, then something is wrong
                    //

                    if (Index >= MAX_ATTEMPTS) {

#if DBG
                        //
                        // Temp breakpoint to see where this is failing
                        // and why
                        //

                        DbgBreakPoint();
#endif

                        Average = 0;
                        for (Index = 0; Index < MAX_ATTEMPTS; Index++) {
                            Average += Samples[Index].MHz;
                        }
                        pSamp[-1].MHz = Average / MAX_ATTEMPTS;
                        break;
                    }

                }

                KeGetCurrentPrcb()->MHz = (USHORT) pSamp[-1].MHz;
            }

            //
            // If MTRRs are supported and PAT not supported, initialize MTRRs
            // per processor
            //

            if (KeFeatureBits & KF_MTRR) {
                KiInitializeMTRR ( (BOOLEAN) (ActiveProcessors ? FALSE : TRUE));
            }

            //
            // If the processor is a AMD K6 with MTRR support then
            // perform processor specific initialization.
            //

            if (KeFeatureBits & KF_AMDK6MTRR) {
                KiAmdK6InitializeMTRR();
            }

            //
            // Apply Pentium workaround if needed
            //

            if (KiI386PentiumLockErrataPresent) {
                KiI386PentiumLockErrataFixup ();
            }

            //
            // If this processor supports fast floating save/restore,
            // determine the MXCSR mask value that should be used.
            //

            if (KeFeatureBits & KF_FXSR) {

                //
                // Get base of NPX save area.
                //
                //

                PFX_SAVE_AREA NpxFrame;
                KFLOATING_SAVE Save;
                ULONG MXCsrMask = 0xFFBF;

                NpxFrame = (PFX_SAVE_AREA)
                    (((ULONG)(KeGetCurrentThread()->InitialStack) -
                    sizeof(FX_SAVE_AREA)));

                NpxFrame->U.FxArea.MXCsrMask = 0;
                Kix86FxSave(NpxFrame);

                //
                // If the processor supplied a mask value, use
                // that, otherwise set the default value.
                //

                if (NpxFrame->U.FxArea.MXCsrMask != 0) {
                    MXCsrMask = NpxFrame->U.FxArea.MXCsrMask;
                }

                //
                // All processors must use the same (most restrictive)
                // value.
                //

                if (KiMXCsrMask == 0) {
                    KiMXCsrMask = MXCsrMask;
                } else if (KiMXCsrMask != MXCsrMask) {
                    KeBugCheckEx(MULTIPROCESSOR_CONFIGURATION_NOT_SUPPORTED,
                                 KF_FXSR,
                                 KiMXCsrMask,
                                 MXCsrMask,
                                 0);
                }

                KiMXCsrMask &= MXCsrMask;
            }
        }
    }

    KeRevertToUserAffinityThread();
    return TRUE;
}


VOID
KeOptimizeProcessorControlState (
    VOID
    )
{
    Ke386ConfigureCyrixProcessor ();
}



VOID
KeSetup80387OrEmulate (
    IN PVOID *R3EmulatorTable
    )

/*++

Routine Description:

    This routine is called by PS initialization after loading NTDLL.

    If this is a 386 system without 387s (all processors must be
    symmetrical) then this function will set the trap 07 vector on all
    processors to point to the address passed in (which should be the
    entry point of the 80387 emulator in NTDLL, NPXNPHandler).

Arguments:

    HandlerAddress - Supplies the address of the trap07 handler.

Return Value:

    None.

--*/

{
    PKINTERRUPT_ROUTINE HandlerAddress;
    KAFFINITY           ActiveProcessors, CurrentAffinity;
    KIRQL               OldIrql;
    ULONG               disposition;
    HANDLE              SystemHandle, SourceHandle, DestHandle;
    NTSTATUS            Status;
    UNICODE_STRING      unicodeString;
    OBJECT_ATTRIBUTES   ObjectAttributes;

    if (!KeI386NpxPresent) {

        //
        // Use the user mode floating point emulator
        //

        HandlerAddress = (PKINTERRUPT_ROUTINE) ((PULONG) R3EmulatorTable)[0];
        Ki387RoundModeTable = (PVOID) ((PULONG) R3EmulatorTable)[1];

        ActiveProcessors = KeActiveProcessors;
        for (CurrentAffinity = 1; ActiveProcessors; CurrentAffinity <<= 1) {

            if (ActiveProcessors & CurrentAffinity) {
                ActiveProcessors &= ~CurrentAffinity;

                //
                // Run this code on each processor.
                //

                KeSetSystemAffinityThread(CurrentAffinity);

                //
                // Raise IRQL and lock dispatcher database.
                //

                KiLockDispatcherDatabase(&OldIrql);

                //
                // Make the trap 07 IDT entry point at the passed-in handler
                //

                KiSetHandlerAddressToIDT(I386_80387_NP_VECTOR, HandlerAddress);
                KeGetPcr()->IDT[I386_80387_NP_VECTOR].Selector = KGDT_R3_CODE;
                KeGetPcr()->IDT[I386_80387_NP_VECTOR].Access = TRAP332_GATE;


                //
                // Unlock dispatcher database and lower IRQL to its previous value.
                //

                KiUnlockDispatcherDatabase(OldIrql);
            }
        }

        //
        // Set affinity back to the original value.
        //

        KeRevertToUserAffinityThread();

        //
        // Move any entries from ..\System\FloatingPointProcessor to
        // ..\System\DisabledFloatingPointProcessor.
        //

        //
        // Open system tree
        //

        InitializeObjectAttributes(
            &ObjectAttributes,
            &CmRegistryMachineHardwareDescriptionSystemName,
            OBJ_CASE_INSENSITIVE,
            NULL,
            NULL
            );

        Status = ZwOpenKey( &SystemHandle,
                            KEY_ALL_ACCESS,
                            &ObjectAttributes
                            );

        if (NT_SUCCESS(Status)) {

            //
            // Open FloatingPointProcessor key
            //

            InitializeObjectAttributes(
                &ObjectAttributes,
                &CmTypeName[FloatingPointProcessor],
                OBJ_CASE_INSENSITIVE,
                SystemHandle,
                NULL
                );

            Status = ZwOpenKey ( &SourceHandle,
                                 KEY_ALL_ACCESS,
                                 &ObjectAttributes
                                 );

            if (NT_SUCCESS(Status)) {

                //
                // Create DisabledFloatingPointProcessor key
                //

                RtlInitUnicodeString (
                    &unicodeString,
                    CmDisabledFloatingPointProcessor
                    );

                InitializeObjectAttributes(
                    &ObjectAttributes,
                    &unicodeString,
                    OBJ_CASE_INSENSITIVE,
                    SystemHandle,
                    NULL
                    );

                Status = ZwCreateKey( &DestHandle,
                                      KEY_ALL_ACCESS,
                                      &ObjectAttributes,
                                      0,
                                      NULL,
                                      REG_OPTION_VOLATILE,
                                      &disposition
                                      );

                if (NT_SUCCESS(Status)) {

                    //
                    // Move it
                    //

                    KiMoveRegTree (SourceHandle, DestHandle);
                    ZwClose (DestHandle);
                }
                ZwClose (SourceHandle);
            }
            ZwClose (SystemHandle);
        }
    }
}



NTSTATUS
KiMoveRegTree(
    HANDLE  Source,
    HANDLE  Dest
    )
{
    NTSTATUS                    Status;
    PKEY_BASIC_INFORMATION      KeyInformation;
    PKEY_VALUE_FULL_INFORMATION KeyValue;
    OBJECT_ATTRIBUTES           ObjectAttributes;
    HANDLE                      SourceChild;
    HANDLE                      DestChild;
    ULONG                       ResultLength;
    UCHAR                       buffer[1024];           // hmm....
    UNICODE_STRING              ValueName;
    UNICODE_STRING              KeyName;


    KeyValue = (PKEY_VALUE_FULL_INFORMATION)buffer;

    //
    // Move values from source node to dest node
    //

    for (; ;) {
        //
        // Get first value
        //

        Status = ZwEnumerateValueKey(Source,
                                     0,
                                     KeyValueFullInformation,
                                     buffer,
                                     sizeof (buffer),
                                     &ResultLength);

        if (!NT_SUCCESS(Status)) {
            break;
        }


        //
        // Write value to dest node
        //

        ValueName.Buffer = KeyValue->Name;
        ValueName.Length = (USHORT) KeyValue->NameLength;
        ZwSetValueKey( Dest,
                       &ValueName,
                       KeyValue->TitleIndex,
                       KeyValue->Type,
                       buffer+KeyValue->DataOffset,
                       KeyValue->DataLength
                      );

        //
        // Delete value and get first value again
        //

        Status = ZwDeleteValueKey (Source, &ValueName);
        if (!NT_SUCCESS(Status)) {
            break;
        }
    }


    //
    // Enumerate node's children and apply ourselves to each one
    //

    KeyInformation = (PKEY_BASIC_INFORMATION)buffer;
    for (; ;) {

        //
        // Open node's first key
        //

        Status = ZwEnumerateKey(
                    Source,
                    0,
                    KeyBasicInformation,
                    KeyInformation,
                    sizeof (buffer),
                    &ResultLength
                    );

        if (!NT_SUCCESS(Status)) {
            break;
        }

        KeyName.Buffer = KeyInformation->Name;
        KeyName.Length = (USHORT) KeyInformation->NameLength;

        InitializeObjectAttributes(
            &ObjectAttributes,
            &KeyName,
            OBJ_CASE_INSENSITIVE,
            Source,
            NULL
            );

        Status = ZwOpenKey(
                    &SourceChild,
                    KEY_ALL_ACCESS,
                    &ObjectAttributes
                    );

        if (!NT_SUCCESS(Status)) {
            break;
        }

        //
        // Create key in dest tree
        //

        InitializeObjectAttributes(
            &ObjectAttributes,
            &KeyName,
            OBJ_CASE_INSENSITIVE,
            Dest,
            NULL
            );

        Status = ZwCreateKey(
                    &DestChild,
                    KEY_ALL_ACCESS,
                    &ObjectAttributes,
                    0,
                    NULL,
                    REG_OPTION_VOLATILE,
                    NULL
                    );

        if (!NT_SUCCESS(Status)) {
            break;
        }

        //
        // Move subtree
        //

        Status = KiMoveRegTree(SourceChild, DestChild);

        ZwClose(DestChild);
        ZwClose(SourceChild);

        if (!NT_SUCCESS(Status)) {
            break;
        }

        //
        // Loop and get first key.  (old first key was deleted by the
        // call to KiMoveRegTree).
        //
    }

    //
    // Remove source node
    //

    return NtDeleteKey (Source);
}

VOID
KiI386PentiumLockErrataFixup (
    VOID
    )

/*++

Routine Description:

    This routine is called once on every processor when
    KiI386PentiumLockErrataPresent is TRUE.

    This routine replaces the local IDT with an IDT that has the first 7 IDT
    entries on their own page and returns the first page to the caller to
    be marked as read-only.  This causes the processor to trap-0e fault when
    the errata occurs.  Special code in the trap-0e handler detects the
    problem and performs the proper fixup.

Arguments:

    FixupPage   - Returns a virtual address of a page to be marked read-only

Return Value:

    None.

--*/

{
    KDESCRIPTOR IdtDescriptor;
    ULONG       OrginalBase;
    PUCHAR      NewBase, BasePage;
    BOOLEAN     Enable;
    BOOLEAN     Status;


#define IDT_SKIP   (7 * sizeof (KIDTENTRY))

    //
    // Allocate memory for a new copy of the processor's IDT
    //

    BasePage = MmAllocateIndependentPages (2*PAGE_SIZE, 0);

    //
    // The IDT base is such that the first 7 entries are on the
    // first (read-only) page, and the remaining entries are on the
    // second (read-write) page
    //

    NewBase = BasePage + PAGE_SIZE - IDT_SKIP;

    //
    // Disable interrupts on this processor while updating the IDT base
    //

    Enable = KeDisableInterrupts();

    //
    // Copy Old IDT to new IDT
    //

    _asm {
        sidt IdtDescriptor.Limit
    }

    RtlCopyMemory ((PVOID) NewBase,
                   (PVOID) IdtDescriptor.Base,
                   IdtDescriptor.Limit + 1
                  );

    IdtDescriptor.Base = (ULONG) NewBase;

    //
    // Set the new IDT
    //

    _asm {
        lidt IdtDescriptor.Limit
    }

    //
    // Update the PCR
    //

    KeGetPcr()->IDT = (PKIDTENTRY) NewBase;

    //
    // Restore interrupts
    //

    KeEnableInterrupts(Enable);

    //
    // Mark the first page which contains IDT entries 0-6 as read-only
    //

    Status = MmSetPageProtection (BasePage, PAGE_SIZE, PAGE_READONLY);
    ASSERT (Status);
}

VOID
KiLogMcaErrors (
    VOID
    )

/*++
    Routine Description:
        This routine is called to check for remembered machine checks.
        If a machine check crashed the system, this is the last chance
        to save information about it, at the time the system reboots.

    Arguments:
        none

    Return Value:
        none
--*/

{
    PIO_ERROR_LOG_PACKET ErrLog;
    MCA_EXCEPTION Exception;
    PKPRCB        Prcb;
    PWSTR         PDest;
    PWSTR         PSrc;
    WCHAR         PString[20];
    NTSTATUS      Status;
    ULONG         ToDo;
    ULONG         Length;
    PUCHAR        PData;
    ULONG         RegSet;
    KAFFINITY     ActiveProcessors, CurrentAffinity;
    HAL_MCA_INTERFACE   Mca;
    UCHAR       BankNumber;

    //
    // Current Pentium IIIs don't support the retention of machine
    // check information across a warm reset.  This should've meant
    // that this function wouldn't generate any eventlog entries on
    // those processors.  However, this code was logging non-fatal
    // MCAs created by BIOS initialization resulting in spurious
    // eventlog entries that did not indicate a reliability problem.
    //
    // Now this code is only run on Intel processors that are
    // Pentium 4-derived or later i.e. where the MCA warm reset retention
    // functionality is definitely implemented.
    //

    Prcb = KeGetCurrentPrcb();
    if ((strcmp((PCHAR) Prcb->VendorString, CmpIntelID) == 0) &&
        (KeProcessorLevel < 0xF)) {
        return;
    }

    //
    // Obtain the HAL interface to the Machine Check Architecture
    // register set.
    //

    Status = HalQuerySystemInformation(HalQueryMcaInterface,
                                       sizeof(Mca),
                                       &Mca,
                                       &Length);

    if (Status != STATUS_SUCCESS) {
        return;
    }
    ASSERT(Length == sizeof(Mca));

    //
    // Clear the exception data and indicate to the HAL that we
    // support extended MCA data. (VersionNumber = 2).
    //

    RtlZeroMemory(&Exception, sizeof(Exception));
    Exception.VersionNumber = 2;

    //
    // Obtain exclusive access to the MCA registers.
    //

    Mca.Lock();

    //
    // Check for remembered machine check errors on each processor
    // in the system.
    //

    ActiveProcessors = KeActiveProcessors;

    for (CurrentAffinity = 1; ActiveProcessors; CurrentAffinity <<= 1) {

        if ((CurrentAffinity & ActiveProcessors) == 0) {
            continue;
        }
        ActiveProcessors ^= CurrentAffinity;

        KeSetSystemAffinityThread(CurrentAffinity);

        //
        // Look for a machine check on this processor.
        //

        for (BankNumber = 0; TRUE; BankNumber++) {

            Status = Mca.ReadRegister(BankNumber, &Exception);

            if (Status == STATUS_UNSUCCESSFUL) {

                //
                // No error this bank, examine next.
                //

                continue;
            }

            if (Status != STATUS_SUCCESS) {

                //
                // No such register, done looking on this processor.
                //

                break;
            }

            //
            // Have a machine check, try to report it.
            //

            ErrLog = IoAllocateGenericErrorLogEntry(ERROR_LOG_MAXIMUM_SIZE);
            if (!ErrLog) {

                //
                // Unable to allocate an error log entry?   Give up.
                //

                break;
            }

            //
            // Fill it in and write it out.
            //
            // "Application popup: %1 %2"
            //

            ErrLog->DumpDataSize = (USHORT)MCA_EXCEPTION_V1_SIZE;
            ErrLog->NumberOfStrings = 0;
            ErrLog->StringOffset =
                FIELD_OFFSET(IO_ERROR_LOG_PACKET, DumpData)
                + MCA_EXCEPTION_V1_SIZE;
            ErrLog->StringOffset =
                (ErrLog->StringOffset + sizeof(PWSTR)-1)
                / sizeof(PWSTR);
            ErrLog->StringOffset *= sizeof(PWSTR);
            ErrLog->ErrorCode = STATUS_LOG_HARD_ERROR;
            ErrLog->UniqueErrorValue = Exception.u.Mca.BankNumber;
            ErrLog->FinalStatus = STATUS_LOG_HARD_ERROR;

            RtlCopyMemory(ErrLog->DumpData, &Exception, MCA_EXCEPTION_V1_SIZE);

            PDest = (PWSTR)((PCHAR)ErrLog+ErrLog->StringOffset);
            PSrc = L"Machine Check:";
            RtlCopyMemory(PDest, PSrc, (wcslen(PSrc)+1) * sizeof(WCHAR));
            PDest += wcslen(PSrc)+1;
            ErrLog->NumberOfStrings++;

            PSrc = L"Banks";
            RtlCopyMemory(PDest, PSrc, (wcslen(PSrc)+1) * sizeof(WCHAR));
            PDest += wcslen(PSrc)+1;
            ErrLog->NumberOfStrings++;

            IoWriteErrorLogEntry(ErrLog);

            //
            // There are Exception.ExtCnt entries in Exception.ExtReg,
            // plus Exception.ExtCnt itself.
            //

            ToDo = (Exception.ExtCnt+1) * sizeof(Exception.ExtReg[0]);
            PData = ((PUCHAR)&Exception)+MCA_EXCEPTION_V1_SIZE;
            RegSet = 0;
            while (ToDo > 0) {

                //
                // Have additional machine check data, try to report it.
                //

                if (ToDo > 8 * sizeof(ULONGLONG)) {
                    Length = 8 * sizeof(ULONGLONG);
                } else {
                    Length = ToDo;
                }
                ErrLog = IoAllocateGenericErrorLogEntry(ERROR_LOG_MAXIMUM_SIZE);
                if (!ErrLog) {
                    break; // Inner loop only
                }

                //
                // Fill it in and write it out.
                //
                // "Application popup: %1 %2"
                //

                ErrLog->DumpDataSize = (USHORT)Length;
                ErrLog->NumberOfStrings = 0;
                ErrLog->StringOffset =
                    FIELD_OFFSET(IO_ERROR_LOG_PACKET, DumpData)
                    + (USHORT)Length;
                ErrLog->StringOffset =
                    (ErrLog->StringOffset + sizeof(PWSTR)-1)
                    / sizeof(PWSTR);
                ErrLog->StringOffset *= sizeof(PWSTR);
                ErrLog->ErrorCode = STATUS_LOG_HARD_ERROR;
                ErrLog->UniqueErrorValue = RegSet;
                ErrLog->FinalStatus = STATUS_LOG_HARD_ERROR;

                RtlCopyMemory(ErrLog->DumpData, PData, Length);

                PDest = (PWSTR)((PCHAR)ErrLog+ErrLog->StringOffset);
                PSrc = L"Machine Check: Regs";
                RtlCopyMemory(PDest, PSrc, (wcslen(PSrc)+1) * sizeof(WCHAR));
                PDest += wcslen(PSrc)+1;
                ErrLog->NumberOfStrings++;

                PSrc = _itow(RegSet, PString, 10);
                RtlCopyMemory(PDest, PSrc, (wcslen(PSrc)+1) * sizeof(WCHAR));
                PDest += wcslen(PSrc)+1;
                ErrLog->NumberOfStrings++;

                IoWriteErrorLogEntry(ErrLog);
                PData += Length;
                ToDo  -= Length;
                RegSet+= 8;
            }

            //
            // Reset for next iteration.
            //

            RtlZeroMemory(&Exception, sizeof(Exception));
            Exception.VersionNumber = 2;
        }
    }
    KeRevertToUserAffinityThread();
    Mca.Unlock();
}
