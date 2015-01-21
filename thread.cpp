#include "stdafx.h"

#include "thread.h"
#include "err.h"

unsigned  __stdcall kanshi(void * p)
{
	threadData* pTD = (threadData*)p;
//	if(pTD->bAppKanshi_	)
//	{
//		HANDLE waits[] = {pTD->hDie_, pTD->hApp_};
//		DWORD dwWaited = WaitForMultipleObjects(_countof(waits), waits, FALSE, INFINITE);
//
//		PostMessage(pTD->hWnd_, WM_APP_APPKANHIDONE, 0, 0);
//	}
//	else


	{
		tstring dir = GetDirFromPath(pTD->pWorkingFile_);
		HANDLE hn = FindFirstChangeNotification(dir.c_str(),
			FALSE,
			FILE_NOTIFY_CHANGE_LAST_WRITE|FILE_NOTIFY_CHANGE_SIZE);
		if(hn==INVALID_HANDLE_VALUE)
			errExit(NS("could not be notified changes"));

		HANDLE waits[] = {pTD->hDie_, hn};
		BOOL done = FALSE;
		FILETIME lastChecked = pTD->ftWork_;
		for(;!done;)
		{
			DWORD dwWaited = WaitForMultipleObjects(_countof(waits), waits, FALSE, INFINITE);
			switch(dwWaited)
			{
			case WAIT_OBJECT_0:
				done = TRUE;
				break;
			case WAIT_OBJECT_0+1:
				{
					FILETIME ft;
					if(0==GetFileLastWriteTime(pTD->pWorkingFile_, &ft))
						continue;

					if(0 != CompareFileTime (&ft, &lastChecked))
					{
						lastChecked = ft;
						PostMessage(pTD->hWnd_, WM_APP_LACHANGED, 0, 0);
					}
				}
				break;
			default:
				break;
			}
			if(done)
				break;

			if(!FindNextChangeNotification(hn))
				errExit(NS("could not find next notification"));
		}
		FindCloseChangeNotification(hn);
	}
	return 0;
}