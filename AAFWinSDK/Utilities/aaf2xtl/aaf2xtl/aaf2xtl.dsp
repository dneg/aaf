# Microsoft Developer Studio Project File - Name="aaf2xtl" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=aaf2xtl - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "aaf2xtl.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "aaf2xtl.mak" CFG="aaf2xtl - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "aaf2xtl - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "aaf2xtl - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "aaf2xtl - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\Debug"
# PROP BASE Intermediate_Dir ".\Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\Debug"
# PROP Intermediate_Dir ".\Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /I "../../../../AAFWinSDK/include" /I "../../../../examples2/axLib" /I "..\..\..\..\Utilities\aaf2xtl\aif2xtllib" /Zi /W3 /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /GR /YX /Fp".\Debug/aaf2xtl.pch" /Fo".\Debug/" /Fd".\Debug/" /GZ /c /GX 
# ADD CPP /nologo /MDd /I "../../../../AAFWinSDK/include" /I "../../../../examples2/axLib" /I "..\..\..\..\Utilities\aaf2xtl\aif2xtllib" /Zi /W3 /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /GR /YX /Fp".\Debug/aaf2xtl.pch" /Fo".\Debug/" /Fd".\Debug/" /GZ /c /GX 
# ADD BASE MTL /nologo /tlb".\Debug\aaf2xtl.tlb" /win32 
# ADD MTL /nologo /tlb".\Debug\aaf2xtl.tlb" /win32 
# ADD BASE RSC /l 1033 /d "_DEBUG" 
# ADD RSC /l 1033 /d "_DEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo 
# ADD BSC32 /nologo 
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib aiflib.lib aif2xtllib.lib aifsantefelib.lib axLib.lib aafd.lib aafiidd.lib odbc32.lib odbccp32.lib /nologo /out:".\Debug\aaf2xtl.exe" /incremental:no /libpath:"../aiflib/Debug" /libpath:"../aif2xtllib/Debug" /libpath:"../aifsantefelib/Debug" /libpath:"../../../../examples2/axLib/Debug" /libpath:"../..//Debug/Refimpl" /libpath:"../../../lib" /debug /pdb:".\Debug\aaf2xtl.pdb" /pdbtype:sept /subsystem:console /machine:ix86 
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib aiflib.lib aif2xtllib.lib aifsantefelib.lib axLib.lib aafd.lib aafiidd.lib odbc32.lib odbccp32.lib /nologo /out:".\Debug\aaf2xtl.exe" /incremental:no /libpath:"../aiflib/Debug" /libpath:"../aif2xtllib/Debug" /libpath:"../aifsantefelib/Debug" /libpath:"../../../../examples2/axLib/Debug" /libpath:"../..//Debug/Refimpl" /libpath:"../../../lib" /debug /pdb:".\Debug\aaf2xtl.pdb" /pdbtype:sept /subsystem:console /machine:ix86 

!ELSEIF  "$(CFG)" == "aaf2xtl - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\Release"
# PROP BASE Intermediate_Dir ".\Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\Release"
# PROP Intermediate_Dir ".\Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /I "../../../../AAFWinSDK/include" /I "../../../../examples2/axLib" /I "..\..\..\..\Utilities\aaf2xtl\aif2xtllib" /W3 /O2 /Ob1 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /GF /Gy /GR /YX /Fp".\Release/aaf2xtl.pch" /Fo".\Release/" /Fd".\Release/" /c /GX 
# ADD CPP /nologo /MD /I "../../../../AAFWinSDK/include" /I "../../../../examples2/axLib" /I "..\..\..\..\Utilities\aaf2xtl\aif2xtllib" /W3 /O2 /Ob1 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /GF /Gy /GR /YX /Fp".\Release/aaf2xtl.pch" /Fo".\Release/" /Fd".\Release/" /c /GX 
# ADD BASE MTL /nologo /tlb".\Release\aaf2xtl.tlb" /win32 
# ADD MTL /nologo /tlb".\Release\aaf2xtl.tlb" /win32 
# ADD BASE RSC /l 1033 /d "NDEBUG" 
# ADD RSC /l 1033 /d "NDEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo 
# ADD BSC32 /nologo 
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib aiflib.lib aif2xtllib.lib aifsantefelib.lib axLib.lib aaf.lib aafiid.lib odbc32.lib odbccp32.lib /nologo /out:".\Release\aaf2xtl.exe" /incremental:no /libpath:"../aiflib/Release" /libpath:"../aif2xtllib/Release" /libpath:"../aifsantefelib/Release" /libpath:"../../../../examples2/axLib/Release" /libpath:"../..//Release/Refimpl" /libpath:"../../../lib" /pdb:".\Release\aaf2xtl.pdb" /pdbtype:sept /subsystem:console /machine:ix86 
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib aiflib.lib aif2xtllib.lib aifsantefelib.lib axLib.lib aaf.lib aafiid.lib odbc32.lib odbccp32.lib /nologo /out:".\Release\aaf2xtl.exe" /incremental:no /libpath:"../aiflib/Release" /libpath:"../aif2xtllib/Release" /libpath:"../aifsantefelib/Release" /libpath:"../../../../examples2/axLib/Release" /libpath:"../..//Release/Refimpl" /libpath:"../../../lib" /pdb:".\Release\aaf2xtl.pdb" /pdbtype:sept /subsystem:console /machine:ix86 

!ENDIF

# Begin Target

# Name "aaf2xtl - Win32 Debug"
# Name "aaf2xtl - Win32 Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\..\..\Utilities\aaf2xtl\aaf2xtl\aaf2xtl.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project

