/*++

Copyright (c) 1996  Microsoft Corporation

Module Name:

    autoprox.hxx

Abstract:

    Contains interface definition for a specialized DLL that automatically configurares WININET proxy
      information. The DLL can reroute proxy infromation based on a downloaded set of data that matches
      it registered MIME type.  The DLL can also control WININET proxy use, on a request by request basis.

    Contents:
        AUTO_PROXY_LIST_ENTRY
        AUTO_PROXY_DLLS
        AUTO_PROXY_ASYNC_MSG
        PROXY_STATE

Author:

    Arthur L Bierer (arthurbi) 01-Dec-1996

Revision History:

    26-Aug-2001 ssulzer
        WinHttp Autoproxy
--*/

#ifndef _AUTOPROX_HXX_
#define _AUTOPROX_HXX_


#define WINHTTP_AUTO_DETECT_FLAGS_MASK  \
                (WINHTTP_AUTO_DETECT_TYPE_DHCP      |   \
                 WINHTTP_AUTO_DETECT_TYPE_DNS_A     |   \
                 WINHTTP_AUTO_DETECT_ON_NET_CHANGE)

#define CSZMAINSECKEY "AutoProxyTypes"

//
// functions
//


class CAutoProxy
{
    INTERNET_HANDLE_OBJECT *    _hSession;
    CCritSec                    _CritSec;
    RESOURCE_LOCK               _ScriptResLock;

    //
    // Detection information
    //
    LPSTR       _pszAutoConfigUrl;
    FILETIME    _ftLastDetectionTime;
    DWORD *     _pdwDetectedInterfaceIp;
    int         _cDetectedInterfaceIpCount;

    LPSTR       _pszConfigScript;

    //
    // Expiry & LastModified timestamps for config script
    //
    FILETIME    _ftExpiryTime;
    BOOL        _fHasExpiry;
    FILETIME    _ftLastModifiedTime;
    BOOL        _fHasLastModifiedTime;
    BOOL        _fMustRevalidate;
    FILETIME    _ftLastSyncTime;


    BOOL  IsDetectionNeeded();

    DWORD DetectAutoProxyUrl(
                DWORD       dwAutoDetectFlags,
                LPSTR *     ppszAutoConfigUrl);

    DWORD DownloadAutoConfigUrl(
                LPSTR                       lpszAutoConfigUrl,
                WINHTTP_AUTOPROXY_OPTIONS * pAutoProxyOptions,
                LPSTR *                     ppszConfigScript);
    
    BOOL  IsSupportedMimeType(char * szType);
    BOOL  IsSupportedFileExtension(LPCSTR lpszUrl);

    DWORD RunProxyScript(
                LPCSTR      lpszUrl,
                LPCSTR      pszProxyScript,
                LPSTR *     ppszQueryResults);

    DWORD ParseProxyQueryResults(
                LPSTR                   pszQueryResults,
                WINHTTP_PROXY_INFO *    pProxyInfo);
    
    VOID  CalculateTimeStampsForCache(HINTERNET hRequest);
    BOOL  IsCachedProxyScriptExpired();
    VOID  AddIfModifiedSinceHeaders(HINTERNET hRequest);

    static DWORD GetHostAddresses(int * pcInterfaces, DWORD ** ppdwInterfaceIp);

public:
    DWORD GetProxyForURL(
                LPCWSTR                     lpcwszUrl,
                WINHTTP_AUTOPROXY_OPTIONS * pAutoProxyOptions,
                WINHTTP_PROXY_INFO *        pProxyInfo);

    CAutoProxy(INTERNET_HANDLE_OBJECT * hSession)
    {
        _hSession = hSession;
    }

    ~CAutoProxy();

    BOOL Initialize();
};

//
// Callback functions implimented in WINHTTP
//

class AUTO_PROXY_HELPER_APIS {

public:

    virtual
    BOOL IsResolvable(
        IN LPSTR lpszHost
        );

    virtual
    DWORD GetIPAddress(
        IN OUT LPSTR   lpszIPAddress,
        IN OUT LPDWORD lpdwIPAddressSize
        );

    virtual
    DWORD ResolveHostName(
        IN LPSTR lpszHostName,
        IN OUT LPSTR   lpszIPAddress,
        IN OUT LPDWORD lpdwIPAddressSize
        );

    virtual
    BOOL IsInNet(
        IN LPSTR   lpszIPAddress,
        IN LPSTR   lpszDest,
        IN LPSTR   lpszMask
        );

};



void UnloadAutoProxy();


//
// external func declariations
//


#define AUTO_PROXY_VERSION 0x00010000
#define EXT_PROXY_FUNC *

typedef BOOL
(EXT_PROXY_FUNC PROXY_INFO_INVALID_FN) (
    IN LPSTR lpszMime,
    IN LPSTR lpszUrl,
    IN DWORD dwUrlLength,
    IN LPSTR lpszProxyHostName,
    IN DWORD dwProxyHostNameLength
    );

typedef BOOL
(EXT_PROXY_FUNC PROXY_DLL_DEINIT_FN) (
    IN LPSTR lpszMime,
    IN DWORD dwReserved
    );

typedef BOOL
(EXT_PROXY_FUNC GET_PROXY_INFO_FN) (
    IN LPCSTR lpszUrl,
    IN DWORD dwUrlLength,
    IN LPSTR lpszUrlHostName,
    IN DWORD dwUrlHostNameLength,
    OUT LPSTR * lplpszProxyHostName,
    OUT LPDWORD lpdwProxyHostNameLength
    ) ;


typedef BOOL
(EXT_PROXY_FUNC GET_PROXY_INFO_EX_FN) (
    IN INTERNET_SCHEME tUrlProtocol,
    IN LPCSTR lpszUrl,
    IN DWORD dwUrlLength,
    IN LPSTR lpszUrlHostName,
    IN DWORD dwUrlHostNameLength,
    IN INTERNET_PORT nUrlPort,
    OUT LPINTERNET_SCHEME lptProxyScheme,
    OUT LPSTR * lplpszProxyHostName,
    OUT LPDWORD lpdwProxyHostNameLength,
    OUT LPINTERNET_PORT lpProxyHostPort
    ) ;

typedef BOOL
(EXT_PROXY_FUNC PROXY_DLL_INIT_FN) (
    IN DWORD                  dwVersion,
    IN LPSTR                  lpszDownloadedTempFile,
    IN LPSTR                  lpszMime,
    IN AUTO_PROXY_HELPER_APIS *pAutoProxyCallbacks,
    IN DWORD_PTR              dwReserved
    );

#define PROXY_INFO_INVALID_FN_NAME       "InternetProxyInfoInvalid"
#define GET_PROXY_INFO_FN_NAME           "InternetGetProxyInfo"
#define GET_PROXY_INFO_EX_FN_NAME        "InternetGetProxyInfoEx"
#define PROXY_DLL_INIT_FN_NAME           "InternetInitializeAutoProxyDll"
#define PROXY_DLL_DEINIT_FN_NAME         "InternetDeInitializeAutoProxyDll"




//
// Flags stored in registry for Auto-Proxy Entry
//

#define AUTOPROX_FLAGS_SINGLE_THREAD 0x01

//
// defines ...
//

#define DEFAULT_AUTO_PROXY_THREAD_TIME_OUT 30000 // 30 secs  BUGBUG, up before checkin

//
// PROXY_MESSAGE_TYPE - this represents a type of query that is used
//   to submit a request through proxy "knowledge-base" ( ie script, db, etc )
//   for async processing of Java-Script auto-proxy scripts.
//

typedef enum {
    PROXY_MSG_INVALID,
    PROXY_MSG_INIT,
    PROXY_MSG_DEINIT,
    PROXY_MSG_SELF_DESTRUCT,
    PROXY_MSG_GET_PROXY_INFO,
    PROXY_MSG_SET_BAD_PROXY
} PROXY_MESSAGE_TYPE;


//
// PROXY_MESSAGE_ALLOC_TYPE - this is needed to track what allocation of member vars
//   are currently used in my object.  This allows us to use stack vars
//   for simple, local calls, but then have allocation done-on-the-fly
//   when we need to queue the message.
//

typedef enum {
    MSG_ALLOC_NONE,
    MSG_ALLOC_STACK_ONLY,
    MSG_ALLOC_HEAP,
    MSG_ALLOC_HEAP_MSG_OBJ_OWNS
} PROXY_MESSAGE_ALLOC_TYPE;


//
// AUTO_PROXY_STATE - is used to track the state of the auto-proxy list object
//   in order to maintain syncronization across readers and writers.
//

typedef enum {
    AUTO_PROXY_DISABLED,
    AUTO_PROXY_BLOCKED,
    AUTO_PROXY_PENDING, // blocked, but we can still fallback to static settings
    AUTO_PROXY_ENABLED
} AUTO_PROXY_STATE;


class BAD_PROXY_LIST; // forward decl


//
// PROXY_STATE - keeps track of multiple proxies stored in a Netscape Style
//   proxy list string format.  Ex:
//   "PROXY itgproxy:80; PROXY proxy:80; PROXY 192.168.100.2:1080; SOCKS 192.168.100.2; DIRECT"
//
//   Can also be used to link the GetProxyInfo return with a failure in the proxies it gave.
//   This needed so we call back into an Auto-Proxy DLL letting them know that we failed
//   to be able to utilize its auto-proxy that it gave us.
//

class PROXY_STATE {

private:

    //
    // _lpszAutoProxyList - List, or Proxy that we are tracking.
    //

    LPSTR _lpszAutoProxyList;

    //
    // _dwcbAutoProxyList - size of _lpszAutoProxyList.
    //

    DWORD _dwcbAutoProxyList;

    //
    // _lpszOffset - Current Offset into _lpszAutoProxyList
    //

    LPSTR _lpszOffset;

    //
    // _lpszLastProxyUsed - Tracks the last proxy that was returned.
    //

    LPSTR _lpszLastProxyUsed;

    //
    // _LastProxyUsedPort - Last used Internet Port on the Proxy
    //

    INTERNET_PORT _LastProxyUsedPort;

    //
    // _Error - Error code in constructor.
    //

    DWORD _Error;

    //
    // _fIsMultiProxyList - TRUE if we a Netscape type list of proxies, as opposed to a simple proxyhostname string.
    //

    BOOL _fIsMultiProxyList;

    //
    //  _fIsAnotherProxyAvail - TRUE if we have another proxy to process
    //

    BOOL _fIsAnotherProxyAvail;

    //
    // tProxyScheme - for the non-multi proxy case we track the type of proxy we are storing.
    //

    INTERNET_SCHEME _tProxyScheme;

    //
    // ProxyHostPort - for the non-multi proxy case we track the host port.
    //

    INTERNET_PORT _proxyHostPort;

public:

    PROXY_STATE(
        LPSTR lpszAutoProxyList,
        DWORD dwcbAutoProxyList,
        BOOL  fIsMultiProxyList,
        INTERNET_SCHEME tProxyScheme,
        INTERNET_PORT   proxyHostPort
        )
    {
        _lpszAutoProxyList = NULL;
        _dwcbAutoProxyList = 0;
        _lpszOffset        = NULL;
        _Error             = ERROR_SUCCESS;
        _lpszLastProxyUsed = NULL;
        _LastProxyUsedPort = INTERNET_DEFAULT_HTTP_PORT;
        _fIsMultiProxyList = fIsMultiProxyList;
        _tProxyScheme      = tProxyScheme;
        _proxyHostPort     = proxyHostPort;

        _fIsAnotherProxyAvail = FALSE;
        
        if ( lpszAutoProxyList &&
             dwcbAutoProxyList > 0 )
        {

            _lpszAutoProxyList = (LPSTR)
                ALLOCATE_MEMORY(dwcbAutoProxyList+1);


            if ( _lpszAutoProxyList )
            {
                lstrcpyn(_lpszAutoProxyList, lpszAutoProxyList, dwcbAutoProxyList );

                _lpszAutoProxyList[dwcbAutoProxyList] = '\0';
                _dwcbAutoProxyList = dwcbAutoProxyList;
                _lpszOffset        = _lpszAutoProxyList;
            }
            else
            {
                _Error = ERROR_NOT_ENOUGH_MEMORY;
            }
        }
    }


    ~PROXY_STATE()
    {
        DEBUG_ENTER((DBG_OBJECTS,
                    None,
                    "~PROXY_STATE",
                    NULL
                    ));

        if ( _lpszAutoProxyList )
        {
            FREE_MEMORY(_lpszAutoProxyList );
            _lpszAutoProxyList = NULL;

        }

        DEBUG_LEAVE(0);
    }



    DWORD
    GetError(VOID) const
    {
        return _Error;
    }

    BOOL
    IsEmpty(VOID) const
    {
        return (!_lpszOffset || *_lpszOffset == '\0' ) ? TRUE : FALSE;
    }

    BOOL 
    IsAnotherProxyAvail(VOID) const
    {
        return _fIsAnotherProxyAvail;
    }

    LPSTR
    GetLastProxyUsed(INTERNET_PORT *pProxyPort)
    {
        *pProxyPort = _LastProxyUsedPort;
        return _lpszLastProxyUsed;
    }

    BOOL
    GetNextProxy(
        IN  INTERNET_SCHEME   tUrlScheme,
        IN  BAD_PROXY_LIST &  BadProxyList,
        OUT LPINTERNET_SCHEME lptProxyScheme,
        OUT LPSTR * lplpszProxyHostName,
        OUT LPBOOL lpbFreeProxyHostName,
        OUT LPDWORD lpdwProxyHostNameLength,
        OUT LPINTERNET_PORT lpProxyHostPort
        );

};


//
// AUTO_PROXY_ASYNC_MSG - this object is used to pass queries for proxy information across
//   from requestors (HTTP_REQUEST_HANDLE_OBJECT, parseUrl, etc) to responders
//   (AutoProxy DLL, PROXY_INFO object) who can process the request.
//

class AUTO_PROXY_ASYNC_MSG {

    friend class AUTO_PROXY_LIST_ENTRY;

public:

    //
    // _List - We're a list entry on a queue.  But we don't always have to be,
    //   We can be borne a structure that justs gets passed around and is never
    //   queued.
    //

    LIST_ENTRY _List;

    //
    // _tUrlProtocol - Protocol that this request is using.
    //

    INTERNET_SCHEME _tUrlProtocol;


    //
    // _lpszUrl - The requested URL that we are navigating to.
    //

    LPSTR _lpszUrl;

    //
    // _dwUrlLength - The size of the  URL.
    //

    DWORD _dwUrlLength;

    //
    // _lpszUrlHostName - The host name found in the URL.
    //


    LPSTR _lpszUrlHostName;

    //
    // _dwUrlHostNameLength - Host name Length
    //

    DWORD _dwUrlHostNameLength;

    //
    // _nUrlPort - Dest port used on this Url.
    //

    INTERNET_PORT _nUrlPort;

    //
    // _tProxyScheme - Type of proxy we will use. ( HTTP, HTTPS, etc )
    //

    INTERNET_SCHEME _tProxyScheme;

    //
    // _lpszProxyHostName - Host name of the proxy we are to use.
    //

    LPSTR _lpszProxyHostName;

    //
    // _bFreeProxyHostName - TRUE if _lpszProxyHostName is a copy
    //

    BOOL _bFreeProxyHostName;

    //
    // _dwProxyHostNameLength - Proxy Host name length.
    //

    DWORD _dwProxyHostNameLength;

    //
    // _nProxyHostPort - Proxy port to use
    //

    INTERNET_PORT _nProxyHostPort;

    //
    // _pProxyState - State for enumerating multiple proxies.
    //

    PROXY_STATE *_pProxyState;

    //
    // _pmProxyQuery - The Action aka Query we're are doing with this message.
    //

    PROXY_MESSAGE_TYPE _pmProxyQuery;

    //
    // _pmaAllocMode - Used to track who owns the allocated string ptrs.
    //

    PROXY_MESSAGE_ALLOC_TYPE _pmaAllocMode;

    //
    // _dwQueryResult - TRUE if we are going through a proxy
    //

    DWORD _dwQueryResult;

    //
    // _Error - Return code after this call is made.
    //

    DWORD _Error;

    //
    // dwProxyVersion - Proxy Verion this was issue a result for
    //

    DWORD _dwProxyVersion; 

    VOID SetVersion(VOID) {
        _dwProxyVersion = GlobalProxyVersionCount;
    }

    DWORD GetVersion(VOID) {
        return _dwProxyVersion;
    }

    //
    // _MessageFlags - Various
    //

    union {

        struct {

            //
            // DontWantProxyStrings, don't waste time with allocating strings for my result,
            //      since I all I care about is generalized proxy info (ie do I have a proxy for ...).
            //

            DWORD DontWantProxyStrings    : 1;

            //
            // BlockUntilCompletetion, don't return from an async call until the async thread has
            //   completed processing my proxy query.
            //

            DWORD BlockUntilCompletetion  : 1;

            //
            // AvoidAsyncCall, don't go async under any curcumstances, so we will return
            //   what every we can get, even if this means giving incorrect data.
            //

            DWORD AvoidAsyncCall : 1;

            //
            // BlockedOnFsm, true a fsm is blocked on this message to complete
            //

            DWORD BlockedOnFsm : 1 ;

            //
            // QueryOnCallback, TRUE if we're making a call with this object
            //   on a FSM callback.  Basically we've received results from
            //   the async thread, but we need to do a final a call so
            //   the results can be parsed out.
            //

            DWORD QueryOnCallback : 1;

            //
            //  ForceRefresh, TRUE if we are to force a refresh of cached settings and scripts.
            //

            DWORD ForceRefresh : 1;

            //
            // ShowIndication, TRUE if we indicate via callback
            //   to the user that we performing auto-detection
            // 

            DWORD ShowIndication : 1;

            //
            // BackroundDetectionPending, TRUE if a backround detection
            //   is being done while this page is loading
            //

            DWORD BackroundDetectionPending : 1;

            //
            // CanCacheResult - TRUE if this result can be cached
            // 

            DWORD CanCacheResult : 1;

        } Flags;

        //
        // Dword - used in initialization ONLY, do NOT use ELSEWHERE !
        //

        DWORD Dword;

    } _MessageFlags;


//public:

    AUTO_PROXY_ASYNC_MSG(
        IN INTERNET_SCHEME isUrlScheme,
        IN LPSTR lpszUrl,
        IN LPSTR lpszUrlHostName,
        IN DWORD dwUrlHostNameLength
        );


    AUTO_PROXY_ASYNC_MSG(
        IN INTERNET_SCHEME isUrlScheme,
        IN LPSTR lpszUrl,
        IN DWORD dwUrlLength,
        IN LPSTR lpszUrlHostName,
        IN DWORD dwUrlHostNameLength,
        IN INTERNET_PORT nUrlPort
        );

    AUTO_PROXY_ASYNC_MSG(
        PROXY_MESSAGE_TYPE pmProxyQuery
        );

    AUTO_PROXY_ASYNC_MSG(
        AUTO_PROXY_ASYNC_MSG *pStaticAutoProxy
        );

    AUTO_PROXY_ASYNC_MSG(
        IN INTERNET_SCHEME isUrlScheme,
        IN LPSTR lpszUrlHostName,
        IN DWORD dwUrlHostNameLength
        );


    AUTO_PROXY_ASYNC_MSG(
        VOID
        )
    {
        Initalize();
    }


    ~AUTO_PROXY_ASYNC_MSG(
        VOID
        );


    VOID Initalize(
        VOID
        )
    {

        InitializeListHead(&_List);

        // this is odd, we should just memset this.

        _tUrlProtocol = INTERNET_SCHEME_UNKNOWN;
        _lpszUrl      = NULL;
        _dwUrlLength  = 0;
        _lpszUrlHostName = NULL;
        _dwUrlHostNameLength = 0;
        _nUrlPort     = INTERNET_INVALID_PORT_NUMBER;
        _tProxyScheme = INTERNET_SCHEME_UNKNOWN;
        _lpszProxyHostName = NULL;
        _bFreeProxyHostName = FALSE;
        _dwProxyHostNameLength = 0;
        _nProxyHostPort = INTERNET_INVALID_PORT_NUMBER;
        _pmProxyQuery = PROXY_MSG_INVALID;
        _pmaAllocMode = MSG_ALLOC_NONE;
        _pProxyState  = NULL;
        _dwQueryResult = FALSE;
        _Error        = ERROR_SUCCESS;
        _dwProxyVersion = 0;

        _MessageFlags.Dword = 0;

    }

    VOID
    SetProxyMsg(
        IN INTERNET_SCHEME isUrlScheme,
        IN LPSTR lpszUrl,
        IN DWORD dwUrlLength,
        IN LPSTR lpszUrlHostName,
        IN DWORD dwUrlHostNameLength,
        IN INTERNET_PORT nUrlPort
        );


    BOOL IsProxyEnumeration(VOID) const {

        if ( _pProxyState        &&
             !IsDontWantProxyStrings() &&
             !_pProxyState->IsEmpty() )
        {
            return TRUE;
        }

        return FALSE;
    }

    BOOL IsAlloced(VOID) const {
        return ( _pmaAllocMode == MSG_ALLOC_HEAP ||
                 _pmaAllocMode == MSG_ALLOC_HEAP_MSG_OBJ_OWNS ) ?
                    TRUE : FALSE;
    }

    BOOL IsUrl(VOID) const {
        return (_lpszUrl && _dwUrlLength > 0 );
    }

    BOOL IsUseProxy(VOID) const {
        return _dwQueryResult;
    }

    VOID SetUseProxy(BOOL Value) {
        _dwQueryResult = Value;
    }

    INTERNET_SCHEME GetProxyScheme(VOID) const {
        return _tProxyScheme;
    }

    INTERNET_SCHEME GetUrlScheme(VOID) const {
        return _tUrlProtocol;
    }
       
    BOOL IsShowIndication(VOID) const {
        return (BOOL) _MessageFlags.Flags.ShowIndication;
    }

    VOID SetShowIndication(BOOL Value) {
        _MessageFlags.Flags.ShowIndication = (Value) ? TRUE : FALSE;
    }

    BOOL IsBackroundDetectionPending(VOID) const {
        return (BOOL) _MessageFlags.Flags.BackroundDetectionPending;
    }

    VOID SetBackroundDetectionPending(BOOL Value) {
        _MessageFlags.Flags.BackroundDetectionPending = (Value) ? TRUE : FALSE;
    }

    BOOL IsDontWantProxyStrings(VOID) const {
        return (BOOL) _MessageFlags.Flags.DontWantProxyStrings;
    }

    VOID SetDontWantProxyStrings(BOOL Value) {
        _MessageFlags.Flags.DontWantProxyStrings = (Value) ? TRUE : FALSE;
    }

    BOOL IsAvoidAsyncCall(VOID) const {
        return (BOOL) _MessageFlags.Flags.AvoidAsyncCall;
    }

    VOID SetAvoidAsyncCall(BOOL Value) {
        _MessageFlags.Flags.AvoidAsyncCall = (Value) ? TRUE : FALSE;
    }

    BOOL IsQueryOnCallback(VOID) const {
        return (BOOL) _MessageFlags.Flags.QueryOnCallback;
    }

    VOID SetQueryOnCallback(BOOL Value) {
        _MessageFlags.Flags.QueryOnCallback = (Value) ? TRUE : FALSE;
    }

    BOOL IsForceRefresh(VOID) const {
        return (BOOL) _MessageFlags.Flags.ForceRefresh;
    }

    VOID SetForceRefresh(BOOL Value) {
        _MessageFlags.Flags.ForceRefresh = (Value) ? TRUE : FALSE;
    }

    BOOL IsBlockUntilCompletetion(VOID) const {
        return (BOOL) _MessageFlags.Flags.BlockUntilCompletetion;
    }

    VOID SetBlockUntilCompletetion(BOOL Value) {
        _MessageFlags.Flags.BlockUntilCompletetion = (Value) ? TRUE : FALSE;
    }

    BOOL IsCanCacheResult(VOID) const {
        return (BOOL) _MessageFlags.Flags.CanCacheResult;
    }
    VOID SetCanCacheResult(BOOL Value) {
        _MessageFlags.Flags.CanCacheResult = (Value) ? TRUE : FALSE;
    }

    BOOL IsBlockedOnFsm(VOID) const {
        return (BOOL) _MessageFlags.Flags.BlockedOnFsm;
    }

    VOID SetBlockedOnFsm(BOOL Value) {
        _MessageFlags.Flags.BlockedOnFsm = (Value) ? TRUE : FALSE;
    }


    PROXY_MESSAGE_TYPE QueryForInfoMessage(VOID) const {
        return _pmProxyQuery;
    }

    DWORD GetError(VOID) {
        return _Error;
    }

    DWORD GetNextProxy(
        IN  BAD_PROXY_LIST & BadProxyList
        )
    {
        BOOL fSuccess;

        INET_ASSERT(_pProxyState);

        fSuccess = _pProxyState->GetNextProxy(
                                    _tUrlProtocol,
                                    BadProxyList,
                                    &_tProxyScheme,
                                    &_lpszProxyHostName,
                                    &_bFreeProxyHostName,
                                    &_dwProxyHostNameLength,
                                    &_nProxyHostPort
                                    );

        SetUseProxy(fSuccess);

        return ERROR_SUCCESS;

    }

};


//
// AUTO_PROXY_LIST_ENTRY - contains DLL entry points, and state information,
//   for a particalar Auto-Proxy DLL.
//

class AUTO_PROXY_LIST_ENTRY {
private:

    friend class AUTO_PROXY_DLLS;

    //
    // _fDelayedInitialization - TRUE when we are to have queued initializiation of
    //      this DLL so, it is not done until the first GetProxyInfo call.
    //

    BOOL _fDelayedInitialization;

    //
    // _List - there may be a variable number of bypass entries
    //

    LIST_ENTRY _List;


    //
    // _fDefault - TRUE, if this entry is the default auto-proxy entry.
    //

    BOOL  _fDefault;

    //
    // _fInitializedSuccessfully - TRUE, if we have an initalization function
    //      in the autoproxy dll that initalized successfully.  This is to
    //      prevent calling other callbacks in the auto-proxy dll if it
    //      failed to initalize.
    //

    BOOL _fInitializedSuccessfully;

    //
    //  _lpszDllFilePath - file path of DLL to load
    //

    LPSTR _lpszDllFilePath;

    //
    // _lpszFileExtensions - string of ';' delimited file extensions to match against
    //

    LPSTR _lpszFileExtensions;

    //
    // _lpszMimeType - Mime type string associated with this Auto-Proxy type.
    //

    LPSTR _lpszMimeType;

    //
    // _hAutoConfigDLL  - open handle to DLL that we are handling.
    //

    HINSTANCE _hAutoConfigDLL;

    //
    // (_pGetProxyInfo) ( ... ) - Callback made into auto-proxy DLL to gather
    //   what proxy(s) we should use for a specific connection.
    //

    GET_PROXY_INFO_FN       _pGetProxyInfo;

    //
    // (_pGetProxyInfoEx) ( ... ) - Callback made into auto-proxy DLL to gather
    //   what proxy(s) we should use for a specific connection.  Ex versions
    //   givers caller more control.  If Ex is exported, it is used otherwise
    //   GetProyInfo is used.
    //

    GET_PROXY_INFO_EX_FN    _pGetProxyInfoEx;


    //
    // (_pProxyDllInit) ( ... ) - Callback made into auto-proxy DLL to notify
    //   DLL of initialization.
    //

    PROXY_DLL_INIT_FN       _pProxyDllInit;

    //
    // (_pProxyDllInit) ( ... ) - Notifies DLL before shutdown
    //

    PROXY_DLL_DEINIT_FN     _pProxyDllDeInit;

    //
    // (_pProxyInfoInvalid) ( ... ) - A request using the returned proxy in GetProxyInfo was invalid.
    //

    PROXY_INFO_INVALID_FN   _pProxyInfoInvalid;

    //
    // _dwFlags - DWORD flags stored in the registry for this entry.
    //

    DWORD _dwFlags;

#ifdef INET_DEBUG

    BOOL _fUnInited;

#endif


public:

    AUTO_PROXY_LIST_ENTRY(
        IN LPSTR lpszDllName,
        IN LPSTR lpszExtensions,
        IN LPSTR lpszMimeType,
        IN BOOL  fIsDefault,
        IN DWORD dwFlags
        )
    {

        DEBUG_ENTER((DBG_OBJECTS,
                    None,
                    "AUTO_PROXY_LIST_ENTRY",
                    NULL
                    ));


        InitializeListHead(&_List);

        _pGetProxyInfo      = NULL;
        _pGetProxyInfoEx    = NULL;
        _pProxyDllInit      = NULL;
        _pProxyDllDeInit    = NULL;
        _pProxyInfoInvalid  = NULL;

        _hAutoConfigDLL     = NULL;

        _lpszDllFilePath    = lpszDllName    ? NewString(lpszDllName)    : NULL;
        _lpszFileExtensions = lpszExtensions ? NewString(lpszExtensions) : NULL;
        _lpszMimeType       = lpszMimeType   ? NewString(lpszMimeType)   : NULL;

        _fDefault           = fIsDefault;
        _fInitializedSuccessfully = FALSE;
        _fDelayedInitialization = FALSE;

        _dwFlags            = dwFlags;


#ifdef INET_DEBUG
        _fUnInited          = FALSE;
#endif

        DEBUG_LEAVE(0);
    }



    ~AUTO_PROXY_LIST_ENTRY() {

        DEBUG_ENTER((DBG_OBJECTS,
                    None,
                    "~AUTO_PROXY_LIST_ENTRY",
                    NULL
                    ));

        if ( _lpszFileExtensions )
        {
            _lpszFileExtensions = (LPSTR)
                FREE_MEMORY(_lpszFileExtensions);

            INET_ASSERT(_lpszFileExtensions == NULL);
        }

        if ( _lpszDllFilePath )
        {
            _lpszDllFilePath = (LPSTR) FREE_MEMORY(_lpszDllFilePath );
            INET_ASSERT(_lpszDllFilePath == NULL);
        }

        if ( _lpszMimeType )
        {
            _lpszMimeType = (LPSTR) FREE_MEMORY(_lpszMimeType);
            INET_ASSERT(_lpszMimeType == NULL);
        }

        //INET_ASSERT(_hAutoConfigDLL == NULL); // should have free libed DLL before calling destructor

        DEBUG_LEAVE(0);
    }

    VOID
    SetDllName(
        IN LPSTR lpszDllPath
        )
    {
        if ( _lpszDllFilePath )
        {
            _lpszDllFilePath = (LPSTR)
                FREE_MEMORY(_lpszDllFilePath);
            INET_ASSERT(_lpszDllFilePath == NULL);
        }

        _lpszDllFilePath = lpszDllPath;
    }

    DWORD
    LoadEntry(
        VOID
        );

    VOID
    UnloadEntry(
        VOID
        );


    BOOL
    ProxyInfoInvalid(
        IN LPSTR lpszMime,
        IN LPSTR lpszUrl,
        IN DWORD dwUrlLength,
        IN LPSTR lpszProxyHostName,
        IN DWORD dwProxyHostNameLength
        );

    BOOL
    ProxyDllDeInit(
        IN LPSTR lpszMime,
        IN DWORD dwReserved
        );

    DWORD
    GetProxyInfoEx(
        IN AUTO_PROXY_ASYNC_MSG *pQueryForProxyInfo
        );

    DWORD
    GetProxyInfo(
        IN AUTO_PROXY_ASYNC_MSG *pQueryForProxyInfo
        );


    BOOL
    DelayProxyDllInit(
        IN DWORD                  dwVersion,
        IN LPSTR                  lpszDownloadedTempFile,
        IN LPSTR                  lpszMime,
        IN AUTO_PROXY_HELPER_APIS *pAutoProxyCallbacks,
        IN DWORD                  dwReserved,
        OUT LPBOOL                lpfCleanupFile
        );

    BOOL
    ProxyDllInit (
        IN DWORD                  dwVersion,
        IN LPSTR                  lpszDownloadedTempFile,
        IN LPSTR                  lpszMime,
        IN AUTO_PROXY_HELPER_APIS *pAutoProxyCallbacks,
        IN DWORD_PTR              dwReserved
        );

    BOOL
    IsGetProxyInfoEx(
        VOID
        )
    {
        if ( _pGetProxyInfoEx )
        {
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }

    BOOL
    IsGetProxyInfo(
        VOID
        )
    {
        if ( _pGetProxyInfo )
        {
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }

    BOOL
    IsLoaded(
        VOID
        )
    {
        if ( _hAutoConfigDLL )
        {
            return TRUE;
        }

        return FALSE;
    }

    BOOL
    IsDefault(
        VOID
        )
    {
        return _fDefault;
    }

};


//
// AUTO_PROXY_DLLS  - tracks a list of regisitered DLLs that handle auto-proxy.
//  Auto-Proxy is the ability to download a specialized configuration file,
//  match the file with a registered DLL.  The DLL, once matched, will calculate
//  which proxy(s) to use.
//

class AUTO_PROXY_DLLS {

private:

    //
    // _List - serialized list of AUTO_PROXY_LIST_ENTRY objects
    //

    SERIALIZED_LIST _List;

    //
    // _Error - errors stored here from initialization
    //

    DWORD _Error;

    //
    // _ProxySettings - the various fields to track our proxy settings
    //

    WINHTTP_AUTOPROXY_RESULTS   _ProxySettings;


    //
    // _pSelectedAutoProxyEntry - selected auto-proxy entry that will
    //   accept all auto-proxy operations.
    //

    AUTO_PROXY_LIST_ENTRY * _pSelectedAutoProxyEntry;

    //
    // _aphAutoProxyAPIs - A V-Table that is passed into to the Auto-ProxyDll
    //   to allow them to call us for DNS requests.
    //

    AUTO_PROXY_HELPER_APIS _aphAutoProxyAPIs;

    //
    // _CritSec - acquire this when accessing header structure - stops multiple
    // threads clashing while modifying or changing auto-proxy information
    //

    CCritSec _CritSec;

    //
    // _apsState - Used to track the state of this object state, for
    //   syncronization purposes, ( ie when to go async or not )
    //

    AUTO_PROXY_STATE _apsState;

    //
    // _dwWaitTimeOut - time till WaitForSingleObject times out.
    //

    DWORD _dwWaitTimeOut;

    //
    // _fEnableReload - Forces a reload of auto-proxy files every X seconds.
    //

    BOOL _fEnableReload;

    //
    // _dwLastThreadAccess - Last GetTickCount().  Used to calculate a basic
    //      timer for reloads of auto-proxy files.
    //

    DWORD _dwLastThreadAccess;

    //
    // _lpszUserAgent - User agent passed in.
    //

    LPSTR _lpszUserAgent; 

    //
    // _fModifiedInProcess - TRUE, if the settings passed in, SHOULD NOT BE PERSISTED to the registry
    //

    BOOL _fModifiedInProcess;

    //
    // _dwUpdatedProxySettingsVersion - an counter that is updated to match the current version.
    //   that has been refreshed to.  Used to note when we need to refresh proxy settings.
    //

    DWORD _dwUpdatedProxySettingsVersion;


    VOID
    ResetTimerCounter(
        BOOL fEnableTimer,
        DWORD dwWaitTimeOut
        )
    {
        _fEnableReload = fEnableTimer ;

        if ( _fEnableReload )
        {
            _dwWaitTimeOut = dwWaitTimeOut;
            _dwLastThreadAccess = GetTickCountWrap();

            INET_ASSERT(_dwWaitTimeOut != 0);
        }
        else
        {
            _dwWaitTimeOut = DEFAULT_AUTO_PROXY_THREAD_TIME_OUT;
        }
    }

    BOOL
    ChkForAndUpdateTimerCounter(
        VOID
        )
    {
        BOOL fTimetoUpdate = FALSE;

        if ( _fEnableReload )
        {
            DWORD dwMsecsElapsed = GetTickCountWrap() - _dwLastThreadAccess;

            if ( dwMsecsElapsed >= _dwWaitTimeOut )
            {
                _dwWaitTimeOut = 0;
                fTimetoUpdate = TRUE;
            }
            else
            {
                fTimetoUpdate = FALSE;
                _dwWaitTimeOut -= dwMsecsElapsed;
            }

            _dwLastThreadAccess = GetTickCountWrap();
        }

        return fTimetoUpdate;
    }


    DWORD
    ReadAutoProxyRegistrySettings(
        VOID
        );

    VOID
    SelectAutoProxy(
        AUTO_PROXY_LIST_ENTRY * pSelectedEntry
        ) 
    {
        _pSelectedAutoProxyEntry = pSelectedEntry;
    }


    VOID SetState(AUTO_PROXY_STATE apsState) {
       _apsState = apsState;
    }

    AUTO_PROXY_STATE GetState(VOID) {
        return _apsState;
    }

    //
    // _hAutoProxyThread - handle to thread that processes Auto-Proxy commands.
    //

    HANDLE _hAutoProxyThread;

    //
    // _hAutoProxyThreadEvent - handle to event that will become signaled when
    //    new auto-proxy messages are to be processed
    //

    HANDLE _hAutoProxyThreadEvent;

    //
    // _hAutoProxyStartEvent - handle to event that will be used to block
    //   until the auto-proxy thread starts to run.
    //

    HANDLE _hAutoProxyStartEvent;

    //
    // _dwAutoProxyCurrentThreadId - thread ID of our Auto-Proxy Thread.
    //

    DWORD _dwAutoProxyThreadId;

    DWORD
    DownloadProxyInfo(
        IN LPCSTR lpszAutoProxy
        );

    DWORD
    DoNestedProxyInfoDownload(
        IN LPCSTR lpszAutoProxy,
        IN WINHTTP_AUTOPROXY_RESULTS * lpProxySettings,
        IN BOOL fForceRefresh
        );

    DWORD
    StartBackroundDetectionIfNeeded(
        VOID
        );

    DWORD
    GetAutoProxyStringEntry(
        IN LPSTR lpszRegName,
        IN OUT LPSTR * lplpszAllocatedRegValue
        );

    DWORD
    GetHostAddresses(
        DWORD ** ppdwDetectedInterfaceIp,
        DWORD *  pdwDetectedInterfaceIpCount
        ) ;

    BOOL 
    IsExpiredUrl(
        LPCSTR lpszUrl
        );


    VOID
    SetExpiredUrl(
        LPCSTR lpszUrl
        );


    BOOL IsModifiedInProcess(VOID) {
        return _fModifiedInProcess;
    }

public:


    AUTO_PROXY_DLLS() {

        DEBUG_ENTER((DBG_OBJECTS,
                    None,
                    "AUTO_PROXY_DLLS",
                    ""
                    ));

        InitializeSerializedList(&_List);

        //
        // Warning: Initalize vars to NULL, BEFORE AddListFromRegistry is called.
        //

        _pSelectedAutoProxyEntry = NULL;
        _hAutoProxyStartEvent    = NULL;

        _apsState                = AUTO_PROXY_DISABLED;
        _dwWaitTimeOut           = DEFAULT_AUTO_PROXY_THREAD_TIME_OUT;
        _fEnableReload           = FALSE;
        _dwUpdatedProxySettingsVersion = 0;

        
        //
        // Zero out our settings structure
        //
        memset((void *) &_ProxySettings, 0, sizeof(_ProxySettings));

        _lpszUserAgent      = NULL;

        if (!_CritSec.Init())
        {
            _Error = ERROR_NOT_ENOUGH_MEMORY;
        }
        else
        {
            _Error = ReadAutoProxyRegistrySettings();
        }

        DEBUG_LEAVE(0);
    }

    ~AUTO_PROXY_DLLS() {

        DEBUG_ENTER((DBG_OBJECTS,
                    None,
                    "~AUTO_PROXY_DLLS",
                    NULL
                    ));

        DestroyAutoProxyDll(FALSE);    // only destroys reg vars.

        WipeProxySettings();

        TerminateSerializedList(&_List);

        _CritSec.FreeLock();

        if ( _lpszUserAgent ) {
            FREE_MEMORY(_lpszUserAgent);
        }
        
        DEBUG_LEAVE(0);
    }

    VOID
    DestroyAutoProxyDll(
        IN BOOL fUnloadDlls
        )
    {
        LockAutoProxy();

        LockSerializedList(&_List);

        while (!IsSerializedListEmpty(&_List)) {

            //
            // remove the PROXY_BYPASS_LIST_ENTRY at the head of the serialized
            // list
            //

            LPVOID entry = SlDequeueHead(&_List);
            AUTO_PROXY_LIST_ENTRY *pAutoProxyDllEntry;

            //
            // entry should not be NULL - IsSerializedListEmpty() told us we
            // could expect something
            //

            INET_ASSERT(entry != NULL);
            pAutoProxyDllEntry = CONTAINING_RECORD(entry, AUTO_PROXY_LIST_ENTRY, _List);


            if ( fUnloadDlls )
            {
                pAutoProxyDllEntry->UnloadEntry();
            }

            delete pAutoProxyDllEntry;
        }

        UnlockSerializedList(&_List);

        UnlockAutoProxy();
    }

    VOID
    WipeProxySettings(
        WINHTTP_AUTOPROXY_RESULTS * lpProxySettings
        );

    VOID
    WipeProxySettings(VOID) {
        WipeProxySettings(&_ProxySettings);
    }

    DWORD GetError(VOID) const {
        return _Error;
    }

    BOOL
    SelectAutoProxyByFileExtension(
        LPCSTR lpszAutoProxyPath
        );

    BOOL
    SelectAutoProxyByDefault(
        VOID
        );

    BOOL
    SelectAutoProxyByMime(
        IN LPSTR lpszMimeType
        );

    BOOL
    IsAutoProxy(
        VOID
        )
    {
        BOOL fIsAutoProxy;

        LockAutoProxy();

        fIsAutoProxy = ! IsSerializedListEmpty(&_List);

        UnlockAutoProxy();

        return fIsAutoProxy;
    }


    AUTO_PROXY_LIST_ENTRY *
    GetSelectedAutoProxyEntry(
        VOID
        )
    {
        return _pSelectedAutoProxyEntry;
    }

    BOOL LockAutoProxy(VOID) {
        return _CritSec.Lock();
    }

    VOID UnlockAutoProxy(VOID) {
        _CritSec.Unlock();
    }

    BOOL IsProxyAutoDetectEnabled(WINHTTP_AUTOPROXY_RESULTS * lpProxySettings) {
        return (lpProxySettings->dwFlags & PROXY_TYPE_AUTO_DETECT);
    }

    BOOL IsProxyAutoDetectEnabled(VOID) {
        return (IsProxyAutoDetectEnabled(&_ProxySettings));
    }

    BOOL IsProxyAutoConfigEnabled(WINHTTP_AUTOPROXY_RESULTS * lpProxySettings) {
        return (lpProxySettings->dwFlags & PROXY_TYPE_AUTO_PROXY_URL);
    }

    BOOL 
    IsConfigValidForAutoProxyThread(
        VOID
        );

    BOOL 
    IsProxyAutoDetectNeeded(
        WINHTTP_AUTOPROXY_RESULTS * lpProxySettings
        );

    DWORD 
    CheckForTimerConfigChanges(
        IN DWORD dwMinsToPoll
        );

    DWORD
    ProcessProxyQueryForInfo(
        IN OUT AUTO_PROXY_ASYNC_MSG * ppQueryForInfo
        ) ;

    DWORD 
    GetProxySettings(
        OUT WINHTTP_AUTOPROXY_RESULTS *  lpProxySettings,
        IN BOOL fCheckVersion = FALSE
        );

    DWORD
    QueryProxySettings(
        IN OUT AUTO_PROXY_ASYNC_MSG **ppQueryForInfo
        );

    DWORD
    StartDownloadOfProxyInfo(
        IN WINHTTP_AUTOPROXY_OPTIONS * pAutoProxyOptions
        );
};



//
// external func declariations, note that the DLL does not have to export the full set of these
//  functions, rather the DLL can export only the functions it impliments.
//



BOOL
CALLBACK
InternetProxyInfoInvalid (  // NOT implimented
    IN LPSTR lpszMime,
    IN LPSTR lpszUrl,
    IN DWORD dwUrlLength,
    IN LPSTR lpszProxyHostName,
    IN DWORD dwProxyHostNameLength
    );

BOOL
CALLBACK
InternetDeInitializeAutoProxyDll(
    IN LPSTR lpszMime,
    IN DWORD dwReserved
    );

BOOL
CALLBACK
InternetGetProxyInfo(
    IN LPCSTR lpszUrl,
    IN DWORD dwUrlLength,
    IN LPSTR lpszUrlHostName,
    IN DWORD dwUrlHostNameLength,
    OUT LPSTR * lplpszProxyHostName,
    OUT LPDWORD lpdwProxyHostNameLength
    ) ;


BOOL
CALLBACK
InternetGetProxyInfoEx(
    IN INTERNET_SCHEME tUrlProtocol,
    IN LPCSTR lpszUrl,
    IN DWORD dwUrlLength,
    IN LPSTR lpszUrlHostName,
    IN DWORD dwUrlHostNameLength,
    IN INTERNET_PORT nUrlPort,
    OUT LPINTERNET_SCHEME lptProxyScheme,
    OUT LPSTR * lplpszProxyHostName,
    OUT LPDWORD lpdwProxyHostNameLength,
    OUT LPINTERNET_PORT lpProxyHostPort
    ) ;

BOOL
CALLBACK
InternetInitializeAutoProxyDll(
    IN DWORD                  dwVersion,
    IN LPSTR                  lpszDownloadedTempFile,
    IN LPSTR                  lpszMime,
    IN AUTO_PROXY_HELPER_APIS *pAutoProxyCallbacks,
    IN DWORD                  dwReserved
    );

DWORD
DetectAutoProxyUrl(
    IN  DWORD   dwDetectFlags,
    OUT LPSTR * lpszAutoProxyUrl
    );


#ifdef unix
extern "C"
#endif /* unix */
BOOL
CALLBACK
InternetInitializeAutoProxyDll(
    DWORD dwReserved
    );

#endif /* _AUTOPROX_HXX_ */
