/*++

Copyright (c) 1996-1999  Microsoft Corporation

Module Name:

    pstub.c

Abstract:

    Stubs for protocol routines

Author:

    Charlie Wickham (charlwi)  22-Apr-1996
    Rajesh Sundaram (rajeshsu) 01-Aug-1998.

Environment:

    Kernel Mode

Revision History:

--*/

#include "psched.h"

#pragma hdrstop

/* External */

/* Static */

/* Forward */ /* Generated by Emacs 19.17.0 on Wed May 08 10:48:06 1996 */

VOID
ClResetComplete(
    IN  NDIS_HANDLE ProtocolBindingContext,
    IN  NDIS_STATUS Status
    );

/* End Forward */

NDIS_STATUS
ClPnPEventHandler(
    IN NDIS_HANDLE Handle,
    IN PNET_PNP_EVENT NetPnPEvent
    )
{
    PADAPTER                 Adapter     = (PADAPTER) Handle;
    PNDIS_DEVICE_POWER_STATE DeviceState = (PNDIS_DEVICE_POWER_STATE) (NetPnPEvent->Buffer);
    NDIS_DEVICE_POWER_STATE  PrevDeviceState;
    NDIS_STATUS              PnPStatus, Status;
    ULONG                    Count;

    PnPStatus = NDIS_STATUS_SUCCESS;
    
    switch(NetPnPEvent->NetEvent)
    {
      case NetEventSetPower:

        PsStructAssert(Adapter);
        PrevDeviceState        = Adapter->PTDeviceState;
        Adapter->PTDeviceState = *DeviceState;
        
        //
        // if we are being sent to standby, block outstanding requests and sends
        //

        if(*DeviceState > NdisDeviceStateD0)
        {
            //
            // For state transistion to > D0, we have to pass the request up before doing anything.
            //

            if(Adapter->PsNdisHandle)
            {
                PnPStatus = NdisIMNotifyPnPEvent(Adapter->PsNdisHandle, NetPnPEvent);
            }

            //
            // If the physical miniport is going to standby, fail all incoming requests.
            //

            if(PrevDeviceState == NdisDeviceStateD0)
            {
                Adapter->StandingBy = TRUE;
            }

            // 
            // Wait till Oustanding IO on the binding completes. 
            //
            do
            {
                Status = NdisQueryPendingIOCount(Adapter->LowerMpHandle, &Count);

                if(Status == NDIS_STATUS_SUCCESS && Count != 0)
                {
                    NdisMSleep(10);
                }
                else 
                {
                    break;
                }

            } while(TRUE);

            PsAssert(Adapter->OutstandingNdisRequests == 0);
        }
        else 
        {
            //
            // If the physical miniport is powering up (from low power state to D0), clear the flag.
            //

            if(PrevDeviceState > NdisDeviceStateD0)
            {
                Adapter->StandingBy = FALSE;
            }

            // This is where we originate the pended ndis requests.
            if(Adapter->PendedNdisRequest)
            {
                PNDIS_REQUEST PendedRequest = (PNDIS_REQUEST)Adapter->PendedNdisRequest;
                Adapter->PendedNdisRequest = NULL;

                NdisRequest(&Status, Adapter->LowerMpHandle, PendedRequest);
                if(NDIS_STATUS_PENDING != Status)
                {
                    ClRequestComplete(Adapter, PendedRequest, Status);
                }
            }
    
            // Update link speed.
            if(IsDeviceStateOn(Adapter))
            {
                PsGetLinkSpeed(Adapter);
            }

            //
            // For state transistion to D0, we have to pass the request after doing our work.
            //

            if(Adapter->PsNdisHandle) 
            {
                PnPStatus = NdisIMNotifyPnPEvent(Adapter->PsNdisHandle, NetPnPEvent);
            }

        }

        break;

      case NetEventReconfigure:

        if(Adapter == NULL)
        {
            NdisReEnumerateProtocolBindings(ClientProtocolHandle);
        }
        else 
        {
            //
            // For all other events, pass the request before doing anything.
            //
            if(Adapter->PsNdisHandle)
            {
                PnPStatus = NdisIMNotifyPnPEvent(Adapter->PsNdisHandle, NetPnPEvent);
            }
        }

        break;

      default:    

        //
        // For all other events, pass the request before doing anything.
        //

        if(Adapter && Adapter->PsNdisHandle) 
        {
            PnPStatus = NdisIMNotifyPnPEvent(Adapter->PsNdisHandle, NetPnPEvent);
        }
        break;
    }

    return(PnPStatus);
}

VOID
ClResetComplete(
    IN  NDIS_HANDLE ProtocolBindingContext,
    IN  NDIS_STATUS Status
    )

/*++

Routine Description:

    Completion routine for NdisReset

Arguments:

    See the DDK...

Return Values:

    None

--*/

{
    PADAPTER Adapter = (PADAPTER)ProtocolBindingContext;

    PsStructAssert( Adapter );

    PsDbgOut(DBG_TRACE, DBG_PROTOCOL, ("(%08X) ClResetComplete: Status = %08x\n",
                                       Adapter, Status));
}

VOID
ClCoStatus(
    IN  NDIS_HANDLE ProtocolBindingContext,
    IN  NDIS_HANDLE ProtocolVcContext OPTIONAL,
    IN  NDIS_STATUS GeneralStatus,
    IN  PVOID StatusBuffer,
    IN  UINT StatusBufferSize
    )
{
    PsDbgOut(DBG_TRACE,
             DBG_PROTOCOL,
             ("(%08X) PcStatusIndication: %08X\n",
              ProtocolBindingContext, GeneralStatus));

    return;
}


VOID
ClCoAfRegisterNotifyHandler(
        IN      NDIS_HANDLE                             ProtocolBindingContext,
        IN      PCO_ADDRESS_FAMILY              AddressFamily
        )
{
    PADAPTER Adapter = (PADAPTER)ProtocolBindingContext;

    PsStructAssert( Adapter );

    if(AddressFamily->AddressFamily == CO_ADDRESS_FAMILY_PPP)
    {
        OpenWanAddressFamily(Adapter, AddressFamily);
    }

}

/* end pstub.c */
