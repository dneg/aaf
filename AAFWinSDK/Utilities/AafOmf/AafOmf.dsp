# Microsoft Developer Studio Project File - Name="AafOmf" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=AafOmf - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "AafOmf.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "AafOmf.mak" CFG="AafOmf - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "AafOmf - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "AafOmf - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "AafOmf - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../../Release/Utilities"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /WX /GX /O2 /I "..\..\include" /I "..\..\..\OMF\include" /I "..\..\..\OMF\portinc" /I "../../../Utilities/Include" /D "COM_NO_WINDOWS_H" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_UNICODE" /D "UNICODE" /D "NO_CONTAINER_EXPORTS" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 aaf.lib aafiid.lib libbento.lib libjpeg.lib omftoolkit.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib AAFConverterLib.lib /nologo /subsystem:console /incremental:yes /machine:I386 /nodefaultlib:"libc.lib" /nodefaultlib:"libcmt.lib" /nodefaultlib:"libcd.lib" /nodefaultlib:"libcmtd.lib" /nodefaultlib:"msvcrtd.lib" /nodefaultlib:"msvcirtd.lib" /libpath:"..\..\Release\Refimpl" /libpath:"..\..\..\OMF\WinLibs" /libpath:"..\..\Release\Utilities"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "AafOmf - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "AafOmf___Win32_Debug"
# PROP BASE Intermediate_Dir "AafOmf___Win32_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../../Debug/Utilities"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /WX /Gm /GX /ZI /Od /I "..\..\include" /I "..\..\..\OMF\include" /I "..\..\..\OMF\portinc" /I "../../../Utilities/Include" /D "COM_NO_WINDOWS_H" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_UNICODE" /D "UNICODE" /D "NO_CONTAINER_EXPORTS" /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 aafd.lib aafiidd.lib libbentod.lib libjpegd.lib omftoolkitd.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib AAFConverterLib.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libc.lib" /nodefaultlib:"libcmt.lib" /nodefaultlib:"msvcrt.lib" /nodefaultlib:"msvcirt.lib" /nodefaultlib:"libcd.lib" /nodefaultlib:"libcmtd.lib" /pdbtype:sept /libpath:"..\..\Debug\Refimpl" /libpath:"..\..\..\OMF\WinLibs" /libpath:"..\..\Debug\Utilities"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "AafOmf - Win32 Release"
# Name "AafOmf - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\..\Utilities\AafOmf\AafOmfMain.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\..\Utilities\AafOmf\Aaf2Omf.h
# End Source File
# Begin Source File

SOURCE=.\AAFDomainExtensions.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Utilities\AafOmf\AafOmf.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Utilities\AafOmf\EffectTranslate.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Utilities\AafOmf\Omf2Aaf.h
# End Source File
# Begin Source File

SOURCE=.\OMFDomainUtils.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Group "AAF Libraries"

# PROP Default_Filter "lib"
# Begin Source File

SOURCE=..\..\Release\Refimpl\AAF.lib

!IF  "$(CFG)" == "AafOmf - Win32 Release"

!ELSEIF  "$(CFG)" == "AafOmf - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\Debug\Refimpl\AAFD.lib

!IF  "$(CFG)" == "AafOmf - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "AafOmf - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\Release\Refimpl\AAFIID.lib

!IF  "$(CFG)" == "AafOmf - Win32 Release"

!ELSEIF  "$(CFG)" == "AafOmf - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\Debug\Refimpl\AAFIIDD.lib

!IF  "$(CFG)" == "AafOmf - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "AafOmf - Win32 Debug"

!ENDIF 

# End Source File
# End Group
# End Target
# End Project
