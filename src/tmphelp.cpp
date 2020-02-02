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
#include "resource.h"

#include "common.h"
#include "heavyboost.h"
#include "err.h"
#include "app.h"

#include "tmphelp.h"


#include "../../lsMisc/HelpDefines.h"
#include "../../lsMisc/WritePrivateProfileWString.h"

using namespace Ambiesoft;

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


//tstring GetRecent()
//{
//	HMENU hM = CreateMenu();
//	HMENU hPop = CreatePopupMenu();
//	if(!hPop)
//		return L"";
//
//	if(!InsertMenu(hPop, 0, MF_STRING, 1000, L"AAA"))
//		return L"";
//
//	if(!InsertMenu(hM, 0, MF_POPUP, (UINT_PTR)hPop, NULL))
//		return L"";
//
//	TrackPopupMenu(hPop,
//		TPM_RETURNCMD,
//		100,100,
//		0,
//		NULL,
//		NULL);
//
//	return L"";
//}




BOOL CALLBACK NewCmdDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
std::vector<std::wstring> OpenRecent()
{
	std::vector<std::wstring> ret;
	if(IDOK != DialogBoxParam(GetModuleHandle(NULL),
		MAKEINTRESOURCE(IDD_DIALOG_RECENT),
		NULL,
		NewCmdDlgProc,
		(LPARAM)&ret))
	{
		return std::vector<std::wstring>();
	}
	return ret;
}






std::vector<std::wstring> GetSelected(HWND hwndList, const RECENTSTYPE& recents)
{
	const int maxcount = 32766;
	vector<int> vI;
	vI.resize(maxcount);
	assert(vI.size() == maxcount);
	int count = SendMessage(hwndList, LB_GETSELITEMS, maxcount, (LPARAM)&vI[0]);
	if (count == LB_ERR)
		return std::vector<std::wstring>();
	
	std::vector<std::wstring> fileret;
	assert(maxcount >= count);
	for (int i = 0; i < count; ++i)
	{
		int index = vI[i];
		RECENTSTYPE::const_iterator it = recents.cbegin();
		advance(it, index);
		fileret.push_back(*it);
	}
	//int cur = SendMessage(hwndList, LB_GETCURSEL, 0, 0);
	//if(cur != LB_ERR)
	//{
	//	int index = SendMessage(hwndList, LB_GETITEMDATA, cur, 0);
	//	if (index != LB_ERR)
	//	{
	//		RECENTSTYPE::const_iterator it = recents.cbegin();
	//		advance(it, index);
	//		fileret.push_back(*it);
	//	}
	//}
	return fileret;
}


LONG getWidth(const RECT& r)
{
	return r.right - r.left;
}
LONG getHeight(const RECT& r)
{
	return r.bottom - r.top;
}
LONG getS2C(HWND h,LONG l)
{
	POINT pt;
	pt.x = l;
	pt.y = 0;
	ScreenToClient(h, &pt);
	return pt.x;
}
BOOL getS2C(HWND h, POINT& pt)
{
	return ScreenToClient(h, &pt);
}
BOOL getS2C(HWND h, RECT& r)
{
	POINT pt1,pt2;
	pt1.x=r.left;
	pt1.y=r.top;
	pt2.x=r.right;
	pt2.y=r.bottom;
	ScreenToClient(h, &pt1);
	ScreenToClient(h, &pt2);

	r.left=pt1.x;
	r.top=pt1.y;
	r.right=pt2.x;
	r.bottom=pt2.y;

	return TRUE;
}

static WNDPROC sprevlistProc;
LRESULT CALLBACK listProc(
	HWND hwnd,        // handle to window
	UINT uMsg,        // message identifier
	WPARAM wParam,    // first message parameter
	LPARAM lParam)    // second message parameter
{
	if (uMsg == WM_MBUTTONDOWN)
	{
		LRESULT result = SendMessage(hwnd, LB_ITEMFROMPOINT, 0, lParam);
		if (HIWORD(result) == 0)
		{
			int index = LOWORD(result);

			// inside client
			bool selected = SendMessage(hwnd, LB_GETSEL, index, 0) > 0;
			
			SendMessage(hwnd, LB_SETSEL, selected ? FALSE : TRUE, index);
		}
	}
	return CallWindowProc(sprevlistProc, hwnd, uMsg, wParam, lParam);
}

BOOL CALLBACK NewCmdDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static std::vector<std::wstring>* spRet;


	static RECT sdeltaL;
	static POINT sdeltaO;
	static POINT sdeltaC;


	static HWND shwndList;
	static HWND shwndBtnOK;
	static HWND shwndBtnCancel;

	static RECENTSTYPE srecents;
	switch(uMsg)
	{
		case WM_INITDIALOG:
		{
			// prepare static values
			spRet = (std::vector<std::wstring>*)lParam;
			shwndList = GetDlgItem(hDlg, IDC_LIST_RECENT);
			// subclass list
			sprevlistProc = (WNDPROC)SetWindowLong(shwndList, GWL_WNDPROC, (LONG)listProc);
			assert(sprevlistProc);

			shwndBtnOK = GetDlgItem(hDlg, IDOK);
			shwndBtnCancel = GetDlgItem(hDlg, IDCANCEL);
			GetRecents(srecents);

			// Set icon
			SendMessage(hDlg, WM_SETICON, ICON_SMALL, (LPARAM)g_hTrayIcon);
			SendMessage(hDlg, WM_SETICON, ICON_BIG, (LPARAM)g_hTrayIcon);
			
			// Internationalize dialog controls
			i18nChangeChildWindowText(hDlg);

			// set title
			wstring title = boostformat(L"%s | %s", NS("Recent Files"), APP_NAME);
			SetWindowText(hDlg, title.c_str());

			
			HDC hdcList = GetWindowDC(shwndList);
			LONG maxLength = SendMessage(shwndList, LB_GETHORIZONTALEXTENT, 0, 0);
			LONG maxLengthOrig = maxLength;
			for (RECENTSTYPE::iterator it = srecents.begin(); it != srecents.end(); ++it)
			{
				tstring str = PathFileExists(it->c_str()) ? L"OK" : L"NA";
				str += L": ";
				str += it->c_str();

				int pos = (int)SendMessage(shwndList,
					LB_ADDSTRING,
					0,
					(LPARAM)str.c_str());
				// Set the array index of the player as item data.
				// This enables us to retrieve the item from the array
				// even after the items are sorted by the list box.
				SendMessage(shwndList, LB_SETITEMDATA, pos, (LPARAM)distance(srecents.begin(), it));

				SIZE size;
				if (GetTextExtentPoint32(hdcList,
					str.c_str(),
					str.size(), // *sizeof((*it)[0]),
					&size))
				{
					maxLength = max(maxLength, size.cx);
				}

			}

			if(maxLength != maxLengthOrig)
			{
				POINT point;
				point.x = 0;
				point.y = maxLength;
				LPtoDP(hdcList, &point, 1);

				SendMessage(shwndList, LB_SETHORIZONTALEXTENT, point.y, 0);
			}
			ReleaseDC(shwndList, hdcList);

			if(SendMessage(shwndList, LB_GETCOUNT,0,0) > 0)
			{
				SendMessage(shwndList, LB_SETCURSEL,0,0);
			}
				
			// Set input focus to the list box.
			SetFocus(shwndList); 

			CenterWindow(hDlg, NULL);
			return TRUE;
		}
		break;

		case WM_SIZE:
		{
			static bool bSet;
			if(!bSet)
			{
				RECT rD,rL,rO,rC;
				GetWindowRect(hDlg, &rD);
				// AdjustWindowRect(&rD, GetWindowLong(hDlg, GWL_STYLE), FALSE);
				GetWindowRect(shwndList, &rL);
				GetWindowRect(shwndBtnOK, &rO);
				GetWindowRect(shwndBtnCancel, &rC);
		
				sdeltaL.left = rD.left - rL.left;
				sdeltaL.top = rD.top - rL.top;
				sdeltaL.right = rD.right - rL.right;
				sdeltaL.bottom = rD.bottom - rL.bottom;

				sdeltaO.x = rD.right - rO.left;
				sdeltaO.y = rD.bottom - rO.top;

				sdeltaC.x = rD.right - rC.left;
				sdeltaC.y = rD.bottom - rC.top;


				bSet = true;
			}
			if(wParam==SIZE_RESTORED || wParam==SIZE_MAXIMIZED)
			{

				RECT rD;
				GetWindowRect(hDlg, &rD);
				// AdjustWindowRect(&rD, GetWindowLong(hDlg, GWL_STYLE), FALSE);
				
				RECT rT;
				
				rT.left = (rD.left - sdeltaL.left);
				rT.top = rD.top - sdeltaL.top;
				rT.right = rD.right - sdeltaL.right;
				rT.bottom = rD.bottom-sdeltaL.bottom;
				getS2C(hDlg,rT);
				MoveWindow(shwndList,
					rT.left, 
					rT.top,
					getWidth(rT),
					getHeight(rT),
					TRUE);


				POINT pT;
				pT.x = rD.right - sdeltaO.x;
				pT.y = rD.bottom-sdeltaO.y;
				getS2C(hDlg,pT);
				SetWindowPos(shwndBtnOK, NULL,
					pT.x,
					pT.y,
					0,
					0,
					SWP_NOZORDER|SWP_NOSIZE);

				pT.x = rD.right - sdeltaC.x;
				pT.y = rD.bottom-sdeltaC.y;
				getS2C(hDlg,pT);
				SetWindowPos(shwndBtnCancel, NULL,
					pT.x,
					pT.y,
					0,
					0,
					SWP_NOZORDER|SWP_NOSIZE);
			}
		}
		break;

		case WM_COMMAND:
		{
			switch(LOWORD(wParam))
			{
				case IDC_LIST_RECENT:
				{
					if(HIWORD(wParam)==LBN_DBLCLK)
					{
						*spRet = GetSelected(shwndList, srecents);

						EndDialog(hDlg, IDOK);
						return 1;
					}
				}
				break;

				case IDOK:
				{
					*spRet = GetSelected(shwndList, srecents);

					EndDialog(hDlg, IDOK);
					return 1;
				}
				break;

				case IDCANCEL:
				{
					EndDialog(hDlg, IDCANCEL);
					return 1;
				}
				break;

			}
			break;
		}
		break;
	}
	return FALSE;
}
