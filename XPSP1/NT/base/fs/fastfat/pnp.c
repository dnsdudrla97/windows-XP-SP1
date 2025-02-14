/*++

Copyright (c) 1997-2000 Microsoft Corporation

Module Name:

    Pnp.c

Abstract:

    This module implements the Plug and Play routines for FAT called by
    the dispatch driver.

// @@BEGIN_DDKSPLIT

Author:

    Dan Lovinger    [DanLo]     23-Jul-1997

Revision History:

// @@END_DDKSPLIT

--*/

#include "FatProcs.h"

//
//  The Bug check file id for this module
//

#define BugCheckFileId                   (FAT_BUG_CHECK_PNP)

NTSTATUS
FatPnpQueryRemove (
    PIRP_CONTEXT IrpContext,
    PIRP Irp,
    PVCB Vcb
    );

NTSTATUS
FatPnpRemove (
    PIRP_CONTEXT IrpContext,
    PIRP Irp,
    PVCB Vcb
    );

NTSTATUS
FatPnpSurpriseRemove (
    PIRP_CONTEXT IrpContext,
    PIRP Irp,
    PVCB Vcb
    );

NTSTATUS
FatPnpCancelRemove (
    PIRP_CONTEXT IrpContext,
    PIRP Irp,
    PVCB Vcb
    );

NTSTATUS
FatPnpCompletionRoutine (
    IN PDEVICE_OBJECT DeviceObject,
    IN PIRP Irp,
    IN PVOID Contxt
    );

#ifdef ALLOC_PRAGMA
#pragma alloc_text(PAGE, FatCommonPnp)
#pragma alloc_text(PAGE, FatFsdPnp)
#pragma alloc_text(PAGE, FatPnpCancelRemove)
#pragma alloc_text(PAGE, FatPnpQueryRemove)
#pragma alloc_text(PAGE, FatPnpRemove)
#pragma alloc_text(PAGE, FatPnpSurpriseRemove)
#endif


NTSTATUS
FatFsdPnp (
    IN PVOLUME_DEVICE_OBJECT VolumeDeviceObject,
    IN PIRP Irp
    )

/*++

Routine Description:

    This routine implements the FSD part of PnP operations

Arguments:

    VolumeDeviceObject - Supplies the volume device object where the
        file exists

    Irp - Supplies the Irp being processed

Return Value:

    NTSTATUS - The FSD status for the IRP

--*/

{
    NTSTATUS Status;
    PIRP_CONTEXT IrpContext = NULL;

    BOOLEAN TopLevel;
    BOOLEAN Wait;

    DebugTrace(+1, Dbg, "FatFsdPnp\n", 0);

    FsRtlEnterFileSystem();

    TopLevel = FatIsIrpTopLevel( Irp );

    try {

        //
        //  We expect there to never be a fileobject, in which case we will always
        //  wait.  Since at the moment we don't have any concept of pending Pnp
        //  operations, this is a bit nitpicky.
        //
        
        if (IoGetCurrentIrpStackLocation( Irp )->FileObject == NULL) {

            Wait = TRUE;

        } else {

            Wait = CanFsdWait( Irp );
        }

        IrpContext = FatCreateIrpContext( Irp, Wait );

        Status = FatCommonPnp( IrpContext, Irp );

    } except(FatExceptionFilter( IrpContext, GetExceptionInformation() )) {

        //
        //  We had some trouble trying to perform the requested
        //  operation, so we'll abort the I/O request with
        //  the error status that we get back from the
        //  execption code
        //

        Status = FatProcessException( IrpContext, Irp, GetExceptionCode() );
    }

    if (TopLevel) { IoSetTopLevelIrp( NULL ); }

    FsRtlExitFileSystem();

    //
    //  And return to our caller
    //

    DebugTrace(-1, Dbg, "FatFsdPnp -> %08lx\n", Status);

    UNREFERENCED_PARAMETER( VolumeDeviceObject );

    return Status;
}


NTSTATUS
FatCommonPnp (
    IN PIRP_CONTEXT IrpContext,
    IN PIRP Irp
    )

/*++

Routine Description:

    This is the common routine for doing PnP operations called
    by both the fsd and fsp threads

Arguments:

    Irp - Supplies the Irp to process

Return Value:

    NTSTATUS - The return status for the operation

--*/

{
    NTSTATUS Status;
    
    PIO_STACK_LOCATION IrpSp;

    PVOLUME_DEVICE_OBJECT OurDeviceObject;
    PVCB Vcb;

    //
    //  Get the current Irp stack location.
    //

    IrpSp = IoGetCurrentIrpStackLocation( Irp );

    //
    //  Find our Vcb.  This is tricky since we have no file object in the Irp.
    //

    OurDeviceObject = (PVOLUME_DEVICE_OBJECT) IrpSp->DeviceObject;

    //
    //  Make sure this device object really is big enough to be a volume device
    //  object.  If it isn't, we need to get out before we try to reference some
    //  field that takes us past the end of an ordinary device object.
    //
    
    if (OurDeviceObject->DeviceObject.Size != sizeof(VOLUME_DEVICE_OBJECT) ||
        NodeType( &OurDeviceObject->Vcb ) != FAT_NTC_VCB) {
        
        //
        //  We were called with something we don't understand.
        //
        
        Status = STATUS_INVALID_PARAMETER;
        FatCompleteRequest( IrpContext, Irp, Status );
        return Status;
    }

    //
    //  Force everything to wait.
    //

    SetFlag(IrpContext->Flags, IRP_CONTEXT_FLAG_WAIT);
    
    Vcb = &OurDeviceObject->Vcb;

    //
    //  Case on the minor code.
    //
    
    switch ( IrpSp->MinorFunction ) {

        case IRP_MN_QUERY_REMOVE_DEVICE:
            
            Status = FatPnpQueryRemove( IrpContext, Irp, Vcb );
            break;
        
        case IRP_MN_SURPRISE_REMOVAL:
        
            Status = FatPnpSurpriseRemove( IrpContext, Irp, Vcb );
            break;

        case IRP_MN_REMOVE_DEVICE:

            Status = FatPnpRemove( IrpContext, Irp, Vcb );
            break;

        case IRP_MN_CANCEL_REMOVE_DEVICE:
    
            Status = FatPnpCancelRemove( IrpContext, Irp, Vcb );
            break;

        default:
    
            //
            //  Just pass the IRP on.  As we do not need to be in the
            //  way on return, ellide ourselves out of the stack.
            //
            
            IoSkipCurrentIrpStackLocation( Irp );
    
            Status = IoCallDriver(Vcb->TargetDeviceObject, Irp);
            
            //
            //  Cleanup our Irp Context.  The driver has completed the Irp.
            //
        
            FatCompleteRequest( IrpContext, NULL, STATUS_SUCCESS );
            
            break;
    }
        
    return Status;
}


NTSTATUS
FatPnpQueryRemove (
    PIRP_CONTEXT IrpContext,
    PIRP Irp,
    PVCB Vcb
    )

/*++

Routine Description:

    This routine handles the PnP query remove operation.  The filesystem
    is responsible for answering whether there are any reasons it sees
    that the volume can not go away (and the device removed).  Initiation
    of the dismount begins when we answer yes to this question.
    
    Query will be followed by a Cancel or Remove.

Arguments:

    Irp - Supplies the Irp to process
    
    Vcb - Supplies the volume being queried.

Return Value:

    NTSTATUS - The return status for the operation

--*/

{
    NTSTATUS Status;
    KEVENT Event;
    BOOLEAN VcbDeleted = FALSE;
    BOOLEAN GlobalHeld = FALSE;

    //
    //  Having said yes to a QUERY, any communication with the
    //  underlying storage stack is undefined (and may block)
    //  until the bounding CANCEL or REMOVE is sent.
    //

    //
    //  Acquire the global resource so that we can try to vaporize
    //  the volume, and the vcb resource itself.
    //
    
    FatAcquireExclusiveVcb( IrpContext, Vcb );

    try {
        
        Status = FatLockVolumeInternal( IrpContext, Vcb, NULL );

        //
        //  Reacquire the resources in the right order.
        //

        FatReleaseVcb( IrpContext, Vcb );
        FatAcquireExclusiveGlobal( IrpContext );
        GlobalHeld = TRUE;
        FatAcquireExclusiveVcb( IrpContext, Vcb );

        if (NT_SUCCESS( Status )) {

            //
            //  With the volume held locked, note that we must finalize as much
            //  as possible right now.
            //

            FatFlushAndCleanVolume( IrpContext, Irp, Vcb, Flush );

            //
            //  We need to pass this down before starting the dismount, which
            //  could disconnect us immediately from the stack.
            //

            //
            //  Get the next stack location, and copy over the stack location
            //

            IoCopyCurrentIrpStackLocationToNext( Irp );

            //
            //  Set up the completion routine
            //

            KeInitializeEvent( &Event, NotificationEvent, FALSE );
            IoSetCompletionRoutine( Irp,
                                    FatPnpCompletionRoutine,
                                    &Event,
                                    TRUE,
                                    TRUE,
                                    TRUE );

            //
            //  Send the request and wait.
            //

            Status = IoCallDriver(Vcb->TargetDeviceObject, Irp);

            if (Status == STATUS_PENDING) {

                KeWaitForSingleObject( &Event,
                                       Executive,
                                       KernelMode,
                                       FALSE,
                                       NULL );

                Status = Irp->IoStatus.Status;
            }

            //
            //  Now if no one below us failed already, initiate the dismount
            //  on this volume, make it go away.  PnP needs to see our internal
            //  streams close and drop their references to the target device.
            //
            //  Since we were able to lock the volume, we are guaranteed to
            //  move this volume into dismount state and disconnect it from
            //  the underlying storage stack.  The force on our part is actually
            //  unnecesary, though complete.
            //
            //  What is not strictly guaranteed, though, is that the closes
            //  for the metadata streams take effect synchronously underneath
            //  of this call.  This would leave references on the target device
            //  even though we are disconnected!
            //

            if (NT_SUCCESS( Status )) {

                VcbDeleted = FatCheckForDismount( IrpContext, Vcb, TRUE );

                ASSERT( VcbDeleted || Vcb->VcbCondition == VcbBad );

            }
        }

        //
        //  Release the Vcb if it could still remain.
        //
        //  Note: if everything else succeeded and the Vcb is persistent because the
        //  internal streams did not vaporize, we really need to pend this IRP off on
        //  the side until the dismount is completed.  I can't think of a reasonable
        //  case (in FAT) where this would actually happen, though it might still need
        //  to be implemented.
        //
        //  The reason this is the case is that handles/fileobjects place a reference
        //  on the device objects they overly.  In the filesystem case, these references
        //  are on our target devices.  PnP correcly thinks that if references remain
        //  on the device objects in the stack that someone has a handle, and that this
        //  counts as a reason to not succeed the query - even though every interrogated
        //  driver thinks that it is OK.
        //

        ASSERT( !(NT_SUCCESS( Status ) && !VcbDeleted ));

    } finally {
        
        if (!VcbDeleted) {

            FatReleaseVcb( IrpContext, Vcb );
        }

        if (GlobalHeld) {
            
            FatReleaseGlobal( IrpContext );
        }
    }
    
    //
    //  Cleanup our IrpContext and complete the IRP if neccesary.
    //

    FatCompleteRequest( IrpContext, Irp, Status );

    return Status;
}


NTSTATUS
FatPnpRemove (
    PIRP_CONTEXT IrpContext,
    PIRP Irp,
    PVCB Vcb
    )

/*++

Routine Description:

    This routine handles the PnP remove operation.  This is our notification
    that the underlying storage device for the volume we have is gone, and
    an excellent indication that the volume will never reappear. The filesystem
    is responsible for initiation or completion of the dismount.
    
Arguments:

    Irp - Supplies the Irp to process
    
    Vcb - Supplies the volume being removed.

Return Value:

    NTSTATUS - The return status for the operation

--*/

{
    NTSTATUS Status;
    KEVENT Event;
    BOOLEAN VcbDeleted;
    
    //
    //  REMOVE - a storage device is now gone.  We either got
    //  QUERY'd and said yes OR got a SURPRISE OR a storage
    //  stack failed to spin back up from a sleep/stop state
    //  (the only case in which this will be the first warning).
    //
    //  Note that it is entirely unlikely that we will be around
    //  for a REMOVE in the first two cases, as we try to intiate
    //  dismount.
    //
    
    //
    //  Acquire the global resource so that we can try to vaporize
    //  the volume, and the vcb resource itself.
    //

    FatAcquireExclusiveGlobal( IrpContext );
    FatAcquireExclusiveVcb( IrpContext, Vcb );

    //
    //  The device will be going away.  Remove our lock (benign
    //  if we never had it).
    //

    (VOID) FatUnlockVolumeInternal( IrpContext, Vcb, NULL );
    
    //
    //  We need to pass this down before starting the dismount, which
    //  could disconnect us immediately from the stack.
    //
    
    //
    //  Get the next stack location, and copy over the stack location
    //

    IoCopyCurrentIrpStackLocationToNext( Irp );

    //
    //  Set up the completion routine
    //

    KeInitializeEvent( &Event, NotificationEvent, FALSE );
    IoSetCompletionRoutine( Irp,
                            FatPnpCompletionRoutine,
                            &Event,
                            TRUE,
                            TRUE,
                            TRUE );

    //
    //  Send the request and wait.
    //

    Status = IoCallDriver(Vcb->TargetDeviceObject, Irp);

    if (Status == STATUS_PENDING) {

        KeWaitForSingleObject( &Event,
                               Executive,
                               KernelMode,
                               FALSE,
                               NULL );

        Status = Irp->IoStatus.Status;
    }

    try {
        
        //
        //  Knock as many files down for this volume as we can.
        //

        FatFlushAndCleanVolume( IrpContext, Irp, Vcb, NoFlush );

        //
        //  Now make our dismount happen.  This may not vaporize the
        //  Vcb, of course, since there could be any number of handles
        //  outstanding if we were not preceeded by a QUERY.
        //
        //  PnP will take care of disconnecting this stack if we
        //  couldn't get off of it immediately.
        //

        VcbDeleted = FatCheckForDismount( IrpContext, Vcb, TRUE );

    } finally {
        
        //
        //  Release the Vcb if it could still remain.
        //

        if (!VcbDeleted) {

            FatReleaseVcb( IrpContext, Vcb );
        }

        FatReleaseGlobal( IrpContext );
    }

    //
    //  Cleanup our IrpContext and complete the IRP.
    //

    FatCompleteRequest( IrpContext, Irp, Status );

    return Status;
}


NTSTATUS
FatPnpSurpriseRemove (
    PIRP_CONTEXT IrpContext,
    PIRP Irp,
    PVCB Vcb
    )

/*++

Routine Description:

    This routine handles the PnP surprise remove operation.  This is another
    type of notification that the underlying storage device for the volume we
    have is gone, and is excellent indication that the volume will never reappear.
    The filesystem is responsible for initiation or completion the dismount.
    
    For the most part, only "real" drivers care about the distinction of a
    surprise remove, which is a result of our noticing that a user (usually)
    physically reached into the machine and pulled something out.
    
    Surprise will be followed by a Remove when all references have been shut down.

Arguments:

    Irp - Supplies the Irp to process
    
    Vcb - Supplies the volume being removed.

Return Value:

    NTSTATUS - The return status for the operation

--*/

{
    NTSTATUS Status;
    KEVENT Event;
    BOOLEAN VcbDeleted;
    
    //
    //  SURPRISE - a device was physically yanked away without
    //  any warning.  This means external forces.
    //
    
    FatAcquireExclusiveGlobal( IrpContext );
    FatAcquireExclusiveVcb( IrpContext, Vcb );
        
    //
    //  We need to pass this down before starting the dismount, which
    //  could disconnect us immediately from the stack.
    //
    
    //
    //  Get the next stack location, and copy over the stack location
    //

    IoCopyCurrentIrpStackLocationToNext( Irp );

    //
    //  Set up the completion routine
    //

    KeInitializeEvent( &Event, NotificationEvent, FALSE );
    IoSetCompletionRoutine( Irp,
                            FatPnpCompletionRoutine,
                            &Event,
                            TRUE,
                            TRUE,
                            TRUE );

    //
    //  Send the request and wait.
    //

    Status = IoCallDriver(Vcb->TargetDeviceObject, Irp);

    if (Status == STATUS_PENDING) {

        KeWaitForSingleObject( &Event,
                               Executive,
                               KernelMode,
                               FALSE,
                               NULL );

        Status = Irp->IoStatus.Status;
    }
    
    try {
        
        //
        //  Knock as many files down for this volume as we can.
        //

        FatFlushAndCleanVolume( IrpContext, Irp, Vcb, NoFlush );

        //
        //  Now make our dismount happen.  This may not vaporize the
        //  Vcb, of course, since there could be any number of handles
        //  outstanding since this is an out of band notification.
        //

        VcbDeleted = FatCheckForDismount( IrpContext, Vcb, TRUE );

    } finally {
        
        //
        //  Release the Vcb if it could still remain.
        //

        if (!VcbDeleted) {

            FatReleaseVcb( IrpContext, Vcb );
        }

        FatReleaseGlobal( IrpContext );
    }
    
    //
    //  Cleanup our IrpContext and complete the IRP.
    //

    FatCompleteRequest( IrpContext, Irp, Status );

    return Status;
}


NTSTATUS
FatPnpCancelRemove (
    PIRP_CONTEXT IrpContext,
    PIRP Irp,
    PVCB Vcb
    )

/*++

Routine Description:

    This routine handles the PnP cancel remove operation.  This is our
    notification that a previously proposed remove (query) was eventually
    vetoed by a component.  The filesystem is responsible for cleaning up
    and getting ready for more IO.
    
Arguments:

    Irp - Supplies the Irp to process
    
    Vcb - Supplies the volume being removed.

Return Value:

    NTSTATUS - The return status for the operation

--*/

{
    NTSTATUS Status;

    //
    //  CANCEL - a previous QUERY has been rescinded as a result
    //  of someone vetoing.  Since PnP cannot figure out who may
    //  have gotten the QUERY (think about it: stacked drivers),
    //  we must expect to deal with getting a CANCEL without having
    //  seen the QUERY.
    //
    //  For FAT, this is quite easy.  In fact, we can't get a
    //  CANCEL if the underlying drivers succeeded the QUERY since
    //  we disconnect the Vpb on our dismount initiation.  This is
    //  actually pretty important because if PnP could get to us
    //  after the disconnect we'd be thoroughly unsynchronized
    //  with respect to the Vcb getting torn apart - merely referencing
    //  the volume device object is insufficient to keep us intact.
    //
    
    FatAcquireExclusiveVcb( IrpContext, Vcb );
    
    //
    //  Unlock the volume.  This is benign if we never had seen
    //  a QUERY.
    //

    Status = FatUnlockVolumeInternal( IrpContext, Vcb, NULL );

    try {
        
        //
        //  We must re-enable allocation support if we got through
        //  the first stages of a QUERY_REMOVE; i.e., we decided we
        //  could place a lock on the volume.
        //

        if (NT_SUCCESS( Status )) {

            FatSetupAllocationSupport( IrpContext, Vcb );
        }
    
    } finally {
        
        FatReleaseVcb( IrpContext, Vcb );
    }

    //
    //  Send the request.  The underlying driver will complete the
    //  IRP.  Since we don't need to be in the way, simply ellide
    //  ourselves out of the IRP stack.
    //

    IoSkipCurrentIrpStackLocation( Irp );

    Status = IoCallDriver(Vcb->TargetDeviceObject, Irp);

    FatCompleteRequest( IrpContext, NULL, STATUS_SUCCESS );

    return Status;
}


//
//  Local support routine
//

NTSTATUS
FatPnpCompletionRoutine (
    IN PDEVICE_OBJECT DeviceObject,
    IN PIRP Irp,
    IN PVOID Contxt
    )
{
    PKEVENT Event = (PKEVENT) Contxt;

    KeSetEvent( Event, 0, FALSE );

    return STATUS_MORE_PROCESSING_REQUIRED;

    UNREFERENCED_PARAMETER( DeviceObject );
    UNREFERENCED_PARAMETER( Contxt );
}

