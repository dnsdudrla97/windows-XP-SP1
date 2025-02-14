//  Copyright (C) 1995-1999 Microsoft Corporation.  All rights reserved.
//
// *** NOTICE ***
//
// New CLSIDs added to this file MUST be within the range reserved for use
// here. The reserved range for each file is shown below:
//
// iid.c           from - ecabaeb0-7f19-11d2-978e-0000f8757e2a
//                 thru - ecabafa9-7f19-11d2-978e-0000f8757e2a
//
// ccprivat.idl    from - ecabafaa-7f19-11d2-978e-0000f8757e2a
//                 thru - ecabb0a3-7f19-11d2-978e-0000f8757e2a
//
// ccpublic.idl    from - ecabb0a4-7f19-11d2-978e-0000f8757e2a
//                 thru - ecabb19d-7f19-11d2-978e-0000f8757e2a
//
// unassigned      from - ecabb19e-7f19-11d2-978e-0000f8757e2a
//                 thru - ecabb297-7f19-11d2-978e-0000f8757e2a
//
//

#include "comsvcs_i.c"
//#include "mtx_i.c"
#include "mtxas_i.c"
#include "secctx_i.c"
#include "svcintfs_i.c"
#include "trust_i.c"
#include "txctx_i.c"
#include "byot_i.c"
#include "volpmidl_i.c"
#include "casperpriv_i.c"
#include "eventcpts_i.c"
#include "mtsevents_i.c"
#include "comadmin_i.c"
#include "vsevfire_i.c"
#include "EventSys_i.c"
#include "EventSys2_i.c"
#include "comvbdbg_i.c"
#include "QCPublic_i.c"
#include "QCPrivat_i.c"
#include <comsvcsevents_i.c>
#include "enteract_i.c"
#include "svcevents_i.c"

//
//	define any guids not put in IDL files here
//
const CLSID guidTransactionProperty			= {0xecabaeb1, 0x7f19, 0x11d2, {0x97, 0x8e, 0x00, 0x00, 0xf8, 0x75, 0x7e, 0x2a}};
const CLSID guidJITActivationPolicy			= {0xecabaeb2, 0x7f19, 0x11d2, {0x97, 0x8e, 0x00, 0x00, 0xf8, 0x75, 0x7e, 0x2a}};
const CLSID guidTrkPropPolicy				= {0xecabaeb3, 0x7f19, 0x11d2, {0x97, 0x8e, 0x00, 0x00, 0xf8, 0x75, 0x7e, 0x2a}};
const CLSID guidActivityPolicy				= {0xecabaeb4, 0x7f19, 0x11d2, {0x97, 0x8e, 0x00, 0x00, 0xf8, 0x75, 0x7e, 0x2a}};
const CLSID guidContextMarshaler			= {0xecabaeb5, 0x7f19, 0x11d2, {0x97, 0x8e, 0x00, 0x00, 0xf8, 0x75, 0x7e, 0x2a}};    
const CLSID guidUserPropertiesProperty		= {0xecabaeb6, 0x7f19, 0x11d2, {0x97, 0x8e, 0x00, 0x00, 0xf8, 0x75, 0x7e, 0x2a}};    
const CLSID guidApplicationInfo				= {0xecabaeb7, 0x7f19, 0x11d2, {0x97, 0x8e, 0x00, 0x00, 0xf8, 0x75, 0x7e, 0x2a}};    
const CLSID guidSecurityPolicy				= {0xecabaeb8, 0x7f19, 0x11d2, {0x97, 0x8e, 0x00, 0x00, 0xf8, 0x75, 0x7e, 0x2a}};    
const CLSID guidSPMPolicy					= {0xecabaeb9, 0x7f19, 0x11d2, {0x97, 0x8e, 0x00, 0x00, 0xf8, 0x75, 0x7e, 0x2a}};
const CLSID guidPartitionProperty			= {0xecabaeba, 0x7f19, 0x11d2, {0x97, 0x8e, 0x00, 0x00, 0xf8, 0x75, 0x7e, 0x2a}};

// Moved from itm/itmuuid/itmuuid.c
const CLSID CLSID_TrustManager				= {0xecabaebb, 0x7f19, 0x11d2, {0x97, 0x8e, 0x00, 0x00, 0xf8, 0x75, 0x7e, 0x2a}};
const CLSID CLSID_StdPolicyManager			= {0xecabaebc, 0x7f19, 0x11d2, {0x97, 0x8e, 0x00, 0x00, 0xf8, 0x75, 0x7e, 0x2a}};
const CLSID CLSID_StdPolicyPackage			= {0xecabaebd, 0x7f19, 0x11d2, {0x97, 0x8e, 0x00, 0x00, 0xf8, 0x75, 0x7e, 0x2a}};
const CLSID CLSID_ScriptPolicyMaker			= {0xecabaebe, 0x7f19, 0x11d2, {0x97, 0x8e, 0x00, 0x00, 0xf8, 0x75, 0x7e, 0x2a}};
const CLSID CLSID_VolatilePolicyManager		= {0xecabaebf, 0x7f19, 0x11d2, {0x97, 0x8e, 0x00, 0x00, 0xf8, 0x75, 0x7e, 0x2a}};
const CLSID CLSID_TrustGlobals				= {0xecabaec0, 0x7f19, 0x11d2, {0x97, 0x8e, 0x00, 0x00, 0xf8, 0x75, 0x7e, 0x2a}};

// This GUID is present in a Com+ Setup .infsrc file (in string form), as well as the QC Listener and QC QueueAdmin
// This is the Application ID of the QC Dead Letter Queue Listener Application

const GUID  guidQCDeadLetterQueueListenerAppId 
											= {0x7b4e1f3c, 0xa702, 0x11d2, {0xa3, 0x36, 0x00, 0xc0, 0x4f, 0x79, 0x78, 0xe0}};

// This is default application partition GUID. The default
// application partition is only identifiable through this unique
// identifier, therefore once COM+ 1.X ships, this ID is forever.

const CLSID GUID_DefaultAppPartition = 
{ 0x41e90f3e, 0x56c1, 0x4633, { 0x81, 0xc3, 0x6e, 0x8b, 0xac, 0x8b, 0xdd, 0x70 } };
