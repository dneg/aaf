# Microsoft Developer Studio Project File - Name="ComPersonnelExtension" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=ComPersonnelExtension - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ComPersonnelExtension.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ComPersonnelExtension.mak" CFG="ComPersonnelExtension - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ComPersonnelExtension - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "ComPersonnelExtension - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ComPersonnelExtension - Win32 Release"

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
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "COMPERSONNELEXTENSION_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /I "..\..\..\include" /I "..\..\..\..\examples\com-api\ComExtension" /I "..\..\..\..\ref-impl\src\com-api\com-dll" /I "../../../../Utilities/Include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "UNICODE" /D "_UNICODE" /D "_USRDLL" /D "COMPERSONNELEXTENSION_EXPORTS" /FD /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 aafiid.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386 /out:"../../../Release/Examples/Com/AAFPersonnelExtension.dll" /libpath:"..\..\..\lib"

!ELSEIF  "$(CFG)" == "ComPersonnelExtension - Win32 Debug"

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
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "COMPERSONNELEXTENSION_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I "..\..\..\include" /I "..\..\..\..\examples\com-api\ComExtension" /I "..\..\..\..\ref-impl\src\com-api\com-dll" /I "../../../../Utilities/Include" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "UNICODE" /D "_UNICODE" /D "_USRDLL" /D "COMPERSONNELEXTENSION_EXPORTS" /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 aafiidd.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /out:"../../../Debug/Examples/Com/AAFPersonnelExtension.dll" /pdbtype:sept /libpath:"..\..\..\lib"

!ENDIF 

# Begin Target

# Name "ComPersonnelExtension - Win32 Release"
# Name "ComPersonnelExtension - Win32 Debug"
# Begin Group "AAF Libraries"

# PROP Default_Filter "lib"
# Begin Source File

SOURCE=..\..\..\lib\AAF.lib

!IF  "$(CFG)" == "ComPersonnelExtension - Win32 Release"

!ELSEIF  "$(CFG)" == "ComPersonnelExtension - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\lib\AAFD.lib

!IF  "$(CFG)" == "ComPersonnelExtension - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ComPersonnelExtension - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\lib\AAFIID.lib

!IF  "$(CFG)" == "ComPersonnelExtension - Win32 Release"

!ELSEIF  "$(CFG)" == "ComPersonnelExtension - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\lib\AAFIIDD.lib

!IF  "$(CFG)" == "ComPersonnelExtension - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ComPersonnelExtension - Win32 Debug"

!ENDIF 

# End Source File
# End Group
# Begin Source File

SOURCE="..\..\..\..\examples\com-api\ComExtension\ComPersonnelExtension\AAFPersonnelExtension.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\examples\com-api\ComExtension\ComPersonnelExtension\AAFPersonnelExtension.def"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\examples\com-api\ComExtension\ComPersonnelExtension\CAAFAdminMob.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\ref-impl\src\com-api\com-dll\CAAFClassFactory.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\examples\com-api\ComExtension\ComPersonnelExtension\CAAFPersonnelResource.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\ref-impl\src\com-api\com-dll\CAAFServer.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\ref-impl\src\com-api\com-dll\CAAFUnknown.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\examples\com-api\ComExtension\extensionUtils.cpp"
# End Source File
# End Target
# End Project
