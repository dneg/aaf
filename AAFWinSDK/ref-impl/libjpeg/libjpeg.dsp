# Microsoft Developer Studio Project File - Name="libjpeg" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=libjpeg - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "libjpeg.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "libjpeg.mak" CFG="libjpeg - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "libjpeg - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "libjpeg - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "libjpeg - Win32 Release"

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
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MD /W3 /WX /GX /O2 /I "..\..\..\ref-impl\libjpeg\\" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "libjpeg - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /WX /Gm /GX /ZI /Od /I "..\..\..\ref-impl\libjpeg\\" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"Debug\libjpegd.lib"

!ENDIF 

# Begin Target

# Name "libjpeg - Win32 Release"
# Name "libjpeg - Win32 Debug"
# Begin Source File

SOURCE="..\..\..\ref-impl\libjpeg\jcapimin.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\libjpeg\jcapistd.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\libjpeg\jccoefct.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\libjpeg\jccolor.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\libjpeg\jcdctmgr.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\libjpeg\jchuff.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\libjpeg\jcinit.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\libjpeg\jcmainct.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\libjpeg\jcmarker.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\libjpeg\jcmaster.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\libjpeg\jcomapi.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\libjpeg\jcparam.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\libjpeg\jcphuff.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\libjpeg\jcprepct.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\libjpeg\jcsample.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\libjpeg\jctrans.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\libjpeg\jdapimin.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\libjpeg\jdapistd.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\libjpeg\jdatadst.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\libjpeg\jdatasrc.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\libjpeg\jdcoefct.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\libjpeg\jdcolor.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\libjpeg\jddctmgr.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\libjpeg\jdhuff.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\libjpeg\jdinput.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\libjpeg\jdmainct.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\libjpeg\jdmarker.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\libjpeg\jdmaster.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\libjpeg\jdmerge.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\libjpeg\jdphuff.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\libjpeg\jdpostct.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\libjpeg\jdsample.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\libjpeg\jdtrans.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\libjpeg\jerror.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\libjpeg\jfdctflt.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\libjpeg\jfdctfst.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\libjpeg\jfdctint.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\libjpeg\jidctflt.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\libjpeg\jidctfst.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\libjpeg\jidctint.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\libjpeg\jidctred.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\libjpeg\jmemansi.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\libjpeg\jmemmgr.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\libjpeg\jquant1.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\libjpeg\jquant2.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\libjpeg\jutils.c"
# End Source File
# End Target
# End Project
