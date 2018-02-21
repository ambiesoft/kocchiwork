#include "stdafx.h"




#ifdef _DLL
#pragma message("_DLL is defined")
#endif

// #include <boost/format.hpp>
#include "heavyboost.h"

/**
#define BOOST_LIB_DIAGNOSTIC
#include <boost/program_options.hpp>
namespace po = boost::program_options;
**/

#include "resource.h"
#include "thread.h"
#include "err.h"
#include "common.h"
// #include "systeminfo.h"

using namespace Ambiesoft;

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

BOOL IsFiletimeFuture(FILETIME* pFT)
{
	SYSTEMTIME systime;
	GetSystemTime(&systime);
	FILETIME ftSystime;
	if(!SystemTimeToFileTime(&systime, &ftSystime))
		return FALSE;

	long ret = CompareFileTime(pFT, &ftSystime);
	return ret > 0;
}


BOOL InitApp()
{
	return TRUE;
}























int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR     lpCmdLine,
                     int       nCmdShow )
{
	// MessageBox(NULL, L"AAA", L"BBBB", MB_ICONINFORMATION);
/**
	po::options_description desc("Allowed options");
	desc.add_options()
		("help", "produce help message")
		("compression", po::value<int>(), "set compression level")
		;

	po::variables_map vm;
	po::store(po::parse_command_line(__argc, __targv, desc), vm);
	po::notify(vm);

	if (vm.count("help")) {
		// cout << desc << "\n";
		stringstream wss;
		desc.print(wss);
		//errExit(getdtdws wss.str());
		return 1;
	}
**/
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF | _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG));
#endif

	Ambiesoft::i18nInitLangmap(hInstance, NULL, _T(""));
	g_hTrayIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON_MAIN));

	if(!InitApp())
	{
		errExit(NS("Fatal : InitApp"));
	}

#ifdef _DEBUG
//		g_remotefile = _T("\\\\Thexp\\Share\\ttt.txt");
//		g_remotefile = _T("\\\\Thexp\\Share\\CurR\\LFS-BOOK-7.1.pdf");
//		g_remotefile = _T("\\\\Thexp\\Share\\CurR\\LFSƒ\BOOK-7.1.pdf");
//		g_remotefile = _T("\\\\Thexp\\Share\\KocchiTest\\ttt.ods");
//		g_remotefile = _T("\\\\inpsrv\\Share\\pass\\text.txt");
#endif

	bool f_noSaveRecent = false;
	try
	{
		for (int i = 1; i < __argc; ++i)
		{
			wstring arg = __targv[i];
			if (lstrcmpi(arg.c_str(), L"/P") == 0)
			{
				g_progfile = stdwin32::trim( __targv[i + 1]);
				++i;
			}
			else if (lstrcmpi(arg.c_str(), L"/N") == 0)
			{
				f_noSaveRecent = true;
			}
			else
			{
				g_remotefile = __targv[i];
			}
		}
	}
	catch (...)
	{
		errExit(NS("Fatal : Illegal Option"));
	}


	if(hasEndingI(g_remotefile, _T(".lnk")))
	{
		TCHAR szT[MAX_PATH]={0};
		if(!ResolveLink(g_remotefile.c_str(), NULL, szT))
		{
			errExit(NS("Fatal : Failed to resolve link"));
		}
		g_remotefile = szT;
	}

	if (g_remotefile.empty())
	{
		 g_remotefile = OpenRecent();
		 if(g_remotefile.empty())
		 {
			return 0;
		 }
	}

	if (g_remotefile.empty())
	{
		errExit(NS("Fatal : No File Specified"));
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
		// tstring message = ( boost::wformat(NS("%1% is not a file.")) % g_remotefile.c_str() ).str();
		tstring message = doformat(g_remotefile);
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
		lstrcpyn(szT, g_remotefile.c_str(), 4);
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

	if(IsFiletimeFuture(&g_wfdRemote.ftLastWriteTime))
		errExit(NS("file time is future"));

	if(PathFileExists(g_workfile.c_str()))
	{
		tstring message = g_workfile;
		message += _T(" ");
		message += NS("already exists.");
		message += _T("\r\n\r\n");
		message += NS("Do you want to trash it and copy remote file?");

		if(IDYES != MessageBox(NULL, message.c_str(), APP_NAME, MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2))
			return 0;

		FILETIME ftCurrent;
		if(!GetFileLastWriteTime(g_workfile.c_str(), &ftCurrent))
			errExit(NS("Could not get filetime"), GetLastError());

		if(CompareFileTime(&ftCurrent, &g_wfdRemote.ftLastWriteTime) > 0)
			errExit(NS("Existing file is newer than the remote file. exiting."));

		if(0 != SHDeleteFile(g_workfile.c_str()))
			errExit(NS("Could not move file to Recycle Bin"));
	}

	
	if(!CopyFile(g_remotefile.c_str(), g_workfile.c_str(), TRUE))
		errExit(NS("Copy failed"));

	// save original file in trashcan
	{
		tstring savefile = g_workfile;
		savefile += _T(".ks");
		tstring savefilebase(savefile);
		for(int i=0 ; IsFileExists(savefile.c_str()) ; ++i)
		{
			savefile = savefilebase + stdItoT(i);
		}
		if(!CopyFile(g_workfile.c_str(), savefile.c_str(), TRUE))
			errExit(NS("Copy failed"));

		if(0 != SHDeleteFile(savefile.c_str(), FOF_ALLOWUNDO|FOF_SILENT|FOF_FILESONLY))
			errExit(NS("could not trash file"));
	}

	if(!GetFileData(g_workfile.c_str(), &g_wfdWork))
		errExit(NS("could not obtain file info"));



	SHELLEXECUTEINFO sei = {0};
	sei.cbSize = sizeof(sei);
	sei.lpVerb = NULL;
	if (g_progfile.empty())
	{
		sei.lpFile = g_workfile.c_str();
	}
	else
	{
		sei.lpFile = g_progfile.c_str();
		sei.lpParameters = g_workfile.c_str();
	}
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
		if(IDYES == MessageBox(g_hWnd,
			message.c_str(),
			APP_NAME,
			MB_ICONWARNING|MB_YESNO))
		{
			if(0 != SHDeleteFile(g_workfile.c_str()))
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


	
	SendMessage(g_hWnd, WM_SETICON, ICON_SMALL, (LPARAM)g_hTrayIcon);
	SendMessage(g_hWnd, WM_SETICON, ICON_BIG, (LPARAM)g_hTrayIcon);

	tstring traytip =string_format( NS("Monitoring \"%s\""), g_workfile.c_str());


	// traytip += _T("original ");
	// tstring tremotefile = g_remotefile;
	// traytip += _T("fffffffffffffffffffffffffff");
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
			//HWND h = CreateSimpleWindow();
			//stlsoft::scoped_handle<HWND> dh(h, DestroyWindow);

			//SendMessage(h, WM_SETICON, ICON_SMALL, (LPARAM)g_hTrayIcon);
			//SendMessage(h, WM_SETICON, ICON_BIG, (LPARAM)g_hTrayIcon);

			tstring message;
			message = NS("File not changed. Do you want to delete the copied file?");
			message += CRLF;
			message += g_workfile.c_str();
			// ShowWindow(g_hWnd, SW_SHOW);
			if (IDYES == MessageBox(NULL, message.c_str(), APP_NAME,
				MB_APPLMODAL | MB_DEFBUTTON2 | MB_ICONQUESTION | MB_YESNO))
			{
				BOOL done = FALSE;
				while(!done)
				{
					done=TRUE;
					if(0 != SHDeleteFile(g_workfile.c_str(), FOF_ALLOWUNDO|FOF_SILENT|FOF_FILESONLY))
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

	if(!f_noSaveRecent)
	{
		if(!SaveRecent(g_progfile.c_str(), g_remotefile.c_str()))
		{
			errExit(NS("SaveRecent failed."));
		}
	}

	RemoveDirectory(workdir.c_str());
	DestroyIcon(g_hTrayIcon);
	return msg.wParam;
}



