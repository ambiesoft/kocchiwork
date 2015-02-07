#include "stdafx.h"

HICON g_hTrayIcon;
HWND g_hWnd;
HANDLE g_hKanshiApp;
WIN32_FIND_DATA g_wfdRemote;
WIN32_FIND_DATA g_wfdWork;

tstring g_workfile;
tstring g_remotefile;

DWORD g_dwRemoteSize;
time_t g_starttime;
