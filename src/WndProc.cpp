#include "stdafx.h"

#include "resource.h"
#include "thread.h"
#include "err.h"
#include "common.h"
// #include "systeminfo.h"

#include "../../MyUtility/StdStringReplace.h"
#include "../../MyUtility/IsFileExists.h"
#include "../../MyUtility/UrlEncode.h"
#include "../../MyUtility/UTF16toUTF8.h"
#include "../../MyUtility/IsFileOpen.h"
#include "../../MyUtility/showballoon.h"
#include "../../MyUtility/I18N.h"

#include "C:\\Linkout\\CommonDLL\\TimedMessageBox.h"

wstring myUrlEncode(wstring strIN)
{
	BYTE* pUtf8 = UTF16toUTF8(strIN.c_str());
	stlsoft::scoped_handle<void*> mapUtf8(pUtf8, free);

	char* pUtf8Encoded=NULL;
	// UrlEncode(pUtf8, strlen((char*)pUtf8), &pUtf8Encoded, TRUE);
	pUtf8Encoded= urlencodenew((char*)pUtf8);
	stlsoft::scoped_handle<void*> mapUtf8Encoded(pUtf8Encoded, free);

	wchar_t* pLast = UTF8toUTF16((LPBYTE)pUtf8Encoded);
	stlsoft::scoped_handle<void*> mapLast(pLast, free);
	return pLast;
}

BOOL doQueryEndSession()
{
	HMODULE hModule = LoadLibrary(_T("TimedMessageBox.dll"));
	if(!hModule)
		return TRUE;

	FNTimedMessageBox func=NULL;
	func = (FNTimedMessageBox)GetProcAddress(hModule, "fnTimedMessageBox");
	if(!func)
		return TRUE;

	if(IDOK != func(300, APP_NAME, NS("message"), MB_SYSTEMMODAL))
		return FALSE;
	return TRUE; 
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	static UINT sTaskBarCreated;
	if(nMsg >= 0xC000 && nMsg == sTaskBarCreated)
	{
		tstring traytip = NS("watching");
		traytip += _T(" ");
		traytip += g_workfile.c_str();

		

		if(!AddTrayIcon(
			g_hWnd, 
			WM_APP_TRAY_NOTIFY, 
			g_hTrayIcon, 
			traytip.c_str()))
		{
			errExit(NS("could not register tray icon."),GetLastError());
		}
		return 0;
	}

	static bool btimerprocessing;
	switch(nMsg)
	{
		case WM_CREATE:
		{
			sTaskBarCreated = RegisterWindowMessage(_T("TaskbarCreated"));

//			EnableDebugPriv();
			SetTimer(hWnd, 1, 5000,NULL);
		}
		break;
		
		case WM_TIMER:
		{
			if(btimerprocessing)
				break;
			btimerprocessing = true;
			if(g_hKanshiApp)
			{
				DWORD dw =WaitForSingleObject(g_hKanshiApp, 1);
				if(dw == WAIT_OBJECT_0)
				{// app is gone
					g_hKanshiApp = NULL;
					if(IsFileOpen(g_workfile.c_str()))
					{
						wstring title=L"kocchiwork";
						wstring text = NS("App was closed but file still opens, changed mode to monitor file");

/**
						wchar_t file[MAX_PATH];
						GetModuleFileName(NULL, file, MAX_PATH);
						*wcsrchr(file, L'\\') = 0;
						lstrcat(file, L"\\showballoon.exe");

						HICON hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON_MAIN));
						showballoon(g_hWnd,title ,text, g_hTrayIcon,WM_APP_TRAY_NOTIFY,TRUE);
**/


						if(!PopupTrayIcon(
							g_hWnd, 
							WM_APP_TRAY_NOTIFY, 
							g_hTrayIcon, 
							text.c_str()))
						{
							errExit(NS("could not register tray icon."),GetLastError());
						}




						//wstring param;
						//param += L"/title:";
						//param += myUrlEncode(title);
						//param += L" /icon:kocchiwork.exe";
						//param += _T(" \"") + myUrlEncode(message) + _T("\"");						

						//ShellExecute(NULL,
						//	NULL,
						//	file,
						//	param.c_str(),
						//	NULL,
						//	SW_SHOW);
						
						//SetForegroundWindow(hWnd);
						//MessageBoxA(hWnd,
						 	//"sss",
							//"sss",
							//0);
						// KillTimer(hWnd,1);
					}
					else
					{ // no one opens file
						int mret = IDNO;
						if( (time(NULL)-g_starttime) < 10)
						{
							SetForegroundWindow(hWnd);
							mret = MessageBox(hWnd,
								NS("App closed. Do you want to continue monitor file?"),
								L"kocchiwork",
								MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2);
						}

						if(mret==IDYES)
						{//use file monitor
							KillTimer(hWnd,1);
						}
						else
						{
							//quit
							KillTimer(hWnd,1);
							PostQuitMessage(0);
						}
					}
				}
			}
			else
			{// no kanshiapp
				static int count;				
				if(!IsFileOpen(g_workfile.c_str()))
				{
					count++;
					if(count > 1)
					{
						ReturnFileAndQuit(hWnd);
						KillTimer(hWnd,1);
						PostQuitMessage(0);
					}
				}
				else
				{
					count=0;
				}
			}
			btimerprocessing = false;
		}
		break;
		
		case WM_APP_TRAY_NOTIFY:
		switch(lParam)
		{
			case WM_LBUTTONUP:
			{
				SetForegroundWindow(hWnd);
			}
			break;

			case WM_RBUTTONUP:
			{
				POINT apos;
				HMENU hSubMenu = CreatePopupMenu();
#ifdef _DEBUG
				AppendMenu(hSubMenu, MF_BYCOMMAND, IDC_START, NS("TEST"));
				AppendMenu(hSubMenu, MF_SEPARATOR, 0, NULL);
#endif
				AppendMenu(hSubMenu, MF_BYCOMMAND, IDC_ABOUT, NS("&About..."));
				AppendMenu(hSubMenu, MF_SEPARATOR, 0, NULL);

				AppendMenu(hSubMenu, MF_BYCOMMAND, IDC_REOPENFILE, NS("&Reopen"));
				AppendMenu(hSubMenu, MF_BYCOMMAND, IDC_OPENWITHEXPLORER, NS("&Open File Location"));
				AppendMenu(hSubMenu, MF_SEPARATOR, 0, NULL);
				
				AppendMenu(hSubMenu, MF_BYCOMMAND, IDC_QUIT, NS("&Exit"));

				Ambiesoft::i18nChangeMenuText(hSubMenu);

				SetForegroundWindow(hWnd);
				GetCursorPos((LPPOINT)&apos);

				TrackPopupMenu(hSubMenu,
					TPM_TOPALIGN | TPM_RIGHTBUTTON | TPM_LEFTBUTTON,
					apos.x, apos.y, 0, hWnd, NULL);

				DestroyMenu(hSubMenu);
			}
			break;

		}
		break;
	
		case WM_APP_LACHANGED:
		{
			if(!g_hKanshiApp)
				ReturnFileAndQuit(hWnd);
		}
		break;
	
		case WM_APP_APPKANHIDONE:
		{
			PostQuitMessage(0);
		}
		break;
	
		case WM_QUERYENDSESSION:
		{
			return doQueryEndSession();
		}
		break;

		case WM_COMMAND:
		switch(LOWORD(wParam))
		{
			case IDC_ABOUT:
			{
				tstring message;
				message += APP_NAME L" " APP_VERSION;
				message += _T("\r\n\r\n");
				message += _T("Watching\t");
				message += g_workfile;
				message += _T("\r\n");
				message += _T("Remote\t");
				message += g_remotefile;
				MessageBox(hWnd, 
					message.c_str(),
					APP_NAME, 
					MB_ICONINFORMATION);
			}
			break;
	
			case IDC_QUIT:
			{
				DestroyWindow(hWnd);
				PostQuitMessage(0);
			}
			break;

			case IDC_REOPENFILE:
			{
				SHELLEXECUTEINFO sei = {0};
				sei.cbSize = sizeof(sei);
				sei.lpVerb = NULL;
				sei.lpFile = g_workfile.c_str();
				sei.nShow = SW_SHOW;
				tstring dir = GetDirFromPath(g_workfile.c_str());
				sei.lpDirectory = dir.c_str();
				if(!ShellExecuteEx(&sei)) 
				{
					MessageBox(hWnd,
						NS("Failed to ShellExecuteEx"),
						APP_NAME,
						MB_ICONERROR);
				}
			}
			break;

			case IDC_OPENWITHEXPLORER:
			{
				tstring param = _T("/select,");
				param += g_workfile;
				ShellExecute(NULL, _T("open"), _T("Explorer"), param.c_str(), NULL, SW_SHOW);
			}
			break;
		}
		break;

	default:
		return DefWindowProc(hWnd, nMsg, wParam, lParam);
	}

	return 0;
}
