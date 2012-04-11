#include "stdafx.h"


int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow )
{
	GetModuleFileName(NULL, szT, countof(szT));

	LPCTSTR pRemoveFile = _T("\\\\Thexp\\Share\\CurR\\AIPAC.pdf");


	return 0;
}



