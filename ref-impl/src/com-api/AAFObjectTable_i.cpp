//=---------------------------------------------------------------------=
//
// This file was GENERATED for the AAF SDK
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
// The Original Code of this file is Copyright 1998-2001, Licensor of the
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
#include "CAAFAIFCDescriptor.h"
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
#include "CAAFDefObject.h"
#include "CAAFPluginDef.h"
#include "CAAFDictionary.h"
#include "CAAFEdgecode.h"
#include "CAAFOperationDef.h"
#include "CAAFEssenceAccess.h"
#include "CAAFEssenceData.h"
#include "CAAFEssenceDescriptor.h"
#include "CAAFEvent.h"
#include "CAAFEventMobSlot.h"
#include "CAAFFile.h"
#include "CAAFFileDescriptor.h"
#include "CAAFFilmDescriptor.h"
#include "CAAFDigitalImageDescriptor.h"
#include "CAAFCDCIDescriptor.h"
#include "CAAFEssenceFormat.h"
#include "CAAFEssenceGroup.h"
#include "CAAFFiller.h"
#include "CAAFFindSourceInfo.h"
#include "CAAFOperationGroup.h"
#include "CAAFGPITrigger.h"
#include "CAAFHeader.h"
#include "CAAFIdentification.h"
#include "CAAFInterpolationDef.h"
#include "CAAFKLVData.h"
#include "CAAFLocator.h"
#include "CAAFMasterMob.h"
#include "CAAFMetaDefinition.h"
#include "CAAFMob.h"
#include "CAAFMobSlot.h"
#include "CAAFNestedScope.h"
#include "CAAFNetworkLocator.h"
#include "CAAFObject.h"
#include "CAAFParameter.h"
#include "CAAFParameterDef.h"
#include "CAAFProperty.h"
#include "CAAFPropertyDef.h"
#include "CAAFPropertyValue.h"
#include "CAAFPluginManager.h"
#include "CAAFPulldown.h"
#include "CAAFRGBADescriptor.h"
#include "CAAFScopeReference.h"
#include "CAAFSegment.h"
#include "CAAFSelector.h"
#include "CAAFSequence.h"
#include "CAAFSourceClip.h"
#include "CAAFSourceMob.h"
#include "CAAFSourceReference.h"
#include "CAAFStaticMobSlot.h"
#include "CAAFTapeDescriptor.h"
#include "CAAFTaggedValue.h"
#include "CAAFTextLocator.h"
#include "CAAFTimecode.h"
#include "CAAFTimelineMobSlot.h"
#include "CAAFTransition.h"
#include "CAAFTIFFDescriptor.h"
#include "CAAFTimecodeStream.h"
#include "CAAFTimecodeStream12M.h"
#include "CAAFTypeDef.h"
#include "CAAFTypeDefCharacter.h"
#include "CAAFTypeDefIndirect.h"
#include "CAAFTypeDefInt.h"
#include "CAAFTypeDefRename.h"
#include "CAAFTypeDefEnum.h"
#include "CAAFTypeDefExtEnum.h"
#include "CAAFTypeDefFixedArray.h"
#include "CAAFTypeDefRecord.h"
#include "CAAFTypeDefSet.h"
#include "CAAFTypeDefStream.h"
#include "CAAFTypeDefString.h"
#include "CAAFTypeDefStrongObjRef.h"
#include "CAAFTypeDefWeakObjRef.h"
#include "CAAFTypeDefObjectRef.h"
#include "CAAFTypeDefOpaque.h"
#include "CAAFTypeDefVariableArray.h"
#include "CAAFVaryingValue.h"
#include "CAAFWAVEDescriptor.h"
#include "CEnumAAFClassDefs.h"
#include "CEnumAAFCodecDefs.h"
#include "CEnumAAFCodecFlavours.h"
#include "CEnumAAFComponents.h"
#include "CEnumAAFContainerDefs.h"
#include "CEnumAAFControlPoints.h"
#include "CEnumAAFDataDefs.h"
#include "CEnumAAFIdentifications.h"
#include "CEnumAAFInterpolationDefs.h"
#include "CEnumAAFOperationDefs.h"
#include "CEnumAAFEssenceData.h"
#include "CEnumAAFKLVData.h"
#include "CEnumAAFLoadedPlugins.h"
#include "CEnumAAFLocators.h"
#include "CEnumAAFMobSlots.h"
#include "CEnumAAFMobs.h"
#include "CEnumAAFParameterDefs.h"
#include "CEnumAAFParameters.h"
#include "CEnumAAFProperties.h"
#include "CEnumAAFPropertyDefs.h"
#include "CEnumAAFPropertyValues.h"
#include "CEnumAAFPluginDefs.h"
#include "CEnumAAFPluginLocators.h"
#include "CEnumAAFSegments.h"
#include "CEnumAAFTaggedValues.h"
#include "CEnumAAFTypeDefs.h"
#include "CAAFRawStorage.h"
#include "CAAFRandomRawStorage.h"
#include "CAAFGetFileBits.h"
#include "CAAFSetFileBits.h"
#include "CAAFRandomFile.h"

//
// Include Private AAF Objects in Table:
//
#define AAF_PRIVATE_OBJECT_ENTRY(name) AAF_OBJECT_ENTRY(name)

//
// Include AAF Object Table:
//
#include "AAFObjectTable.h"
