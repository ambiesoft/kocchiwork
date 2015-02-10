# Microsoft Developer Studio Project File - Name="kocchiwork" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** 編集しないでください **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=kocchiwork - Win32 UnicodeDebug
!MESSAGE これは有効なﾒｲｸﾌｧｲﾙではありません。 このﾌﾟﾛｼﾞｪｸﾄをﾋﾞﾙﾄﾞするためには NMAKE を使用してください。
!MESSAGE [ﾒｲｸﾌｧｲﾙのｴｸｽﾎﾟｰﾄ] ｺﾏﾝﾄﾞを使用して実行してください
!MESSAGE 
!MESSAGE NMAKE /f "kocchiwork.mak".
!MESSAGE 
!MESSAGE NMAKE の実行時に構成を指定できます
!MESSAGE ｺﾏﾝﾄﾞ ﾗｲﾝ上でﾏｸﾛの設定を定義します。例:
!MESSAGE 
!MESSAGE NMAKE /f "kocchiwork.mak" CFG="kocchiwork - Win32 UnicodeDebug"
!MESSAGE 
!MESSAGE 選択可能なﾋﾞﾙﾄﾞ ﾓｰﾄﾞ:
!MESSAGE 
!MESSAGE "kocchiwork - Win32 UnicodeDebug" ("Win32 (x86) Application" 用)
!MESSAGE "kocchiwork - Win32 UnicodeRelease" ("Win32 (x86) Application" 用)
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "kocchiwork - Win32 UnicodeDebug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "kocchiwork___Win32_UnicodeDebug"
# PROP BASE Intermediate_Dir "kocchiwork___Win32_UnicodeDebug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "UnicodeDebug"
# PROP Intermediate_Dir "UnicodeDebug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "L:\check" /I "L:\stlsoft-1.9.118\include" /I "L:\MSSDK\2003\Include" /I "L:\boost1.35" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "UNICODE" /D "_UNICODE" /D "WINNT" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /out:"C:\LinkOut\kocchiwork\kocchiwork.exe" /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /debug /machine:I386 /out:"C:\LinkOut\kocchiwork\kocchiworkD.exe" /pdbtype:sept /libpath:"L:\MSSDK\2003\Lib"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy       C:\Linkout\CommonExe\showballoon.exe       C:\Linkout\kocchiwork\      	mkdir       C:\Linkout\kocchiwork\lang      	copy       lang\jpn.txt       C:\Linkout\kocchiwork\lang\ 
# End Special Build Tool

!ELSEIF  "$(CFG)" == "kocchiwork - Win32 UnicodeRelease"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "kocchiwork___Win32_UnicodeRelease"
# PROP BASE Intermediate_Dir "kocchiwork___Win32_UnicodeRelease"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "UnicodeRelease"
# PROP Intermediate_Dir "UnicodeRelease"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FR /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "L:\check" /I "L:\stlsoft-1.9.118\include" /I "L:\MSSDK\2003\Include" /I "L:\boost1.35" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "UNICODE" /D "_UNICODE" /D "WINNT" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386 /out:"C:\LinkOut\kocchiwork\kocchiwork.exe"
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /machine:I386 /out:"C:\LinkOut\kocchiwork\kocchiwork.exe" /libpath:"L:\MSSDK\2003\Lib"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy       C:\Linkout\CommonExe\showballoon.exe       C:\Linkout\kocchiwork\      	mkdir       C:\Linkout\kocchiwork\lang      	copy       lang\jpn.txt       C:\Linkout\kocchiwork\lang\ 
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "kocchiwork - Win32 UnicodeDebug"
# Name "kocchiwork - Win32 UnicodeRelease"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "MyU"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\MyUtility\CreateSimpleWindow.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\MyUtility\GetDirFromPath.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\MyUtility\GetModuleDirectory.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\MyUtility\I18N.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\MyUtility\IsFileExists.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\MyUtility\IsFileOpen.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\MyUtility\SHDeleteFile.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\MyUtility\StdStringReplace.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\MyUtility\UrlEncode.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\MyUtility\UTF16toUTF8.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# End Group
# Begin Source File

SOURCE=.\src\common.cpp
# End Source File
# Begin Source File

SOURCE=.\src\err.cpp
# End Source File
# Begin Source File

SOURCE=.\src\kocchiwork.cpp
# End Source File
# Begin Source File

SOURCE=.\src\main.cpp
# End Source File
# Begin Source File

SOURCE=.\src\ReturnFileAndQuit.cpp
# End Source File
# Begin Source File

SOURCE=.\src\StdAfx.cpp
# ADD CPP /Yc
# End Source File
# Begin Source File

SOURCE=.\src\thread.cpp
# End Source File
# Begin Source File

SOURCE=.\src\tmphelp.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\src\WndProc.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "MyUH"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\MyUtility\CreateSimpleWindow.h
# End Source File
# Begin Source File

SOURCE=..\MyUtility\GetModuleDirectory.h
# End Source File
# Begin Source File

SOURCE=..\MyUtility\I18N.h
# End Source File
# Begin Source File

SOURCE=..\MyUtility\IsDirectory.h
# End Source File
# Begin Source File

SOURCE=..\MyUtility\IsFileExists.h
# End Source File
# Begin Source File

SOURCE=..\MyUtility\IsFileOpen.h
# End Source File
# Begin Source File

SOURCE=..\MyUtility\StdStringReplace.h
# End Source File
# Begin Source File

SOURCE=..\MyUtility\UrlEncode.h
# End Source File
# Begin Source File

SOURCE=..\MyUtility\UTF16toUTF8.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\src\common.h
# End Source File
# Begin Source File

SOURCE=.\src\err.h
# End Source File
# Begin Source File

SOURCE=.\src\kocchiwork.h
# End Source File
# Begin Source File

SOURCE=.\src\resource.h
# End Source File
# Begin Source File

SOURCE=.\src\ReturnFileAndQuit.h
# End Source File
# Begin Source File

SOURCE=.\src\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\src\thread.h
# End Source File
# Begin Source File

SOURCE=.\src\tmphelp.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\src\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\src\kocchiwork.rc
# End Source File
# End Group
# End Target
# End Project
