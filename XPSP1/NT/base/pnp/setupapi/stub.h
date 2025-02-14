//
// Prototype and function pointer types
//

typedef BOOL (GETFILEATTRIBUTESEXA_PROTOTYPE)(LPCSTR, GET_FILEEX_INFO_LEVELS, LPVOID);
typedef GETFILEATTRIBUTESEXA_PROTOTYPE * GETFILEATTRIBUTESEXA_PROC;

typedef UINT (GETSYSTEMWINDOWSDIRECTORYA_PROTOTYPE)(LPSTR, UINT);
typedef GETSYSTEMWINDOWSDIRECTORYA_PROTOTYPE * GETSYSTEMWINDOWSDIRECTORYA_PROC;

typedef BOOL (VERIFYVERSIONINFOA_PROTOTYPE)(LPOSVERSIONINFOEXA, DWORD, DWORDLONG);
typedef VERIFYVERSIONINFOA_PROTOTYPE * VERIFYVERSIONINFOA_PROC;

typedef ULONGLONG (VERSETCONDITIONMASK_PROTOTYPE)(ULONGLONG, DWORD, BYTE);
typedef VERSETCONDITIONMASK_PROTOTYPE * VERSETCONDITIONMASK_PROC;

typedef HANDLE (WINAPI SFCONNECTTOSERVER_PROTOTYPE)(LPCWSTR ServerName);
typedef VOID (SFCCLOSE_PROTOTYPE)(IN HANDLE RpcHandle);
typedef DWORD (WINAPI SFCFILEEXCEPTION_PROTOTYPE)(HANDLE RpcHandle,PCWSTR FileName,DWORD ExpectedChangeType);
typedef BOOL (WINAPI SFCISFILEPROTECTED_PROTOTYPE)(HANDLE RpcHandle,LPCWSTR ProtFileName);
typedef SFCONNECTTOSERVER_PROTOTYPE * SFCONNECTTOSERVER_PROC;
typedef SFCCLOSE_PROTOTYPE * SFCCLOSE_PROC;
typedef SFCFILEEXCEPTION_PROTOTYPE * SFCFILEEXCEPTION_PROC;
typedef SFCISFILEPROTECTED_PROTOTYPE * SFCISFILEPROTECTED_PROC;

#ifdef ANSI_SETUPAPI

typedef CONFIGRET (WINAPI *CM_GET_CLASS_REGISTRY_PROPERTYA)(
    IN  LPGUID      ClassGUID,
    IN  ULONG       ulProperty,
    OUT PULONG      pulRegDataType,    OPTIONAL
    OUT PVOID       Buffer,            OPTIONAL
    IN  OUT PULONG  pulLength,
    IN  ULONG       ulFlags,
    IN  HMACHINE    hMachine
    );

typedef CONFIGRET (WINAPI *CM_SET_CLASS_REGISTRY_PROPERTYA)(
    IN LPGUID      ClassGUID,
    IN ULONG       ulProperty,
    IN PCVOID      Buffer,       OPTIONAL
    IN ULONG       ulLength,
    IN ULONG       ulFlags,
    IN HMACHINE    hMachine
    );

typedef CONFIGRET (WINAPI *CM_GET_DEVICE_INTERFACE_ALIAS_EXA)(
            IN     LPCSTR   pszDeviceInterface,
            IN     LPGUID   AliasInterfaceGuid,
            OUT    LPSTR    pszAliasDeviceInterface,
            IN OUT PULONG   pulLength,
            IN     ULONG    ulFlags,
            IN     HMACHINE hMachine
            );

typedef CONFIGRET (WINAPI *CM_GET_DEVICE_INTERFACE_LIST_EXA)(
            IN  LPGUID      InterfaceClassGuid,
            IN  DEVINSTID_A pDeviceID,      OPTIONAL
            OUT PCHAR       Buffer,
            IN  ULONG       BufferLen,
            IN  ULONG       ulFlags,
            IN  HMACHINE    hMachine
            );

typedef CONFIGRET (WINAPI *CM_GET_DEVICE_INTERFACE_LIST_SIZE_EXA)(
            IN  PULONG      pulLen,
            IN  LPGUID      InterfaceClassGuid,
            IN  DEVINSTID_A pDeviceID,      OPTIONAL
            IN  ULONG       ulFlags,
            IN  HMACHINE    hMachine
            );

typedef CONFIGRET (WINAPI *CM_GET_LOG_CONF_PRIORITY_EX)(
            IN  LOG_CONF  lcLogConf,
            OUT PPRIORITY pPriority,
            IN  ULONG     ulFlags,
            IN  HMACHINE  hMachine
            );

typedef CONFIGRET (WINAPI *CM_QUERY_AND_REMOVE_SUBTREE_EXA)(
            IN  DEVINST        dnAncestor,
            OUT PPNP_VETO_TYPE pVetoType,
            OUT LPSTR          pszVetoName,
            IN  ULONG          ulNameLength,
            IN  ULONG          ulFlags,
            IN  HMACHINE       hMachine
            );

typedef CONFIGRET (WINAPI *CM_REGISTER_DEVICE_INTERFACE_EXA)(
            IN  DEVINST   dnDevInst,
            IN  LPGUID    InterfaceClassGuid,
            IN  LPCSTR    pszReference,         OPTIONAL
            OUT LPSTR     pszDeviceInterface,
            IN OUT PULONG pulLength,
            IN  ULONG     ulFlags,
            IN  HMACHINE  hMachine
            );

typedef CONFIGRET (WINAPI *CM_SET_DEVNODE_PROBLEM_EX)(
            IN DEVINST   dnDevInst,
            IN ULONG     ulProblem,
            IN  ULONG    ulFlags,
            IN  HMACHINE hMachine
            );

typedef CONFIGRET (WINAPI *CM_UNREGISTER_DEVICE_INTERFACE_EXA)(
            IN LPCSTR   pszDeviceInterface,
            IN ULONG    ulFlags,
            IN HMACHINE hMachine
            );

typedef BOOL (WINAPI *CRYPTCATADMINACQUIRECONTEXT)(
            OUT HCATADMIN *phCatAdmin,
            IN const GUID *pgSubsystem,
            IN DWORD dwFlags
            );

typedef BOOL (WINAPI *CRYPTCATADMINRELEASECONTEXT)(
            IN HCATADMIN hCatAdmin,
            IN DWORD dwFlags
            );

typedef BOOL (WINAPI *CRYPTCATADMINRELEASECATALOGCONTEXT)(
            IN HCATADMIN hCatAdmin,
            IN HCATINFO hCatInfo,
            IN DWORD dwFlags
            );

typedef HCATINFO (WINAPI *CRYPTCATADMINADDCATALOG)(
            IN HCATADMIN hCatAdmin,
            IN WCHAR *pwszCatalogFile,
            IN OPTIONAL WCHAR *pwszSelectBaseName,
            IN DWORD dwFlags
            );

typedef BOOL (WINAPI *CRYPTCATCATALOGINFOFROMCONTEXT)(
            IN HCATINFO hCatInfo,
            IN OUT CATALOG_INFO *psCatInfo,
            IN DWORD dwFlags
            );

typedef BOOL (WINAPI *CRYPTCATADMINCALCHASHFROMFILEHANDLE)(
            IN HANDLE hFile,
            IN OUT DWORD *pcbHash,
            OUT OPTIONAL BYTE *pbHash,
            IN DWORD dwFlags
            );

typedef HCATINFO (WINAPI *CRYPTCATADMINENUMCATALOGFROMHASH)(
            IN HCATADMIN hCatAdmin,
            IN BYTE *pbHash,
            IN DWORD cbHash,
            IN DWORD dwFlags,
            IN OUT HCATINFO *phPrevCatInfo
            );

typedef BOOL (WINAPI *CRYPTCATADMINREMOVECATALOG)(
            IN HCATADMIN hCatAdmin,
            IN WCHAR *pwszCatalogFile,
            IN DWORD dwFlags
            );

typedef BOOL (WINAPI *CRYPTCATADMINRESOLVECATALOGPATH)(
            IN HCATADMIN hCatAdmin,
            IN WCHAR *pwszCatalogFile,
            IN OUT CATALOG_INFO *psCatInfo,
            IN DWORD dwFlags
            );

typedef BOOL (WINAPI *CERTFREECERTIFICATECONTEXT)(
            IN PCCERT_CONTEXT pCertContext
            );

typedef LONG (WINAPI *WINVERIFYTRUST)(
            HWND hwnd,
            GUID *pgActionID,
            LPVOID pWVTData
            );

typedef CONFIGRET (WINAPI *CM_QUERY_RESOURCE_CONFLICT_LIST)(
             OUT PCONFLICT_LIST pclConflictList,
             IN  DEVINST        dnDevInst,
             IN  RESOURCEID     ResourceID,
             IN  PCVOID         ResourceData,
             IN  ULONG          ResourceLen,
             IN  ULONG          ulFlags,
             IN  HMACHINE       hMachine
             );

typedef CONFIGRET (WINAPI *CM_FREE_RESOURCE_CONFLICT_HANDLE)(
             IN CONFLICT_LIST   clConflictList
             );

typedef CONFIGRET (WINAPI *CM_GET_RESOURCE_CONFLICT_COUNT)(
             IN CONFLICT_LIST   clConflictList,
             OUT PULONG         pulCount
             );

typedef CONFIGRET (WINAPI *CM_GET_RESOURCE_CONFLICT_DETAILSA)(
             IN CONFLICT_LIST         clConflictList,
             IN ULONG                 ulIndex,
             IN OUT PCONFLICT_DETAILS_A pConflictDetails
             );

#endif

//
// Pointers declared in stub.c and initialized in InitializeStubFnPtrs
//

extern GETFILEATTRIBUTESEXA_PROC  Dyn_GetFileAttributesExA;
extern GETSYSTEMWINDOWSDIRECTORYA_PROC Dyn_GetSystemWindowsDirectoryA;
extern VERIFYVERSIONINFOA_PROC    Dyn_VerifyVersionInfoA;
extern VERSETCONDITIONMASK_PROC   Dyn_VerSetConditionMask;
extern SFCONNECTTOSERVER_PROC     Dyn_SfcConnectToServer;
extern SFCCLOSE_PROC              Dyn_SfcClose;
extern SFCFILEEXCEPTION_PROC      Dyn_SfcFileException;
extern SFCISFILEPROTECTED_PROC    Dyn_SfcIsFileProtected;

#ifdef ANSI_SETUPAPI

extern CM_QUERY_RESOURCE_CONFLICT_LIST Dyn_CM_Query_Resource_Conflict_List;
extern CM_FREE_RESOURCE_CONFLICT_HANDLE Dyn_CM_Free_Resource_Conflict_Handle;
extern CM_GET_RESOURCE_CONFLICT_COUNT Dyn_CM_Get_Resource_Conflict_Count;
extern CM_GET_RESOURCE_CONFLICT_DETAILSA Dyn_CM_Get_Resource_Conflict_DetailsA;
extern CM_GET_CLASS_REGISTRY_PROPERTYA Dyn_CM_Get_Class_Registry_PropertyA;
extern CM_SET_CLASS_REGISTRY_PROPERTYA Dyn_CM_Set_Class_Registry_PropertyA;
extern CM_GET_DEVICE_INTERFACE_ALIAS_EXA Dyn_CM_Get_Device_Interface_Alias_ExA;
extern CM_GET_DEVICE_INTERFACE_LIST_EXA Dyn_CM_Get_Device_Interface_List_ExA;
extern CM_GET_DEVICE_INTERFACE_LIST_SIZE_EXA Dyn_CM_Get_Device_Interface_List_Size_ExA;
extern CM_GET_LOG_CONF_PRIORITY_EX Dyn_CM_Get_Log_Conf_Priority_Ex;
extern CM_QUERY_AND_REMOVE_SUBTREE_EXA Dyn_CM_Query_And_Remove_SubTree_ExA;
extern CM_REGISTER_DEVICE_INTERFACE_EXA Dyn_CM_Register_Device_Interface_ExA;
extern CM_SET_DEVNODE_PROBLEM_EX Dyn_CM_Set_DevNode_Problem_Ex;
extern CM_UNREGISTER_DEVICE_INTERFACE_EXA Dyn_CM_Unregister_Device_Interface_ExA;

extern CRYPTCATADMINACQUIRECONTEXT Dyn_CryptCATAdminAcquireContext;
extern CRYPTCATADMINRELEASECONTEXT Dyn_CryptCATAdminReleaseContext;
extern CRYPTCATADMINRELEASECATALOGCONTEXT Dyn_CryptCATAdminReleaseCatalogContext;
extern CRYPTCATADMINADDCATALOG Dyn_CryptCATAdminAddCatalog;
extern CRYPTCATCATALOGINFOFROMCONTEXT Dyn_CryptCATCatalogInfoFromContext;
extern CRYPTCATADMINCALCHASHFROMFILEHANDLE Dyn_CryptCATAdminCalcHashFromFileHandle;
extern CRYPTCATADMINENUMCATALOGFROMHASH Dyn_CryptCATAdminEnumCatalogFromHash;
extern CRYPTCATADMINREMOVECATALOG Dyn_CryptCATAdminRemoveCatalog;
extern CRYPTCATADMINRESOLVECATALOGPATH Dyn_CryptCATAdminResolveCatalogPath;

extern CERTFREECERTIFICATECONTEXT Dyn_CertFreeCertificateContext;

extern WINVERIFYTRUST Dyn_WinVerifyTrust;

#endif

//
// "Not Implemented" Stubs
//

BOOL
WINAPI
Stub_VerifyVersionInfoA(
    IN LPOSVERSIONINFOEXA lpVersionInformation,
    IN DWORD dwTypeMask,
    IN DWORDLONG dwlConditionMask
    );

BOOL
WINAPI
Stub_VerifyVersionInfoW(
    IN LPOSVERSIONINFOEXW lpVersionInformation,
    IN DWORD dwTypeMask,
    IN DWORDLONG dwlConditionMask
    );

ULONGLONG
NTAPI
Stub_VerSetConditionMask(
    IN ULONGLONG ConditionMask,
    IN DWORD TypeMask,
    IN BYTE Condition
    );

HANDLE
WINAPI
Stub_SfcConnectToServer(
    IN LPCWSTR ServerName
    );

VOID
Stub_SfcClose(
    IN HANDLE RpcHandle
    );

DWORD
WINAPI
Stub_SfcFileException(
    IN HANDLE RpcHandle,
    IN PCWSTR FileName,
    IN DWORD ExpectedChangeType
    );

BOOL
WINAPI
Stub_SfcIsFileProtected(
    IN HANDLE RpcHandle,
    IN LPCWSTR ProtFileName
    );

HANDLE
WINAPI
FirstLoad_SfcConnectToServer(
    IN LPCWSTR ServerName
    );

VOID
FirstLoad_SfcClose(
    IN HANDLE RpcHandle
    );

DWORD
WINAPI
FirstLoad_SfcFileException(
    IN HANDLE RpcHandle,
    IN PCWSTR FileName,
    IN DWORD ExpectedChangeType
    );

BOOL
WINAPI
FirstLoad_SfcIsFileProtected(
    IN HANDLE RpcHandle,
    IN LPCWSTR ProtFileName
    );

#ifdef ANSI_SETUPAPI

CONFIGRET
WINAPI
Stub_CM_Get_Class_Registry_PropertyA(
    IN  LPGUID      ClassGUID,
    IN  ULONG       ulProperty,
    OUT PULONG      pulRegDataType,    OPTIONAL
    OUT PVOID       Buffer,            OPTIONAL
    IN  OUT PULONG  pulLength,
    IN  ULONG       ulFlags,
    IN  HMACHINE    hMachine
    );

CONFIGRET
WINAPI
Stub_CM_Set_Class_Registry_PropertyA(
    IN LPGUID      ClassGUID,
    IN ULONG       ulProperty,
    IN PCVOID      Buffer,       OPTIONAL
    IN ULONG       ulLength,
    IN ULONG       ulFlags,
    IN HMACHINE    hMachine
    );

CONFIGRET
WINAPI
Stub_CM_Get_Device_Interface_Alias_ExA (
    IN     LPCSTR   pszDeviceInterface,
    IN     LPGUID   AliasInterfaceGuid,
    OUT    LPSTR    pszAliasDeviceInterface,
    IN OUT PULONG   pulLength,
    IN     ULONG    ulFlags,
    IN     HMACHINE hMachine
    );

CONFIGRET
WINAPI
Stub_CM_Get_Device_Interface_List_ExA(
    IN  LPGUID      InterfaceClassGuid,
    IN  DEVINSTID_A pDeviceID,      OPTIONAL
    OUT PCHAR       Buffer,
    IN  ULONG       BufferLen,
    IN  ULONG       ulFlags,
    IN  HMACHINE    hMachine
    );

CONFIGRET
WINAPI
Stub_CM_Get_Device_Interface_List_Size_ExA(
    IN  PULONG      pulLen,
    IN  LPGUID      InterfaceClassGuid,
    IN  DEVINSTID_A pDeviceID,      OPTIONAL
    IN  ULONG       ulFlags,
    IN  HMACHINE    hMachine
    );

CONFIGRET
WINAPI
Stub_CM_Get_Log_Conf_Priority_Ex(
    IN  LOG_CONF  lcLogConf,
    OUT PPRIORITY pPriority,
    IN  ULONG     ulFlags,
    IN  HMACHINE  hMachine
    );

CONFIGRET
WINAPI
Stub_CM_Query_And_Remove_SubTree_ExA(
    IN  DEVINST        dnAncestor,
    OUT PPNP_VETO_TYPE pVetoType,
    OUT LPSTR          pszVetoName,
    IN  ULONG          ulNameLength,
    IN  ULONG          ulFlags,
    IN  HMACHINE       hMachine
    );

CONFIGRET
WINAPI
Stub_CM_Register_Device_Interface_ExA(
    IN  DEVINST   dnDevInst,
    IN  LPGUID    InterfaceClassGuid,
    IN  LPCSTR    pszReference,         OPTIONAL
    OUT LPSTR     pszDeviceInterface,
    IN OUT PULONG pulLength,
    IN  ULONG     ulFlags,
    IN  HMACHINE  hMachine
    );

CONFIGRET
WINAPI
Stub_CM_Set_DevNode_Problem_Ex(
    IN DEVINST   dnDevInst,
    IN ULONG     ulProblem,
    IN  ULONG    ulFlags,
    IN  HMACHINE hMachine
    );

CONFIGRET
WINAPI
Stub_CM_Unregister_Device_Interface_ExA(
    IN LPCSTR   pszDeviceInterface,
    IN ULONG    ulFlags,
    IN HMACHINE hMachine
    );

CONFIGRET
WINAPI
Stub_CM_Query_Resource_Conflict_List(
             OUT PCONFLICT_LIST pclConflictList,
             IN  DEVINST        dnDevInst,
             IN  RESOURCEID     ResourceID,
             IN  PCVOID         ResourceData,
             IN  ULONG          ResourceLen,
             IN  ULONG          ulFlags,
             IN  HMACHINE       hMachine
             );

CONFIGRET
WINAPI
Stub_CM_Free_Resource_Conflict_Handle(
             IN CONFLICT_LIST   clConflictList
             );

CONFIGRET
WINAPI
Stub_CM_Get_Resource_Conflict_Count(
             IN CONFLICT_LIST   clConflictList,
             OUT PULONG         pulCount
             );

CONFIGRET
WINAPI
Stub_CM_Get_Resource_Conflict_DetailsA(
             IN CONFLICT_LIST         clConflictList,
             IN ULONG                 ulIndex,
             IN OUT PCONFLICT_DETAILS_A pConflictDetails
             );


BOOL
WINAPI
Stub_CryptCATAdminAcquireContext (
    OUT HCATADMIN *phCatAdmin,
    IN const GUID *pgSubsystem,
    IN DWORD dwFlags
    );

BOOL
WINAPI
Stub_CryptCATAdminReleaseContext (
    IN HCATADMIN hCatAdmin,
    IN DWORD dwFlags
    );

BOOL
WINAPI
Stub_CryptCATAdminReleaseCatalogContext (
    IN HCATADMIN hCatAdmin,
    IN HCATINFO hCatInfo,
    IN DWORD dwFlags
    );

HCATINFO
WINAPI
Stub_CryptCATAdminAddCatalog (
    IN HCATADMIN hCatAdmin,
    IN WCHAR *pwszCatalogFile,
    IN OPTIONAL WCHAR *pwszSelectBaseName,
    IN DWORD dwFlags
    );

BOOL
WINAPI
Stub_CryptCATCatalogInfoFromContext (
    IN HCATINFO hCatInfo,
    IN OUT CATALOG_INFO *psCatInfo,
    IN DWORD dwFlags
    );

BOOL
WINAPI
Stub_CryptCATAdminCalcHashFromFileHandle (
    IN HANDLE hFile,
    IN OUT DWORD *pcbHash,
    OUT OPTIONAL BYTE *pbHash,
    IN DWORD dwFlags
    );

HCATINFO
WINAPI
Stub_CryptCATAdminEnumCatalogFromHash(
    IN HCATADMIN hCatAdmin,
    IN BYTE *pbHash,
    IN DWORD cbHash,
    IN DWORD dwFlags,
    IN OUT HCATINFO *phPrevCatInfo
    );

BOOL
WINAPI
Stub_CryptCATAdminRemoveCatalog(
    IN HCATADMIN hCatAdmin,
    IN WCHAR *pwszCatalogFile,
    IN DWORD dwFlags
    );

BOOL
WINAPI
Stub_CryptCATAdminResolveCatalogPath(
    IN HCATADMIN hCatAdmin,
    IN WCHAR *pwszCatalogFile,
    IN OUT CATALOG_INFO *psCatInfo,
    IN DWORD dwFlags
    );

BOOL
WINAPI
Stub_CertFreeCertificateContext(
    IN PCCERT_CONTEXT pCertContext
    );

LONG
WINAPI
Stub_WinVerifyTrust(
    HWND hwnd,
    GUID *pgActionID,
    LPVOID pWVTData
    );

#endif


//
// Macro repairs
//

#define SfcConnectToServer Dyn_SfcConnectToServer
#define SfcClose           Dyn_SfcClose
#define SfcFileException   Dyn_SfcFileException
#define SfcIsFileProtected Dyn_SfcIsFileProtected

#ifdef ANSI_SETUPAPI

#undef VerifyVersionInfo
#undef VerSetConditionMask
#undef GetFileAttributesEx
#undef GetSystemWindowsDirectory
#undef CM_Get_Class_Registry_Property
#undef CM_Set_Class_Registry_Property
#undef CM_Get_Device_Interface_Alias_Ex
#undef CM_Get_Device_Interface_List_Ex
#undef CM_Get_Device_Interface_List_Size_Ex
#undef CM_Query_And_Remove_SubTree_Ex
#undef CM_Register_Device_Interface_Ex
#undef CM_Unregister_Device_Interface_Ex
#undef CM_Get_Resource_Conflict_Details

#define VerifyVersionInfo  Dyn_VerifyVersionInfoA
#define VerSetConditionMask Dyn_VerSetConditionMask
#define CM_Get_Class_Registry_Property Dyn_CM_Get_Class_Registry_PropertyA
#define CM_Set_Class_Registry_Property Dyn_CM_Set_Class_Registry_PropertyA
#define CM_Get_Device_Interface_Alias_Ex Dyn_CM_Get_Device_Interface_Alias_ExA
#define CM_Get_Device_Interface_List_Ex Dyn_CM_Get_Device_Interface_List_ExA
#define CM_Get_Device_Interface_List_Size_Ex Dyn_CM_Get_Device_Interface_List_Size_ExA
#define CM_Get_Log_Conf_Priority_Ex Dyn_CM_Get_Log_Conf_Priority_Ex
#define CM_Query_And_Remove_SubTree_Ex Dyn_CM_Query_And_Remove_SubTree_ExA
#define CM_Register_Device_Interface_Ex Dyn_CM_Register_Device_Interface_ExA
#define CM_Set_DevNode_Problem_Ex Dyn_CM_Set_DevNode_Problem_Ex
#define CM_Unregister_Device_Interface_Ex Dyn_CM_Unregister_Device_Interface_ExA
#define CM_Query_Resource_Conflict_List Dyn_CM_Query_Resource_Conflict_List
#define CM_Free_Resource_Conflict_Handle Dyn_CM_Free_Resource_Conflict_Handle
#define CM_Get_Resource_Conflict_Count Dyn_CM_Get_Resource_Conflict_Count
#define CM_Get_Resource_Conflict_Details Dyn_CM_Get_Resource_Conflict_DetailsA

#define CryptCATAdminAcquireContext Dyn_CryptCATAdminAcquireContext
#define CryptCATAdminReleaseContext Dyn_CryptCATAdminReleaseContext
#define CryptCATAdminReleaseCatalogContext Dyn_CryptCATAdminReleaseCatalogContext
#define CryptCATAdminAddCatalog Dyn_CryptCATAdminAddCatalog
#define CryptCATCatalogInfoFromContext Dyn_CryptCATCatalogInfoFromContext
#define CryptCATAdminCalcHashFromFileHandle Dyn_CryptCATAdminCalcHashFromFileHandle
#define CryptCATAdminEnumCatalogFromHash Dyn_CryptCATAdminEnumCatalogFromHash
#define CryptCATAdminRemoveCatalog Dyn_CryptCATAdminRemoveCatalog
#define CryptCATAdminResolveCatalogPath Dyn_CryptCATAdminResolveCatalogPath
#define CertFreeCertificateContext Dyn_CertFreeCertificateContext

#define WinVerifyTrust Dyn_WinVerifyTrust

#define GetFileAttributesEx Dyn_GetFileAttributesExA
#define GetSystemWindowsDirectory Dyn_GetSystemWindowsDirectoryA

#endif

//
// Functions called from DllMain
//

VOID
InitializeStubFnPtrs (
    VOID
    );

VOID
CleanUpStubFns (
    VOID
    );

FARPROC
ObtainFnPtr (
    PCSTR DllName,
    PCSTR FnName,
    FARPROC Default
    );


