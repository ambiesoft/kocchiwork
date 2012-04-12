#if !defined(AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_)
#define AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <tchar.h>
#include <process.h>
#include <shlwapi.h>
#include <Shellapi.h>
#include <string>

using namespace std;

#include "../MyUtility/tstring.h"
#include "../MyUtility/GetModuleDirectory.h"
#include "../MyUtility/IsDirectory.h"
#include "../MyUtility/HelpDefines.h"
#include "../MyUtility/CreateSimpleWindow.h"
#include "../MyUtility/GetDirFromPath.h"
#include "../MyUtility/SHDeleteFile.h"

#include "tmphelp.h"

#define APP_NAME _T("kocchiwork")
#define NS(s) _T(s)
enum {
	WM_APP_TRAY_NOTIFY = (WM_APP+1),
	WM_APP_LACHANGED,
	WM_APP_APPKANHIDONE,
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++

#endif // !defined(AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_)
