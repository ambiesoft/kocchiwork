#include "stdafx.h"
#include "tmphelp.h"
#include "../../MyUtility/HelpDefines.h"

BOOL GetFileData(LPCTSTR pFile, WIN32_FIND_DATA* pData)
{
	HANDLE h = FindFirstFile(pFile, pData);
	if(INVALID_HANDLE_VALUE == h)
	{
		return FALSE;
	}
	FindClose(h);
	return TRUE;
}
BOOL GetFileLastWriteTime(LPCTSTR pFile, FILETIME* pFT)
{
	WIN32_FIND_DATA wfd;
	if(!GetFileData(pFile, &wfd))
		return FALSE;

	*pFT = wfd.ftLastWriteTime;
	return TRUE;
}

BOOL AddTrayIcon(HWND hWnd, UINT dwIDandCallbackMessage, HICON hIcon, LPCTSTR pszTip)
{

	NOTIFYICONDATA tnd = {0};
	tnd.cbSize = sizeof(tnd);
	tnd.hWnd = hWnd;
	tnd.uID	= dwIDandCallbackMessage;
	tnd.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
	tnd.uCallbackMessage = dwIDandCallbackMessage;
	tnd.hIcon = hIcon;
	if( pszTip )
		lstrcpyn(tnd.szTip, pszTip, _countof(tnd.szTip)-1);

	return Shell_NotifyIcon(NIM_ADD, &tnd);
}

BOOL RemoveTrayIcon(HWND hWnd, UINT dwIDandCallbackMessage)
{

	NOTIFYICONDATA tnd = {0};
	tnd.cbSize = sizeof(tnd);
	tnd.hWnd = hWnd;
	tnd.uID	= dwIDandCallbackMessage;
	tnd.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
	tnd.uCallbackMessage = dwIDandCallbackMessage;

	return Shell_NotifyIcon(NIM_DELETE, &tnd);
}

//tstring GetRecent()
//{
//	HMENU hM = CreateMenu();
//	HMENU hPop = CreatePopupMenu();
//	if(!hPop)
//		return L"";
//
//	if(!InsertMenu(hPop, 0, MF_STRING, 1000, L"AAA"))
//		return L"";
//
//	if(!InsertMenu(hM, 0, MF_POPUP, (UINT_PTR)hPop, NULL))
//		return L"";
//
//	TrackPopupMenu(hPop,
//		TPM_RETURNCMD,
//		100,100,
//		0,
//		NULL,
//		NULL);
//
//	return L"";
//}


void OpenRecent()
{
	// launch openrecent
}