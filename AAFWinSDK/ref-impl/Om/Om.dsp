# Microsoft Developer Studio Project File - Name="OM" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 5.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=OM - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Om.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Om.mak" CFG="OM - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "OM - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "OM - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe

!IF  "$(CFG)" == "OM - Win32 Release"

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
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GR /GX /O2 /I "../ref-impl/src/Om" /I "../../" /I "./../ref-impl/src/OM" /I "../../ref-impl/include/Om" /I "../../ref-impl/include/cpp-api/" /I "../../../ref-impl/src/OM" /I "../../../ref-impl/include/cpp-api/" /I "../../../ref-impl/include/Om" /D "NDEBUG" /D "_WINDOWS" /D "UNICODE" /D "_UNICODE" /D "WIN32" /D "_WIN32" /D "BOB_TEST" /YX /FD /c
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "OM - Win32 Debug"

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
# ADD BASE CPP /nologo /W3 /GX /Z7 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MTd /W3 /GR /GX /Z7 /Od /I "../../../ref-impl/src/OM" /I "../../../ref-impl/include/cpp-api/" /I "../../../ref-impl/include/Om" /D "_WIN32" /D "_WINDOWS" /D "_DEBUG" /D "UNICODE" /D "_UNICODE" /D "WIN32" /D "BOB_TEST" /D "OM_ENABLE_DEBUG" /FR /YX /FD /c
# SUBTRACT CPP /X
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo /o"Debug/Impl.bsc"
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "OM - Win32 Release"
# Name "OM - Win32 Debug"
# Begin Source File

SOURCE="..\..\..\ref-impl\src\OM\OMAssertions.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\OM\OMClassFactory.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\OM\OMFile.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\OM\OMObjectDirectory.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\OM\OMProperty.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\OM\OMPropertySet.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\OM\OMStorable.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\OM\OMStoredObject.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\OM\OMStoredPropertySetIndex.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\OM\OMStoredVectorIndex.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\OM\OMUtilities.cpp"
# End Source File
# End Target
# End Project
