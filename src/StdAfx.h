#if !defined(AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_)
#define AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#pragma warning(disable:4786)

#define _CRT_SECURE_NO_WARNINGS

#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINNT _WIN32_WINNT_WIN2K
#define _WIN32_IE 0x0500

//#ifdef _DEBUG
//#define _CRTDBG_MAP_ALLOC  
//#include <stdlib.h>  
//#include <crtdbg.h>  
//#endif

#if _MSC_VER <= 1400 // below vc2005
#include <checkaccess.h>
#endif

// #include <afx.h>
#include <windows.h>
#include <tchar.h>
#include <process.h>
#include <time.h>

#include <shlwapi.h>
#include <Shellapi.h>
#include <string>
#include <list>


#include <stlsoft/smartptr/scoped_handle.hpp>


using namespace std;

#include "../../lsMisc/tstring.h"
#include "../../lsMisc/GetModuleDirectory.h"
#include "../../lsMisc/IsDirectory.h"
#include "../../lsMisc/HelpDefines.h"
#include "../../lsMisc/CreateSimpleWindow.h"
#include "../../lsMisc/GetDirFromPath.h"
#include "../../lsMisc/SHMoveFile.h"
#include "../../lsMisc/I18N.h"


#include "../../lsMisc/IsFileExists.h"
#include "../../lsMisc/UrlEncode.h"
#include "../../lsMisc/UTF16toUTF8.h"
#include "../../lsMisc/IsFileOpen.h"
#include "../../lsMisc/WritePrivateProfileInt.h"
#include "../../lsMisc/CenterWindow.h"
#include "../../lsMisc/ResolveLink.h"
#include "../../lsMisc/stlScopedClear.h"
#include "../../lsMisc/CommandLineParser.h"


#include "../../lsMisc/stdwin32/stdwin32.h"
using namespace stdwin32;

#include "tmphelp.h"

#define APP_NAME _T("kocchiwork")
#define APP_VERSION L"1.5"

#define NS(s) Ambiesoft::I18N(_T(s))
enum {
	WM_APP_TRAY_NOTIFY = (WM_APP+1),
	WM_APP_LACHANGED,
	WM_APP_APPKANHIDONE,
};
#define CRLF _T("\r\n")


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++

#endif // !defined(AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_)
