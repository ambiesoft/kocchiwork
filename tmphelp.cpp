#include <windows.h>
#include "tmphelp.h"

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


