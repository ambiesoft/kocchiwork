#include "stdafx.h"
#include "tmphelp.h"
#include "resource.h"
#include "heavyboost.h"
#include "err.h"

#include "../../MyUtility/HelpDefines.h"
#include "../../MyUtility/WritePrivateProfileWString.h"

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

BOOL PopupTrayIcon(HWND hWnd, UINT dwIDandCallbackMessage, HICON hIcon, LPCTSTR pszTip)
{
	NOTIFYICONDATA tnd = {0};
	tnd.cbSize = sizeof(tnd);
	tnd.hWnd = hWnd;
	tnd.uID	= dwIDandCallbackMessage;
	tnd.uFlags = NIF_ICON | NIF_INFO;
//	tnd.uCallbackMessage = dwIDandCallbackMessage;
	tnd.hIcon = hIcon;
	if( pszTip )
	{
		lstrcpyn(tnd.szInfo, pszTip, _countof(tnd.szTip)-1);
	}

	return Shell_NotifyIcon(NIM_MODIFY, &tnd);
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


tstring GetIniFile()
{
	tstring inifile = stdGetModuleFileName();
	inifile += _T(".ini");

	return inifile;
}

#define MAX_RECENT_SIZE 16
typedef list<tstring> RECENTSTYPE;

BOOL CALLBACK NewCmdDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
tstring OpenRecent()
{
	tstring ret;
	if(IDOK != DialogBoxParam(GetModuleHandle(NULL),
		MAKEINTRESOURCE(IDD_DIALOG_RECENT),
		NULL,
		NewCmdDlgProc,
		(LPARAM)&ret))
	{
		return _T("");
	}
	return ret;
}


class ProcessMutex {
	HANDLE h_;
public:
	ProcessMutex() {
		HANDLE h = CreateMutex(NULL, FALSE, _T("kocciwork_recent_mutex"));
		if(!h)
			errExit(_T("CreateMutex"), GetLastError());

		h_=h;
		WaitForSingleObject(h, INFINITE);
	}
	~ProcessMutex() {
		ReleaseMutex(h_);
	}
};

void GetRecents(RECENTSTYPE& recents)
{
	tstring inifile = GetIniFile();

	{
		ProcessMutex mut;
		// MessageBox(NULL, L"Mutex Aquired", APP_NAME, MB_OK);

		int count = GetPrivateProfileInt(_T("recents"),
			_T("count"),
			0,
			inifile.c_str());
	//	TCHAR szT[1024];
		for(int i=0 ; i < count ; ++i)
		{
	//		szT[0]=0;
			tstring sec(_T("recent_"));
			sec += dolex(i);
		
			tstring tout;
			if(GetPrivateProfileWString(_T("recents"),
				sec.c_str(),
				_T(""),
				tout,
				inifile.c_str()))
			{
				if(!tout.empty())
				{
					recents.push_back(tout);
				}
			}
		}
	}
}



BOOL SaveRecent(LPCTSTR pApp, LPCTSTR pFile)
{
	tstring inifile = GetIniFile();

	RECENTSTYPE recents;
	GetRecents(recents);

	recents.remove(pFile);
	recents.push_front(pFile);

	int count = min(recents.size(), MAX_RECENT_SIZE);
	RECENTSTYPE::iterator it;
	int i=0;
	{
		ProcessMutex mut;
		for(it=recents.begin() ; it != recents.end() ; ++it, ++i)
		{
			tstring sec(_T("recent_"));
			sec += dolex(i);

			if(!WritePrivateProfileWString(_T("recents"),
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
	}
	return TRUE;
}



tstring GetSelected(HWND hwndList)
{
	tstring fileret;
	int cur = SendMessage(hwndList, LB_GETCURSEL, 0, 0);
	if(cur != LB_ERR)
	{
		int len = SendMessage(hwndList, LB_GETTEXTLEN, cur, 0);
		LPTSTR p = new TCHAR[len+1];
		p[0]=0;
		SendMessage(hwndList, LB_GETTEXT, cur, (LPARAM)p);
		fileret = p;
		delete[] p;

/*		
		tstring thisexe = stdGetModuleFileName();
		if(ShellExecute(hwndList,
			NULL,
			thisexe.c_str(),
			file.c_str(),
			NULL,
			SW_SHOW) <= (HINSTANCE)32)
		{
			errExit(NS("Fatal : ShellExecute"));
		}
*/
	}
	return fileret;
}

BOOL CALLBACK NewCmdDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static tstring* spRet;
	switch(uMsg)
	{
		case WM_INITDIALOG:
		{
			spRet = (tstring*)lParam;
			SetWindowText(hDlg, APP_NAME);

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
				case IDC_LIST_RECENT:
				{
					if(HIWORD(wParam)==LBN_DBLCLK)
					{
						HWND hwndList = GetDlgItem(hDlg, IDC_LIST_RECENT);
						*spRet = GetSelected(hwndList);

						EndDialog(hDlg, IDOK);
						return 1;
					}
				}
				break;

				case IDOK:
				{
					HWND hwndList = GetDlgItem(hDlg, IDC_LIST_RECENT);
					*spRet = GetSelected(hwndList);

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
