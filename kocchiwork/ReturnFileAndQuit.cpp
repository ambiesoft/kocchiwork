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



#include "kocchiwork.h"
#include "err.h"
#include "common.h"

using namespace Ambiesoft;
using namespace std;

bool gBusy;
bool gMovebackCanceled;  // suppress multiple moveback check,
                         // Once user choose cancel to move back the file, this becomes 'true'

CHECKFILERESULT ReturnFileAndQuit(HWND hWnd)
{
	if(gBusy)
		return CHECKFILE_BUSY;

	if (gMovebackCanceled)
		return CHECKFILE_ALREADYCANCELED;

	HANDLE hFile = CreateFile (g_workfile.c_str(), 
		GENERIC_READ, 
		FILE_SHARE_READ|FILE_SHARE_WRITE, 
		NULL, 
		OPEN_EXISTING, 
		FILE_ATTRIBUTE_NORMAL, 
		NULL);
	if(hFile==INVALID_HANDLE_VALUE)
		return CHECKFILE_OPENED;

	CloseHandle(hFile);

	int nCmp = CompareSizeAndLastWrite(g_workfile.c_str(), &g_wfdRemote);
	if(nCmp==0)
	{
		return CHECKFILE_NOTMODIFIED;
	}


	BlockedBool busybacker(&gBusy);

	wstringstream message;
	message << NS("The file have been updated. Do you want to move the changed file back to the original location?") << endl;
	message << endl;
	message << NS("copy source: ") << g_workfile << endl;
	message << NS("copy destination: ") << g_remotefile << endl;

	if (IDYES != MessageBox(
		GetDesktopWindow(),
		message.str().c_str(),
		APP_NAME,
		MB_APPLMODAL | MB_ICONINFORMATION | MB_YESNO))
	{
		return CHECKFILE_MODIFIED_BUTUSERCANCELED;
	}

	if(PathFileExists(g_remotefile.c_str()))
	{
		BOOL bChanged = CompareSizeAndLastWrite(&g_wfdRemote, g_remotefile.c_str()) != 0;

		if(bChanged)
		{
			wstringstream message;
			message << NS("Operation canceled.") << endl;
			message << NS("The remote file may be updated after this application starts.") << endl;
			message << endl;
			message << NS("Fix the conflict.");

			MessageBox(GetDesktopWindow(), message.str().c_str(), APP_NAME, MB_ICONWARNING);
			return CHECKFILE_ERROR;
		}
	}

	BOOL ok = FALSE;
	do
	{
		ok = MoveFileEx(g_workfile.c_str(), g_remotefile.c_str(),
			MOVEFILE_COPY_ALLOWED | MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH);

		if (!ok)
		{
			wstring strLE = GetLastErrorString(GetLastError());

			wstringstream message;
			message << NS("Failed to move the file back to original location.") << endl;
			message << strLE << endl;

			int ret = MessageBox(GetDesktopWindow(), message.str().c_str(), APP_NAME, MB_ICONERROR | MB_RETRYCANCEL);
			if (ret == IDCANCEL)
			{
				gMovebackCanceled = true;
				return CHECKFILE_MODIFIED_BUTUSERCANCELED;
			}
		}
	} while (!ok);

	doPostQuitMessage(0);
	return CHECKFILE_MOVEBACKED;
}