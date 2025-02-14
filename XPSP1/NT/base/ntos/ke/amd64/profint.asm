        title  "Profile Interrupt"
;++
;
; Copyright (c) 2000  Microsoft Corporation
;
; Module Name:
;
;   profint.asm
;
; Abstract:
;
;   This module implements the architecture dependent code necessary to
;   process the profile interrupt.
;
; Author:
;
;   David N. Cutler (davec) 12-Sep-2000
;
; Environment:
;
;   Kernel mode only.
;
;--

include ksamd64.inc

        extern  KiProfileListHead:qword
        extern  KiProfileLock:qword

        subttl  "Profile Interrupt"
;++
;
; VOID
; KeProfileInterruptWithSource (
;     IN KPROFILE_SOURCE ProfileSource
;     )
;
; Routine Description:
;
;   This routine is executed is response to an interrupt generated by one
;   of the profile sources. Its function is to process the system and process
;   profile lists and update bucket hit counters.
;
; Arguments:
;
;   TrapFrame (rcx) - Supplies the address of a trap frame.
;
;   ProfileSource (rdx) - Supplies the source of profile interrupt.
;
; Return Value:
;
;    None.
;
;--

PiFrame struct
        Source  dq ?                    ; profile interrupt source
        Fill    dq ?                    ; fill to 8 mod 16
        SavedRbp dq ?                   ; saved register RBP
PiFrame ends

        NESTED_ENTRY KeProfileInterruptWithSource, _TEXT$00

        push_reg rbp                    ; save nonvolatile register
        alloc_stack (sizeof PiFrame - (1 * 8)) ; allocate stack frame

        END_PROLOGUE

        mov     PiFrame.Source[rsp], rdx ; save interrupt source
        lea     r11, KiProfileLock      ; get address of spin lock
        AcquireSpinLock r11             ; acquire profile spin lock
        mov     rcx, PiFrame.Source[rsp] ; set interrupt source
        mov     rdx, gs:[PcCurrentThread] ; get current thread address
        mov     rdx, ThApcState + AsProcess[rdx] ; get current process address
        add     rdx, PrProfileListHead  ; compute profile listhead address
        call    KiProcessProfileList    ; process profile list
        mov     rcx, PiFrame.Source[rsp] ; set interrupt source
        lea     rdx, KiProfileListHead  ; get system profile listhead address
        call    KiProcessProfileList    ; process profile list
        lea     rcx, KiProfileLock      ; get address of spin lock
        ReleaseSpinLock r11             ; release spin lock
        add     rsp, sizeof PiFrame - (1 * 8) ; deallocate stack frame
        pop     rbp                     ; restore nonvolatile register
        ret                             ; return

        NESTED_END KeProfileInterruptWithSource, _TEXT$00

        subttl  "Process Profile List"
;++
;
; VOID
; KiProcessProfileList (
;     IN KPROFILE_SOURCE Source,
;     IN PLIST_ENTRY ListHead
;     )
;
; Routine Description:
;
;   This routine processes a profile list.
;
; Arguments:
;
;   Source (cx) - Supplies the source of profile interrupt.
;
;   ListHead (rdx) - Supplies a pointer to a profile list.
;
; Implicit Arguments:
;
;   rbp - Supplies a pointer to a trap frame.
;
; Return Value:
;
;    None.
;
;--

        LEAF_ENTRY KiProcessProfileList, _TEXT$00

        mov     r8, LsFlink[rdx]        ; get first entry address
        cmp     r8, rdx                 ; check if list is empty
        je      short KiPP30            ; if e, list is empty
        mov     r9, gs:[PcSetMember]    ; get procecessor set member
        mov     r10, TrRip[rbp]         ; get profile interrupt address
        mov     ax, cx                  ; save profile source

;
; Process list entry.
;

KiPP10: cmp     ax, (PfSource - PfProfileListEntry)[r8] ; check for source match
        jne     short KiPP20            ; if ne, source mismatch
        cmp     r10, (PfRangeBase - PfProfileListEntry)[r8] ; check if below base
        jb      short KiPP20            ; if b, address below base
        cmp     r10, (PfRangeLimit - PfProfileListEntry)[r8] ; check if above limit
        jae     short KiPP20            ; if ae, address above limit
        test    r9, (PfAffinity - PfProfileListEntry)[r8] ; check if in set
        jz      short KiPP20            ; if z, processor not in set
        mov     cl, (PfBucketShift - PfProfileListEntry)[r8] ; get shift count
        mov     r11, r10                ; compute offset into profile buffer
        sub     r11, (PfRangeBase - PfProfileListEntry)[r8] ;
        shr     r11, cl                 ;
        and     r11, NOT 3              ;
        mov     rcx, (PfBuffer - PfProfileListEntry)[r8] ; get profile buffer address
        inc     dword ptr [r11][rcx]    ; increment profile bucket
KiPP20: mov     r8, LsFlink[r8]         ; get next entry address
        cmp     r8, rdx                 ; check if end of list
        jne     short KiPP10            ; if ne, not end of list
KiPP30: ret                             ; return

        LEAF_END KiProcessProfileList, _TEXT$00

        end
