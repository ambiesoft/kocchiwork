#include "stdafx.h"

HICON g_hTrayIcon;
HWND g_hWnd;
HANDLE g_hKanshiApp;
WIN32_FIND_DATA g_wfdRemote;
WIN32_FIND_DATA g_wfdWork;

tstring g_workfile;
tstring g_remotefile;
tstring g_progfile;

DWORD g_dwRemoteSize;
time_t g_starttime;

BOOL g_bQuerying;
BOOL g_bQuittedWhileQuerying;

void doPostQuitMessage(int nQM)
{
	if(g_bQuerying)
	{
		g_bQuittedWhileQuerying = TRUE;
		return;
	}
	PostQuitMessage(nQM);
}
