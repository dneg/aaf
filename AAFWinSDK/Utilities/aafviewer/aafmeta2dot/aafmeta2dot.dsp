# Microsoft Developer Studio Project File - Name="aafmeta2dot" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=aafmeta2dot - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "aafmeta2dot.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "aafmeta2dot.mak" CFG="aafmeta2dot - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "aafmeta2dot - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "aafmeta2dot - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "aafmeta2dot - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /I "../../../../Utilities/aafviewer/src/aafmeta2dot" /I "../../../include" /I "../../../../examples2/axLib" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib  kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 axlib.lib aaf.lib aafiid.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib  kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386 /libpath:"../../../lib" /libpath:"../../../../examples2/axLib/Release"

!ELSEIF  "$(CFG)" == "aafmeta2dot - Win32 Debug"

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
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ  /c
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I "../../../../Utilities/aafviewer/src/aafmeta2dot" /I "../../../include" /I "../../../../examples2/axLib" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ  /c
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib  kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 axlib.lib aafd.lib aafiidd.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib  kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept /libpath:"../../../lib" /libpath:"../../../../examples2/axLib/Debug"

!ENDIF 

# Begin Target

# Name "aafmeta2dot - Win32 Release"
# Name "aafmeta2dot - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\..\..\Utilities\aafviewer\src\aafmeta2dot\AAFClassDefinition.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Utilities\aafviewer\src\aafmeta2dot\AAFDefinitionHome.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Utilities\aafviewer\src\aafmeta2dot\AAFDotHome.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Utilities\aafviewer\src\aafmeta2dot\aafmeta2dot.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Utilities\aafviewer\src\aafmeta2dot\AAFMetaDefinition.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Utilities\aafviewer\src\aafmeta2dot\AAFMetaMapProfile.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Utilities\aafviewer\src\aafmeta2dot\AAFPropertyDefinition.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Utilities\aafviewer\src\aafmeta2dot\AAFTypeDefinition.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Utilities\aafviewer\src\aafmeta2dot\AAFXsMapper.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Utilities\aafviewer\src\aafmeta2dot\DotEdge.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Utilities\aafviewer\src\aafmeta2dot\DotElement.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Utilities\aafviewer\src\aafmeta2dot\DotFactory.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Utilities\aafviewer\src\aafmeta2dot\DotGraph.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Utilities\aafviewer\src\aafmeta2dot\DotLogging.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Utilities\aafviewer\src\aafmeta2dot\DotProfile.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Utilities\aafviewer\src\aafmeta2dot\DotRecordNode.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Utilities\aafviewer\src\aafmeta2dot\DotSubGraph.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Utilities\aafviewer\src\aafmeta2dot\DotThingWithID.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Utilities\aafviewer\src\aafmeta2dot\DotUtilities.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Utilities\aafviewer\src\aafmeta2dot\Logging.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Utilities\aafviewer\src\aafmeta2dot\Utilities.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\..\..\Utilities\aafviewer\src\aafmeta2dot\AAFClassDefinition.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Utilities\aafviewer\src\aafmeta2dot\AAFDefinitionHome.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Utilities\aafviewer\src\aafmeta2dot\AAFDotHome.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Utilities\aafviewer\src\aafmeta2dot\AAFMetaDefinition.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Utilities\aafviewer\src\aafmeta2dot\AAFMetaMapProfile.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Utilities\aafviewer\src\aafmeta2dot\AAFPropertyDefinition.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Utilities\aafviewer\src\aafmeta2dot\AAFTypeDefinition.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Utilities\aafviewer\src\aafmeta2dot\AAFXsMapper.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Utilities\aafviewer\src\aafmeta2dot\DotEdge.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Utilities\aafviewer\src\aafmeta2dot\DotElement.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Utilities\aafviewer\src\aafmeta2dot\DotFactory.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Utilities\aafviewer\src\aafmeta2dot\DotGraph.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Utilities\aafviewer\src\aafmeta2dot\DotLogging.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Utilities\aafviewer\src\aafmeta2dot\DotProfile.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Utilities\aafviewer\src\aafmeta2dot\DotRecordNode.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Utilities\aafviewer\src\aafmeta2dot\DotSubGraph.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Utilities\aafviewer\src\aafmeta2dot\DotThingWithID.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Utilities\aafviewer\src\aafmeta2dot\DotUtilities.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Utilities\aafviewer\src\aafmeta2dot\Logging.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Utilities\aafviewer\src\aafmeta2dot\Utilities.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
