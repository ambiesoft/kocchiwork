extern HICON g_hTrayIcon;
extern HWND g_hWnd;
extern HANDLE g_hKanshiApp;
extern WIN32_FIND_DATA g_wfdRemote;
extern WIN32_FIND_DATA g_wfdWork;

extern tstring g_workfile;
extern tstring g_remotefile;
extern tstring g_progfile;

extern DWORD g_dwRemoteSize;
extern time_t g_starttime;


LRESULT CALLBACK WndProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam);
bool  ReturnFileAndQuit(HWND hWnd);