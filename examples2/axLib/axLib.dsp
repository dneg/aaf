# Microsoft Developer Studio Project File - Name="axLib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=axLib - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "axLib.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "axLib.mak" CFG="axLib - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "axLib - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "axLib - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "axLib - Win32 Release"

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
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /I "../../AAFWinSDK/include" /I "../axLib" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "axLib - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "axLib___Win32_Debug"
# PROP BASE Intermediate_Dir "axLib___Win32_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /w /W0 /Gm /GR /GX /ZI /Od /I "../../AAFWinSDK/include" /I "../axLib" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
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

# Name "axLib - Win32 Release"
# Name "axLib - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AxBaseObj.cpp
# End Source File
# Begin Source File

SOURCE=.\AxBaseObjIter.cpp
# End Source File
# Begin Source File

SOURCE=.\AxComponent.cpp

!IF  "$(CFG)" == "axLib - Win32 Release"

!ELSEIF  "$(CFG)" == "axLib - Win32 Debug"

# ADD CPP /w /W0

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\AxContentStorage.cpp

!IF  "$(CFG)" == "axLib - Win32 Release"

!ELSEIF  "$(CFG)" == "axLib - Win32 Debug"

# ADD CPP /w /W0

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\AxDictionary.cpp

!IF  "$(CFG)" == "axLib - Win32 Release"

!ELSEIF  "$(CFG)" == "axLib - Win32 Debug"

# ADD CPP /w /W0

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\AxEssence.cpp

!IF  "$(CFG)" == "axLib - Win32 Release"

!ELSEIF  "$(CFG)" == "axLib - Win32 Debug"

# ADD CPP /w /W0

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\AxEx.cpp

!IF  "$(CFG)" == "axLib - Win32 Release"

!ELSEIF  "$(CFG)" == "axLib - Win32 Debug"

# ADD CPP /w /W0

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\AxFile.cpp

!IF  "$(CFG)" == "axLib - Win32 Release"

!ELSEIF  "$(CFG)" == "axLib - Win32 Debug"

# ADD CPP /w /W0

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\AxHeader.cpp

!IF  "$(CFG)" == "axLib - Win32 Release"

!ELSEIF  "$(CFG)" == "axLib - Win32 Debug"

# ADD CPP /w /W0

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\AxHrMap.cpp

!IF  "$(CFG)" == "axLib - Win32 Release"

!ELSEIF  "$(CFG)" == "axLib - Win32 Debug"

# ADD CPP /w /W0

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\AxInit.cpp

!IF  "$(CFG)" == "axLib - Win32 Release"

!ELSEIF  "$(CFG)" == "axLib - Win32 Debug"

# ADD CPP /w /W0

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\AxIterator.cpp

!IF  "$(CFG)" == "axLib - Win32 Release"

!ELSEIF  "$(CFG)" == "axLib - Win32 Debug"

# ADD CPP /w /W0

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\AxKLVData.cpp

!IF  "$(CFG)" == "axLib - Win32 Release"

!ELSEIF  "$(CFG)" == "axLib - Win32 Debug"

# ADD CPP /w /W0

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\AxMetaDef.cpp

!IF  "$(CFG)" == "axLib - Win32 Release"

!ELSEIF  "$(CFG)" == "axLib - Win32 Debug"

# ADD CPP /w /W0

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\AxMob.cpp

!IF  "$(CFG)" == "axLib - Win32 Release"

!ELSEIF  "$(CFG)" == "axLib - Win32 Debug"

# ADD CPP /w /W0

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\AxMobSlot.cpp

!IF  "$(CFG)" == "axLib - Win32 Release"

!ELSEIF  "$(CFG)" == "axLib - Win32 Debug"

# ADD CPP /w /W0

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\AxObject.cpp

!IF  "$(CFG)" == "axLib - Win32 Release"

!ELSEIF  "$(CFG)" == "axLib - Win32 Debug"

# ADD CPP /w /W0

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\AxPluginMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\AxProperty.cpp

!IF  "$(CFG)" == "axLib - Win32 Release"

!ELSEIF  "$(CFG)" == "axLib - Win32 Debug"

# ADD CPP /w /W0

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\AxPropertyValue.cpp

!IF  "$(CFG)" == "axLib - Win32 Release"

!ELSEIF  "$(CFG)" == "axLib - Win32 Debug"

# ADD CPP /w /W0

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\AxPropertyValueDump.cpp

!IF  "$(CFG)" == "axLib - Win32 Release"

!ELSEIF  "$(CFG)" == "axLib - Win32 Debug"

# ADD CPP /w /W0

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\AxSmartPointer.cpp

!IF  "$(CFG)" == "axLib - Win32 Release"

!ELSEIF  "$(CFG)" == "axLib - Win32 Debug"

# ADD CPP /w /W0

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\AxTypes.cpp

!IF  "$(CFG)" == "axLib - Win32 Release"

!ELSEIF  "$(CFG)" == "axLib - Win32 Debug"

# ADD CPP /w /W0

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\AxUtil.cpp

!IF  "$(CFG)" == "axLib - Win32 Release"

!ELSEIF  "$(CFG)" == "axLib - Win32 Debug"

# ADD CPP /w /W0

!ENDIF 

# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AxBaseObj.h
# End Source File
# Begin Source File

SOURCE=.\AxBaseObjIter.h
# End Source File
# Begin Source File

SOURCE=.\AxComponent.h
# End Source File
# Begin Source File

SOURCE=.\AxContentStorage.h
# End Source File
# Begin Source File

SOURCE=.\AxDictionary.h
# End Source File
# Begin Source File

SOURCE=.\AxEssence.h
# End Source File
# Begin Source File

SOURCE=.\AxEx.h
# End Source File
# Begin Source File

SOURCE=.\AxFile.h
# End Source File
# Begin Source File

SOURCE=.\AxHeader.h
# End Source File
# Begin Source File

SOURCE=.\AxHrMap.h
# End Source File
# Begin Source File

SOURCE=.\AxInit.h
# End Source File
# Begin Source File

SOURCE=.\AxIterator.h
# End Source File
# Begin Source File

SOURCE=.\AxKLVData.h
# End Source File
# Begin Source File

SOURCE=.\AxMetaDef.h
# End Source File
# Begin Source File

SOURCE=.\AxMob.h
# End Source File
# Begin Source File

SOURCE=.\AxMobSlot.h
# End Source File
# Begin Source File

SOURCE=.\AxObject.h
# End Source File
# Begin Source File

SOURCE=.\AxPluginMgr.h
# End Source File
# Begin Source File

SOURCE=.\AxProperty.h
# End Source File
# Begin Source File

SOURCE=.\AxPropertyValue.h
# End Source File
# Begin Source File

SOURCE=.\AxPropertyValueDump.h
# End Source File
# Begin Source File

SOURCE=.\AxSmartPointer.h
# End Source File
# Begin Source File

SOURCE=.\AxTypes.h
# End Source File
# Begin Source File

SOURCE=.\AxUtil.h
# End Source File
# End Group
# End Target
# End Project
