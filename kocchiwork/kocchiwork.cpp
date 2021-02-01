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
#include "thread.h"
#include "err.h"
#include "common.h"

// #include "systeminfo.h"


#include "../../lsMisc/IsFileExists.h"
#include "../../lsMisc/UrlEncode.h"
#include "../../lsMisc/UTF16toUTF8.h"
#include "../../lsMisc/IsFileOpen.h"






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





