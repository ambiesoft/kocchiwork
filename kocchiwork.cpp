#include "stdafx.h"

#include "resource.h"



#include "thread.h"
#include "err.h"
void ReturnFileAndQuit(HWND hWnd);

HICON g_hTrayIcon;
HWND g_hWnd;
WIN32_FIND_DATA g_wfdRemote;
WIN32_FIND_DATA g_wfdWork;

tstring g_workfile;
tstring g_remotefile;

DWORD g_dwRemoteSize;

LRESULT CALLBACK WndProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	switch(nMsg)
	{
	case WM_APP_TRAY_NOTIFY:
		switch(lParam)
		{
		case WM_RBUTTONUP:
			{
				POINT apos;
				HMENU hSubMenu = CreatePopupMenu();
				AppendMenu(hSubMenu, MF_BYCOMMAND, IDC_START, NS("TEST"));
				AppendMenu(hSubMenu, MF_SEPARATOR, 0, NULL);
				AppendMenu(hSubMenu, MF_BYCOMMAND, IDC_ABOUT, NS("&About"));
				AppendMenu(hSubMenu, MF_BYCOMMAND, IDC_QUIT, NS("&Exit"));

				SetForegroundWindow(hWnd);
				GetCursorPos((LPPOINT)&apos);

				TrackPopupMenu(hSubMenu,
					TPM_TOPALIGN | TPM_RIGHTBUTTON | TPM_LEFTBUTTON,
					apos.x, apos.y, 0, hWnd, NULL);

				DestroyMenu(hSubMenu);
			}
			break;

		}
		break;
	case WM_APP_LACHANGED:
		{
			ReturnFileAndQuit(hWnd);
		}
		break;
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDC_ABOUT:
			MessageBox(hWnd, _T("aa"), APP_NAME, MB_ICONINFORMATION);
			break;

		case IDC_QUIT:
			DestroyWindow(hWnd);
			PostQuitMessage(0);
			break;
		}
		break;

	default:
		return DefWindowProc(hWnd, nMsg, wParam, lParam);
	}

	return 0;
}



BOOL SetTrayIcon(HWND hWnd, DWORD dwMessage, UINT uID, HICON hIcon, LPCTSTR pszTip)
{

	NOTIFYICONDATA tnd = {0};
	tnd.cbSize = sizeof(tnd);
	tnd.hWnd = hWnd;
	tnd.uID	= uID;
	tnd.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
	tnd.uCallbackMessage = WM_APP_TRAY_NOTIFY;
	tnd.hIcon = hIcon;
	if( pszTip )
		lstrcpy(tnd.szTip, pszTip);


	return Shell_NotifyIcon(dwMessage, &tnd);
}

int CompareSizeAndLastWrite(WIN32_FIND_DATA* p1, WIN32_FIND_DATA* p2)
{
	int ret=0;
	if( (ret=CompareFileTime (&p1->ftLastWriteTime, &p2->ftLastWriteTime)) != 0)
	{
		return ret;
	}

	ret = p1->nFileSizeHigh - p2->nFileSizeHigh;
	if(ret !=0)
		return ret;

	ret = p1->nFileSizeLow - p2->nFileSizeLow;
	if(ret != 0)
		return ret;

	return ret;
}

BOOL gBusy;
void ReturnFileAndQuit(HWND hWnd)
{
	if(gBusy)
		return;

	HANDLE hFile = CreateFile (g_workfile.c_str(), 
		GENERIC_READ, 
		0, 
		NULL, 
		OPEN_EXISTING, 
		FILE_ATTRIBUTE_NORMAL, 
		NULL);
	if(hFile==INVALID_HANDLE_VALUE)
		return;

	CloseHandle(hFile);

	struct BUSYBACKER {
		BOOL* pb_;
		BUSYBACKER(BOOL* pb) {
			*pb = TRUE;
			pb_ = pb;
		}
		~BUSYBACKER() {
			*pb_ = FALSE;
		}
	} busybacker(&gBusy);

	tstring message;
	message += NS("The File was changed. Do you want to move the changed file back to the original location?");
	message += _T("\r\n\r\n");


	message += NS("copy source :") + g_workfile;
	message += _T("\r\n");
	message += NS("copy destination :") + g_remotefile;

	if(IDYES != MessageBox(
		NULL, 
		message.c_str(), 
		APP_NAME, 
		MB_SYSTEMMODAL|MB_ICONINFORMATION|MB_DEFBUTTON2|MB_YESNO))
	{
		return;
	}

	if(PathFileExists(g_remotefile.c_str()))
	{
		WIN32_FIND_DATA wfdRemoteNow;
		if(!GetFileData(g_remotefile.c_str(), &wfdRemoteNow))
		{
			errExit(NS("could not obtain remote file time"), GetLastError(), TRUE);
			return;
		}

		BOOL bChanged = CompareSizeAndLastWrite(&g_wfdRemote, &wfdRemoteNow) != 0;

		if(bChanged)
		{
			tstring message;
			message = NS("Operation canceled.");
			message += _T("\r\n");
			message += NS("The source file information and preserved one is different.");
			message += _T("\r\n");
			message += _T("\r\n");
			message += NS("If you want to continue, delete or rename source file first and try again.");

			MessageBox(NULL, message.c_str(), APP_NAME, MB_ICONWARNING);
			return;
		}
	}

	if(!MoveFileEx( g_workfile.c_str(), g_remotefile.c_str(), 
		MOVEFILE_COPY_ALLOWED | MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH ) )
	{
		errExit(NS("could not move file"), GetLastError());
	}
	
	PostQuitMessage(0);
}
int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow )
{
	if(__argc==1)
	{
		g_remotefile = _T("\\\\Thexp\\Share\\ttt.txt");
		// g_remotefile = _T("C:\\Documents and Settings\\dualin\\SendTo\\kocchiwork.lnk");
	}
	else
	{
		g_remotefile = __targv[1];
	}

	const tstring thisdir = GetModuleDirectory(NULL);
	if(thisdir.empty() || !IsDirectory(thisdir.c_str()))
	{
		errExit(NS("Fatal : could not find module directory"));
	}

	const tstring workdir = thisdir + _T("working");
	CreateDirectory(workdir.c_str(), NULL);
	if(!IsDirectory(workdir.c_str()))
	{
		tstring message = NS("could not create a folder");
		message += _T(" : ");
		message += workdir;
		errExit(message.c_str());
	}
	
	{
		TCHAR szT[1024];
		LPTSTR pFile = NULL;
		DWORD ret = GetFullPathName(g_remotefile.c_str(), _countof(szT), szT, &pFile);
		if(ret==0 || ret > _countof(szT))
			errExit(NS("Too long path"));

		if(pFile==NULL)
			errExit(NS("cound not find filename"));

		g_remotefile = szT;
		g_workfile = workdir + _T("\\") + pFile;
	}
	
	if(g_remotefile[0]==_T('\\') && g_remotefile[1]==_T('\\') )
	{
	}
	else
	{
		TCHAR szT[8];
		lstrcpyn(szT, g_workfile.c_str(), 4);
		UINT driveType = GetDriveType(szT);
		if(driveType==DRIVE_UNKNOWN||
			driveType==DRIVE_FIXED)
		{
			errExit(NS("The file is not on remote drive."));
			return 0;
		}
	}


	

	if(!GetFileData(g_remotefile.c_str(), &g_wfdRemote))
		errExit(NS("could not obtain remote file info"));


	if(PathFileExists(g_workfile.c_str()))
	{
		tstring message = g_workfile;
		message += " ";
		message += NS("already exists.");
		message += _T("\r\n\r\n");
		message += NS("Do you want to trash it and copy remote file?");

		if(IDYES != MessageBox(NULL, message.c_str(), APP_NAME, MB_ICONQUESTION|MB_YESNO))
			return 0;

		FILETIME ftCurrent;
		if(!GetFileLastWriteTime(g_workfile.c_str(), &ftCurrent))
			errExit(NS("could not get filetime"), GetLastError());

		if(CompareFileTime(&ftCurrent, &g_wfdRemote.ftLastWriteTime) > 0)
			errExit(NS("Existing file is newer than the remove file. exiting."));

		if(!SHDeleteFile(g_workfile.c_str()))
			errExit(NS("could not gomibako file"));
	}

	


	if(!CopyFile(g_remotefile.c_str(), g_workfile.c_str(), TRUE))
		errExit(NS("Copy failed"));


	if(!GetFileData(g_workfile.c_str(), &g_wfdWork))
		errExit(NS("could not obtain file info"));



	SHELLEXECUTEINFO sei = {0};
	sei.cbSize = sizeof(sei);
	sei.lpVerb = _T("open");
	sei.lpFile = g_workfile.c_str();
	sei.nShow = SW_SHOW;
//	sei.hInstApp = GetModuleHandle(NULL);
//	sei.fMask = SEE_MASK_NOCLOSEPROCESS;
	{
		tstring dir = GetDirFromPath(g_workfile.c_str());
		sei.lpDirectory = dir.c_str();
	}
	if(!ShellExecuteEx(&sei)) 
	{
		if(IDYES == MessageBox(NULL,
			NS("Execute failed. Do you want to remove the copied file?"),
			APP_NAME,
			MB_ICONQUESTION|MB_YESNO))
		{
			if(!SHDeleteFile(g_workfile.c_str()))
				errExit(NS("could not trash file"));

			return 0;
		}
	}

	g_hWnd = CreateSimpleWindow(GetModuleHandle(NULL), _T("kocchiwork class"), _T(""), WndProc);
	if(!g_hWnd)
		errExit("could not create a winoow");

	g_hTrayIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON_MAIN));
	SetTrayIcon(g_hWnd, NIM_ADD, WM_APP_TRAY_NOTIFY, g_hTrayIcon, _T("TIP"));


	threadData td;
	td.hDie_ = CreateEvent(NULL, FALSE, FALSE, NULL);
	if(!td.hDie_)
		errExit(NS("could not create event"), GetLastError());
	td.hWnd_ = g_hWnd;
	td.ftWork_ = g_wfdWork.ftLastAccessTime;
	td.pWorkingFile_ = g_workfile.c_str();

	HANDLE hThread = (HANDLE)_beginthreadex(
		NULL,
		0, 
		kanshi,
		(void*)&td,
		CREATE_SUSPENDED, 
		NULL);

	if ( hThread == 0 )
		errExit(NS("could not create a thread"));

	if(1!=ResumeThread(hThread))
		errExit(NS("could not resume thread"));
	
	MSG msg;
	while( GetMessage(&msg, NULL, 0, 0) ) 
	{
		// if( !TranslateAccelerator (msg.hwnd, hAccelTable, &msg) ) 
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
	}

	if(!SetEvent(td.hDie_))
		errExit(NS("could not SetEvent"), GetLastError());

	WaitForSingleObject(hThread, INFINITE);

	if(PathFileExists(g_workfile.c_str()))
	{
		WIN32_FIND_DATA wfdWorkCurrent;
		if(!GetFileData(g_workfile.c_str(), &wfdWorkCurrent))
			errExit(NS("could not obtain work file data"), GetLastError());


		int nCmp = CompareSizeAndLastWrite(&wfdWorkCurrent, &g_wfdRemote);
		if(nCmp==0)
		{
			tstring message;
			message = NS("File not changed. Do you want to delete the copied file?");
			if(IDYES==MessageBox(NULL, message.c_str(), APP_NAME, 
				MB_DEFBUTTON2|MB_ICONQUESTION|MB_YESNO))
			{
				if(!SHDeleteFile(g_workfile.c_str()))
					errExit(NS("could not trash file. exiting."));
			}
		}
		else if(nCmp > 0)
		{
			ReturnFileAndQuit(NULL);
		}
	}
	RemoveDirectory(workdir.c_str());
	SetTrayIcon(g_hWnd, NIM_DELETE, WM_APP_TRAY_NOTIFY, g_hTrayIcon, _T("TIP"));
	DestroyIcon(g_hTrayIcon);
	return msg.wParam;
}



