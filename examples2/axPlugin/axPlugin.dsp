# Microsoft Developer Studio Project File - Name="axPlugin" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=axPlugin - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "axPlugin.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "axPlugin.mak" CFG="axPlugin - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "axPlugin - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "axPlugin - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "axPlugin - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "AXPLUGIN_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /I "../../AAFWinSDK/include" /I "../axLib" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "AXPLUGIN_EXPORTS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 axLib.lib aaf.lib aafiid.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386 /libpath:"../axLib/Debug" /libpath:"../../AAFWinSDK/lib"

!ELSEIF  "$(CFG)" == "axPlugin - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "AXPLUGIN_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /w /W0 /Gm /GX /ZI /Od /I "../../AAFWinSDK/include" /I "../axLib" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "AXPLUGIN_EXPORTS" /D "UNICODE" /D "_UNICODE" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 axLib.lib aafd.lib aafiidd.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept /libpath:"../axLib/Debug" /libpath:"../../AAFWinSDK/Debug/Refimpl"

!ENDIF 

# Begin Target

# Name "axPlugin - Win32 Release"
# Name "axPlugin - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AxImplNullEssenceCodec.cpp
# End Source File
# Begin Source File

SOURCE=.\AxImplPlugin.cpp
# End Source File
# Begin Source File

SOURCE=.\AxPlugin.cpp
# End Source File
# Begin Source File

SOURCE=.\AxPluginEntryPoints.cpp
# End Source File
# Begin Source File

SOURCE=.\AxPluginFctry.cpp
# End Source File
# Begin Source File

SOURCE=.\AxPluginFctryPrtcl.cpp
# End Source File
# Begin Source File

SOURCE=.\AxPluginFormatSpecifiers.cpp
# End Source File
# Begin Source File

SOURCE=.\AxPluginRegistry.cpp
# End Source File
# Begin Source File

SOURCE=.\AxPluginUtil.cpp
# End Source File
# Begin Source File

SOURCE=.\AxPluginWin.cpp
# End Source File
# Begin Source File

SOURCE=.\AxPluginWin.def
# End Source File
# Begin Source File

SOURCE=.\CAxClassFactory.cpp
# End Source File
# Begin Source File

SOURCE=.\CAxEssenceCodec.cpp
# End Source File
# Begin Source File

SOURCE=.\CAxNullEssenceCodec.cpp
# End Source File
# Begin Source File

SOURCE=.\CAxPlugin.cpp
# End Source File
# Begin Source File

SOURCE=.\CAxUnknown.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AxImplNullEssenceCodec.h
# End Source File
# Begin Source File

SOURCE=.\AxImplPlugin.h
# End Source File
# Begin Source File

SOURCE=.\AxPlugin.h
# End Source File
# Begin Source File

SOURCE=.\AxPluginFctry.h
# End Source File
# Begin Source File

SOURCE=.\AxPluginFctryPrtcl.h
# End Source File
# Begin Source File

SOURCE=.\AxPluginFormatSpecifiers.h
# End Source File
# Begin Source File

SOURCE=.\AxPluginRegistry.h
# End Source File
# Begin Source File

SOURCE=.\AxPluginUtil.h
# End Source File
# Begin Source File

SOURCE=.\CAxClassFactory.h
# End Source File
# Begin Source File

SOURCE=.\CAxEssenceCodec.h
# End Source File
# Begin Source File

SOURCE=.\CAxNullEssenceCodec.h
# End Source File
# Begin Source File

SOURCE=.\CAxPlugin.h
# End Source File
# Begin Source File

SOURCE=.\CAxUnknown.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
