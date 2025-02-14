#define IDC_DUMMY                   -1
#define IDD_ABOUT	100
#define IDD_FIND	101
#define IDD_INSTALL	102
#define IDD_INFO	103
#define IDD_QUERY	104
#define IDD_LANG	105

#define IDC_FILENAME    201
#define IDC_SUBLANGID   202
#define IDC_LANGID      203

#define IDM_VERSION	300

#define IDM_ABOUT	301
#define IDM_EXIT	302
#define IDM_FIND	303
#define IDM_INSTALL	304
#define IDM_INFO	305
#define IDM_QUERY	306
#define IDM_LANG	307
#define IDM_FREE	308

BOOL	InitApplication(HANDLE);
BOOL	InitInstance(HANDLE, int);
LRESULT	MainWndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR	About(HWND, UINT, WPARAM, LPARAM);
INT_PTR	Find(HWND, UINT, WPARAM, LPARAM);
INT_PTR	Install(HWND, UINT, WPARAM, LPARAM);
INT_PTR	Information(HWND, UINT, WPARAM, LPARAM);
INT_PTR	Query(HWND, UINT, WPARAM, LPARAM);
INT_PTR	Language(HWND, UINT, WPARAM, LPARAM);
