# Microsoft Developer Studio Project File - Name="AAFShellExt" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=AAFShellExt - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "AAFShellExt.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "AAFShellExt.mak" CFG="AAFShellExt - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "AAFShellExt - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "AAFShellExt - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "AAFShellExt - Win32 Release"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GR /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /I "WindowsShellProgramming/include" /I "../axLib" /I "../../AAFWinSDK/include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /D "UNICODE" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 shlwapi.lib axLib.lib aafd.lib aafiidd.lib /nologo /subsystem:windows /dll /machine:I386 /nodefaultlib:"MSVCRTD" /libpath:"WindowsShellProgramming/lib" /libpath:"../axLib/Release" /libpath:"../../AAFWinSDK/lib"
# Begin Custom Build - Performing Registration
OutDir=.\Release
TargetPath=.\Release\AAFShellExt.dll
InputPath=.\Release\AAFShellExt.dll
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "AAFShellExt - Win32 Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I "WindowsShellProgramming/include" /I "../axLib" /I "../../AAFWinSDK/include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "UNICODE" /D "_WINDLL" /D "_AFXDLL" /Yu"StdAfx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 shlwapi.lib axLib.lib aafd.lib aafiidd.lib /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept /libpath:"WindowsShellProgramming/lib" /libpath:"../axLib/Debug" /libpath:"../../AAFWinSDK/Debug/Refimpl"
# Begin Custom Build - Performing Registration
OutDir=.\Debug
TargetPath=.\Debug\AAFShellExt.dll
InputPath=.\Debug\AAFShellExt.dll
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ENDIF 

# Begin Target

# Name "AAFShellExt - Win32 Release"
# Name "AAFShellExt - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AAFShellExt.cpp
# End Source File
# Begin Source File

SOURCE=.\AAFShellExt.def
# End Source File
# Begin Source File

SOURCE=.\AAFShellExt.idl
# ADD MTL /h "AAFShellExt_i.h" /iid "AAFShellExt_i.c"
# End Source File
# Begin Source File

SOURCE=.\AAFShellExt.rc
# End Source File
# Begin Source File

SOURCE=.\AAFShellExtContextMenu.cpp
# End Source File
# Begin Source File

SOURCE=.\AAFShellExtDebug.cpp
# End Source File
# Begin Source File

SOURCE=.\AAFShellExtFolder.cpp
# End Source File
# Begin Source File

SOURCE=.\AAFShellExtIcon.cpp
# End Source File
# Begin Source File

SOURCE=.\AAFShellExtIDList.cpp
# End Source File
# Begin Source File

SOURCE=.\AAFShellExtListView.cpp
# End Source File
# Begin Source File

SOURCE=.\AAFShellExtView.cpp
# End Source File
# Begin Source File

SOURCE=.\AboutDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\AxShellExt.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AAFShellExt.H
# End Source File
# Begin Source File

SOURCE=.\AAFShellExtContextMenu.H
# End Source File
# Begin Source File

SOURCE=.\AAFShellExtDebug.h
# End Source File
# Begin Source File

SOURCE=.\AAFShellExtFolder.h
# End Source File
# Begin Source File

SOURCE=.\AAFShellExtIcon.h
# End Source File
# Begin Source File

SOURCE=.\AAFShellExtIDList.h
# End Source File
# Begin Source File

SOURCE=.\AAFShellExtListView.h
# End Source File
# Begin Source File

SOURCE=.\AAFShellExtPIDL.h
# End Source File
# Begin Source File

SOURCE=.\AAFShellExtView.h
# End Source File
# Begin Source File

SOURCE=.\AboutDlg.h
# End Source File
# Begin Source File

SOURCE=.\AxShellExt.h
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\AAFShellExt.rc2
# End Source File
# Begin Source File

SOURCE=.\AAFShellExt.rgs
# End Source File
# Begin Source File

SOURCE=.\AAFShellExtContextMenu.rgs
# End Source File
# Begin Source File

SOURCE=.\AAFShellExtFolder.rgs
# End Source File
# Begin Source File

SOURCE=.\AAFShellExtIcon.rgs
# End Source File
# Begin Source File

SOURCE=.\AAFShellExtIDList.rgs
# End Source File
# Begin Source File

SOURCE=.\AAFShellExtView.rgs
# End Source File
# Begin Source File

SOURCE=.\res\app_icon.ico
# End Source File
# Begin Source File

SOURCE=.\res\data_item.ico
# End Source File
# Begin Source File

SOURCE=.\res\folder.ico
# End Source File
# Begin Source File

SOURCE=.\res\folderop.ico
# End Source File
# Begin Source File

SOURCE=.\res\tbextens.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.TXT
# End Source File
# End Target
# End Project
