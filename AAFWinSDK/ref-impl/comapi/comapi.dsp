# Microsoft Developer Studio Project File - Name="comapi" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 5.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=comapi - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "comapi.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "comapi.mak" CFG="comapi - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "comapi - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "comapi - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "comapi - Win32 Release"

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
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GR /GX /O2 /I "../../../ref-impl/src/com-api" /I "../../../ref-impl/include/com-api/" /I "../../../ref-impl/src/impl" /I "../../../ref-impl/src/Om" /I "../../../ref-impl/include/OM" /I "../../../ref-impl/src/com-api/com-dll" /I "../include/comidl" /D "NDEBUG" /D "_WIN32" /D "_WINDOWS" /D "UNICODE" /D "_UNICODE" /D "WIN32" /D "BOB_TEST" /D "AAF_MODULE_TEST" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o NUL /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o NUL /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386 /out:"Release/AAFCOAPI.dll"
# Begin Special Build Tool
SOURCE=$(InputPath)
PostBuild_Cmds=regsvr32 /s Release/AAFCOAPI.dll
# End Special Build Tool

!ELSEIF  "$(CFG)" == "comapi - Win32 Debug"

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
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MTd /W3 /Gm /GR /GX /Zi /Od /I "../../../ref-impl/src/com-api" /I "../../../ref-impl/include/com-api/" /I "../../../ref-impl/src/impl" /I "../../../ref-impl/src/Om" /I "../../../ref-impl/include/OM" /I "../../../ref-impl/src/com-api/com-dll" /I "../include/comidl" /D "_DEBUG" /D "OM_ENABLE_DEBUG" /D "_WIN32" /D "_WINDOWS" /D "UNICODE" /D "_UNICODE" /D "WIN32" /D "BOB_TEST" /D "AAF_MODULE_TEST" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o NUL /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o NUL /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /incremental:no /debug /machine:I386 /nodefaultlib:"libcd.lib libcid.lib" /out:"Debug/AAFCOAPI.dll" /pdbtype:sept
# SUBTRACT LINK32 /nodefaultlib
# Begin Special Build Tool
SOURCE=$(InputPath)
PostBuild_Cmds=regsvr32 /s Debug/AAFCOAPI.dll
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "comapi - Win32 Release"
# Name "comapi - Win32 Debug"
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\com-dll\AAFCOM.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\com-dll\AAFCOM.DEF"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\com-dll\AAFCOMPlatform.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\com-dll\AAFCOMPlatformTypes.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\com-dll\AAFObjectCreation.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFAttribute.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFAttributeTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFClassDef.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFClassDefTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\com-dll\CAAFClassFactory.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\com-dll\CAAFClassFactory.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFComponent.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFComponentTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFCompositionMob.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFCompositionMobTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFConstValue.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFConstValueTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFControlCodeDef.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFControlCodeDefTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFControlPoint.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFControlPointTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFDataDef.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFDataDefTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFDefObject.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFDefObjectTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFDictionary.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFDictionaryTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFEdgecode.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFEdgecodeTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFEffectDef.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFEffectDefTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFEffectInvocation.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFEffectInvocationTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFFile.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFFileTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFFiller.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFFillerTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFFindSourceInfo.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFFindSourceInfoTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFHeader.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFHeaderTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFIdentification.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFIdentificationTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\com-dll\CAAFInProcServer.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\com-dll\CAAFInProcServer.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFIntegerAttribute.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFIntegerAttributeTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFLocator.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFLocatorTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFMacLocator.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFMacLocatorTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFMasterMob.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFMasterMobTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFMedia.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFMediaDescriptor.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFMediaDescriptorTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFMediaFileDescriptor.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFMediaFileDescriptorTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFMediaFilmDescriptor.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFMediaFilmDescriptorTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFMediaGroup.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFMediaGroupTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFMediaTapeDescriptor.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFMediaTapeDescriptorTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFMediaTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFMob.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFMobSlot.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFMobSlotTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFMobTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\com-dll\CAAFModuleTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\com-dll\CAAFModuleTest.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFNestedScope.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFNestedScopeTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFObject.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFObjectAttribute.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFObjectAttributeTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFObjectTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFParameter.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFParameterSlot.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFParameterSlotTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFParameterTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFPropertyDef.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFPropertyDefTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFPulldown.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFPulldownTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFReferenceValue.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFReferenceValueTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFRoot.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFRootTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFScopeReference.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFScopeReferenceTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFSegment.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFSegmentTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFSequence.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFSequenceTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\com-dll\CAAFServer.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\com-dll\CAAFServer.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFSession.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFSessionTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFSourceClip.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFSourceClipTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFSourceMob.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFSourceMobTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFSourceReference.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFSourceReferenceTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFStringAttribute.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFStringAttributeTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFTimecode.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFTimecodeStream.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFTimecodeStream12M.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFTimecodeStream12MTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFTimecodeStreamTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFTimecodeTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFTimelineMobSlot.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFTimelineMobSlotTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFTransition.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFTransitionTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFTypeDef.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFTypeDefTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFUnixLocator.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFUnixLocatorTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\com-dll\CAAFUnknown.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\com-dll\CAAFUnknown.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFVaryingValue.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFVaryingValueTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFWindowsLocator.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFWindowsLocatorTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CEnumAAFAttributes.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CEnumAAFAttributesTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CEnumAAFClassDefs.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CEnumAAFClassDefsTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CEnumAAFComponents.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CEnumAAFComponentsTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CEnumAAFControlCodeDefs.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CEnumAAFControlCodeDefsTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CEnumAAFControlPoints.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CEnumAAFControlPointsTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CEnumAAFDataDefs.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CEnumAAFDataDefsTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CEnumAAFDefs.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CEnumAAFDefsTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CEnumAAFEffectDefs.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CEnumAAFEffectDefsTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CEnumAAFIdentifications.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CEnumAAFIdentificationsTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CEnumAAFLocators.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CEnumAAFLocatorsTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CEnumAAFMedia.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CEnumAAFMediaTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CEnumAAFMobComments.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CEnumAAFMobCommentsTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CEnumAAFMobs.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CEnumAAFMobSlots.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CEnumAAFMobSlotsTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CEnumAAFMobsTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CEnumAAFParameterSlots.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CEnumAAFParameterSlotsTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CEnumAAFPropertyDefs.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CEnumAAFPropertyDefsTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CEnumAAFReferenceValues.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CEnumAAFReferenceValuesTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CEnumAAFSegments.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CEnumAAFSegmentsTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CEnumAAFTypeDefs.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CEnumAAFTypeDefsTest.cpp"
# End Source File
# End Target
# End Project
