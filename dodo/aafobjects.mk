###############################################################################
#
# The contents of this file are subject to the AAF SDK Public
# Source License Agreement (the "License"); You may not use this file
# except in compliance with the License.  The License is available in
# AAFSDKPSL.TXT, or you may obtain a copy of the License from the AAF
# Association or its successor.
# 
# Software distributed under the License is distributed on an "AS IS"
# basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
# the License for the specific language governing rights and limitations
# under the License.
# 
# The Original Code of this file is Copyright 1998-2001, Licensor of the
# AAF Association.
# 
# The Initial Developer of the Original Code of this file and the
# Licensor of the AAF Association is Avid Technology.
# All rights reserved.
#
###############################################################################
AAFPERSISTENTOBJECTS="\
	 "


# These are all of the other non-persistent objects:
AAFNONPERSISTENTOBJECTS="\
	 "


# These are files which specify COM interfaces, but which are
# implemented in other objects.
AAFCOMINTERFACESONLY="\
	AAFEndian \
	AAFSearchSource \
	AAFEssenceMultiAccess \
	AAFTypeDefVariableArrayEx "


# These are the impl files that are typed by humans.  Be sure to
# remove them from the other list.  As you check each one in, just add
# its name here and remove it from either list above where it occurs.
# Do not add the 'Impl'prefix here.
HUMAN_TYPED_IMPL="\
	AAFAIFCDescriptor \
	AAFClassDef \
	AAFCodecDef \
	AAFCommentMarker \
	AAFComponent \
	AAFCompositionMob \
	AAFConstantValue \
	AAFContainerDef \
	AAFContentStorage \
	AAFControlPoint \
	AAFDataDef \
	AAFDefObject \
	AAFPluginDef \
	AAFDictionary \
	AAFEdgecode \
	AAFOperationDef \
	AAFEssenceAccess \
	AAFEssenceData \
	AAFEssenceDescriptor \
	AAFEvent \
	AAFEventMobSlot \
	AAFFile \
	AAFFileDescriptor \
	AAFFilmDescriptor \
	AAFDigitalImageDescriptor \
	AAFCDCIDescriptor \
	AAFEssenceFormat \
	AAFEssenceGroup \
	AAFFiller \
	AAFFindSourceInfo \
	AAFOperationGroup \
	AAFGPITrigger \
	AAFHeader \
	AAFIdentification \
	AAFInterpolationDef \
	AAFKLVData \
	AAFLocator \
	AAFMasterMob \
	AAFMetaDefinition \
	AAFMob  \
	AAFMobSlot \
	AAFNestedScope\
	AAFNetworkLocator \
	AAFObject \
	AAFParameter \
	AAFParameterDef \
	AAFProperty \
	AAFPropertyDef \
	AAFPropertyValue \
	AAFPluginManager \
	AAFPulldown \
	AAFRGBADescriptor \
	AAFScopeReference \
	AAFSegment \
	AAFSelector \
	AAFSequence \
	AAFSourceClip \
	AAFSourceMob \
	AAFSourceReference \
	AAFStaticMobSlot \
	AAFTapeDescriptor \
	AAFTaggedValue \
	AAFTextLocator \
	AAFTimecode \
	AAFTimelineMobSlot \
	AAFTransition \
	AAFTIFFDescriptor \
	AAFTimecodeStream \
	AAFTimecodeStream12M \
	AAFTypeDef \
	AAFTypeDefCharacter \
	AAFTypeDefIndirect \
	AAFTypeDefInt \
	AAFTypeDefRename \
	AAFTypeDefEnum \
	AAFTypeDefExtEnum \
	AAFTypeDefFixedArray \
	AAFTypeDefRecord \
	AAFTypeDefSet \
	AAFTypeDefStream \
	AAFTypeDefString \
	AAFTypeDefStrongObjRef \
	AAFTypeDefWeakObjRef \
	AAFTypeDefObjectRef \
	AAFTypeDefOpaque \
	AAFTypeDefVariableArray \
	AAFVaryingValue \
	AAFWAVEDescriptor \
	EnumAAFClassDefs \
	EnumAAFCodecDefs \
	EnumAAFCodecFlavours \
	EnumAAFComponents \
	EnumAAFContainerDefs \
	EnumAAFControlPoints \
	EnumAAFDataDefs \
	EnumAAFIdentifications \
	EnumAAFInterpolationDefs \
	EnumAAFOperationDefs \
	EnumAAFEssenceData \
	EnumAAFKLVData \
	EnumAAFLoadedPlugins \
	EnumAAFLocators \
	EnumAAFMobSlots \
	EnumAAFMobs \
	EnumAAFParameterDefs \
	EnumAAFParameters \
	EnumAAFProperties \
	EnumAAFPropertyDefs \
	EnumAAFPropertyValues \
	EnumAAFPluginDefs \
	EnumAAFPluginLocators \
	EnumAAFSegments \
	EnumAAFTaggedValues \
	EnumAAFTypeDefs \
	AAFRawStorage \
	AAFRandomRawStorage \
	AAFGetFileBits \
	AAFSetFileBits \
	AAFRandomFile "

# Defered interfaces
#	AAFIntraFrameMarker \

# The list of standard dodo targets.
# AAFTypes have to be handled separately since no object is to be created.
DODO_TARGET_NAMES="\
	${AAFPERSISTENTOBJECTS} \
	${AAFNONPERSISTENTOBJECTS} \
	${HUMAN_TYPED_IMPL} "


# Build the list of all public objects that need to be built into the COM DLL
AAFOBJECTS="\
	${AAFPERSISTENTOBJECTS} \
	${AAFNONPERSISTENTOBJECTS} \
	${HUMAN_TYPED_IMPL} "

# Build the list of all private objects that need to be built into the COM DLL
# These objects will 
PRIVATE_AAFOBJECTS="\
	AAFHTMLClip \
	AAFHTMLDescriptor \
	AAFMetaDictionary \
	AAFPropValData \
	AAFStreamPropertyValue \
	AAFStrongRefArrayValue \
	AAFStrongRefSetValue \
	AAFStrongRefValue \
	AAFTextClip \
	AAFWeakRefArrayValue \
	AAFWeakRefSetValue \
	AAFWeakRefValue \
	EnumAAFStorablePropVals "


# Build the list of all objects that have automatically-generated
# implementation classes.
AUTO_GEN_IMPL="\
	${AAFPERSISTENTOBJECTS} \
	${AAFNONPERSISTENTOBJECTS} "

#---------------------------------------------
# Build the list of all objects which are built into plugin.idl and
# NOT into AAF.idl.

PLUGIN_OBJECTS="\
	AAFPlugin \
	AAFClassExtension \
	AAFEssenceStream \
	AAFEssenceDataStream \
	AAFEssenceCodec \
	AAFMultiEssenceCodec \
	AAFEssenceContainer \
	AAFInterpolator "


# objects in PLUGIN_OBJECTS are NOT built into the main interface IDL
#---------------------------------------------
