# Microsoft Developer Studio Project File - Name="AAFStandardCodecs" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=AAFStandardCodecs - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "AAFStandardCodecs.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "AAFStandardCodecs.mak" CFG="AAFStandardCodecs - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "AAFStandardCodecs - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "AAFStandardCodecs - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "AAFStandardCodecs"
# PROP Scc_LocalPath "..\..\.."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "AAFStandardCodecs - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../../Release/RefImpl"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 1
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /WX /GR /GX /O2 /I "../../../ref-impl/src/com-api" /I "../../../ref-impl/plugins" /I "../../../ref-impl/include/com-api/" /I "../../../ref-impl/src/impl" /I "../../../ref-impl/src/Om" /I "../../../ref-impl/include/OM" /I "../../../ref-impl/src/com-api/com-dll" /I "../../../ref-impl/include" /I "../../../ref-impl/libjpeg" /I "../../../Utilities/Include" /D "NDEBUG" /D "_WIN32" /D "_WINDOWS" /D "UNICODE" /D "_UNICODE" /D "WIN32" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 kernel32.lib advapi32.lib ole32.lib uuid.lib /nologo /subsystem:windows /dll /machine:I386 /nodefaultlib:"libc.lib" /nodefaultlib:"libcmt.lib" /nodefaultlib:"libcd.lib" /nodefaultlib:"libcmtd.lib" /nodefaultlib:"msvcrtd.lib" /out:"../../Release/RefImpl/aafext/AAFPGAPI.dll" /libpath:"..\..\Release\aafsdk\lib"

!ELSEIF  "$(CFG)" == "AAFStandardCodecs - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../../Debug/RefImpl"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 1
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MDd /W3 /WX /Gm /GR /GX /ZI /Od /I "../../../ref-impl/src/com-api" /I "../../../ref-impl/plugins" /I "../../../ref-impl/include/com-api/" /I "../../../ref-impl/src/impl" /I "../../../ref-impl/src/Om" /I "../../../ref-impl/include/OM" /I "../../../ref-impl/src/com-api/com-dll" /I "../../../ref-impl/include" /I "../../../ref-impl/libjpeg" /I "../../../Utilities/Include" /D "_DEBUG" /D "OM_DEBUG" /D "_WIN32" /D "_WINDOWS" /D "UNICODE" /D "_UNICODE" /D "WIN32" /YX /FD /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib advapi32.lib ole32.lib uuid.lib /nologo /subsystem:windows /dll /debug /machine:I386 /nodefaultlib:"libc.lib" /nodefaultlib:"libcmt.lib" /nodefaultlib:"msvcrt.lib" /nodefaultlib:"libcd.lib" /nodefaultlib:"libcmtd.lib" /out:"../../Debug/RefImpl/aafext/AAFPGAPI.dll" /pdbtype:sept
# SUBTRACT LINK32 /verbose /pdb:none

!ENDIF 

# Begin Target

# Name "AAFStandardCodecs - Win32 Release"
# Name "AAFStandardCodecs - Win32 Debug"
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\aafCvt.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\plugins\AAFPluginCOM.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\plugins\AafPlugincom.def"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\AAFUtils.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\plugins\CAAFAIFCCodec.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\plugins\CAAFCDCICodec.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\plugins\CAAFCDCIDescriptorHelper.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\com-dll\CAAFClassFactory.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\plugins\CAAFEssenceDataStream.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\plugins\CAAFEssenceFileContainer.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\plugins\CAAFEssenceFileStream.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\com-dll\CAAFInProcServer.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\plugins\CAAFJPEGCodec.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\plugins\CAAFJPEGDescriptorHelper.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\com-dll\CAAFServer.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\com-dll\CAAFUnknown.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\plugins\CAAFWAVECodec.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\plugins\jpegesdatadst.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\plugins\jpegesdatasrc.cpp"
# End Source File
# End Target
# End Project
