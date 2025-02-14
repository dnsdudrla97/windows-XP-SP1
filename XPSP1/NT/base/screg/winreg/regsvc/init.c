/*++

Copyright (c) 1992 Microsoft Corporation

Module Name:

    Init.c

Abstract:

    This module contains the initialization routine for the Win32 Registry
    API RPC server.

Author:

    David J. Gilman (davegi) 15-May-1992

--*/

#include <ntrpcp.h>
#include <rpc.h>
#include <svcs.h>
#include "regrpc.h"

BOOL InitializeRemoteSecurity( );

BOOL InitializeRegCreateKey( );

extern PSVCHOST_GLOBAL_DATA    g_svcsGlobalData;

BOOL
StartWinregRPCServer( 
                     )
{
    LPWSTR              ServiceName;
    NTSTATUS            Status;

    if( !g_svcsGlobalData ) {
        return FALSE;
    }
    ServiceName = INTERFACE_NAME;
    Status = g_svcsGlobalData->StartRpcServer(
                ServiceName,
                winreg_ServerIfHandle
                );
    ASSERT( NT_SUCCESS( Status ));
    if( ! NT_SUCCESS( Status )) {
        return FALSE;
    }
    
    return TRUE;
}

BOOL
InitializeWinreg(
    )

/*++

Routine Description:

    Initialize the Winreg RPC server by creating the notify thread,
    starting the server and creating the external synchronization event.

Arguments:

    None.

Return Value:

    BOOL - Returns TRUE if initialization is succesful.

--*/
{
    BOOL                Success;
    HANDLE              PublicEvent;

    //
    // Create the notify thread.
    //

    Success = InitializeRegNotifyChangeKeyValue( );
    ASSERT( Success == TRUE );
    if( Success == FALSE ) {
        return FALSE;
    }

    //
    // Initialize BaseRegCreateKey
    //

    Success = InitializeRegCreateKey( );
    ASSERT( Success == TRUE );
    if( Success == FALSE ) {
        return FALSE;
    }

    //
    // Initialize support for remote security
    //

    Success = InitializeRemoteSecurity( );
    if ( Success == FALSE )
    {
        return( FALSE );
    }

    //
    // Start the Winreg RPC server.
    //
    Success = StartWinregRPCServer( );
    if ( Success == FALSE )
    {
        return( FALSE );
    }

    //
    //  Let the world know that the server is running.
    //
    PublicEvent = CreateEvent( NULL, TRUE, TRUE, PUBLIC_EVENT );
    ASSERT( PublicEvent );
    if( !PublicEvent  ) {
        return FALSE;
    }

    //
    // Success!
    //

    return TRUE;
}

BOOL
ShutdownWinreg(
    )

/*++

Routine Description:

    Stops the Winreg RPC server.

Arguments:

    None.

Return Value:

    None

--*/
{
    // 
    // Stop the rpc server
    //
    if( !g_svcsGlobalData ) {
        return FALSE;
    }
    g_svcsGlobalData->StopRpcServer( winreg_ServerIfHandle );

    return TRUE;
}
