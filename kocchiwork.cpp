#include "stdafx.h"

#include "resource.h"
#include "thread.h"
#include "err.h"
#include "common.h"

// #include "systeminfo.h"

#include "../MyUtility/StdStringReplace.h"
#include "../MyUtility/IsFileExists.h"
#include "../MyUtility/UrlEncode.h"
#include "../MyUtility/UTF16toUTF8.h"
#include "../MyUtility/IsFileOpen.h"

bool ReturnFileAndQuit(HWND hWnd);



void EnableDebugPriv( void )
{
	HANDLE hToken;
	LUID sedebugnameValue;
	TOKEN_PRIVILEGES tkp;

	// enable the SeDebugPrivilege
	if ( ! OpenProcessToken( GetCurrentProcess(),
		TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken ) )
	{
		_tprintf( _T("OpenProcessToken() failed, Error = %d SeDebugPrivilege is not available.\n") , GetLastError() );
		return;
	}

	if ( ! LookupPrivilegeValue( NULL, SE_DEBUG_NAME, &sedebugnameValue ) )
	{
		_tprintf( _T("LookupPrivilegeValue() failed, Error = %d SeDebugPrivilege is not available.\n"), GetLastError() );
		CloseHandle( hToken );
		return;
	}

	tkp.PrivilegeCount = 1;
	tkp.Privileges[0].Luid = sedebugnameValue;
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	if ( ! AdjustTokenPrivileges( hToken, FALSE, &tkp, sizeof tkp, NULL, NULL ) )
		_tprintf( _T("AdjustTokenPrivileges() failed, Error = %d SeDebugPrivilege is not available.\n"), GetLastError() );
		
	CloseHandle( hToken );
}

LPCTSTR GetFileNamePosition( LPCTSTR lpPath )
{
	LPCTSTR lpAct = lpPath + _tcslen( lpPath );

	while ( lpAct > lpPath && *lpAct != _T('\\') && *lpAct != _T('/') )
		lpAct--;

	if ( lpAct > lpPath )
		lpAct++;

	return lpAct;
}











/*
BOOL IsFileOpenedt(LPCTSTR pFile)
{
	BOOL bFullPathCheck = TRUE;
	BOOL bShow = FALSE;
	CString name;
	CString processName;
	CString fsFilePath;
	CString deviceFileName;

	if ( bFullPathCheck )
	{
		if ( !SystemInfoUtils::GetDeviceFileName( pFile, deviceFileName ) )
		{
			// _tprintf( _T("GetDeviceFileName() failed.\n") );
			return FALSE;
		}
	}
	SystemProcessInformation::SYSTEM_PROCESS_INFORMATION* p;
	SystemProcessInformation pi;
	SystemHandleInformation hi( -1 );
	hi.SetFilter( _T("File"), TRUE ); // Refresh

	for ( POSITION pos = hi.m_HandleInfos.GetHeadPosition(); pos != NULL; )
	{
		SystemHandleInformation::SYSTEM_HANDLE& h = hi.m_HandleInfos.GetNext(pos);

		if ( pi.m_ProcessInfos.Lookup( h.ProcessID, p ) )
		{
			SystemInfoUtils::Unicode2CString( &p->usName, processName );
		}
		else
			processName = "";

		//NT4 Stupid thing if it is the services.exe and I call GetName :((
		if ( INtDll::dwNTMajorVersion == 4 && _tcsicmp( processName, _T("services.exe" ) ) == 0 )
			continue;
		
		hi.GetName( (HANDLE)h.HandleNumber, name, h.ProcessID );

		if ( bFullPathCheck )
			bShow =	_tcsicmp( name, deviceFileName ) == 0;
		else
			bShow =	_tcsicmp( GetFileNamePosition(name), pFile ) == 0;

		if ( bShow )
		{
			return TRUE;

//			if ( !bFullPathCheck )
//			{
//				fsFilePath = "";
//				SystemInfoUtils::GetFsFileName( name, fsFilePath );
//			}
//			
//			_tprintf( _T("0x%04X  %-20s  %s\n"), 
//				h.ProcessID, 
//				processName,
//				!false ? fsFilePath : g_workfile.c_str() );
		}
	}
	return FALSE;
}
*/





