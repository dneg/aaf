# Microsoft Developer Studio Project File - Name="Impl" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=Impl - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Impl.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Impl.mak" CFG="Impl - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Impl - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "Impl - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Impl - Win32 Release"

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
# ADD CPP /nologo /MD /W3 /WX /GR /GX /O2 /I "../../../ref-impl/src/Impl" /I "../../../ref-impl/include/Om" /I "../../../ref-impl/include/com-api/" /I "../../../ref-impl/src/OM" /I "../include/comidl" /I "../../../ref-impl/include/" /I "../../../ref-impl/plugins/" /I "../../../ref-impl/aaflib/" /FI"stdafx.h" /D "NDEBUG" /D "COM_NO_WINDOWS_H" /D "_WIN32" /D "_WINDOWS" /D "UNICODE" /D "_UNICODE" /D "WIN32" /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "Impl - Win32 Debug"

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
# ADD CPP /nologo /MDd /W3 /WX /GR /GX /ZI /Od /I "../../../ref-impl/src/Impl" /I "../../../ref-impl/include/Om" /I "../../../ref-impl/include/com-api/" /I "../../../ref-impl/src/OM" /I "../include/comidl" /I "../../../ref-impl/include/" /I "../../../ref-impl/plugins/" /I "../../../ref-impl/aaflib/" /FI"stdafx.h" /D "_DEBUG" /D "OM_ENABLE_DEBUG" /D "COM_NO_WINDOWS_H" /D "_WIN32" /D "_WINDOWS" /D "UNICODE" /D "_UNICODE" /D "WIN32" /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "Impl - Win32 Release"
# Name "Impl - Win32 Debug"
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\aafCvt.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\aafErr.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\aaflib\aaflibwin32.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\AAFObjectModel.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\AAFObjectModelProcs.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\aafTable.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\AAFUtils.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFAIFCDescriptor.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFBaseClassFactory.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFBuiltinClasses.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFBuiltinTypes.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFCDCIDescriptor.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFClassDef.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFCodecDef.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFCommentMarker.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFComponent.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFCompositionMob.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFConstantValue.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFContainerDef.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFContentStorage.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFContext.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFControlPoint.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFDataDef.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFDefObject.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFDictionary.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFDigitalImageDescrip.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFEdgecode.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFEssenceAccess.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFEssenceData.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFEssenceDescriptor.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFEssenceFormat.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFEssenceGroup.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFEvent.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFEventMobSlot.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFFile.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFFileDescriptor.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFFiller.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFFilmDescriptor.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFFindSourceInfo.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFGetFileBits.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFGPITrigger.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFHeader.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFHTMLClip.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFHTMLDescriptor.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFIdentification.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFInterpolationDef.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFKLVData.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFLocator.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFMasterMob.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFMetaDefinition.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFMetaDictionary.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFMob.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFMobSlot.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFModule.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFNestedScope.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFNetworkLocator.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFObject.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFOMRawStorage.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFOperationDef.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFOperationGroup.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFParameter.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFParameterDef.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFPluginDef.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFPluginFile.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFPluginManager.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFProperty.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFPropertyDef.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFPropertyValue.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFPropValData.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFPulldown.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFRandomFile.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFRandomRawStorage.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFRawStorage.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFRefArrayValue.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFRefContainerValue.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFRefSetValue.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFRefValue.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFRGBADescriptor.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFRoot.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFScopeReference.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFSegment.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFSelector.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFSequence.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFSetFileBits.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFSourceClip.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFSourceMob.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFSourceReference.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFStaticMobSlot.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFStorable.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFStreamPropertyValue.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFStrongRefArrayValue.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFStrongRefSetValue.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFStrongRefValue.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFTaggedValue.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFTapeDescriptor.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFTextClip.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFTextLocator.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFTIFFDescriptor.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFTimecode.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFTimecodeStream.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFTimecodeStream12M.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFTimelineMobSlot.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFTransition.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFTypeDef.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFTypeDefArray.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFTypeDefCharacter.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFTypeDefEnum.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFTypeDefExtEnum.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFTypeDefFixedArray.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFTypeDefIndirect.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFTypeDefInt.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFTypeDefObjectRef.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFTypeDefOpaque.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFTypeDefRecord.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFTypeDefRename.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFTypeDefSet.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFTypeDefStream.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFTypeDefString.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFTypeDefStrongObjRef.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFTypeDefVariableArry.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFTypeDefWeakObjRef.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFVaryingValue.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFWAVEDescriptor.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFWeakRefArrayValue.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFWeakRefSetValue.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFWeakRefValue.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplEnumAAFClassDefs.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplEnumAAFCodecDefs.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplEnumAAFCodecFlavours.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplEnumAAFComponents.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplEnumAAFContainerDefs.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplEnumAAFControlPoints.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplEnumAAFDataDefs.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplEnumAAFEssenceData.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplEnumAAFIdentifications.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplEnumAAFInterpolateDefs.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplEnumAAFKLVData.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplEnumAAFLoadedPlugins.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplEnumAAFLocators.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplEnumAAFMobs.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplEnumAAFMobSlots.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplEnumAAFOperationDefs.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplEnumAAFParameterDefs.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplEnumAAFParameters.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplEnumAAFPluginDefs.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplEnumAAFPluginLocators.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplEnumAAFProperties.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplEnumAAFPropertyDefs.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplEnumAAFPropertyValues.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplEnumAAFSegments.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplEnumAAFStorablePropVals.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplEnumAAFTaggedValues.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplEnumAAFTypeDefs.cpp"
# End Source File
# Begin Source File

SOURCE=.\stdafx.cpp
# ADD CPP /Yc"StdAfx.h"
# End Source File
# End Target
# End Project
