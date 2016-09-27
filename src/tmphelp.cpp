#include "stdafx.h"
#include "tmphelp.h"
#include "resource.h"
#include "heavyboost.h"
#include "err.h"

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

#define MAX_RECENT_SIZE 16
typedef list<tstring> RECENTSTYPE;

BOOL CALLBACK NewCmdDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
void OpenRecent()
{


	if(IDOK != DialogBoxParam(GetModuleHandle(NULL),
		MAKEINTRESOURCE(IDD_DIALOG_RECENT),
		NULL,
		NewCmdDlgProc,
		NULL))
	{
		return;
	}


	// launch openrecent
}


void GetRecents(RECENTSTYPE& recents)
{
	tstring inifile = stdGetModuleFileName();
	inifile += _T(".ini");


	int count = GetPrivateProfileInt(_T("recents"),
		_T("count"),
		0,
		inifile.c_str());
	TCHAR szT[1024];
	for(int i=0 ; i < count ; ++i)
	{
		szT[0]=0;
		tstring sec(_T("recent_"));
		sec += dolex(i);

		GetPrivateProfileString(_T("recents"),
			sec.c_str(),
			_T(""),
			szT,
			_countof(szT),
			inifile.c_str());

		if(szT[0] != 0)
		{
			recents.push_back(szT);
		}
	}
}



BOOL SaveRecent(LPCTSTR pApp, LPCTSTR pFile)
{
	tstring inifile = stdGetModuleFileName();
	inifile += _T(".ini");

	RECENTSTYPE recents;
	GetRecents(recents);

	recents.remove(pFile);
	recents.push_front(pFile);

	int count = min(recents.size(), MAX_RECENT_SIZE);
	RECENTSTYPE::iterator it;
	int i=0;
	for(it=recents.begin() ; it != recents.end() ; ++it, ++i)
	{
		tstring sec(_T("recent_"));
		sec += dolex(i);

		if(!WritePrivateProfileString(_T("recents"),
			sec.c_str(),
			it->c_str(),
			inifile.c_str()))
		{
			return FALSE;
		}
	}
	if(!WritePrivateProfileInt(_T("recents"),
		_T("count"),
		i,
		inifile.c_str()))
	{
		return FALSE;
	}

	return TRUE;
}




BOOL CALLBACK NewCmdDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
		case WM_INITDIALOG:
		{

			RECENTSTYPE recents;
			GetRecents(recents);

			HWND hwndList = GetDlgItem(hDlg, IDC_LIST_RECENT);
			RECENTSTYPE::iterator it;
			int i=0;
			for (it=recents.begin() ; it != recents.end() ; ++it, ++i)
			{ 
				int pos = (int)SendMessage(hwndList,
					LB_ADDSTRING,
					0, 
					(LPARAM)it->c_str());
				// Set the array index of the player as item data.
				// This enables us to retrieve the item from the array
				// even after the items are sorted by the list box.
				SendMessage(hwndList, LB_SETITEMDATA, pos, (LPARAM) i); 
			} 

			if(SendMessage(hwndList, LB_GETCOUNT,0,0) > 0)
			{
				SendMessage(hwndList, LB_SETCURSEL,0,0);
			}
				
			// Set input focus to the list box.
			SetFocus(hwndList); 

			CenterWindow(hDlg, NULL);
			return TRUE;
		}
		break;

		case WM_COMMAND:
		{
			switch(LOWORD(wParam))
			{
				case IDOK:
				{
					HWND hwndList = GetDlgItem(hDlg, IDC_LIST_RECENT);
					int cur = SendMessage(hwndList, LB_GETCURSEL, 0, 0);
					if(cur != LB_ERR)
					{
						int len = SendMessage(hwndList, LB_GETTEXTLEN, cur, 0);
						LPTSTR p = new TCHAR[len+1];
						p[0]=0;
						SendMessage(hwndList, LB_GETTEXT, cur, (LPARAM)p);
						tstring file = _T("\"");
						file += p;
						file += _T("\"");
						delete[] p;
						
						tstring thisexe = stdGetModuleFileName();
						if(ShellExecute(hDlg,
							NULL,
							thisexe.c_str(),
							file.c_str(),
							NULL,
							SW_SHOW) <= (HINSTANCE)32)
						{
							errExit(NS("Fatal : ShellExecute"));
						}
					}

					EndDialog(hDlg, IDOK);
					return 1;
				}
				break;

				case IDCANCEL:
				{
					EndDialog(hDlg, IDCANCEL);
					return 1;
				}
				break;

			}
			break;
		}
		break;
	}
	return FALSE;
}
