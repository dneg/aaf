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
# ADD CPP /nologo /MD /W3 /GR /GX /Zi /O2 /I "../../../ref-impl/src/com-api" /I "../../../ref-impl/src/com-api/test" /I "../../../ref-impl/plugins" /I "../../../ref-impl/include/com-api/" /I "../../../ref-impl/src/impl" /I "../../ref-impl/include/comidl" /I "../../../ref-impl/include" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "UNICODE" /D "_UNICODE" /YX /FD /c
# SUBTRACT CPP /Fr
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
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I "../../../ref-impl/src/com-api" /I "../../../ref-impl/src/com-api/test" /I "../../../ref-impl/plugins" /I "../../../ref-impl/include/com-api/" /I "../../../ref-impl/src/impl" /I "../../ref-impl/include/comidl" /I "../../../ref-impl/include" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "UNICODE" /D "_UNICODE" /YX /FD /c
# SUBTRACT CPP /Fr
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

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFAIFCDataTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFAIFCDescriptorTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFCDCIDescriptorTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFClassDefTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFCodecDefTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFCommentMarkerTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFComponentTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFCompositionMobTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFConstantValueTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFContainerDefTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFContentStorageTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFControlPointTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFDataDefTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFDefObjectTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFDictionaryTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFDigitalImageDescripTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFEdgecodeTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFEssenceAccessTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFEssenceDataTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFEssenceDescriptorTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFEssenceFormatTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFEssenceGroupTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFEssencePluginTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFEventMobSlotTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFEventTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFFileDescriptorTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFFileTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFFillerTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFFilmDescriptorTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFFindSourceInfoTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFGPITriggerTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFHeaderTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFHTMLClipTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFHTMLDataTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFHTMLDescriptorTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFIdentificationTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFImageDataTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFInterpolationDefTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFIntraFrameMarkerTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFJPEGDataTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFLocatorTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFMasterMobTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFMIDIDataTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFMIDIFileDescriptorTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFMobSlotTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFMobTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFNestedScopeTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFNetworkLocatorTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFObjectTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFOperationDefTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFOperationGroupTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFParameterDefTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFParameterTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFPluggableCodeTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFPluginDescriptorTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFPluginManagerTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFPropertyDefTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFPropertyTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFPropertyValueTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFPropValDataTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFPulldownTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFReferenceValueTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFRGBADescriptorTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFRootTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFScopeReferenceTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFSegmentTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFSelectorTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFSequenceTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFSourceClipTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFSourceMobTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFSourceReferenceTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFStaticMobSlotTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFTaggedValueTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFTapeDescriptorTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFTextClipTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFTextLocatorTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFTIFFDataTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFTIFFDescriptorTest.cpp"
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

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFTimelineMobSlotTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFTransitionTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFTypeDefEnumTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFTypeDefExtEnumTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFTypeDefFixedArrayTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFTypeDefIntTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFTypeDefObjectRefTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFTypeDefRecordTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFTypeDefRenameTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFTypeDefSetTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFTypeDefStreamTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFTypeDefStringTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFTypeDefStrongObjRefTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFTypeDefTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFTypeDefVariableArryTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFTypeDefWeakObjRefTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFVaryingValueTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFWAVEDataTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CAAFWAVEDescriptorTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CEnumAAFClassDefsTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CEnumAAFCodecDefsTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CEnumAAFCodecFlavoursTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CEnumAAFComponentsTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CEnumAAFContainerDefsTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CEnumAAFControlPointsTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CEnumAAFDataDefsTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CEnumAAFDefsTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CEnumAAFEssenceDataTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CEnumAAFIdentificationsTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CEnumAAFInterpolateDefsTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CEnumAAFLoadedPluginsTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CEnumAAFLocatorsTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CEnumAAFMobSlotsTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CEnumAAFMobsTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CEnumAAFObjectsTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CEnumAAFOperationDefsTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CEnumAAFParameterDefsTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CEnumAAFPluginDescTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CEnumAAFPluginLocatorsTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CEnumAAFPropertiesTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CEnumAAFPropertyDefsTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CEnumAAFPropertyValuesTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CEnumAAFReferenceValuesTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CEnumAAFSegmentsTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CEnumAAFTaggedValuesTest.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\test\CEnumAAFTypeDefsTest.cpp"
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

SOURCE=..\..\..\test\com\ComModTestAAF.cpp
# End Source File
# End Target
# End Project
