#
#
# Advanced Authoring Format
#
# Copyright (c) 1998 Avid Technology, Inc.
#
#
# Place the object root name is this list if it is a "persistent"
# aaf object:
AAFPERSISTENTOBJECTS = \
	AAFAIFCData \
	AAFCommentMarker \
	AAFDataDef \
	AAFDOSLocator \
	AAFEvent \
	AAFEventMobSlot \
	AAFGPITrigger \
	AAFJPEGData \
	AAFHTMLClip \
	AAFHTMLData \
	AAFHTMLDescriptor \
	AAFImageData \
	AAFIntraFrameMarker \
	AAFLocator \
	AAFMacLocator \
	AAFMediaFilmDescriptor \
	AAFMediaGroup \
	AAFMIDIData \
	AAFMIDIFileDescriptor \
	AAFNestedScope\
	AAFPluggableCode \
	AAFPulldown \
	AAFReferenceValue \
	AAFRGBADescriptor \
	AAFScopeReference \
	AAFStaticMobSlot \
	AAFTaggedValue \
	AAFTextClip \
	AAFTextLocator \
	AAFTIFFData \
	AAFTimecodeStream \
	AAFTimecodeStream12M \
	AAFTypeDefSet \
	AAFTypeDefRecord \
	AAFUnixLocator \
	AAFWAVEData \
	AAFWindowsLocator


# These are all of the other non-persistent objects:
AAFNONPERSISTENTOBJECTS = \
	AAFRoot \
	EnumAAFClassDefs \
	EnumAAFCodecFlavours \
	EnumAAFControlPoints \
	EnumAAFDataDefs \
	EnumAAFDefs \
	EnumAAFIdentifications \
	EnumAAFMobComments \
	EnumAAFObjects \
	EnumAAFPropertyDefs \
	EnumAAFPropertyValues \
	EnumAAFReferenceValues \
	EnumAAFTypeDefs 


# These are files which specify COM interfaces, but which are
# implemented in other objects.
AAFCOMINTERFACESONLY = \
	AAFEndian \
	AAFSearchSource

# These are the impl files that are typed by humans.  Be sure to
# remove them from the other list.  As you check each one in, just add
# its name here and remove it from either list above where it occurs.
# Do not add the 'Impl'prefix here.
HUMAN_TYPED_IMPL = \
	AAFAIFCDescriptor \
	AAFClassDef \
	AAFCodecDef \
	AAFComponent \
	AAFCompositionMob \
	AAFConstValue \
	AAFContainerDef \
	AAFContentStorage \
	AAFControlPoint \
	AAFDefObject \
	AAFDictionary \
	AAFEdgecode \
	AAFEffectDef \
	AAFEssenceAccess \
	AAFEssenceData \
	AAFEssenceDescriptor \
	AAFEssencePlugin \
	AAFFile \
	AAFFileDescriptor \
	AAFDigitalImageDescriptor \
	AAFCDCIDescriptor \
	AAFEssenceFormat \
	AAFFiller \
	AAFFindSourceInfo \
	AAFGroup \
	AAFHeader \
	AAFIdentification \
	AAFMasterMob \
	AAFMob  \
	AAFMobSlot \
	AAFNetworkLocator \
	AAFObject \
	AAFParameter \
	AAFParameterDef \
	AAFProperty \
	AAFPropertyDef \
	AAFPropertyValue \
	AAFPropValData \
	AAFPluggableDef \
	AAFPluginDescriptor \
	AAFPluginManager \
	AAFSegment \
	AAFSelector \
	AAFSequence \
	AAFSourceClip \
	AAFSourceMob \
	AAFSourceReference \
	AAFTimecode \
	AAFTimelineMobSlot \
	AAFTapeDescriptor \
	AAFTransition \
	AAFTIFFDescriptor \
	AAFTypeDef \
	AAFTypeDefInt \
	AAFTypeDefRename \
	AAFTypeDefEnum \
	AAFTypeDefFixedArray \
	AAFTypeDefStream \
	AAFTypeDefString \
	AAFTypeDefStrongObjRef \
	AAFTypeDefWeakObjRef \
	AAFTypeDefObjectRef \
	AAFTypeDefVariableArray \
	AAFVaryingValue \
	AAFWAVEDescriptor \
	EnumAAFComponents \
	EnumAAFEffectDefs \
	EnumAAFEssenceData \
	EnumAAFLoadedPlugins \
	EnumAAFLocators \
	EnumAAFMobSlots \
	EnumAAFMobs \
	EnumAAFParameterDefs \
	EnumAAFProperties \
	EnumAAFPluggableDefs \
	EnumAAFPluginDescriptors \
	EnumAAFPluginLocators \
	EnumAAFSegments 

# The list of standard dodo targets.
# AAFTypes have to be handled separately since no object is to be created.
DODO_TARGET_NAMES = \
	$(AAFPERSISTENTOBJECTS) \
	$(AAFNONPERSISTENTOBJECTS) \
	$(HUMAN_TYPED_IMPL)


# Build the list of all objects that need to be built into the COM DLL
AAFOBJECTS = \
	$(AAFPERSISTENTOBJECTS) \
	$(AAFNONPERSISTENTOBJECTS) \
	$(HUMAN_TYPED_IMPL)


# Build the list of all objects that have automatically-generated
# implementation classes.
AUTO_GEN_IMPL = \
	$(AAFPERSISTENTOBJECTS) \
	$(AAFNONPERSISTENTOBJECTS)

#---------------------------------------------
# Build the list of all objects which are built into plugin.idl and
# NOT into AAF.idl.

PLUGIN_OBJECTS = \
	AAFPlugin \
	AAFEssenceStream \
	AAFEssenceSampleStream \
	AAFEssenceDataStream \
	AAFEssenceCodec \
	AAFEssenceContainer

# objects in PLUGIN_OBJECTS are NOT built into the main interface IDL
#---------------------------------------------
