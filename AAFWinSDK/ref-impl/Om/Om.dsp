# Microsoft Developer Studio Project File - Name="OM" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
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
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

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
# ADD CPP /nologo /MD /W3 /WX /GR /GX /O2 /I "../ref-impl/src/Om" /I "../../" /I "./../ref-impl/src/OM" /I "../../ref-impl/include/Om" /I "../../ref-impl/include/cpp-api/" /I "../../../ref-impl/src/OM" /I "../../../ref-impl/include/cpp-api/" /I "../../../ref-impl/include/Om" /D "_WIN32" /D "WIN32" /D "_WINDOWS" /D "UNICODE" /D "_UNICODE" /D "NDEBUG" /YX /FD /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
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
# ADD CPP /nologo /MDd /W3 /WX /GR /GX /Z7 /Od /I "../../../ref-impl/src/OM" /I "../../../ref-impl/include/cpp-api/" /I "../../../ref-impl/include/Om" /D "_WIN32" /D "WIN32" /D "_WINDOWS" /D "UNICODE" /D "_UNICODE" /D "_DEBUG" /D "OM_ENABLE_DEBUG" /FR /YX /FD /c
# SUBTRACT CPP /X
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
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
# Begin Group "Source Files"

# PROP Default_Filter "cpp"
# Begin Source File

SOURCE="..\..\..\ref-impl\src\OM\OMAssertions.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\OM\OMClassDefinition.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\OM\OMContainerElement.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\OM\OMContainerProperty.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\OM\OMDataStream.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\OM\OMDataStreamProperty.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\OM\OMDefinition.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\OM\OMDictionary.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\OM\OMDiskRawStorage.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\OM\OMFile.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\OM\OMIntegerType.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\OM\OMKLVStoredObject.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\OM\OMKLVStoredStream.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\OM\OMMappedFileRawStorage.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\OM\OMMemoryRawStorage.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\OM\OMMSSStoredObject.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\OM\OMMSSStoredStream.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\OM\OMObjectDirectory.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\OM\OMObjectManager.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\OM\OMObjectReference.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\OM\OMOStream.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\OM\OMProperty.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\OM\OMPropertyDefinition.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\OM\OMPropertySet.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\OM\OMPropertySetIterator.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\OM\OMPropertyTable.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\OM\OMRawStorageLockBytes.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\OM\OMRefProperty.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\OM\OMRefSetProperty.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\OM\OMRefVectorProperty.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\OM\OMRootStorable.cpp"
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

SOURCE="..\..\..\ref-impl\src\OM\OMStoredSetIndex.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\OM\OMStoredVectorIndex.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\OM\OMStrongReference.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\OM\OMStrongReferenceSet.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\OM\OMStrongReferenceVector.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\OM\OMType.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\OM\OMTypeDefinition.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\OM\OMUniqueObjectIdentType.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\OM\OMUtilities.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\OM\OMWeakReference.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\OM\OMWeakReferenceSet.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\OM\OMWeakReferenceVector.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\OM\OMWideStringProperty.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\OM\OMXMLStoredObject.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\OM\OMXMLStoredStream.cpp"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h"
# Begin Source File

SOURCE="..\..\..\ref-impl\src\OM\OMAssertions.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\include\OM\OMCharacterStringProperty.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\OM\OMCharacterStringPropertyT.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\include\OM\OMClassDefinition.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\OM\OMClassFactory.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\include\OM\OMContainer.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\include\OM\OMContainerElement.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\OM\OMContainerElementT.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\include\OM\OMContainerIterator.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\include\OM\OMContainerProperty.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\include\OM\OMDataStream.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\include\OM\OMDataStreamProperty.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\include\OM\OMDataTypes.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\include\OM\OMDefinition.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\include\OM\OMDictionary.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\include\OM\OMDiskRawStorage.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\OM\OMFile.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\include\OM\OMFixedSizeProperty.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\OM\OMFixedSizePropertyT.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\include\OM\OMIdentitySet.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\OM\OMIdentitySetT.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\include\OM\OMIntegerType.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\include\OM\OMKLVStoredObject.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\include\OM\OMKLVStoredStream.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\include\OM\OMMappedFileRawStorage.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\include\OM\OMMemoryRawStorage.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\include\OM\OMMSSStoredObject.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\include\OM\OMMSSStoredStream.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\OM\OMMSStructuredStorage.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\include\OM\OMObject.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\OM\OMObjectDirectory.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\include\OM\OMObjectManager.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\include\OM\OMObjectReference.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\OM\OMObjectReferenceT.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\include\OM\OMObjectSet.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\include\OM\OMObjectVector.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\include\OM\OMOStream.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\OM\OMPortability.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\include\OM\OMProperty.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\include\OM\OMPropertyDefinition.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\include\OM\OMPropertySet.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\include\OM\OMPropertySetIterator.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\include\OM\OMPropertyTable.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\include\OM\OMRawStorage.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\include\OM\OMRawStorageLockBytes.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\include\OM\OMRedBlackTree.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\include\OM\OMRedBlackTreeIterator.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\OM\OMRedBlackTreeIteratorT.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\OM\OMRedBlackTreeT.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\include\OM\OMReferenceContainer.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\include\OM\OMReferenceContainerIter.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\include\OM\OMReferenceSet.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\include\OM\OMReferenceSetIter.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\OM\OMReferenceSetIterT.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\OM\OMReferenceSetT.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\include\OM\OMReferenceVector.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\include\OM\OMReferenceVectorIter.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\OM\OMReferenceVectorIterT.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\OM\OMReferenceVectorT.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\include\OM\OMRefProperty.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\include\OM\OMRefSetProperty.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\include\OM\OMRefVectorProperty.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\OM\OMRootStorable.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\include\OM\OMSet.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\include\OM\OMSetIterator.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\OM\OMSetIteratorT.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\OM\OMSetT.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\include\OM\OMSingleton.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\OM\OMSingletonT.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\OM\OMStorable.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\OM\OMStoredObject.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\OM\OMStoredPropertySetIndex.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\OM\OMStoredSetIndex.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\include\OM\OMStoredStream.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\OM\OMStoredVectorIndex.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\include\OM\OMStreamProperty.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\OM\OMStreamPropertyT.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\include\OM\OMStrongReference.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\include\OM\OMStrongReferenceSet.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\include\OM\OMStrongReferenceSetIter.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\OM\OMStrongReferenceSetIterT.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\include\OM\OMStrongReferenceVector.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\include\OM\OMStrongReferenceVectorIter.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\OM\OMStrongReferenceVectorIterT.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\include\OM\OMStrongRefProperty.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\OM\OMStrongRefPropertyT.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\include\OM\OMStrongRefSetProperty.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\OM\OMStrongRefSetPropertyT.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\include\OM\OMStrongRefVectorProperty.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\OM\OMStrongRefVectorPropertyT.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\include\OM\OMType.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\include\OM\OMTypeDefinition.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\include\OM\OMUniqueObjectIdentType.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\OM\OMUtilities.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\include\OM\OMVariableSizeProperty.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\OM\OMVariableSizePropertyT.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\include\OM\OMVector.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\include\OM\OMVectorIterator.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\OM\OMVectorIteratorT.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\OM\OMVectorT.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\include\OM\OMWeakReference.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\include\OM\OMWeakReferenceSet.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\include\OM\OMWeakReferenceSetIter.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\OM\OMWeakReferenceSetIterT.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\include\OM\OMWeakReferenceVector.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\include\OM\OMWeakReferenceVectorIter.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\OM\OMWeakReferenceVectorIterT.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\include\OM\OMWeakRefProperty.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\OM\OMWeakRefPropertyT.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\include\OM\OMWeakRefSetProperty.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\OM\OMWeakRefSetPropertyT.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\include\OM\OMWeakRefVectorProperty.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\OM\OMWeakRefVectorPropertyT.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\include\OM\OMWideStringProperty.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\include\OM\OMXMLStoredObject.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\include\OM\OMXMLStoredStream.h"
# End Source File
# End Group
# End Target
# End Project
