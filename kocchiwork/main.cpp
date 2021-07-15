// BSD 2-Clause License
// 
// Copyright (c) 2019, Ambiesoft
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
// 
// * Redistributions of source code must retain the above copyright notice, this
//   list of conditions and the following disclaimer.
// 
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include "stdafx.h"

#include "../../lsMisc/HighDPI.h"
#include "../../lsMisc/CommandLineString.h"
#include "../../lsMisc/OpenCommon.h"

#include "heavyboost.h"

#include "resource.h"
#include "thread.h"
#include "err.h"
#include "app.h"

#include "common.h"


using namespace Ambiesoft;
using namespace Ambiesoft::stdosd;
// using namespace Ambiesoft::stdwin32;

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
int CompareSizeAndLastWrite(LPCWSTR p1, WIN32_FIND_DATA* p2)
{
	WIN32_FIND_DATA wfd;
	if (!GetFileData(p1, &wfd))
	{
		DWORD dwLE = GetLastError();
		errExit(NS("could not obtain work file data"), &dwLE);
	}

	return CompareSizeAndLastWrite(&wfd, p2);
}
int CompareSizeAndLastWrite(WIN32_FIND_DATA* p1, LPCWSTR p2)
{
	WIN32_FIND_DATA wfd;
	if (!GetFileData(p2, &wfd))
	{
		DWORD dwLE = GetLastError();
		errExit(NS("could not obtain work file data"), &dwLE);
	}

	return CompareSizeAndLastWrite(p1, &wfd);
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



wstring RenameKocchiExt(LPCWSTR pFile)
{
	// LPWSTR p =  _tcsdup(pFile);
	unique_ptr<wchar_t, std::function<void(void*)>> p(_tcsdup(pFile) , free);

	LPWSTR pExt = _tcsrchr(p.get(), L'.');
	if(!pExt)
		return pFile;

	if(lstrcmpi(pExt, L".kocchi") != 0)
		return pFile;

	*pExt = 0;
	return p.get();
}

class SessionCount {
	CSessionGlobalMemory<UINT> gcs_count_;
public:
	SessionCount() : gcs_count_("gcs_count"){
		gcs_count_ = gcs_count_ + 1;
	}
	~SessionCount(){
		gcs_count_ = gcs_count_ - 1;
	}

	UINT count() const{
		return gcs_count_;
	}
};

static void LaunchMeOneByOne(
	tstring& progfile,
	bool& noSaveRecent,
	tstring& lang,
	std::vector<tstring>& remoteFiles)
{
	tstring argBase = GetArgCommand(progfile, noSaveRecent, lang);

	tstring exe = stdGetModuleFileName<wchar_t>();
	tstring dir = stdGetCurrentDirectory();
	for (auto&& elem : remoteFiles)
	{
		tstring arg = stdFormat(L"%s \"%s\"",
			argBase.c_str(),
			elem.c_str());
		if (!OpenCommon(nullptr,
			exe.c_str(),
			arg.c_str(),
			dir.c_str()))
		{
			errExit(NS("Failed to OpenCommon"));
		}
	}

}
int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR     lpCmdLine,
                     int       nCmdShow )
{
//#ifdef _DEBUG
//	_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF | _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG));
//#endif
	InitHighDPISupport();
	g_hInst = hInstance;
	Ambiesoft::i18nInitLangmap(hInstance, NULL, _T(""));
	g_hTrayIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON_MAIN));

	if(!InitApp())
	{
		errExit(NS("Fatal : InitApp"));
	}

#ifdef _DEBUG
//		g_remotefile = _T("\\\\Thexp\\Share\\ttt.txt");
#endif

	bool f_noSaveRecent = false;
	wstring lang;

	{
		vector<tstring> remoteFiles;
		GetArgumentsFromCommandLine(
			g_progfile,
			f_noSaveRecent,
			lang,
			remoteFiles);
		switch (remoteFiles.size())
		{
		case 0:
			break;
		case 1:
			g_remotefile = remoteFiles[0];
			break;
		default:
			LaunchMeOneByOne(g_progfile,
				f_noSaveRecent,
				lang,
				remoteFiles);
			return 0;
		}
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
		OpenRecentCS();
		return 0;
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

		g_workfile = workdir + _T("\\") + RenameKocchiExt(pFile);
	}
	
	if(!IsFileExists(g_remotefile.c_str()))
	{
		tstring message = boostformat(NS("'%1%' is not a file."), g_remotefile.c_str());
		errExit(message.c_str());
	}

	// https://docs.google.com/drawings/d/171nPifJEXYmXnLLvF5a0jnlqW7wIj0yOd2Yw7XUQhu4/edit
	{
		if(g_remotefile.length() >= MAX_PATH)
			errExit(NS("file name too long"));

		LPTSTR pDup = _tcsdup(g_remotefile.c_str());
		_tcslwr(pDup);
		tstring mutexname = pDup;
		free(pDup); pDup = NULL;
		mutexname = stdStringReplace(mutexname, _T("\\"), _T("_"));
		if (!CreateMutex(NULL, TRUE, mutexname.c_str()))
		{
			DWORD dwLE = GetLastError();
			errExit(NS("CreateMutex failed"), &dwLE);
		}

		if(GetLastError()==ERROR_ALREADY_EXISTS)
		{
			tstring message = NS("This file is currently opened.");
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

		if(IDYES != MessageBox(GetDesktopWindow(), message.c_str(), APP_NAME, MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2))
			return 0;

		FILETIME ftCurrent;
		if (!GetFileLastWriteTime(g_workfile.c_str(), &ftCurrent))
		{
			DWORD dwLE = GetLastError();
			errExit(NS("Could not get filetime"), &dwLE);
		}

		if(CompareFileTime(&ftCurrent, &g_wfdRemote.ftLastWriteTime) > 0)
			errExit(NS("Existing file is newer than the remote file. exiting."));

		if(0 != SHDeleteFileEx(g_workfile.c_str()))
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
			savefile = savefilebase + stdToString(i);
		}
		if(!CopyFile(g_workfile.c_str(), savefile.c_str(), TRUE))
			errExit(NS("Copy failed"));

		if(0 != SHDeleteFileEx(savefile.c_str(), FOF_ALLOWUNDO|FOF_SILENT|FOF_FILESONLY))
			errExit(NS("could not trash file"));
	}

	if(!GetFileData(g_workfile.c_str(), &g_wfdWork))
		errExit(NS("could not obtain file info"));

	SessionCount sesstionCount;

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
	tstring dirT = GetDirFromPath(g_workfile.c_str());
	sei.lpDirectory = dirT.c_str();

	if(!ShellExecuteEx(&sei)) 
	{
		tstring message = NS("Execute failed. Do you want to remove the copied file?");
		message += CRLF;
		message += g_workfile.c_str();
		if(IDYES == MessageBox(GetDesktopWindow(),
			message.c_str(),
			APP_NAME,
			MB_ICONWARNING|MB_YESNO))
		{
			if(0 != SHDeleteFileEx(g_workfile.c_str()))
				errExit(NS("could not trash file"));

			return 0;
		}
	}

	g_starttime =time(NULL);


	g_hKanshiApp = sei.hProcess;

	g_hWnd = CreateSimpleWindow(GetModuleHandle(NULL), _T("kocchiwork class"), _T(""), WndProc);
	if(!g_hWnd)
		errExit(NS("could not create a winoow"));
#ifdef _DEBUG
	ShowWindow(g_hWnd, SW_SHOW);
#endif

	SendMessage(g_hWnd, WM_SETICON, ICON_SMALL, (LPARAM)g_hTrayIcon);
	SendMessage(g_hWnd, WM_SETICON, ICON_BIG, (LPARAM)g_hTrayIcon);

	tstring traytip = stdFormat( NS("Monitoring \"%s\""), g_workfile.c_str());

	if(!AddTrayIcon(
		g_hWnd, 
		WM_APP_TRAY_NOTIFY, 
		g_hTrayIcon, 
		traytip.c_str()))
	{
		DWORD dwLE = GetLastError();
		errExit(NS("Failed to register tray icon."), &dwLE, true);
	}
	if (0 != atexit(Untray))
	{
		DWORD dwLE = errno;
		errExit(NS("atexit failed."), &dwLE);
	}


	HANDLE hThreadDie = NULL;
	HANDLE hThread = NULL;
	
	{
		hThreadDie = CreateEvent(NULL, FALSE, FALSE, NULL);
		threadData td;
		td.hDie_ = hThreadDie;
		if (!td.hDie_)
		{
			DWORD dwLE = GetLastError();
			errExit(NS("could not create event"), &dwLE);
		}
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

	if (hThreadDie && !SetEvent(hThreadDie))
	{
		DWORD dwLE = GetLastError();
		errExit(NS("could not SetEvent"), &dwLE);
	}

	if(hThread)
		WaitForSingleObject(hThread, INFINITE);

	CHECKFILERESULT checkFileResult = CHECKFILE_NONE;
	if(PathFileExists(g_workfile.c_str()))
	{
		for (;;)
		{
			int nCmp = CompareSizeAndLastWrite(g_workfile.c_str(), &g_wfdRemote);
			if (nCmp == 0)  // File not changed
			{
				tstring message;
				message = NS("File not changed. Do you want to trash the copied file?");
				message += CRLF;
				message += CRLF;
				message += g_workfile.c_str();
				if (IDYES == ForeMessageBox(
					g_hWnd, 
					message.c_str(), 
					stdFormat(L"%s | %s", NS("Not Changed"), APP_NAME).c_str(),
					MB_APPLMODAL | MB_ICONQUESTION | MB_YESNO))
				{
					// Recheck file is not changed while MessageBox
					if (nCmp != CompareSizeAndLastWrite(g_workfile.c_str(), &g_wfdRemote))
						continue;

					BOOL done = FALSE;
					while (!done)
					{
						done = TRUE;
						if (0 != SHDeleteFileEx(g_workfile.c_str(), FOF_ALLOWUNDO | FOF_SILENT | FOF_FILESONLY))
						{
							done = FALSE;
							if (IDCANCEL == ForeMessageBox(g_hWnd,
								NS("Failed to delete file"),
								stdFormat(L"%s | %s", NS("Failed to delete"), APP_NAME).c_str(),
								MB_APPLMODAL | MB_DEFBUTTON1 | MB_ICONEXCLAMATION | MB_RETRYCANCEL))
							{
								errExit(NS("could not trash file. exiting."));
							}
						}
					}
				}
			}
			else if (nCmp > 0)  // File Changed
			{
				checkFileResult = ReturnFileAndQuit(g_hWnd);
			}
			break;
		}
	}

	if(!f_noSaveRecent)
	{
		if(!SaveRecent(g_progfile.c_str(), g_remotefile.c_str()))
		{
			errExit(NS("SaveRecent failed."));
		}
	}

	if (checkFileResult == CHECKFILE_NONE ||
		checkFileResult == CHECKFILE_NOTMODIFIED ||
		checkFileResult == CHECKFILE_MOVEBACKED)
	{
		if (!RemoveDirectory(workdir.c_str()))
		{
			if (sesstionCount.count() <= 1)
			{
				Sleep(5000);
				if (!RemoveDirectory(workdir.c_str()))
				{
					if (sesstionCount.count() <= 1)
					{
						errExit(NS("RemoveDirectory fails. Files may still exists in the directory."), nullptr, TRUE);
					}
				}
			}
		}
	}

	DestroyIcon(g_hTrayIcon);
	return msg.wParam;
}



