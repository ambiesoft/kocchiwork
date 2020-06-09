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

#include "../../lsMisc/HelpDefines.h"
// #include "../../lsMisc/WritePrivateProfileWString.h"
#include "../../profile/cpp/Profile/include/ambiesoft.profile.h"

#include "resource.h"
#include "heavyboost.h"
#include "err.h"
#include "app.h"
#include "common.h"


using namespace Ambiesoft;
using namespace Ambiesoft::stdosd;
using namespace std;

std::wstring GetIniFile()
{
	wstring inifile = stdCombinePath(stdGetParentDirectory(stdGetModuleFileName<wchar_t>()),
		stdGetFileNameWitoutExtension(stdGetModuleFileName<wchar_t>()) + L".ini");
	

	return inifile;
}

void GetRecents(RECENTSTYPE& recents)
{
	tstring inifile = GetIniFile();

	{
		ProcessMutex mut;
		Profile::CHashIni ini = Profile::ReadAll(inifile);
		int count = 0;
		Profile::GetInt(
			"recents",
			"count",
			0,
			count,
			ini);

		for (int i = 0; i < count; ++i)
		{
			string sec = "recent_" + to_string(i);
			string sout;
			if (Profile::GetString(
				"recents",
				sec,
				"",
				sout,
				ini))
			{
				if (!sout.empty())
				{
					recents.push_back(toStdWstringFromUtf8(sout));
				}
			}
		}
	}
}

BOOL SaveRecent(LPCTSTR pApp, LPCTSTR pFile)
{
	tstring inifile = GetIniFile();

	RECENTSTYPE recents;
	GetRecents(recents);

	recents.remove(pFile);
	recents.push_front(pFile);

	int count = min(recents.size(), MAX_RECENT_SIZE);
	RECENTSTYPE::iterator it;
	int i=0;
	{
		ProcessMutex mut;
		Profile::CHashIni ini = Profile::ReadAll(inifile);
		for(it=recents.begin() ; it != recents.end() ; ++it, ++i)
		{
			string sec = "recent_" + to_string(i);

			if (!Profile::WriteString(
				"recents",
				sec,
				toStdUtf8String(it->c_str()),
				ini))
			{
				return FALSE;
			}
		}
		if(!Profile::WriteInt(
			"recents",
			"count",
			i,
			ini))
		{
			return FALSE;
		}

		if (!Profile::WriteAll(ini, inifile))
			return FALSE;
	}
	return TRUE;
}





