# Microsoft Developer Studio Project File - Name="aif2xtllib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=aif2xtllib - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "aif2xtllib.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "aif2xtllib.mak" CFG="aif2xtllib - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "aif2xtllib - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "aif2xtllib - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "aif2xtllib - Win32 Release"

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
# ADD BASE CPP /nologo /MD /I "../../../../AAFWinSDK/include" /I "../../../../examples2/axLib" /I "..\..\..\..\Utilities\aaf2xtl\aifsantefelib" /I "..\..\..\..\Utilities\aaf2xtl\aiflib" /I "..\..\..\..\Utilities\aaf2xtl\aif2xtllib" /W3 /O2 /Ob1 /D "WIN32" /D "NDEBUG" /D "_LIB" /D "_MBCS" /GF /Gy /GR /YX /Fp".\Release/aif2xtllib.pch" /Fo".\Release/" /Fd".\Release/" /c /GX 
# ADD CPP /nologo /MD /I "../../../../AAFWinSDK/include" /I "../../../../examples2/axLib" /I "..\..\..\..\Utilities\aaf2xtl\aifsantefelib" /I "..\..\..\..\Utilities\aaf2xtl\aiflib" /I "..\..\..\..\Utilities\aaf2xtl\aif2xtllib" /W3 /O2 /Ob1 /D "WIN32" /D "NDEBUG" /D "_LIB" /D "_MBCS" /GF /Gy /GR /YX /Fp".\Release/aif2xtllib.pch" /Fo".\Release/" /Fd".\Release/" /c /GX 
# ADD BASE MTL /nologo /win32 
# ADD MTL /nologo /win32 
# ADD BASE RSC /l 1033 /d "NDEBUG" 
# ADD RSC /l 1033 /d "NDEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo 
# ADD BSC32 /nologo 
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:".\Release\aif2xtllib.lib" 
# ADD LIB32 /nologo /out:".\Release\aif2xtllib.lib" 

!ELSEIF  "$(CFG)" == "aif2xtllib - Win32 Debug"

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
# ADD BASE CPP /nologo /MDd /I "../../../../AAFWinSDK/include" /I "../../../../examples2/axLib" /I "..\..\..\..\Utilities\aaf2xtl\aifsantefelib" /I "..\..\..\..\Utilities\aaf2xtl\aiflib" /I "..\..\..\..\Utilities\aaf2xtl\aif2xtllib" /Zi /W3 /Od /D "WIN32" /D "_DEBUG" /D "_LIB" /D "_MBCS" /GR /YX /Fp".\Debug/aif2xtllib.pch" /Fo".\Debug/" /Fd".\Debug/" /GZ /c /GX 
# ADD CPP /nologo /MDd /I "../../../../AAFWinSDK/include" /I "../../../../examples2/axLib" /I "..\..\..\..\Utilities\aaf2xtl\aifsantefelib" /I "..\..\..\..\Utilities\aaf2xtl\aiflib" /I "..\..\..\..\Utilities\aaf2xtl\aif2xtllib" /Zi /W3 /Od /D "WIN32" /D "_DEBUG" /D "_LIB" /D "_MBCS" /GR /YX /Fp".\Debug/aif2xtllib.pch" /Fo".\Debug/" /Fd".\Debug/" /GZ /c /GX 
# ADD BASE MTL /nologo /win32 
# ADD MTL /nologo /win32 
# ADD BASE RSC /l 1033 /d "_DEBUG" 
# ADD RSC /l 1033 /d "_DEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo 
# ADD BSC32 /nologo 
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:".\Debug\aif2xtllib.lib" 
# ADD LIB32 /nologo /out:".\Debug\aif2xtllib.lib" 

!ENDIF

# Begin Target

# Name "aif2xtllib - Win32 Release"
# Name "aif2xtllib - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\..\..\Utilities\aaf2xtl\aif2xtllib\Aif2XtlEssencePreprocessVisitor.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Utilities\aaf2xtl\aif2xtllib\Aif2XtlGroupVisitor.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Utilities\aaf2xtl\aif2xtllib\Aif2XtlInit.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Utilities\aaf2xtl\aif2xtllib\Aif2XtlMobSelectVisitor.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Utilities\aaf2xtl\aif2xtllib\Aif2XtlObjectRemoveVisitor.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Utilities\aaf2xtl\aif2xtllib\Aif2XtlParseTreeNodeDecoration.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Utilities\aaf2xtl\aif2xtllib\Aif2XtlParseTreeNodeFactory.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Utilities\aaf2xtl\aif2xtllib\Aif2XtlParseTreeNodes.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Utilities\aaf2xtl\aif2xtllib\Aif2XtlParseTreeVisitor.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Utilities\aaf2xtl\aif2xtllib\Aif2XtlSeqEditVisitor.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Utilities\aaf2xtl\aif2xtllib\Aif2XtlSeqOffsetVisitor.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Utilities\aaf2xtl\aif2xtllib\Aif2XtlSourceClipVisitor.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Utilities\aaf2xtl\aif2xtllib\Aif2XtlTimecode.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Utilities\aaf2xtl\aif2xtllib\Aif2XtlTransitionVisitor.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Utilities\aaf2xtl\aif2xtllib\Aif2XtlXmlGen.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Utilities\aaf2xtl\aif2xtllib\Aif2XtlXmlGenVisitor.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\..\..\Utilities\aaf2xtl\aif2xtllib\Aif2XtlEssencePreprocessVisitor.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Utilities\aaf2xtl\aif2xtllib\Aif2XtlGroupVisitor.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Utilities\aaf2xtl\aif2xtllib\Aif2XtlInit.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Utilities\aaf2xtl\aif2xtllib\Aif2XtlMobSelectVisitor.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Utilities\aaf2xtl\aif2xtllib\Aif2XtlObjectRemoveVisitor.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Utilities\aaf2xtl\aif2xtllib\Aif2XtlParseTreeNodeDecoration.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Utilities\aaf2xtl\aif2xtllib\Aif2XtlParseTreeNodeFactory.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Utilities\aaf2xtl\aif2xtllib\Aif2XtlParseTreeNodes.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Utilities\aaf2xtl\aif2xtllib\Aif2XtlParseTreeVisitor.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Utilities\aaf2xtl\aif2xtllib\Aif2XtlSeqEditVisitor.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Utilities\aaf2xtl\aif2xtllib\Aif2XtlSeqOffsetVisitor.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Utilities\aaf2xtl\aif2xtllib\Aif2XtlSourceClipVisitor.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Utilities\aaf2xtl\aif2xtllib\Aif2XtlTimecode.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Utilities\aaf2xtl\aif2xtllib\Aif2XtlTransitionVisitor.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Utilities\aaf2xtl\aif2xtllib\Aif2XtlXmlGen.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Utilities\aaf2xtl\aif2xtllib\Aif2XtlXmlGenVisitor.h
# End Source File
# End Group
# End Target
# End Project

