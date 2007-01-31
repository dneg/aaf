//=---------------------------------------------------------------------=
//
// This file was GENERATED for the AAF SDK
//
// $Id$ $Name$
//
// The contents of this file are subject to the AAF SDK Public
// Source License Agreement (the "License"); You may not use this file
// except in compliance with the License.  The License is available in
// AAFSDKPSL.TXT, or you may obtain a copy of the License from the AAF
// Association or its successor.
//
// Software distributed under the License is distributed on an "AS IS"
// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
// the License for the specific language governing rights and limitations
// under the License.
//
// The Original Code of this file is Copyright 1998-2005, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=


//
// Include the CLSID declarations...
//
#include "AAFCLSIDs.h"

//
// Include the appropriate header files:
//
#include "CAAFHTMLClip.h"
#include "CAAFHTMLDescriptor.h"
#include "CAAFMetaDictionary.h"
#include "CAAFPropValData.h"
#include "CAAFStreamPropertyValue.h"
#include "CAAFStrongRefArrayValue.h"
#include "CAAFStrongRefSetValue.h"
#include "CAAFStrongRefValue.h"
#include "CAAFTextClip.h"
#include "CAAFWeakRefArrayValue.h"
#include "CAAFWeakRefSetValue.h"
#include "CAAFWeakRefValue.h"
#include "CEnumAAFStorablePropVals.h"
#include "CAAFAES3PCMDescriptor.h"
#include "CAAFAIFCDescriptor.h"
#include "CAAFAuxiliaryDescriptor.h"
#include "CAAFBWFImportDescriptor.h"
#include "CAAFCDCIDescriptor.h"
#include "CAAFCachePageAllocator.h"
#include "CAAFClassDef.h"
#include "CAAFCodecDef.h"
#include "CAAFCommentMarker.h"
#include "CAAFComponent.h"
#include "CAAFCompositionMob.h"
#include "CAAFConstantValue.h"
#include "CAAFContainerDef.h"
#include "CAAFContentStorage.h"
#include "CAAFControlPoint.h"
#include "CAAFDataDef.h"
#include "CAAFDataEssenceDescriptor.h"
#include "CAAFDefObject.h"
#include "CAAFDescriptiveClip.h"
#include "CAAFDescriptiveFramework.h"
#include "CAAFDescriptiveMarker.h"
#include "CAAFDescriptiveObject.h"
#include "CAAFDictionary.h"
#include "CAAFDigitalImageDescriptor.h"
#include "CAAFEdgecode.h"
#include "CAAFEssenceAccess.h"
#include "CAAFEssenceData.h"
#include "CAAFEssenceDescriptor.h"
#include "CAAFEssenceFormat.h"
#include "CAAFEssenceGroup.h"
#include "CAAFEvent.h"
#include "CAAFEventMobSlot.h"
#include "CAAFFile.h"
#include "CAAFFileDescriptor.h"
#include "CAAFFileEncoding.h"
#include "CAAFFiller.h"
#include "CAAFFilmDescriptor.h"
#include "CAAFFindSourceInfo.h"
#include "CAAFGPITrigger.h"
#include "CAAFGetFileBits.h"
#include "CAAFHeader.h"
#include "CAAFIdentification.h"
#include "CAAFImportDescriptor.h"
#include "CAAFInterpolationDef.h"
#include "CAAFKLVData.h"
#include "CAAFKLVDataDefinition.h"
#include "CAAFLocator.h"
#include "CAAFMasterMob.h"
#include "CAAFMetaDefinition.h"
#include "CAAFMob.h"
#include "CAAFMobSlot.h"
#include "CAAFMPEGVideoDescriptor.h"
#include "CAAFMultipleDescriptor.h"
#include "CAAFNestedScope.h"
#include "CAAFNetworkLocator.h"
#include "CAAFObject.h"
#include "CAAFOperationDef.h"
#include "CAAFOperationGroup.h"
#include "CAAFPCMDescriptor.h"
#include "CAAFParameter.h"
#include "CAAFParameterDef.h"
#include "CAAFPhysicalDescriptor.h"
#include "CAAFPlainEssenceData.h"
#include "CAAFPlainStreamData.h"
#include "CAAFPluginDef.h"
#include "CAAFPluginManager.h"
#include "CAAFProperty.h"
#include "CAAFPropertyDef.h"
#include "CAAFPropertyValue.h"
#include "CAAFPulldown.h"
#include "CAAFRGBADescriptor.h"
#include "CAAFRandomFile.h"
#include "CAAFRandomRawStorage.h"
#include "CAAFRawStorage.h"
#include "CAAFRIFFChunk.h"
#include "CAAFRecordingDescriptor.h"
#include "CAAFScopeReference.h"
#include "CAAFSegment.h"
#include "CAAFSelector.h"
#include "CAAFSequence.h"
#include "CAAFSetFileBits.h"
#include "CAAFSoundDescriptor.h"
#include "CAAFSourceClip.h"
#include "CAAFSourceMob.h"
#include "CAAFSourceReference.h"
#include "CAAFStaticMobSlot.h"
#include "CAAFSubDescriptor.h"
#include "CAAFTIFFDescriptor.h"
#include "CAAFTaggedValue.h"
#include "CAAFTaggedValueDefinition.h"
#include "CAAFTapeDescriptor.h"
#include "CAAFTextLocator.h"
#include "CAAFTimecode.h"
#include "CAAFTimecodeStream.h"
#include "CAAFTimecodeStream12M.h"
#include "CAAFTimelineMobSlot.h"
#include "CAAFTransition.h"
#include "CAAFTypeDef.h"
#include "CAAFTypeDefCharacter.h"
#include "CAAFTypeDefEnum.h"
#include "CAAFTypeDefExtEnum.h"
#include "CAAFTypeDefFixedArray.h"
#include "CAAFTypeDefIndirect.h"
#include "CAAFTypeDefInt.h"
#include "CAAFTypeDefObjectRef.h"
#include "CAAFTypeDefOpaque.h"
#include "CAAFTypeDefRecord.h"
#include "CAAFTypeDefRename.h"
#include "CAAFTypeDefSet.h"
#include "CAAFTypeDefStream.h"
#include "CAAFTypeDefString.h"
#include "CAAFTypeDefStrongObjRef.h"
#include "CAAFTypeDefVariableArray.h"
#include "CAAFTypeDefWeakObjRef.h"
#include "CAAFVaryingValue.h"
#include "CAAFWAVEDescriptor.h"
#include "CEnumAAFClassDefs.h"
#include "CEnumAAFCodecDefs.h"
#include "CEnumAAFCodecFlavours.h"
#include "CEnumAAFComponents.h"
#include "CEnumAAFContainerDefs.h"
#include "CEnumAAFControlPoints.h"
#include "CEnumAAFDataDefs.h"
#include "CEnumAAFEssenceData.h"
#include "CEnumAAFFileDescriptors.h"
#include "CEnumAAFFileEncodings.h"
#include "CEnumAAFIdentifications.h"
#include "CEnumAAFInterpolationDefs.h"
#include "CEnumAAFKLVData.h"
#include "CEnumAAFKLVDataDefs.h"
#include "CEnumAAFLoadedPlugins.h"
#include "CEnumAAFLocators.h"
#include "CEnumAAFMobSlots.h"
#include "CEnumAAFMobs.h"
#include "CEnumAAFOperationDefs.h"
#include "CEnumAAFParameterDefs.h"
#include "CEnumAAFParameters.h"
#include "CEnumAAFPluginDefs.h"
#include "CEnumAAFPluginLocators.h"
#include "CEnumAAFProperties.h"
#include "CEnumAAFPropertyDefs.h"
#include "CEnumAAFPropertyValues.h"
#include "CEnumAAFRIFFChunks.h"
#include "CEnumAAFSegments.h"
#include "CEnumAAFSubDescriptors.h"
#include "CEnumAAFTaggedValueDefs.h"
#include "CEnumAAFTaggedValues.h"
#include "CEnumAAFTypeDefs.h"

//
// Include Private AAF Objects in Table:
//
#define AAF_PRIVATE_OBJECT_ENTRY(name) AAF_OBJECT_ENTRY(name)

//
// Include AAF Object Table:
//
#include "AAFObjectTable.h"
