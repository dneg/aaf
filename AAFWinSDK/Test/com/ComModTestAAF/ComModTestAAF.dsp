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
# ADD CPP /nologo /MD /W3 /WX /GR /GX /Zi /O2 /I "..\..\..\include" /I "../../../../ref-impl/src/impl" /I "..\..\..\..\ref-impl\src\com-api" /I "../../../../Utilities/Include" /I "..\..\..\..\test\com\commodtestaaf" /FI"stdafx.h" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "UNICODE" /D "_UNICODE" /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 user32.lib gdi32.lib winspool.lib comdlg32.lib shell32.lib oleaut32.lib odbc32.lib odbccp32.lib kernel32.lib advapi32.lib ole32.lib uuid.lib aaf.lib aafiid.lib /nologo /subsystem:console /machine:I386 /nodefaultlib:"libc.lib" /nodefaultlib:"libcmt.lib" /nodefaultlib:"libcd.lib" /nodefaultlib:"libcmtd.lib" /nodefaultlib:"msvcrtd.lib" /out:"../../../Release/Test/COMMODAAF.EXE" /libpath:"..\..\..\lib"
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
# ADD CPP /nologo /MDd /W3 /WX /Gm /GR /GX /ZI /Od /I "..\..\..\include" /I "..\..\..\..\Utilities\Include" /I "../../../../ref-impl/src/impl" /I "..\..\..\..\ref-impl\src\com-api" /I "..\..\..\..\test\com\commodtestaaf" /FI"stdafx.h" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "UNICODE" /D "_UNICODE" /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib oleaut32.lib odbc32.lib odbccp32.lib ole32.lib uuid.lib aafd.lib aafiidd.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libc.lib libcmt.lib msvcrt.lib libcd.lib libcmtd.lib" /nodefaultlib:"libc.lib" /nodefaultlib:"libcmt.lib" /nodefaultlib:"msvcrt.lib" /nodefaultlib:"libcd.lib" /nodefaultlib:"libcmtd.lib" /out:"../../../Debug/Test/COMModAAF.exe" /pdbtype:sept /libpath:"..\..\..\lib"
# SUBTRACT LINK32 /incremental:no /map

!ENDIF 

# Begin Target

# Name "ComModTestAAF - Win32 Release"
# Name "ComModTestAAF - Win32 Debug"
# Begin Group "Module Tests"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\test\com\ComModTestAAF\ModuleTests\CAAFAIFCDescriptorTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\test\com\ComModTestAAF\ModuleTests\CAAFCDCIDescriptorTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\test\com\ComModTestAAF\ModuleTests\CAAFClassDefTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\test\com\ComModTestAAF\ModuleTests\CAAFCodecDefTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\test\com\ComModTestAAF\ModuleTests\CAAFCommentMarkerTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\test\com\ComModTestAAF\ModuleTests\CAAFComponentTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\test\com\ComModTestAAF\ModuleTests\CAAFCompositionMobTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\test\com\ComModTestAAF\ModuleTests\CAAFConstantValueTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\test\com\ComModTestAAF\ModuleTests\CAAFContainerDefTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\test\com\ComModTestAAF\ModuleTests\CAAFContentStorageTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\test\com\ComModTestAAF\ModuleTests\CAAFControlPointTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\test\com\ComModTestAAF\ModuleTests\CAAFDataDefTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\test\com\ComModTestAAF\ModuleTests\CAAFDefObjectTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\test\com\ComModTestAAF\ModuleTests\CAAFDictionaryTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\test\com\ComModTestAAF\ModuleTests\CAAFDigitalImageDescripTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\test\com\ComModTestAAF\ModuleTests\CAAFEdgecodeTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\test\com\ComModTestAAF\ModuleTests\CAAFEssenceAccessTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\test\com\ComModTestAAF\ModuleTests\CAAFEssenceDataTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\test\com\ComModTestAAF\ModuleTests\CAAFEssenceDescriptorTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\test\com\ComModTestAAF\ModuleTests\CAAFEssenceFormatTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\test\com\ComModTestAAF\ModuleTests\CAAFEssenceGroupTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\test\com\ComModTestAAF\ModuleTests\CAAFEssencePluginTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\test\com\ComModTestAAF\ModuleTests\CAAFEventMobSlotTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\test\com\ComModTestAAF\ModuleTests\CAAFEventTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\test\com\ComModTestAAF\ModuleTests\CAAFFileDescriptorTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\test\com\ComModTestAAF\ModuleTests\CAAFFileTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\test\com\ComModTestAAF\ModuleTests\CAAFFillerTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\test\com\ComModTestAAF\ModuleTests\CAAFFilmDescriptorTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\test\com\ComModTestAAF\ModuleTests\CAAFFindSourceInfoTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\test\com\ComModTestAAF\ModuleTests\CAAFGetFileBitsTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\test\com\ComModTestAAF\ModuleTests\CAAFGPITriggerTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\test\com\ComModTestAAF\ModuleTests\CAAFHeaderTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\test\com\ComModTestAAF\ModuleTests\CAAFIdentificationTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\test\com\ComModTestAAF\ModuleTests\CAAFInterpolationDefTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\test\com\ComModTestAAF\ModuleTests\CAAFKLVDataTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\test\com\ComModTestAAF\ModuleTests\CAAFLocatorTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\test\com\ComModTestAAF\ModuleTests\CAAFMasterMobTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\test\com\ComModTestAAF\ModuleTests\CAAFMetaDefinitionTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\test\com\ComModTestAAF\ModuleTests\CAAFMobSlotTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\test\com\ComModTestAAF\ModuleTests\CAAFMobTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\test\com\ComModTestAAF\CAAFModuleTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\test\com\ComModTestAAF\ModuleTests\CAAFNestedScopeTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\test\com\ComModTestAAF\ModuleTests\CAAFNetworkLocatorTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\test\com\ComModTestAAF\ModuleTests\CAAFObjectTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\test\com\ComModTestAAF\ModuleTests\CAAFOperationDefTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\test\com\ComModTestAAF\ModuleTests\CAAFOperationGroupTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\test\com\ComModTestAAF\ModuleTests\CAAFParameterDefTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\test\com\ComModTestAAF\ModuleTests\CAAFParameterTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\test\com\ComModTestAAF\ModuleTests\CAAFPluginDefTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\test\com\ComModTestAAF\ModuleTests\CAAFPluginManagerTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\test\com\ComModTestAAF\ModuleTests\CAAFPropertyDefTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\test\com\ComModTestAAF\ModuleTests\CAAFPropertyTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\test\com\ComModTestAAF\ModuleTests\CAAFPropertyValueTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\test\com\ComModTestAAF\ModuleTests\CAAFPulldownTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\test\com\ComModTestAAF\ModuleTests\CAAFRandomFileTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\test\com\ComModTestAAF\ModuleTests\CAAFRandomRawStorageTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\test\com\ComModTestAAF\ModuleTests\CAAFRawStorageTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\test\com\ComModTestAAF\ModuleTests\CAAFRGBADescriptorTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\test\com\ComModTestAAF\ModuleTests\CAAFScopeReferenceTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\test\com\ComModTestAAF\ModuleTests\CAAFSegmentTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\test\com\ComModTestAAF\ModuleTests\CAAFSelectorTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\test\com\ComModTestAAF\ModuleTests\CAAFSequenceTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\test\com\ComModTestAAF\ModuleTests\CAAFSetFileBitsTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\test\com\ComModTestAAF\ModuleTests\CAAFSourceClipTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\test\com\ComModTestAAF\ModuleTests\CAAFSourceMobTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\test\com\ComModTestAAF\ModuleTests\CAAFSourceReferenceTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\test\com\ComModTestAAF\ModuleTests\CAAFStaticMobSlotTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\test\com\ComModTestAAF\ModuleTests\CAAFTaggedValueTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\test\com\ComModTestAAF\ModuleTests\CAAFTapeDescriptorTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\test\com\ComModTestAAF\ModuleTests\CAAFTextLocatorTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\test\com\ComModTestAAF\ModuleTests\CAAFTIFFDescriptorTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\test\com\ComModTestAAF\ModuleTests\CAAFTimecodeStream12MTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\test\com\ComModTestAAF\ModuleTests\CAAFTimecodeStreamTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\test\com\ComModTestAAF\ModuleTests\CAAFTimecodeTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\test\com\ComModTestAAF\ModuleTests\CAAFTimelineMobSlotTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\test\com\ComModTestAAF\ModuleTests\CAAFTransitionTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\test\com\ComModTestAAF\ModuleTests\CAAFTypeDefCharacterTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\test\com\ComModTestAAF\ModuleTests\CAAFTypeDefEnumTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\test\com\ComModTestAAF\ModuleTests\CAAFTypeDefExtEnumTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\test\com\ComModTestAAF\ModuleTests\CAAFTypeDefFixedArrayTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\test\com\ComModTestAAF\ModuleTests\CAAFTypeDefIndirectTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\test\com\ComModTestAAF\ModuleTests\CAAFTypeDefIntTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\test\com\ComModTestAAF\ModuleTests\CAAFTypeDefObjectRefTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\test\com\ComModTestAAF\ModuleTests\CAAFTypeDefOpaqueTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\test\com\ComModTestAAF\ModuleTests\CAAFTypeDefRecordTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\test\com\ComModTestAAF\ModuleTests\CAAFTypeDefRenameTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\test\com\ComModTestAAF\ModuleTests\CAAFTypeDefSetTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\test\com\ComModTestAAF\ModuleTests\CAAFTypeDefStreamTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\test\com\ComModTestAAF\ModuleTests\CAAFTypeDefStringTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\test\com\ComModTestAAF\ModuleTests\CAAFTypeDefStrongObjRefTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\test\com\ComModTestAAF\ModuleTests\CAAFTypeDefTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\test\com\ComModTestAAF\ModuleTests\CAAFTypeDefVariableArryTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\test\com\ComModTestAAF\ModuleTests\CAAFTypeDefWeakObjRefTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\test\com\ComModTestAAF\ModuleTests\CAAFVaryingValueTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\test\com\ComModTestAAF\ModuleTests\CAAFWAVEDescriptorTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\test\com\ComModTestAAF\ModuleTests\CEnumAAFClassDefsTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\test\com\ComModTestAAF\ModuleTests\CEnumAAFCodecDefsTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\test\com\ComModTestAAF\ModuleTests\CEnumAAFCodecFlavoursTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\test\com\ComModTestAAF\ModuleTests\CEnumAAFComponentsTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\test\com\ComModTestAAF\ModuleTests\CEnumAAFContainerDefsTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\test\com\ComModTestAAF\ModuleTests\CEnumAAFControlPointsTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\test\com\ComModTestAAF\ModuleTests\CEnumAAFDataDefsTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\test\com\ComModTestAAF\ModuleTests\CEnumAAFEssenceDataTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\test\com\ComModTestAAF\ModuleTests\CEnumAAFIdentificationsTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\test\com\ComModTestAAF\ModuleTests\CEnumAAFInterpolateDefsTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\test\com\ComModTestAAF\ModuleTests\CEnumAAFKLVDataTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\test\com\ComModTestAAF\ModuleTests\CEnumAAFLoadedPluginsTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\test\com\ComModTestAAF\ModuleTests\CEnumAAFLocatorsTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\test\com\ComModTestAAF\ModuleTests\CEnumAAFMobSlotsTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\test\com\ComModTestAAF\ModuleTests\CEnumAAFMobsTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\test\com\ComModTestAAF\ModuleTests\CEnumAAFOperationDefsTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\test\com\ComModTestAAF\ModuleTests\CEnumAAFParameterDefsTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\test\com\ComModTestAAF\ModuleTests\CEnumAAFParametersTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\test\com\ComModTestAAF\ModuleTests\CEnumAAFPluginDescTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\test\com\ComModTestAAF\ModuleTests\CEnumAAFPluginLocatorsTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\test\com\ComModTestAAF\ModuleTests\CEnumAAFPropertiesTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\test\com\ComModTestAAF\ModuleTests\CEnumAAFPropertyDefsTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\test\com\ComModTestAAF\ModuleTests\CEnumAAFPropertyValuesTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\test\com\ComModTestAAF\ModuleTests\CEnumAAFSegmentsTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\test\com\ComModTestAAF\ModuleTests\CEnumAAFTaggedValuesTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\test\com\ComModTestAAF\ModuleTests\CEnumAAFTypeDefsTest.cpp
# End Source File
# End Group
# Begin Group "AAF Libraries"

# PROP Default_Filter "lib"
# Begin Source File

SOURCE=..\..\..\lib\AAF.lib

!IF  "$(CFG)" == "ComModTestAAF - Win32 Release"

!ELSEIF  "$(CFG)" == "ComModTestAAF - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\lib\AAFD.lib

!IF  "$(CFG)" == "ComModTestAAF - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ComModTestAAF - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\lib\AAFIID.lib

!IF  "$(CFG)" == "ComModTestAAF - Win32 Release"

!ELSEIF  "$(CFG)" == "ComModTestAAF - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\lib\AAFIIDD.lib

!IF  "$(CFG)" == "ComModTestAAF - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ComModTestAAF - Win32 Debug"

!ENDIF 

# End Source File
# End Group
# Begin Source File

SOURCE=..\..\..\include\AAF.h
# End Source File
# Begin Source File

SOURCE="..\..\..\..\ref-impl\src\impl\aafCvt.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\ref-impl\src\impl\AAFUtils.cpp"
# End Source File
# Begin Source File

SOURCE=..\..\..\..\test\com\ComModTestAAF\ComModTestAAF.cpp
# End Source File
# Begin Source File

SOURCE=.\stdafx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Target
# End Project
