#include "stdafx.h"

#include "../MyUtility/tstring.h"
#include "../MyUtility/GetModuleDirectory.h"
#include "../MyUtility/IsDirectory.h"

#define NS(s) _T(s)

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow )
{
	tstring thisdir = GetModuleDirectory(NULL);
	if(thisdir.empty() || !IsDirectory(thisdir.c_str()))
	{
		MessageBox(NULL,
			NS("Fatal"),
			APP_NAME,
			MB_ICONERROR);
		return -1;
	}

	tstring workdir = thisdir + _T("\\working");
	CreateDirectory(workdir.c_str(), NULL);
	if(!IsDirectory(workdir.c_str()))
	{
		tstring message = NS("could not create a folder");
		message += _T(" : ");
		message += workdir;
		MessageBox(NULL,
			message.c_str(),
			APP_NAME,
			MB_ICONERROR);
		return -1;
	}

	LPCTSTR pRemoteFile = _T("\\\\Thexp\\Share\\CurR\\AIPAC.pdf");
	TCHAR r[33];
	GetFileTitle(pRemoteFile, r, countof(r));
	tstring worfile = 
	CopyFile(pRemoteFile, 

	return 0;
}



