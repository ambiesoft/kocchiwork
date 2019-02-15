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
					if(!FindNextChangeNotification(hn))
						errExit(NS("could not find next notification"));

					FILETIME ft;
					if(!GetFileLastWriteTime(pTD->pWorkingFile_, &ft))
						errExit(NS("could not get last write time"));

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


		}
		FindCloseChangeNotification(hn);
	}
	return 0;
}