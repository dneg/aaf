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
# ADD CPP /nologo /MD /W3 /WX /GR /GX /O2 /I "../ref-impl/src/Om" /I "../../" /I "./../ref-impl/src/OM" /I "../../ref-impl/include/Om" /I "../../ref-impl/include/cpp-api/" /I "../../../ref-impl/src/OM" /I "../../../ref-impl/include/cpp-api/" /I "../../../ref-impl/include/Om" /D "NDEBUG" /D "_WINDOWS" /D "UNICODE" /D "_UNICODE" /D "WIN32" /D "_WIN32" /D "BOB_TEST" /YX /FD /c
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
# ADD CPP /nologo /MDd /W3 /WX /GR /GX /Z7 /Od /I "../../../ref-impl/src/OM" /I "../../../ref-impl/include/cpp-api/" /I "../../../ref-impl/include/Om" /D "_WIN32" /D "_WINDOWS" /D "_DEBUG" /D "UNICODE" /D "_UNICODE" /D "WIN32" /D "BOB_TEST" /D "OM_ENABLE_DEBUG" /FR /YX /FD /c
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

SOURCE="..\..\..\ref-impl\src\OM\OMDataStreamProperty.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\OM\OMFile.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\OM\OMObjectDirectory.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\OM\OMProperty.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\OM\OMPropertySet.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\OM\OMPropertyTable.cpp"
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

SOURCE="..\..\..\ref-impl\src\OM\OMType.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\OM\OMUtilities.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\OM\OMWideStringProperty.cpp"
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

SOURCE="..\..\..\ref-impl\src\OM\OMContainerPropertyT.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\include\OM\OMDataStreamProperty.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\include\OM\OMDataTypes.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\include\OM\OMDictionary.h"
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

SOURCE="..\..\..\ref-impl\src\OM\OMObjectDirectory.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\include\OM\OMObjectReference.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\OM\OMObjectReferenceT.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\OM\OMPortability.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\include\OM\OMProperty.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\OM\OMPropertyBase.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\include\OM\OMPropertyDefinition.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\include\OM\OMPropertySet.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\OM\OMPropertyT.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\include\OM\OMPropertyTable.h"
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

SOURCE="..\..\..\ref-impl\src\OM\OMStoredVectorIndex.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\include\OM\OMStreamProperty.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\OM\OMStreamPropertyT.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\include\OM\OMStrongReferenceSetIter.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\OM\OMStrongReferenceSetIterT.h"
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

SOURCE="..\..\..\ref-impl\include\OM\OMWeakReferenceSetIter.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\ref-impl\src\OM\OMWeakReferenceSetIterT.h"
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

SOURCE="..\..\..\ref-impl\src\OM\OMWideStringPropertyT.h"
# End Source File
# End Group
# End Target
# End Project
