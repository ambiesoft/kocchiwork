#include "stdafx.h"

#include "resource.h"
#include "thread.h"
#include "err.h"

#include "systeminfo.h"

#include "../MyUtility/StdStringReplace.h"
#include "../MyUtility/IsFileExists.h"

bool ReturnFileAndQuit(HWND hWnd);

HICON g_hTrayIcon;
HWND g_hWnd;
HANDLE g_hKanshiApp;
WIN32_FIND_DATA g_wfdRemote;
WIN32_FIND_DATA g_wfdWork;

tstring g_workfile;
tstring g_remotefile;

DWORD g_dwRemoteSize;
time_t g_starttime;

void EnableDebugPriv( void )
{
	HANDLE hToken;
	LUID sedebugnameValue;
	TOKEN_PRIVILEGES tkp;

	// enable the SeDebugPrivilege
	if ( ! OpenProcessToken( GetCurrentProcess(),
		TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken ) )
	{
		_tprintf( _T("OpenProcessToken() failed, Error = %d SeDebugPrivilege is not available.\n") , GetLastError() );
		return;
	}

	if ( ! LookupPrivilegeValue( NULL, SE_DEBUG_NAME, &sedebugnameValue ) )
	{
		_tprintf( _T("LookupPrivilegeValue() failed, Error = %d SeDebugPrivilege is not available.\n"), GetLastError() );
		CloseHandle( hToken );
		return;
	}

	tkp.PrivilegeCount = 1;
	tkp.Privileges[0].Luid = sedebugnameValue;
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	if ( ! AdjustTokenPrivileges( hToken, FALSE, &tkp, sizeof tkp, NULL, NULL ) )
		_tprintf( _T("AdjustTokenPrivileges() failed, Error = %d SeDebugPrivilege is not available.\n"), GetLastError() );
		
	CloseHandle( hToken );
}

LPCTSTR GetFileNamePosition( LPCTSTR lpPath )
{
	LPCTSTR lpAct = lpPath + _tcslen( lpPath );

	while ( lpAct > lpPath && *lpAct != _T('\\') && *lpAct != _T('/') )
		lpAct--;

	if ( lpAct > lpPath )
		lpAct++;

	return lpAct;
}

BOOL IsFileOpened(LPCTSTR pFile)
{
	BOOL bFullPathCheck = TRUE;
	BOOL bShow = FALSE;
	CString name;
	CString processName;
	CString fsFilePath;
	CString deviceFileName;

	if ( bFullPathCheck )
	{
		if ( !SystemInfoUtils::GetDeviceFileName( pFile, deviceFileName ) )
		{
			// _tprintf( _T("GetDeviceFileName() failed.\n") );
			return FALSE;
		}
	}
	SystemProcessInformation::SYSTEM_PROCESS_INFORMATION* p;
	SystemProcessInformation pi;
	SystemHandleInformation hi( -1 );
	hi.SetFilter( _T("File"), TRUE ); // Refresh

	for ( POSITION pos = hi.m_HandleInfos.GetHeadPosition(); pos != NULL; )
	{
		SystemHandleInformation::SYSTEM_HANDLE& h = hi.m_HandleInfos.GetNext(pos);

		if ( pi.m_ProcessInfos.Lookup( h.ProcessID, p ) )
		{
			SystemInfoUtils::Unicode2CString( &p->usName, processName );
		}
		else
			processName = "";

		//NT4 Stupid thing if it is the services.exe and I call GetName :((
		if ( INtDll::dwNTMajorVersion == 4 && _tcsicmp( processName, _T("services.exe" ) ) == 0 )
			continue;
		
		hi.GetName( (HANDLE)h.HandleNumber, name, h.ProcessID );

		if ( bFullPathCheck )
			bShow =	_tcsicmp( name, deviceFileName ) == 0;
		else
			bShow =	_tcsicmp( GetFileNamePosition(name), pFile ) == 0;

		if ( bShow )
		{
			return TRUE;

//			if ( !bFullPathCheck )
//			{
//				fsFilePath = "";
//				SystemInfoUtils::GetFsFileName( name, fsFilePath );
//			}
//			
//			_tprintf( _T("0x%04X  %-20s  %s\n"), 
//				h.ProcessID, 
//				processName,
//				!false ? fsFilePath : g_workfile.c_str() );
		}
	}
	return FALSE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	static UINT sTaskBarCreated;
	if(nMsg >= 0xC000 && nMsg == sTaskBarCreated)
	{
		tstring traytip = NS("watching");
		traytip += _T(" ");
		traytip += g_workfile.c_str();
		if(!AddTrayIcon(
			g_hWnd, 
			WM_APP_TRAY_NOTIFY, 
			g_hTrayIcon, 
			traytip.c_str()))
		{
			errExit(NS("could not register tray icon."),GetLastError());
		}
		return 0;
	}

	static bool btimerprocessing;
	switch(nMsg)
	{
	case WM_CREATE:
		{
			sTaskBarCreated = RegisterWindowMessage(_T("TaskbarCreated"));

//			EnableDebugPriv();
			SetTimer(hWnd, 1, 5000,NULL);
		}
		break;
	case WM_TIMER:
		{
			if(btimerprocessing)
				break;
			btimerprocessing = true;
			if(g_hKanshiApp)
			{
				DWORD dw =WaitForSingleObject(g_hKanshiApp, 1);
				if(dw == WAIT_OBJECT_0)
				{// app is gone
					g_hKanshiApp = NULL;
					if(IsFileOpened(g_workfile.c_str()))
					{
						wstring strMessage=L"aaa";

						wchar_t file[MAX_PATH];
						GetModuleFileName(NULL, file, MAX_PATH);
						*wcsrchr(file, L'\\') = 0;
						lstrcat(file, L"\\showballoon.exe");

						wstring param = L"\"" + strMessage + L"\"";
						copycbr
						ShellExecute(NULL,
							NULL,
							file,
							param.c_str(),
							NULL,
							SW_SHOW);
						// SetForegroundWindow(hWnd);
						// MessageBoxA(hWnd,
						// 	"sss",
						//	"sss",
						//	c0);
						// KillTimer(hWnd,1);
					}
					else
					{
						int mret;
						if( true ) //(time(NULL)-g_starttime) < 10)
						{
							SetForegroundWindow(hWnd);
							mret = MessageBox(hWnd,
								NS("App closed. Do you want to continue monitor file?."),
								L"kocchiwork",
								MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2);
						}

						if(mret==IDYES)
						{//use file monitor
							KillTimer(hWnd,1);
						}
						else
						{
							//quit
							KillTimer(hWnd,1);
							PostQuitMessage(0);
						}
					}
				}
			}
			else
			{// no kanshiapp
				if(IsFileOpened(g_workfile.c_str()))
				{
				}
				else
				{// nobodyisopened
					ReturnFileAndQuit(hWnd);
					PostQuitMessage(0);
				}
			}
			btimerprocessing = false;
		}
		break;
	case WM_APP_TRAY_NOTIFY:
		switch(lParam)
		{
			case WM_LBUTTONUP:
			{
				SetForegroundWindow(hWnd);
			}
			break;

			case WM_RBUTTONUP:
			{
				POINT apos;
				HMENU hSubMenu = CreatePopupMenu();
				AppendMenu(hSubMenu, MF_BYCOMMAND, IDC_START, NS("TEST"));
				AppendMenu(hSubMenu, MF_SEPARATOR, 0, NULL);
				AppendMenu(hSubMenu, MF_BYCOMMAND, IDC_REOPENFILE, NS("&Reopen"));
				AppendMenu(hSubMenu, MF_BYCOMMAND, IDC_OPENWITHEXPLORER, NS("&Open with Explorer"));
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
			if(!g_hKanshiApp)
				ReturnFileAndQuit(hWnd);
		}
		break;
	case WM_APP_APPKANHIDONE:
		{
			PostQuitMessage(0);
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

		case IDC_REOPENFILE:
			{
				SHELLEXECUTEINFO sei = {0};
				sei.cbSize = sizeof(sei);
				sei.lpVerb = NULL;
				sei.lpFile = g_workfile.c_str();
				sei.nShow = SW_SHOW;
				tstring dir = GetDirFromPath(g_workfile.c_str());
				sei.lpDirectory = dir.c_str();
				if(!ShellExecuteEx(&sei)) 
				{
				}
			}
			break;

		case IDC_OPENWITHEXPLORER:
			{
				tstring param = _T("/select,");
				param += g_workfile;
				ShellExecute(NULL, _T("open"), _T("Explorer"), param.c_str(), NULL, SW_SHOW);
			}
			break;
		}
		break;

	default:
		return DefWindowProc(hWnd, nMsg, wParam, lParam);
	}

	return 0;
}


void Untray()
{
	RemoveTrayIcon(g_hWnd, WM_APP_TRAY_NOTIFY);
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


int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR     lpCmdLine,
                     int       nCmdShow )
{
	
	if(__argc==1)
	{
//		g_remotefile = _T("\\\\Thexp\\Share\\ttt.txt");
//		g_remotefile = _T("\\\\Thexp\\Share\\CurR\\LFS-BOOK-7.1.pdf");
//		g_remotefile = _T("\\\\Thexp\\Share\\CurR\\LFSƒ\BOOK-7.1.pdf");
//		g_remotefile = _T("\\\\Thexp\\Share\\KocchiTest\\ttt.ods");
		errExit(NS("Fatal : No File Specified"));
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
	
	if(!IsFileExists(g_remotefile.c_str()))
	{
		tstring message = g_remotefile.c_str();
		message += _T(" ");
		message += NS("is not a file.");
		errExit(message.c_str());
	}

	// https://docs.google.com/drawings/d/171nPifJEXYmXnLLvF5a0jnlqW7wIj0yOd2Yw7XUQhu4/edit
	{
		if(g_remotefile.length() >= MAX_PATH)
			errExit(NS("file name too long"));

		LPTSTR pDup = _tcsdup(g_remotefile.c_str());
		_tcslwr(pDup);
		tstring mutexname = pDup;
		free(pDup);pDup=NULL;
		mutexname=StdStringReplace(mutexname, _T("\\"), _T("_"));
		if(!CreateMutex(NULL, TRUE, mutexname.c_str()))
			errExit(NS("CreateMutex failed"), GetLastError());

		if(GetLastError()==ERROR_ALREADY_EXISTS)
		{
			tstring message = NS("This file is currently opened");
			message += CRLF;
			message += g_remotefile.c_str();
			errExit(message.c_str());
		}
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
		message += _T(" ");
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
	sei.lpVerb = NULL; // _T("open");
	sei.lpFile = g_workfile.c_str();
	sei.nShow = SW_SHOW;
//	sei.hInstApp = GetModuleHandle(NULL);
	sei.fMask = SEE_MASK_NOCLOSEPROCESS;
	{
		tstring dir = GetDirFromPath(g_workfile.c_str());
		sei.lpDirectory = dir.c_str();
	}
	if(!ShellExecuteEx(&sei)) 
	{
		tstring message = NS("Execute failed. Do you want to remove the copied file?");
		message += CRLF;
		message += g_workfile.c_str();
		if(IDYES == MessageBox(NULL,
			message.c_str(),
			APP_NAME,
			MB_ICONWARNING|MB_YESNO))
		{
			if(!SHDeleteFile(g_workfile.c_str()))
				errExit(NS("could not trash file"));

			return 0;
		}
	}

	g_starttime =time(NULL);


	g_hKanshiApp = sei.hProcess;


	// DWORD dwIW = WaitForInputIdle(sei.hProcess,10*1000);
	//DWORD dwPI = WaitForSingleObject(sei.hProcess,5*1000);
	//if(dwPI==0 )
	//{
	//bNoKanshi = TRUE;
	//}

	g_hWnd = CreateSimpleWindow(GetModuleHandle(NULL), _T("kocchiwork class"), _T(""), WndProc);
	if(!g_hWnd)
		errExit(NS("could not create a winoow"));

	g_hTrayIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON_MAIN));
	tstring traytip = NS("watching");
	traytip += _T(" ");
	traytip += g_workfile.c_str();
	if(!AddTrayIcon(
		g_hWnd, 
		WM_APP_TRAY_NOTIFY, 
		g_hTrayIcon, 
		traytip.c_str()))
	{
		errExit(NS("could not register tray icon."),GetLastError());
	}
	if(0!=atexit(Untray))
		errExit(NS("atexit failed."), errno);


	HANDLE hThreadDie = NULL;
	HANDLE hThread = NULL;
//	if(bNoKanshi)
//	{
//		errExit(NS("App might be quitted. Watching is disabled. After editing the local document, quit this manually."), -1, TRUE);
//	}
//	else

	
	{
		hThreadDie = CreateEvent(NULL, FALSE, FALSE, NULL);
		threadData td;
		td.hDie_ = hThreadDie;
		if(!td.hDie_)
			errExit(NS("could not create event"), GetLastError());
		td.hWnd_ = g_hWnd;
		td.ftWork_ = g_wfdWork.ftLastWriteTime;
		td.pWorkingFile_ = g_workfile.c_str();
		// td.bAppKanshi_ = bAppKanshi;
		//td.hApp_ = sei.hProcess;
		hThread = (HANDLE)_beginthreadex(
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
	}
	
	MSG msg;
	while( GetMessage(&msg, NULL, 0, 0) ) 
	{
		// if( !TranslateAccelerator (msg.hwnd, hAccelTable, &msg) ) 
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
	}

	if(sei.hProcess)
		CloseHandle(sei.hProcess);

	if(hThreadDie && !SetEvent(hThreadDie))
		errExit(NS("could not SetEvent"), GetLastError());

	if(hThread)
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
			message += CRLF;
			message += g_workfile.c_str();
			if(IDYES==MessageBox(NULL, message.c_str(), APP_NAME, 
				MB_SYSTEMMODAL| MB_DEFBUTTON2|MB_ICONQUESTION|MB_YESNO))
			{
				BOOL done = FALSE;
				while(!done)
				{
					done=TRUE;
					if(!SHDeleteFile(g_workfile.c_str(), TRUE))
					{
						done=FALSE;
						if(IDCANCEL==MessageBox(NULL,
							NS("Failed to delete file"),
							APP_NAME,
							MB_SYSTEMMODAL|MB_DEFBUTTON1|MB_ICONEXCLAMATION|MB_RETRYCANCEL))
						{
							errExit(NS("could not trash file. exiting."));
						}
					}
				} 
			}
		}
		else if(nCmp > 0)
		{
			ReturnFileAndQuit(NULL);
		}
	}
	RemoveDirectory(workdir.c_str());
	DestroyIcon(g_hTrayIcon);
	return msg.wParam;
}



