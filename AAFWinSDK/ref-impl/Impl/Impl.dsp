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
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /I "../../../ref-impl/src/Impl" /I "../../../ref-impl/include/Om" /I "../../../ref-impl/include/com-api/" /I "../../../ref-impl/src/OM" /I "../include/comidl" /I "../../../ref-impl/include/" /I "../../../ref-impl/plugins/" /D "NDEBUG" /D "COM_NO_WINDOWS_H" /D "AAF_MODULE_TEST" /D "_WINDOWS" /D "UNICODE" /D "_UNICODE" /D "WIN32" /D "_WIN32" /D "BOB_TEST" /YX /FD /c
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
# ADD CPP /nologo /MDd /W3 /GR /GX /Z7 /Od /I "../../../ref-impl/src/Impl" /I "../../../ref-impl/include/Om" /I "../../../ref-impl/include/com-api/" /I "../../../ref-impl/src/OM" /I "../include/comidl" /I "../../../ref-impl/include/" /I "../../../ref-impl/plugins/" /D "COM_NO_WINDOWS_H" /D "AAF_MODULE_TEST" /D "_WIN32" /D "_WINDOWS" /D "_DEBUG" /D "UNICODE" /D "_UNICODE" /D "WIN32" /D "BOB_TEST" /D "OM_ENABLE_DEBUG" /YX /FD /c
# SUBTRACT CPP /Fr
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

SOURCE="..\..\..\ref-impl\src\impl\aafansic.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\aafCvt.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\aafErr.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\aafTable.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\AAFUtils.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFAIFCData.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFAIFCDescriptor.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFBaseClassFactory.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFBuiltins.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFCDCIDescriptor.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFClassDef.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFClassFactory.cpp"
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

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFConstValue.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFContainerDef.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFContentStorage.cpp"
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

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFDOSLocator.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFEdgecode.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFEffectDef.cpp"
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

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFEssencePlugin.cpp"
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

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFFindSourceInfo.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFGPITrigger.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFGroup.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFHeader.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFHTMLClip.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFHTMLData.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFHTMLDescriptor.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFIdentification.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFImageData.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFIntraFrameMarker.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFJPEGData.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFLocator.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFMacLocator.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFMasterMob.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFMediaFilmDescriptor.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFMediaGroup.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFMIDIData.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFMIDIDescriptor.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFMob.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFMobSlot.cpp"
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

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFParameter.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFParameterDef.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFPluggableCode.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFPluggableDef.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFPluginDescriptor.cpp"
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

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFReferenceValue.cpp"
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

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFSession.cpp"
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

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFTIFFData.cpp"
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

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFTypeDefEnum.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFTypeDefFixedArray.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFTypeDefInt.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFTypeDefObjectRef.cpp"
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

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFUnixLocator.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFVaryingValue.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFWAVEData.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFWAVEDescriptor.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplAAFWindowsLocator.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplEnumAAFClassDefs.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplEnumAAFCodecFlavours.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplEnumAAFComponents.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplEnumAAFControlPoints.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplEnumAAFDataDefs.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplEnumAAFDefs.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplEnumAAFEffectDefs.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplEnumAAFEssenceData.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplEnumAAFIdentifications.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplEnumAAFLoadedPlugins.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplEnumAAFLocators.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplEnumAAFMobComments.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplEnumAAFMobs.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplEnumAAFMobSlots.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplEnumAAFParameterDefs.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplEnumAAFPluggableDefs.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplEnumAAFPluginDesc.cpp"
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

SOURCE="..\..\..\ref-impl\src\impl\ImplEnumAAFReferenceValues.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplEnumAAFSegments.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\impl\ImplEnumAAFTypeDefs.cpp"
# End Source File
# End Target
# End Project
