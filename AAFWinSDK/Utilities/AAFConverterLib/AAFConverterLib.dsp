# Microsoft Developer Studio Project File - Name="AAFConverterLib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=AAFConverterLib - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "AAFConverterLib.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "AAFConverterLib.mak" CFG="AAFConverterLib - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "AAFConverterLib - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "AAFConverterLib - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "AAFConverterLib - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../../Release/Utilities"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MD /W3 /WX /GX /O2 /I "..\..\include" /I "..\..\..\OMF\include" /I "..\..\..\OMF\portinc" /I "..\..\..\Utilities/Include" /D "WIN32" /D "NDEBUG" /D "_LIB" /D "_UNICODE" /D "UNICODE" /D "NO_CONTAINER_EXPORTS" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "AAFConverterLib - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../../Debug/Utilities"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /WX /Gm /GX /ZI /Od /I "..\..\include" /I "..\..\..\OMF\include" /I "..\..\..\OMF\portinc" /I "..\..\..\Utilities/Include" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "_UNICODE" /D "UNICODE" /D "NO_CONTAINER_EXPORTS" /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "AAFConverterLib - Win32 Release"
# Name "AAFConverterLib - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\..\Utilities\AafOmf\Aaf2Omf.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Utilities\AafOmf\AAFDomainExtensions.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Utilities\AafOmf\AAFDomainUtils.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Utilities\AafOmf\AAFException.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Utilities\AafOmf\AafOmf.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Utilities\AafOmf\Assertion.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Utilities\AafOmf\EffectTranslate.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Utilities\AafOmf\ExceptionBase.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Utilities\AafOmf\Extensions.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Utilities\AafOmf\LoggerBase.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Utilities\AafOmf\Omf2Aaf.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Utilities\AafOmf\OMFDomainExtensionUtils.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Utilities\AafOmf\OMFDomainUtils.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Utilities\AafOmf\OMFException.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Utilities\AafOmf\StreamLogger.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\Utilities\AafOmf\AafOmf.h
# End Source File
# End Group
# End Target
# End Project
