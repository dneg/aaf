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

// This file contains invocations of the macros described below.
//
// To use this file -
//   1) #define the macros to suit your usage
//   2) #include this file
//   3) #undef the macros

// Default empty definitions so that you only have to define
// those macros you actually want to use.
//
#ifndef AAF_BEGIN_OBJECT_MAP
#define AAF_BEGIN_OBJECT_MAP(x)
#endif

#ifndef AAF_END_OBJECT_MAP
#define AAF_END_OBJECT_MAP()
#endif

#ifndef AAF_OBJECT_ENTRY
#define AAF_OBJECT_ENTRY(name)
#endif

#ifndef AAF_PRIVATE_OBJECT_ENTRY
#define AAF_PRIVATE_OBJECT_ENTRY(name)
#endif


//
// Include all objects in the following table:
//
AAF_BEGIN_OBJECT_MAP(AAFObjectMap)
	AAF_PRIVATE_OBJECT_ENTRY(AAFHTMLClip)
	AAF_PRIVATE_OBJECT_ENTRY(AAFHTMLDescriptor)
	AAF_PRIVATE_OBJECT_ENTRY(AAFMetaDictionary)
	AAF_PRIVATE_OBJECT_ENTRY(AAFPropValData)
	AAF_PRIVATE_OBJECT_ENTRY(AAFStreamPropertyValue)
	AAF_PRIVATE_OBJECT_ENTRY(AAFStrongRefArrayValue)
	AAF_PRIVATE_OBJECT_ENTRY(AAFStrongRefSetValue)
	AAF_PRIVATE_OBJECT_ENTRY(AAFStrongRefValue)
	AAF_PRIVATE_OBJECT_ENTRY(AAFTextClip)
	AAF_PRIVATE_OBJECT_ENTRY(AAFWeakRefArrayValue)
	AAF_PRIVATE_OBJECT_ENTRY(AAFWeakRefSetValue)
	AAF_PRIVATE_OBJECT_ENTRY(AAFWeakRefValue)
	AAF_PRIVATE_OBJECT_ENTRY(EnumAAFStorablePropVals)
	AAF_OBJECT_ENTRY(AAFAIFCDescriptor)
	AAF_OBJECT_ENTRY(AAFClassDef)
	AAF_OBJECT_ENTRY(AAFCodecDef)
	AAF_OBJECT_ENTRY(AAFCommentMarker)
	AAF_OBJECT_ENTRY(AAFComponent)
	AAF_OBJECT_ENTRY(AAFCompositionMob)
	AAF_OBJECT_ENTRY(AAFConstantValue)
	AAF_OBJECT_ENTRY(AAFContainerDef)
	AAF_OBJECT_ENTRY(AAFContentStorage)
	AAF_OBJECT_ENTRY(AAFControlPoint)
	AAF_OBJECT_ENTRY(AAFDataDef)
	AAF_OBJECT_ENTRY(AAFDefObject)
	AAF_OBJECT_ENTRY(AAFPluginDef)
	AAF_OBJECT_ENTRY(AAFDictionary)
	AAF_OBJECT_ENTRY(AAFEdgecode)
	AAF_OBJECT_ENTRY(AAFOperationDef)
	AAF_OBJECT_ENTRY(AAFEssenceAccess)
	AAF_OBJECT_ENTRY(AAFEssenceData)
	AAF_OBJECT_ENTRY(AAFEssenceDescriptor)
	AAF_OBJECT_ENTRY(AAFEvent)
	AAF_OBJECT_ENTRY(AAFEventMobSlot)
	AAF_OBJECT_ENTRY(AAFFile)
	AAF_OBJECT_ENTRY(AAFFileDescriptor)
	AAF_OBJECT_ENTRY(AAFFilmDescriptor)
	AAF_OBJECT_ENTRY(AAFDigitalImageDescriptor)
	AAF_OBJECT_ENTRY(AAFCDCIDescriptor)
	AAF_OBJECT_ENTRY(AAFEssenceFormat)
	AAF_OBJECT_ENTRY(AAFEssenceGroup)
	AAF_OBJECT_ENTRY(AAFFiller)
	AAF_OBJECT_ENTRY(AAFFindSourceInfo)
	AAF_OBJECT_ENTRY(AAFOperationGroup)
	AAF_OBJECT_ENTRY(AAFGPITrigger)
	AAF_OBJECT_ENTRY(AAFHeader)
	AAF_OBJECT_ENTRY(AAFIdentification)
	AAF_OBJECT_ENTRY(AAFInterpolationDef)
	AAF_OBJECT_ENTRY(AAFKLVData)
	AAF_OBJECT_ENTRY(AAFLocator)
	AAF_OBJECT_ENTRY(AAFMasterMob)
	AAF_OBJECT_ENTRY(AAFMetaDefinition)
	AAF_OBJECT_ENTRY(AAFMob)
	AAF_OBJECT_ENTRY(AAFMobSlot)
	AAF_OBJECT_ENTRY(AAFNestedScope)
	AAF_OBJECT_ENTRY(AAFNetworkLocator)
	AAF_OBJECT_ENTRY(AAFObject)
	AAF_OBJECT_ENTRY(AAFParameter)
	AAF_OBJECT_ENTRY(AAFParameterDef)
	AAF_OBJECT_ENTRY(AAFProperty)
	AAF_OBJECT_ENTRY(AAFPropertyDef)
	AAF_OBJECT_ENTRY(AAFPropertyValue)
	AAF_OBJECT_ENTRY(AAFPluginManager)
	AAF_OBJECT_ENTRY(AAFPulldown)
	AAF_OBJECT_ENTRY(AAFRGBADescriptor)
	AAF_OBJECT_ENTRY(AAFScopeReference)
	AAF_OBJECT_ENTRY(AAFSegment)
	AAF_OBJECT_ENTRY(AAFSelector)
	AAF_OBJECT_ENTRY(AAFSequence)
	AAF_OBJECT_ENTRY(AAFSourceClip)
	AAF_OBJECT_ENTRY(AAFSourceMob)
	AAF_OBJECT_ENTRY(AAFSourceReference)
	AAF_OBJECT_ENTRY(AAFStaticMobSlot)
	AAF_OBJECT_ENTRY(AAFTapeDescriptor)
	AAF_OBJECT_ENTRY(AAFTaggedValue)
	AAF_OBJECT_ENTRY(AAFTextLocator)
	AAF_OBJECT_ENTRY(AAFTimecode)
	AAF_OBJECT_ENTRY(AAFTimelineMobSlot)
	AAF_OBJECT_ENTRY(AAFTransition)
	AAF_OBJECT_ENTRY(AAFTIFFDescriptor)
	AAF_OBJECT_ENTRY(AAFTimecodeStream)
	AAF_OBJECT_ENTRY(AAFTimecodeStream12M)
	AAF_OBJECT_ENTRY(AAFTypeDef)
	AAF_OBJECT_ENTRY(AAFTypeDefCharacter)
	AAF_OBJECT_ENTRY(AAFTypeDefIndirect)
	AAF_OBJECT_ENTRY(AAFTypeDefInt)
	AAF_OBJECT_ENTRY(AAFTypeDefRename)
	AAF_OBJECT_ENTRY(AAFTypeDefEnum)
	AAF_OBJECT_ENTRY(AAFTypeDefExtEnum)
	AAF_OBJECT_ENTRY(AAFTypeDefFixedArray)
	AAF_OBJECT_ENTRY(AAFTypeDefRecord)
	AAF_OBJECT_ENTRY(AAFTypeDefSet)
	AAF_OBJECT_ENTRY(AAFTypeDefStream)
	AAF_OBJECT_ENTRY(AAFTypeDefString)
	AAF_OBJECT_ENTRY(AAFTypeDefStrongObjRef)
	AAF_OBJECT_ENTRY(AAFTypeDefWeakObjRef)
	AAF_OBJECT_ENTRY(AAFTypeDefObjectRef)
	AAF_OBJECT_ENTRY(AAFTypeDefOpaque)
	AAF_OBJECT_ENTRY(AAFTypeDefVariableArray)
	AAF_OBJECT_ENTRY(AAFVaryingValue)
	AAF_OBJECT_ENTRY(AAFWAVEDescriptor)
	AAF_OBJECT_ENTRY(EnumAAFClassDefs)
	AAF_OBJECT_ENTRY(EnumAAFCodecDefs)
	AAF_OBJECT_ENTRY(EnumAAFCodecFlavours)
	AAF_OBJECT_ENTRY(EnumAAFComponents)
	AAF_OBJECT_ENTRY(EnumAAFContainerDefs)
	AAF_OBJECT_ENTRY(EnumAAFControlPoints)
	AAF_OBJECT_ENTRY(EnumAAFDataDefs)
	AAF_OBJECT_ENTRY(EnumAAFIdentifications)
	AAF_OBJECT_ENTRY(EnumAAFInterpolationDefs)
	AAF_OBJECT_ENTRY(EnumAAFOperationDefs)
	AAF_OBJECT_ENTRY(EnumAAFEssenceData)
	AAF_OBJECT_ENTRY(EnumAAFKLVData)
	AAF_OBJECT_ENTRY(EnumAAFLoadedPlugins)
	AAF_OBJECT_ENTRY(EnumAAFLocators)
	AAF_OBJECT_ENTRY(EnumAAFMobSlots)
	AAF_OBJECT_ENTRY(EnumAAFMobs)
	AAF_OBJECT_ENTRY(EnumAAFParameterDefs)
	AAF_OBJECT_ENTRY(EnumAAFParameters)
	AAF_OBJECT_ENTRY(EnumAAFProperties)
	AAF_OBJECT_ENTRY(EnumAAFPropertyDefs)
	AAF_OBJECT_ENTRY(EnumAAFPropertyValues)
	AAF_OBJECT_ENTRY(EnumAAFPluginDefs)
	AAF_OBJECT_ENTRY(EnumAAFPluginLocators)
	AAF_OBJECT_ENTRY(EnumAAFSegments)
	AAF_OBJECT_ENTRY(EnumAAFTaggedValues)
	AAF_OBJECT_ENTRY(EnumAAFTypeDefs)
	AAF_OBJECT_ENTRY(AAFRawStorage)
	AAF_OBJECT_ENTRY(AAFRandomRawStorage)
	AAF_OBJECT_ENTRY(AAFGetFileBits)
	AAF_OBJECT_ENTRY(AAFSetFileBits)
	AAF_OBJECT_ENTRY(AAFRandomFile)
AAF_END_OBJECT_MAP()
