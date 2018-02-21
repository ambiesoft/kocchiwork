#include "stdafx.h"
#include "kocchiwork.h"
#include "err.h"
#include "common.h"

BOOL gBusy;

bool  ReturnFileAndQuit(HWND hWnd)
{
	if(gBusy)
		return false;

	HANDLE hFile = CreateFile (g_workfile.c_str(), 
		GENERIC_READ, 
		FILE_SHARE_READ|FILE_SHARE_WRITE, 
		NULL, 
		OPEN_EXISTING, 
		FILE_ATTRIBUTE_NORMAL, 
		NULL);
	if(hFile==INVALID_HANDLE_VALUE)
		return false;

	CloseHandle(hFile);



	{
		WIN32_FIND_DATA wfdWorkCurrent;
		if(!GetFileData(g_workfile.c_str(), &wfdWorkCurrent))
			errExit(NS("could not obtain work file data"), GetLastError());


		int nCmp = CompareSizeAndLastWrite(&wfdWorkCurrent, &g_wfdRemote);
		if(nCmp==0)
		{
			return false;
		}
	}

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
		return false;
	}

	if(PathFileExists(g_remotefile.c_str()))
	{
		WIN32_FIND_DATA wfdRemoteNow;
		if(!GetFileData(g_remotefile.c_str(), &wfdRemoteNow))
		{
			errExit(NS("could not obtain remote file time"), GetLastError(), TRUE);
			return false;
		}

		BOOL bChanged = CompareSizeAndLastWrite(&g_wfdRemote, &wfdRemoteNow) != 0;

		if(bChanged)
		{
			tstring message;
			message = NS("Operation canceled.");
			message += _T("\r\n");
			message += NS("The remote file may be updated after this application starts.");
			message += _T("\r\n");
			message += _T("\r\n");
			message += NS("Fix the conflict.");

			MessageBox(NULL, message.c_str(), APP_NAME, MB_ICONWARNING);
			return false;
		}
	}

	if(!MoveFileEx( g_workfile.c_str(), g_remotefile.c_str(), 
		MOVEFILE_COPY_ALLOWED | MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH ) )
	{
		errExit(NS("could not move file"), GetLastError());
	}
	//if(!CopyFile(
	//	g_workfile.c_str(), 
	//	g_remotefile.c_str(), 
	//	FALSE // overwirte
	//	))
	//{
	//	errExit(NS("could not copy-back file"), GetLastError());
	//}
	//SHDeleteFile(g_workfile.c_str());

	doPostQuitMessage(0);
	return true;
}