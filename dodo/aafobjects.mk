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
	AAFAIFCDescriptor \
	AAFClassDef \
	AAFCodecDef \
	AAFContainerDef \
	AAFDictionary \
	AAFCommentMarker \
	AAFConstValue \
	AAFControlPoint \
	AAFDataDef \
	AAFDefObject \
	AAFDOSLocator \
	AAFEffectDef \
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
	AAFMIDIDescriptor \
	AAFNestedScope\
	AAFParameter \
	AAFParameterDef \
	AAFPluggableCode \
	AAFPluggableDef \
	AAFPluginManager \
	AAFPluginDescriptor \
	AAFPulldown \
	AAFPropertyDef \
	AAFReferenceValue \
	AAFRGBADescriptor \
	AAFScopeReference \
	AAFSelector \
	AAFStaticMobSlot \
	AAFTaggedValue \
	AAFTextClip \
	AAFTextLocator \
	AAFTIFFData \
	AAFTimecodeStream \
	AAFTimecodeStream12M \
	AAFTypeDef \
	AAFTypeDefInt \
	AAFTypeDefRename \
	AAFTypeDefEnum \
	AAFTypeDefFixedArray \
	AAFTypeDefStream \
	AAFTypeDefString \
	AAFTypeDefObjectRef \
	AAFTypeDefComposite \
	AAFUnixLocator \
	AAFVaryingValue \
	AAFWAVEData \
	AAFWindowsLocator


# These are all of the other non-persistent objects:
AAFNONPERSISTENTOBJECTS = \
	AAFRoot \
	AAFProperty \
	AAFPropertyValue \
	EnumAAFClassDefs \
	EnumAAFCodecFlavours \
	EnumAAFControlPoints \
	EnumAAFDataDefs \
	EnumAAFDefs \
	EnumAAFEffectDefs \
	EnumAAFIdentifications \
	EnumAAFLoadedPlugins \
	EnumAAFMobComments \
	EnumAAFObjects \
	EnumAAFParameterDefs \
	EnumAAFPluginDescriptors \
	EnumAAFPluginLocators \
	EnumAAFPropertyDefs \
	EnumAAFPropertyValues \
	EnumAAFProperties \
	EnumAAFReferenceValues \
	EnumAAFSegments \
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
	AAFComponent \
	AAFCompositionMob \
	AAFContentStorage \
	AAFEdgecode \
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
	AAFSegment \
	AAFSequence \
	AAFSourceClip \
	AAFSourceMob \
	AAFSourceReference \
	AAFTimecode \
	AAFTimelineMobSlot \
	AAFTapeDescriptor \
	AAFTransition \
	AAFTIFFDescriptor \
	AAFTypeDefVariableArray \
	AAFWAVEDescriptor \
	EnumAAFComponents \
	EnumAAFEssenceData \
	EnumAAFLocators \
	EnumAAFMobSlots \
	EnumAAFMobs

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
	AAFRoot \
	AAFEssenceFormat \
	AAFPlugin \
	AAFEssenceStream \
	AAFEssenceSampleStream \
	AAFEssenceDataStream \
	AAFEssenceCodec \
	AAFEssenceContainer

# objects in PLUGIN_OBJECTS are NOT built into the main interface IDL
#---------------------------------------------
