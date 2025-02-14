// WTL Version 3.0
// Copyright (C) 1997-1999 Microsoft Corporation
// All rights reserved.
//
// This file is a part of Windows Template Library.
// The code and information is provided "as-is" without
// warranty of any kind, either expressed or implied.

#ifndef __ATLAPP_H__
#define __ATLAPP_H__

#pragma once

#ifndef __cplusplus
	#error ATL requires C++ compilation (use a .cpp suffix)
#endif

#ifndef __ATLBASE_H__
	#error atlapp.h requires atlbase.h to be included first
#endif

#if (WINVER < 0x0400)
	#error WTL requires Windows version 4.0 or higher
#endif


#include <limits.h>
#if !defined(_ATL_MIN_CRT) & defined(_MT)
#include <process.h>	// for _beginthreadex, _endthreadex
#endif

#include <commctrl.h>
#pragma comment(lib, "comctl32.lib")

#include <atlres.h>


// WTL version number
#define _WTL_VER	0x0300


// This is to support using original VC++ 6.0 headers with WTL
#ifndef _ATL_NO_OLD_HEADERS_WIN64
#if !defined(_WIN64) && !defined(_ATL_TMP_IMPL2)

  #ifndef GetWindowLongPtr
    #define GetWindowLongPtrA   GetWindowLongA
    #define GetWindowLongPtrW   GetWindowLongW
    #ifdef UNICODE
      #define GetWindowLongPtr  GetWindowLongPtrW
    #else
      #define GetWindowLongPtr  GetWindowLongPtrA
    #endif // !UNICODE
  #endif // !GetWindowLongPtr

  #ifndef SetWindowLongPtr
    #define SetWindowLongPtrA   SetWindowLongA
    #define SetWindowLongPtrW   SetWindowLongW
    #ifdef UNICODE
      #define SetWindowLongPtr  SetWindowLongPtrW
    #else
      #define SetWindowLongPtr  SetWindowLongPtrA
    #endif // !UNICODE
  #endif // !SetWindowLongPtr

  #ifndef GWLP_WNDPROC
    #define GWLP_WNDPROC        (-4)
  #endif
  #ifndef GWLP_HINSTANCE
    #define GWLP_HINSTANCE      (-6)
  #endif
  #ifndef GWLP_HWNDPARENT
    #define GWLP_HWNDPARENT     (-8)
  #endif
  #ifndef GWLP_USERDATA
    #define GWLP_USERDATA       (-21)
  #endif
  #ifndef GWLP_ID
    #define GWLP_ID             (-12)
  #endif

  typedef long LONG_PTR;
  typedef unsigned long ULONG_PTR;
  typedef ULONG_PTR DWORD_PTR;

  #ifndef HandleToUlong
#define HandleToUlong( h ) ((ULONG)(ULONG_PTR)(h) )
  #endif
  #ifndef HandleToLong
#define HandleToLong( h ) ((LONG)(LONG_PTR) (h) )
  #endif
  #ifndef LongToHandle
#define LongToHandle( h) ((HANDLE)(LONG_PTR) (h))
  #endif
  #ifndef PtrToUlong
#define PtrToUlong( p ) ((ULONG)(ULONG_PTR) (p) )
  #endif
  #ifndef PtrToLong
#define PtrToLong( p ) ((LONG)(LONG_PTR) (p) )
  #endif
  #ifndef PtrToUint
#define PtrToUint( p ) ((UINT)(UINT_PTR) (p) )
  #endif
  #ifndef PtrToInt
#define PtrToInt( p ) ((INT)(INT_PTR) (p) )
  #endif
  #ifndef PtrToUshort
#define PtrToUshort( p ) ((unsigned short)(ULONG_PTR)(p) )
  #endif
  #ifndef PtrToShort
#define PtrToShort( p ) ((short)(LONG_PTR)(p) )
  #endif
  #ifndef IntToPtr
#define IntToPtr( i )    ((VOID *)(INT_PTR)((int)i))
  #endif
  #ifndef UIntToPtr
#define UIntToPtr( ui )  ((VOID *)(UINT_PTR)((unsigned int)ui))
  #endif
  #ifndef LongToPtr
#define LongToPtr( l )   ((VOID *)(LONG_PTR)((long)l))
  #endif
  #ifndef ULongToPtr
#define ULongToPtr( ul )  ((VOID *)(ULONG_PTR)((unsigned long)ul))
  #endif

#endif //!defined(_WIN64) && !defined(_ATL_TMP_IMPL2)
#endif //!_ATL_NO_OLD_HEADERS_WIN64


namespace WTL
{

enum wtlTraceFlags
{
	atlTraceUI = 0x10000000
};

// Windows version helper
inline bool AtlIsOldWindows()
{
	OSVERSIONINFO ovi;
	ovi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	BOOL bRet = ::GetVersionEx(&ovi);
	return (!bRet || !((ovi.dwMajorVersion >= 5) || (ovi.dwMajorVersion == 4 && ovi.dwMinorVersion >= 90)));
}

/////////////////////////////////////////////////////////////////////////////
// Forward declarations

class CMessageFilter;
class CIdleHandler;
class CMessageLoop;
class CAppModule;
class CServerAppModule;

#ifndef _ATL_TMP_IMPL2
class _U_RECT;
class _U_MENUorID;
class _U_STRINGorID;
#endif //!_ATL_TMP_IMPL2


/////////////////////////////////////////////////////////////////////////////
// CMessageFilter - Interface for message filter support

class CMessageFilter
{
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg) = 0;
};


/////////////////////////////////////////////////////////////////////////////
// CIdleHandler - Interface for idle processing

class CIdleHandler
{
public:
	virtual BOOL OnIdle() = 0;
};

#ifndef _ATL_NO_OLD_NAMES
// for compatilibility with old names only
typedef CIdleHandler	CUpdateUIObject;
#define DoUpdate	OnIdle
#endif //!_ATL_NO_OLD_NAMES


/////////////////////////////////////////////////////////////////////////////
// CMessageLoop - message loop implementation

class CMessageLoop
{
public:
	CSimpleArray<CMessageFilter*> m_aMsgFilter;
	CSimpleArray<CIdleHandler*> m_aIdleHandler;
	MSG m_msg;

// Message filter operations
	BOOL AddMessageFilter(CMessageFilter* pMessageFilter)
	{
		return m_aMsgFilter.Add(pMessageFilter);
	}
	BOOL RemoveMessageFilter(CMessageFilter* pMessageFilter)
	{
		return m_aMsgFilter.Remove(pMessageFilter);
	}
// Idle handler operations
	BOOL AddIdleHandler(CIdleHandler* pIdleHandler)
	{
		return m_aIdleHandler.Add(pIdleHandler);
	}
	BOOL RemoveIdleHandler(CIdleHandler* pIdleHandler)
	{
		return m_aIdleHandler.Remove(pIdleHandler);
	}
#ifndef _ATL_NO_OLD_NAMES
	// for compatilibility with old names only
	BOOL AddUpdateUI(CIdleHandler* pIdleHandler)
	{
		ATLTRACE2(atlTraceUI, 0, "CUpdateUIObject and AddUpdateUI are deprecated. Please change your code to use CIdleHandler and OnIdle\n");
		return AddIdleHandler(pIdleHandler);
	}
	BOOL RemoveUpdateUI(CIdleHandler* pIdleHandler)
	{
		ATLTRACE2(atlTraceUI, 0, "CUpdateUIObject and RemoveUpdateUI are deprecated. Please change your code to use CIdleHandler and OnIdle\n");
		return RemoveIdleHandler(pIdleHandler);
	}
#endif //!_ATL_NO_OLD_NAMES
// message loop
	int Run()
	{
		BOOL bDoIdle = TRUE;
		int nIdleCount = 0;
		BOOL bRet;

		for(;;)
		{
			while(!::PeekMessage(&m_msg, NULL, 0, 0, PM_NOREMOVE) && bDoIdle)
			{
				if(!OnIdle(nIdleCount++))
					bDoIdle = FALSE;
			}

			bRet = ::GetMessage(&m_msg, NULL, 0, 0);

			if(bRet == -1)
			{
				ATLTRACE2(atlTraceUI, 0, _T("::GetMessage returned -1 (error)\n"));
				continue;	// error, don't process
			}
			else if(!bRet)
			{
				ATLTRACE2(atlTraceUI, 0, _T("CMessageLoop::Run - exiting\n"));
				break;		// WM_QUIT, exit message loop
			}

			if(!PreTranslateMessage(&m_msg))
			{
				::TranslateMessage(&m_msg);
				::DispatchMessage(&m_msg);
			}

			if(IsIdleMessage(&m_msg))
			{
				bDoIdle = TRUE;
				nIdleCount = 0;
			}
		}

		return (int)m_msg.wParam;
	}

	static BOOL IsIdleMessage(MSG* pMsg)
	{
		// These messages should NOT cause idle processing
		switch(pMsg->message)
		{
		case WM_MOUSEMOVE:
		case WM_NCMOUSEMOVE:
		case WM_PAINT:
		case 0x0118:	// WM_SYSTIMER (caret blink)
			return FALSE;
		}

		return TRUE;
	}

// Overrideables
	// Override to change message filtering
	virtual BOOL PreTranslateMessage(MSG* pMsg)
	{
		// loop backwards
		for(int i = m_aMsgFilter.GetSize() - 1; i >= 0; i--)
		{
			CMessageFilter* pMessageFilter = m_aMsgFilter[i];
			if(pMessageFilter != NULL && pMessageFilter->PreTranslateMessage(pMsg))
				return TRUE;
		}
		return FALSE;	// not translated
	}
	// override to change idle processing
	virtual BOOL OnIdle(int /*nIdleCount*/)
	{
		for(int i = 0; i < m_aIdleHandler.GetSize(); i++)
		{
			CIdleHandler* pIdleHandler = m_aIdleHandler[i];
			if(pIdleHandler != NULL)
				pIdleHandler->OnIdle();
		}
		return FALSE;	// don't continue
	}
};


/////////////////////////////////////////////////////////////////////////////
// CAppModule - module class for an application

class CAppModule : public CComModule
{
public:
	DWORD m_dwMainThreadID;
	CSimpleMap<DWORD, CMessageLoop*>* m_pMsgLoopMap;
	CSimpleArray<HWND>* m_pSettingChangeNotify;

// Overrides of CComModule::Init and Term
	HRESULT Init(_ATL_OBJMAP_ENTRY* pObjMap, HINSTANCE hInstance, const GUID* pLibID = NULL)
	{
		HRESULT hRet = CComModule::Init(pObjMap, hInstance, pLibID);
		if(FAILED(hRet))
			return hRet;

		m_dwMainThreadID = ::GetCurrentThreadId();
		typedef CSimpleMap<DWORD, CMessageLoop*>	mapClass;
		m_pMsgLoopMap = NULL;
		ATLTRY(m_pMsgLoopMap = new mapClass);
		if(m_pMsgLoopMap == NULL)
			return E_OUTOFMEMORY;
		m_pSettingChangeNotify = NULL;

		return hRet;
	}
	void Term()
	{
		if(m_pSettingChangeNotify != NULL && m_pSettingChangeNotify->GetSize() > 0)
		{
			::DestroyWindow((*m_pSettingChangeNotify)[0]);
		}
		delete m_pSettingChangeNotify;
		delete m_pMsgLoopMap;
		CComModule::Term();
	}

// Message loop map methods
	BOOL AddMessageLoop(CMessageLoop* pMsgLoop)
	{
		ATLASSERT(pMsgLoop != NULL);
		ATLASSERT(m_pMsgLoopMap->Lookup(::GetCurrentThreadId()) == NULL);	// not in map yet
		return m_pMsgLoopMap->Add(::GetCurrentThreadId(), pMsgLoop);
	}
	BOOL RemoveMessageLoop()
	{
		return m_pMsgLoopMap->Remove(::GetCurrentThreadId());
	}
	CMessageLoop* GetMessageLoop(DWORD dwThreadID = ::GetCurrentThreadId()) const
	{
		return m_pMsgLoopMap->Lookup(dwThreadID);
	}

// Setting change notify methods
	BOOL AddSettingChangeNotify(HWND hWnd)
	{
		ATLASSERT(::IsWindow(hWnd));
		if(m_pSettingChangeNotify == NULL)
		{
			typedef CSimpleArray<HWND>	notifyClass;
			ATLTRY(m_pSettingChangeNotify = new notifyClass);
			ATLASSERT(m_pSettingChangeNotify != NULL);
			if(m_pSettingChangeNotify == NULL)
				return FALSE;
		}
		if(m_pSettingChangeNotify->GetSize() == 0)
		{
			// init everything
			_ATL_EMPTY_DLGTEMPLATE templ;
			HWND hNtfWnd = ::CreateDialogIndirect(GetModuleInstance(), &templ, NULL, _SettingChangeDlgProc);
			ATLASSERT(::IsWindow(hNtfWnd));
			if(::IsWindow(hNtfWnd))
			{
				::SetWindowLongPtr(hNtfWnd, GWLP_USERDATA, (LONG_PTR)this);
				m_pSettingChangeNotify->Add(hNtfWnd);
			}
		}
		return m_pSettingChangeNotify->Add(hWnd);
	}

	BOOL RemoveSettingChangeNotify(HWND hWnd)
	{
		if(m_pSettingChangeNotify == NULL)
			return FALSE;
		return m_pSettingChangeNotify->Remove(hWnd);
	}

// Implementation - setting change notify dialog template and dialog procedure
	struct _ATL_EMPTY_DLGTEMPLATE : DLGTEMPLATE
	{
		_ATL_EMPTY_DLGTEMPLATE()
		{
			memset(this, 0, sizeof(_ATL_EMPTY_DLGTEMPLATE));
			style = WS_POPUP;
		}
		WORD wMenu, wClass, wTitle;
	};

#ifdef _WIN64
	static INT_PTR CALLBACK _SettingChangeDlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
#else
	static BOOL CALLBACK _SettingChangeDlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
#endif
	{
		if(uMsg == WM_SETTINGCHANGE)
		{
			CAppModule* pModule = (CAppModule*)::GetWindowLongPtr(hWnd, GWLP_USERDATA);
			ATLASSERT(pModule != NULL);
			ATLASSERT(pModule->m_pSettingChangeNotify != NULL);
			for(int i = 1; i < pModule->m_pSettingChangeNotify->GetSize(); i++)
				::SendMessageTimeout((*pModule->m_pSettingChangeNotify)[i], uMsg, wParam, lParam, SMTO_ABORTIFHUNG, 1500, NULL);
			return TRUE;
		}
		return FALSE;
	}
};


/////////////////////////////////////////////////////////////////////////////
// CServerAppModule - module class for a COM server application

class CServerAppModule : public CAppModule
{
public:
	HANDLE m_hEventShutdown;
	bool m_bActivity;
	DWORD m_dwTimeOut;
	DWORD m_dwPause;

// Override of CAppModule::Init
	HRESULT Init(_ATL_OBJMAP_ENTRY* pObjMap, HINSTANCE hInstance, const GUID* pLibID = NULL)
	{
		m_dwTimeOut = 5000;
		m_dwPause = 1000;
		return CAppModule::Init(pObjMap, hInstance, pLibID);
	}
	void Term()
	{
		if (m_hEventShutdown != NULL)
			CloseHandle(m_hEventShutdown);
		CAppModule::Term();
	}

// COM Server methods
	LONG Unlock()
	{
		LONG lRet = CComModule::Unlock();
		if(lRet == 0)
		{
			m_bActivity = true;
			::SetEvent(m_hEventShutdown); // tell monitor that we transitioned to zero
		}
		return lRet;
	}

	void MonitorShutdown()
	{
		while(1)
		{
			::WaitForSingleObject(m_hEventShutdown, INFINITE);
			DWORD dwWait = 0;
			do
			{
				m_bActivity = false;
				dwWait = ::WaitForSingleObject(m_hEventShutdown, m_dwTimeOut);
			}
			while(dwWait == WAIT_OBJECT_0);
			// timed out
			if(!m_bActivity && m_nLockCnt == 0) // if no activity let's really bail
			{
#if ((_WIN32_WINNT >= 0x0400 ) || defined(_WIN32_DCOM)) & defined(_ATL_FREE_THREADED)
				::CoSuspendClassObjects();
				if(!m_bActivity && m_nLockCnt == 0)
#endif
					break;
			}
		}
		::CloseHandle(m_hEventShutdown);
		::PostThreadMessage(m_dwMainThreadID, WM_QUIT, 0, 0);
	}

	bool StartMonitor()
	{
		m_hEventShutdown = ::CreateEvent(NULL, false, false, NULL);
		if(m_hEventShutdown == NULL)
			return false;
		DWORD dwThreadID;
#if !defined(_ATL_MIN_CRT) & defined(_MT)
		HANDLE h = (HANDLE)_beginthreadex(NULL, 0, (UINT (WINAPI*)(void*))MonitorProc, this, 0, (UINT*)&dwThreadID);
#else
		HANDLE h = ::CreateThread(NULL, 0, MonitorProc, this, 0, &dwThreadID);
#endif
		bool b = (h != NULL);
		if (b)
			CloseHandle(h);
		return b;
	}

	static DWORD WINAPI MonitorProc(void* pv)
	{
		CServerAppModule* p = (CServerAppModule*)pv;
		p->MonitorShutdown();
#if !defined(_ATL_MIN_CRT) & defined(_MT)
		_endthreadex(0);
#endif
		return 0;
	}

	// Scan command line and perform registration
	// Return value specifies if server should run

	// Parses the command line and registers/unregisters the rgs file if necessary
	bool ParseCommandLine(LPCTSTR lpCmdLine, UINT nResId, HRESULT* pnRetCode)
	{
		TCHAR szTokens[] = _T("-/");
		*pnRetCode = S_OK;

		LPCTSTR lpszToken = FindOneOf(lpCmdLine, szTokens);
		while(lpszToken != NULL)
		{
			if(lstrcmpi(lpszToken, _T("UnregServer"))==0)
			{
				*pnRetCode = UnregisterServer(TRUE);
				ATLASSERT(SUCCEEDED(*pnRetCode));
				if(FAILED(*pnRetCode))
					return false;
				*pnRetCode = UpdateRegistryFromResource(nResId, FALSE);
				return false;
			}

			// Register as Local Server
			if(lstrcmpi(lpszToken, _T("RegServer"))==0)
			{
				*pnRetCode = UpdateRegistryFromResource(nResId, TRUE);
				ATLASSERT(SUCCEEDED(*pnRetCode));
				if(FAILED(*pnRetCode))
					return false;
				*pnRetCode = RegisterServer(TRUE);
				return false;
			}

			lpszToken = FindOneOf(lpszToken, szTokens);
		}
		return true;
	}
	
	// Parses the command line and registers/unregisters the appid if necessary
	bool ParseCommandLine(LPCTSTR lpCmdLine, LPCTSTR pAppId, HRESULT* pnRetCode)
	{
		TCHAR szTokens[] = _T("-/");
		*pnRetCode = S_OK;

		LPCTSTR lpszToken = FindOneOf(lpCmdLine, szTokens);
		while(lpszToken != NULL)
		{
			if(lstrcmpi(lpszToken, _T("UnregServer"))==0)
			{
				*pnRetCode = UnregisterAppId(pAppId);
				ATLASSERT(SUCCEEDED(*pnRetCode));
				if(FAILED(*pnRetCode))
					return false;
				*pnRetCode = UnregisterServer(TRUE);
				return false;
			}

			// Register as Local Server
			if(lstrcmpi(lpszToken, _T("RegServer"))==0)
			{
				*pnRetCode = RegisterAppId(pAppId);
				ATLASSERT(SUCCEEDED(*pnRetCode));
				if(FAILED(*pnRetCode))
					return false;
				*pnRetCode = RegisterServer(TRUE);
				return false;
			}

			lpszToken = FindOneOf(lpszToken, szTokens);
		}
		return true;
	}

#if !defined(_ATL_TMP_IMPL1) && !defined(_ATL_TMP_IMPL2)
	// search for an occurence of string p2 in string p1
	static LPCTSTR FindOneOf(LPCTSTR p1, LPCTSTR p2)
	{
		while(p1 != NULL && *p1 != NULL)
		{
			LPCTSTR p = p2;
			while(p != NULL && *p != NULL)
			{
				if(*p1 == *p)
					return ::CharNext(p1);
				p = ::CharNext(p);
			}
			p1 = ::CharNext(p1);
		}
		return NULL;
	}

	HRESULT RegisterAppId(LPCTSTR pAppId)
	{
		CRegKey keyAppID;
		if(keyAppID.Open(HKEY_CLASSES_ROOT, _T("AppID"), KEY_READ) == ERROR_SUCCESS)
		{
			TCHAR szModule1[_MAX_PATH];
			TCHAR szModule2[_MAX_PATH];
			TCHAR* pszFileName;
			::GetModuleFileName(GetModuleInstance(), szModule1, _MAX_PATH);
			::GetFullPathName(szModule1, _MAX_PATH, szModule2, &pszFileName);
			CRegKey keyAppIDEXE;
			if(keyAppIDEXE.Create(keyAppID, pszFileName, REG_NONE, REG_OPTION_NON_VOLATILE, KEY_READ | KEY_WRITE) == ERROR_SUCCESS)
				keyAppIDEXE.SetValue(pAppId, _T("AppID"));
			if(keyAppIDEXE.Create(keyAppID, pAppId, REG_NONE, REG_OPTION_NON_VOLATILE, KEY_READ | KEY_WRITE) == ERROR_SUCCESS)
				keyAppIDEXE.SetValue(pszFileName);
		}
		return S_OK;
	}

	HRESULT UnregisterAppId(LPCTSTR pAppId)
	{
		CRegKey keyAppID;
		if(keyAppID.Open(HKEY_CLASSES_ROOT, _T("AppID"), KEY_READ) == ERROR_SUCCESS)
		{
			TCHAR szModule1[_MAX_PATH];
			TCHAR szModule2[_MAX_PATH];
			TCHAR* pszFileName;
			::GetModuleFileName(GetModuleInstance(), szModule1, _MAX_PATH);
			::GetFullPathName(szModule1, _MAX_PATH, szModule2, &pszFileName);
			keyAppID.RecurseDeleteKey(pszFileName);
			keyAppID.RecurseDeleteKey(pAppId);
		}
		return S_OK;
	}
#endif //!defined(_ATL_TMP_IMPL1) && !defined(_ATL_TMP_IMPL2)
};


/////////////////////////////////////////////////////////////////////////////
// ATL 3.0 Add-ons

// protect template members from windowsx.h macros
#ifdef _INC_WINDOWSX
#undef SubclassWindow
#endif //_INC_WINDOWSX

// define useful macros from windowsx.h
#ifndef GET_X_LPARAM
#define GET_X_LPARAM(lParam)	((int)(short)LOWORD(lParam))
#endif
#ifndef GET_Y_LPARAM
#define GET_Y_LPARAM(lParam)	((int)(short)HIWORD(lParam))
#endif


/////////////////////////////////////////////////////////////////////////////
// Dual argument helper classes

#ifndef _ATL_TMP_IMPL2

class _U_RECT
{
public:
	_U_RECT(LPRECT lpRect) : m_lpRect(lpRect)
	{ }
	_U_RECT(RECT& rc) : m_lpRect(&rc)
	{ }
	LPRECT m_lpRect;
};

class _U_MENUorID
{
public:
	_U_MENUorID(HMENU hMenu) : m_hMenu(hMenu)
	{ }
	_U_MENUorID(UINT nID) : m_hMenu((HMENU)LongToHandle(nID))
	{ }
	HMENU m_hMenu;
};

class _U_STRINGorID
{
public:
	_U_STRINGorID(LPCTSTR lpString) : m_lpstr(lpString)
	{ }
	_U_STRINGorID(UINT nID) : m_lpstr(MAKEINTRESOURCE(nID))
	{ }
	LPCTSTR m_lpstr;
};

#endif //!_ATL_TMP_IMPL2


/////////////////////////////////////////////////////////////////////////////
// Forward notifications support for message maps

#if !defined(_ATL_TMP_IMPL1) && !defined(_ATL_TMP_IMPL2)

// forward notifications support
#define FORWARD_NOTIFICATIONS() \
	{ \
		bHandled = TRUE; \
		lResult = Atl3ForwardNotifications(m_hWnd, uMsg, wParam, lParam, bHandled); \
		if(bHandled) \
			return TRUE; \
	}

static LRESULT Atl3ForwardNotifications(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LRESULT lResult = 0;
	switch(uMsg)
	{
	case WM_COMMAND:
	case WM_NOTIFY:
	case WM_PARENTNOTIFY:
	case WM_DRAWITEM:
	case WM_MEASUREITEM:
	case WM_COMPAREITEM:
	case WM_DELETEITEM:
	case WM_VKEYTOITEM:
	case WM_CHARTOITEM:
	case WM_HSCROLL:
	case WM_VSCROLL:
	case WM_CTLCOLORBTN:
	case WM_CTLCOLORDLG:
	case WM_CTLCOLOREDIT:
	case WM_CTLCOLORLISTBOX:
	case WM_CTLCOLORMSGBOX:
	case WM_CTLCOLORSCROLLBAR:
	case WM_CTLCOLORSTATIC:
		lResult = ::SendMessage(::GetParent(hWnd), uMsg, wParam, lParam);
		break;
	default:
		bHandled = FALSE;
		break;
	}
	return lResult;
}


/////////////////////////////////////////////////////////////////////////////
// Reflected message handler macros for message maps

#define REFLECTED_COMMAND_HANDLER(id, code, func) \
	if(uMsg == OCM_COMMAND && id == LOWORD(wParam) && code == HIWORD(wParam)) \
	{ \
		bHandled = TRUE; \
		lResult = func(HIWORD(wParam), LOWORD(wParam), (HWND)lParam, bHandled); \
		if(bHandled) \
			return TRUE; \
	}

#define REFLECTED_COMMAND_ID_HANDLER(id, func) \
	if(uMsg == OCM_COMMAND && id == LOWORD(wParam)) \
	{ \
		bHandled = TRUE; \
		lResult = func(HIWORD(wParam), LOWORD(wParam), (HWND)lParam, bHandled); \
		if(bHandled) \
			return TRUE; \
	}

#define REFLECTED_COMMAND_CODE_HANDLER(code, func) \
	if(uMsg == OCM_COMMAND && code == HIWORD(wParam)) \
	{ \
		bHandled = TRUE; \
		lResult = func(HIWORD(wParam), LOWORD(wParam), (HWND)lParam, bHandled); \
		if(bHandled) \
			return TRUE; \
	}

#define REFLECTED_COMMAND_RANGE_HANDLER(idFirst, idLast, func) \
	if(uMsg == OCM_COMMAND && LOWORD(wParam) >= idFirst  && LOWORD(wParam) <= idLast) \
	{ \
		bHandled = TRUE; \
		lResult = func(HIWORD(wParam), LOWORD(wParam), (HWND)lParam, bHandled); \
		if(bHandled) \
			return TRUE; \
	}

#define REFLECTED_COMMAND_RANGE_CODE_HANDLER(idFirst, idLast, code, func) \
	if(uMsg == OCM_COMMAND && code == HIWORD(wParam) && LOWORD(wParam) >= idFirst  && LOWORD(wParam) <= idLast) \
	{ \
		bHandled = TRUE; \
		lResult = func(HIWORD(wParam), LOWORD(wParam), (HWND)lParam, bHandled); \
		if(bHandled) \
			return TRUE; \
	}

#define REFLECTED_NOTIFY_HANDLER(id, cd, func) \
	if(uMsg == OCM_NOTIFY && id == ((LPNMHDR)lParam)->idFrom && cd == ((LPNMHDR)lParam)->code) \
	{ \
		bHandled = TRUE; \
		lResult = func((int)wParam, (LPNMHDR)lParam, bHandled); \
		if(bHandled) \
			return TRUE; \
	}

#define REFLECTED_NOTIFY_ID_HANDLER(id, func) \
	if(uMsg == OCM_NOTIFY && id == ((LPNMHDR)lParam)->idFrom) \
	{ \
		bHandled = TRUE; \
		lResult = func((int)wParam, (LPNMHDR)lParam, bHandled); \
		if(bHandled) \
			return TRUE; \
	}

#define REFLECTED_NOTIFY_CODE_HANDLER(cd, func) \
	if(uMsg == OCM_NOTIFY && cd == ((LPNMHDR)lParam)->code) \
	{ \
		bHandled = TRUE; \
		lResult = func((int)wParam, (LPNMHDR)lParam, bHandled); \
		if(bHandled) \
			return TRUE; \
	}

#define REFLECTED_NOTIFY_RANGE_HANDLER(idFirst, idLast, func) \
	if(uMsg == OCM_NOTIFY && ((LPNMHDR)lParam)->idFrom >= idFirst && ((LPNMHDR)lParam)->idFrom <= idLast) \
	{ \
		bHandled = TRUE; \
		lResult = func((int)wParam, (LPNMHDR)lParam, bHandled); \
		if(bHandled) \
			return TRUE; \
	}

#define REFLECTED_NOTIFY_RANGE_CODE_HANDLER(idFirst, idLast, cd, func) \
	if(uMsg == OCM_NOTIFY && cd == ((LPNMHDR)lParam)->code && ((LPNMHDR)lParam)->idFrom >= idFirst && ((LPNMHDR)lParam)->idFrom <= idLast) \
	{ \
		bHandled = TRUE; \
		lResult = func((int)wParam, (LPNMHDR)lParam, bHandled); \
		if(bHandled) \
			return TRUE; \
	}

#endif //!defined(_ATL_TMP_IMPL1) && !defined(_ATL_TMP_IMPL2)

}; //namespace WTL

// These are always included
#include <atluser.h>
#include <atlgdi.h>

// old names compatibility
#ifdef _ATL_TMP_IMPL1
namespace WTL
{
typedef CFontHandle		CWinFont;
typedef CMenuHandle		CWinMenu;
#define CWinPropertyPageImpl	CPropertyPageImpl
typedef CDCHandle		CWinDC;
}; //namespace WTL
#endif //_ATL_TMP_IMPL1

#ifndef _WTL_NO_AUTOMATIC_NAMESPACE
using namespace WTL;
#endif //!_WTL_NO_AUTOMATIC_NAMESPACE

#endif // __ATLAPP_H__
