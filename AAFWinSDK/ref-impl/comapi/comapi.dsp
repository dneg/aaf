# Microsoft Developer Studio Project File - Name="comapi" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
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
# PROP AllowPerConfigDependencies 0
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
# PROP Output_Dir "../../Release/RefImpl"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /WX /GR /GX /O2 /I "../../../ref-impl/src/com-api" /I "../../../ref-impl/plugins" /I "../../../ref-impl/include/com-api/" /I "../../../ref-impl/src/impl" /I "../../../ref-impl/src/Om" /I "../../../ref-impl/include/OM" /I "../../../ref-impl/src/com-api/com-dll" /I "../include/comidl" /I "../../../ref-impl/include/ref-api" /I "../../../ref-impl/include" /FI"stdafx.h" /D "NDEBUG" /D "_WIN32" /D "_WINDOWS" /D "UNICODE" /D "_UNICODE" /D "WIN32" /D "WIN32_LEAN_AND_MEAN" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 kernel32.lib advapi32.lib ole32.lib uuid.lib /nologo /subsystem:windows /dll /machine:I386 /nodefaultlib:"libc.lib" /nodefaultlib:"libcmt.lib" /nodefaultlib:"libcd.lib" /nodefaultlib:"libcmtd.lib" /nodefaultlib:"msvcrtd.lib" /out:"../../Release/RefImpl/AAFCOAPI.dll"

!ELSEIF  "$(CFG)" == "comapi - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../../Debug/RefImpl"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MDd /W3 /WX /GR /GX /ZI /Od /I "../../../ref-impl/src/com-api" /I "../../../ref-impl/plugins" /I "../../../ref-impl/include/com-api/" /I "../../../ref-impl/src/impl" /I "../../../ref-impl/src/Om" /I "../../../ref-impl/include/OM" /I "../../../ref-impl/src/com-api/com-dll" /I "../include/comidl" /I "../../../ref-impl/include/ref-api" /I "../../../ref-impl/include" /FI"stdafx.h" /D "_DEBUG" /D "OM_ENABLE_DEBUG" /D "_WIN32" /D "_WINDOWS" /D "UNICODE" /D "_UNICODE" /D "WIN32" /D "WIN32_LEAN_AND_MEAN" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib advapi32.lib ole32.lib uuid.lib /nologo /subsystem:windows /dll /debug /machine:I386 /nodefaultlib:"libc.lib" /nodefaultlib:"libcmt.lib" /nodefaultlib:"msvcrt.lib" /nodefaultlib:"libcd.lib" /nodefaultlib:"libcmtd.lib" /out:"../../Debug/RefImpl/AAFCOAPI.dll" /pdbtype:sept
# SUBTRACT LINK32 /incremental:no /map /nodefaultlib

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

SOURCE="..\..\..\ref-impl\src\com-api\com-dll\AAFObjectCreation.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFAIFCDescriptor.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFCDCIDescriptor.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFClassDef.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\com-dll\CAAFClassFactory.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFCodecDef.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFCommentMarker.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFComponent.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFCompositionMob.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFConstantValue.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFContainerDef.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFContentStorage.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFControlPoint.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFDataDef.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFDefObject.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFDictionary.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFDigitalImageDescriptor.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFEdgecode.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFEnumValidation.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFEssenceAccess.cpp"
# ADD CPP /I "../../../ref-impl/plugins/"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFEssenceData.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFEssenceDescriptor.cpp"
# ADD CPP /I "../../../ref-impl/plugins/"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFEssenceFormat.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFEssenceGroup.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFEvent.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFEventMobSlot.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFFile.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFFileDescriptor.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFFiller.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFFilmDescriptor.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFFindSourceInfo.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFGPITrigger.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFHeader.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFHTMLClip.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFHTMLDescriptor.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFIdentification.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\com-dll\CAAFInProcServer.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFInterpolationDef.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFIntraFrameMarker.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFKLVData.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFLocator.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFMasterMob.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFMIDIFileDescriptor.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFMob.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFMobSlot.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFModule.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFNestedScope.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFNetworkLocator.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFObject.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFOperationDef.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFOperationGroup.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFParameter.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFParameterDef.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFPluginDescriptor.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFPluginManager.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFProperty.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFPropertyDef.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFPropertyValue.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFPropValData.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFPulldown.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFRGBADescriptor.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFRoot.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFScopeReference.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFSegment.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFSelector.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFSequence.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\com-dll\CAAFServer.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFSourceClip.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFSourceMob.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFSourceReference.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFStaticMobSlot.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFTaggedValue.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFTapeDescriptor.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFTextClip.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFTextLocator.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFTIFFDescriptor.cpp"
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

SOURCE="..\..\..\ref-impl\src\com-api\CAAFTimelineMobSlot.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFTransition.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFTypeDef.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFTypeDefCharacter.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFTypeDefEnum.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFTypeDefExtEnum.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFTypeDefFixedArray.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFTypeDefIndirect.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFTypeDefInt.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFTypeDefObjectRef.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFTypeDefRecord.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFTypeDefRename.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFTypeDefSet.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFTypeDefStream.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFTypeDefString.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFTypeDefStrongObjRef.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFTypeDefVariableArray.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFTypeDefWeakObjRef.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\com-dll\CAAFUnknown.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFVaryingValue.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CAAFWAVEDescriptor.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CEnumAAFClassDefs.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CEnumAAFCodecDefs.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CEnumAAFCodecFlavours.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CEnumAAFComponents.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CEnumAAFContainerDefs.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CEnumAAFControlPoints.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CEnumAAFDataDefs.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CEnumAAFEssenceData.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CEnumAAFIdentifications.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CEnumAAFInterpolationDefs.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CEnumAAFKLVData.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CEnumAAFLoadedPlugins.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CEnumAAFLocators.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CEnumAAFMobs.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CEnumAAFMobSlots.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CEnumAAFOperationDefs.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CEnumAAFParameterDefs.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CEnumAAFParameters.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CEnumAAFPluginDescriptors.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CEnumAAFPluginLocators.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CEnumAAFProperties.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CEnumAAFPropertyDefs.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CEnumAAFPropertyValues.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CEnumAAFSegments.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CEnumAAFTaggedValues.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\com-api\CEnumAAFTypeDefs.cpp"
# End Source File
# Begin Source File

SOURCE=.\stdafx.cpp
# ADD CPP /Yc"StdAfx.h"
# End Source File
# End Target
# End Project
