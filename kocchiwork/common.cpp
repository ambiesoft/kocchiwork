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
#include "../../lsMisc/CenterWindow.h"

using namespace Ambiesoft;

HINSTANCE g_hInst;
HICON g_hTrayIcon;
HWND g_hWnd;
HANDLE g_hKanshiApp;
WIN32_FIND_DATA g_wfdRemote;
WIN32_FIND_DATA g_wfdWork;

tstring g_workfile;
tstring g_remotefile;
tstring g_progfile;

DWORD g_dwRemoteSize;
time_t g_starttime;

BOOL g_bQuerying;
BOOL g_bQuittedWhileQuerying;

void doPostQuitMessage(int nQM)
{
	if(g_bQuerying)
	{
		g_bQuittedWhileQuerying = TRUE;
		return;
	}
	PostQuitMessage(nQM);
}

// Show window and settitle before showing messageob
int ForeMessageBox(
	HWND hWnd,
	LPCWSTR lpText,
	LPCWSTR lpCaption,
	UINT uType)
{
	assert(hWnd == g_hWnd || hWnd == NULL);
	CenterWindow(hWnd);
	const bool isHidden = !IsWindowVisible(hWnd);
	ShowWindow(hWnd, SW_SHOW);
	SetForegroundWindow(hWnd);
	SetWindowText(hWnd, lpCaption);
	int ret = MessageBox(hWnd, lpText, lpCaption, uType);
	if (isHidden)
		ShowWindow(hWnd, SW_SHOW);
	return ret;
}
