# Microsoft Developer Studio Project File - Name="ComModTestAAF" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=ComModTestAAF - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ComModTestAAF.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ComModTestAAF.mak" CFG="ComModTestAAF - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ComModTestAAF - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "ComModTestAAF - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ComModTestAAF - Win32 Release"

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
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /Zi /O2 /I "../../../ref-impl/src/com-api" /I "../../../ref-impl/src/com-api/test" /I "../../../ref-impl/plugins" /I "../../../ref-impl/include/com-api/" /I "../../../ref-impl/src/impl" /I "../../ref-impl/include/comidl" /I "../../../ref-impl/include" /FI"stdafx.h" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "UNICODE" /D "_UNICODE" /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib oleaut32.lib odbc32.lib odbccp32.lib ole32.lib uuid.lib /nologo /subsystem:console /machine:I386 /nodefaultlib:"libc.lib" /nodefaultlib:"libcmt.lib" /nodefaultlib:"libcd.lib" /nodefaultlib:"libcmtd.lib" /nodefaultlib:"msvcrtd.lib" /out:"../../Release/Test/COMMODAAF.EXE"
# SUBTRACT LINK32 /debug

!ELSEIF  "$(CFG)" == "ComModTestAAF - Win32 Debug"

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
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MDd /W3 /WX /Gm /GR /GX /ZI /Od /I "../../../ref-impl/src/com-api" /I "../../../ref-impl/src/com-api/test" /I "../../../ref-impl/plugins" /I "../../../ref-impl/include/com-api/" /I "../../../ref-impl/src/impl" /I "../../ref-impl/include/comidl" /I "../../../ref-impl/include" /FI"stdafx.h" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "UNICODE" /D "_UNICODE" /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib oleaut32.lib odbc32.lib odbccp32.lib ole32.lib uuid.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libc.lib libcmt.lib msvcrt.lib libcd.lib libcmtd.lib" /nodefaultlib:"libc.lib" /nodefaultlib:"libcmt.lib" /nodefaultlib:"msvcrt.lib" /nodefaultlib:"libcd.lib" /nodefaultlib:"libcmtd.lib" /out:"../../Debug/Test/COMModAAF.exe" /pdbtype:sept
# SUBTRACT LINK32 /incremental:no /map

!ENDIF 

# Begin Target

# Name "ComModTestAAF - Win32 Release"
# Name "ComModTestAAF - Win32 Debug"
# Begin Group "Module Tests"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\test\com\CAAFAIFCDescriptorTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\test\com\CAAFCDCIDescriptorTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\test\com\CAAFClassDefTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\test\com\CAAFCodecDefTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\test\com\CAAFCommentMarkerTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\test\com\CAAFComponentTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\test\com\CAAFCompositionMobTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\test\com\CAAFConstantValueTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\test\com\CAAFContainerDefTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\test\com\CAAFContentStorageTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\test\com\CAAFControlPointTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\test\com\CAAFDataDefTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\test\com\CAAFDefObjectTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\test\com\CAAFDictionaryTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\test\com\CAAFDigitalImageDescripTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\test\com\CAAFEdgecodeTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\test\com\CAAFEssenceAccessTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\test\com\CAAFEssenceDataTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\test\com\CAAFEssenceDescriptorTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\test\com\CAAFEssenceFormatTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\test\com\CAAFEssenceGroupTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\test\com\CAAFEssencePluginTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\test\com\CAAFEventMobSlotTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\test\com\CAAFEventTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\test\com\CAAFFileDescriptorTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\test\com\CAAFFileTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\test\com\CAAFFillerTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\test\com\CAAFFilmDescriptorTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\test\com\CAAFFindSourceInfoTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\test\com\CAAFGPITriggerTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\test\com\CAAFHeaderTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\test\com\CAAFHTMLClipTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\test\com\CAAFHTMLDescriptorTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\test\com\CAAFIdentificationTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\test\com\CAAFInterpolationDefTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\test\com\CAAFIntraFrameMarkerTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\test\com\CAAFLocatorTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\test\com\CAAFMasterMobTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\test\com\CAAFMIDIFileDescriptorTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\test\com\CAAFMobSlotTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\test\com\CAAFMobTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\test\com\CAAFNestedScopeTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\test\com\CAAFNetworkLocatorTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\test\com\CAAFObjectTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\test\com\CAAFOperationDefTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\test\com\CAAFOperationGroupTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\test\com\CAAFParameterDefTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\test\com\CAAFParameterTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\test\com\CAAFPluggableCodeTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\test\com\CAAFPluginDescriptorTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\test\com\CAAFPluginManagerTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\test\com\CAAFPropertyDefTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\test\com\CAAFPropertyTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\test\com\CAAFPropertyValueTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\test\com\CAAFPropValDataTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\test\com\CAAFPulldownTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\test\com\CAAFRGBADescriptorTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\test\com\CAAFScopeReferenceTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\test\com\CAAFSegmentTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\test\com\CAAFSelectorTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\test\com\CAAFSequenceTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\test\com\CAAFSourceClipTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\test\com\CAAFSourceMobTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\test\com\CAAFSourceReferenceTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\test\com\CAAFStaticMobSlotTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\test\com\CAAFTaggedValueTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\test\com\CAAFTapeDescriptorTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\test\com\CAAFTextClipTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\test\com\CAAFTextLocatorTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\test\com\CAAFTIFFDescriptorTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\test\com\CAAFTimecodeStream12MTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\test\com\CAAFTimecodeStreamTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\test\com\CAAFTimecodeTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\test\com\CAAFTimelineMobSlotTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\test\com\CAAFTransitionTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\test\com\CAAFTypeDefEnumTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\test\com\CAAFTypeDefExtEnumTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\test\com\CAAFTypeDefFixedArrayTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\test\com\CAAFTypeDefIntTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\test\com\CAAFTypeDefObjectRefTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\test\com\CAAFTypeDefRecordTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\test\com\CAAFTypeDefRenameTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\test\com\CAAFTypeDefSetTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\test\com\CAAFTypeDefStreamTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\test\com\CAAFTypeDefStringTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\test\com\CAAFTypeDefStrongObjRefTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\test\com\CAAFTypeDefTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\test\com\CAAFTypeDefVariableArryTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\test\com\CAAFTypeDefWeakObjRefTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\test\com\CAAFVaryingValueTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\test\com\CAAFWAVEDescriptorTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\test\com\CEnumAAFClassDefsTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\test\com\CEnumAAFCodecDefsTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\test\com\CEnumAAFCodecFlavoursTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\test\com\CEnumAAFComponentsTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\test\com\CEnumAAFContainerDefsTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\test\com\CEnumAAFControlPointsTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\test\com\CEnumAAFDataDefsTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\test\com\CEnumAAFDefsTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\test\com\CEnumAAFEssenceDataTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\test\com\CEnumAAFIdentificationsTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\test\com\CEnumAAFInterpolateDefsTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\test\com\CEnumAAFLoadedPluginsTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\test\com\CEnumAAFLocatorsTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\test\com\CEnumAAFMobSlotsTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\test\com\CEnumAAFMobsTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\test\com\CEnumAAFObjectsTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\test\com\CEnumAAFOperationDefsTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\test\com\CEnumAAFParameterDefsTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\test\com\CEnumAAFPluginDescTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\test\com\CEnumAAFPluginLocatorsTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\test\com\CEnumAAFPropertiesTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\test\com\CEnumAAFPropertyDefsTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\test\com\CEnumAAFPropertyValuesTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\test\com\CEnumAAFSegmentsTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\test\com\CEnumAAFTaggedValuesTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\test\com\CEnumAAFTypeDefsTest.cpp
# End Source File
# End Group
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\aafCvt.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\AAFUtils.cpp"
# End Source File
# Begin Source File

SOURCE=..\..\..\test\com\CAAFModuleTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\test\com\CAAFModuleTest.h
# End Source File
# Begin Source File

SOURCE=..\..\..\test\com\ComModTestAAF.cpp
# End Source File
# Begin Source File

SOURCE=.\stdafx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Target
# End Project
