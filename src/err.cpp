#include "stdafx.h"

void errExit(LPCTSTR p , DWORD dwLastError,BOOL bNoExit)
{
	MessageBox(NULL,
		p,
		APP_NAME,
		MB_ICONERROR);

	if(!bNoExit)
		exit(1);

}

//void errExit(LPCTSTR p, BOOL bNoExit = FALSE)
//{
//	errExit(p, -1, bNoExit);
//}