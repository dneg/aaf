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
	AAFAttribute \
	AAFClassDef \
	AAFDictionary \
	AAFConstValue \
	AAFControlCodeDef \
	AAFControlPoint \
	AAFDataDef \
	AAFDefObject \
	AAFEdgecode \
	AAFEffectDef \
	AAFEffectInvocation \
	AAFFindSourceInfo \
	AAFIntegerAttribute \
	AAFLocator \
	AAFMacLocator \
	AAFMediaFilmDescriptor \
	AAFMediaGroup  \
	AAFNestedScope\
	AAFObjectAttribute \
	AAFParameter \
	AAFParameterSlot \
	AAFPulldown \
	AAFPropertyDef \
	AAFReferenceValue \
	AAFScopeReference \
	AAFStringAttribute \
	AAFTimecodeStream \
	AAFTimecodeStream12M \
	AAFTransition \
	AAFTypeDef \
	AAFUnixLocator \
	AAFVaryingValue \
	AAFWindowsLocator



# These are all of the other non-persistent objects:
AAFNONPERSISTENTOBJECTS = \
	AAFRoot \
	AAFMedia \
	EnumAAFAttributes \
	EnumAAFClassDefs \
	EnumAAFControlCodeDefs \
	EnumAAFControlPoints \
	EnumAAFDataDefs \
	EnumAAFDefs \
	EnumAAFEffectDefs \
	EnumAAFIdentifications \
	EnumAAFMedia \
	EnumAAFMobComments \
	EnumAAFParameterSlots \
	EnumAAFPropertyDefs \
	EnumAAFReferenceValues \
	EnumAAFSegments \
	EnumAAFTypeDefs 


# These are files which specify COM interfaces, but which are
# implemented in other objects.
AAFCOMINTERFACESONLY = \
	AAFEndian \
	AAFSearchSource \
	AAFMobPrivate

# These are the impl files that are typed by humans.  Be sure to
# remove them from the other list.  As you check each one in, just add
# its name here and remove it from either list above where it occurs.
# Do not add the 'Impl'prefix here.
HUMAN_TYPED_IMPL = \
	AAFComponent \
	AAFCompositionMob \
	AAFEssenceDescriptor \
	AAFFile \
	AAFFileDescriptor \
	AAFFiller \
	AAFHeader \
	AAFIdentification \
	AAFMasterMob \
	AAFMob  \
	AAFMobSlot \
	AAFNetworkLocator \
	AAFObject \
	AAFSegment \
	AAFSequence \
	AAFSession \
	AAFSourceClip \
	AAFSourceMob \
	AAFSourceReference \
	AAFTimecode \
	AAFTimelineMobSlot \
	AAFTapeDescriptor \
	EnumAAFComponents \
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
