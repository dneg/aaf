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
	AAFCompositionMob \
	AAFConstValue \
	AAFControlCodeDef \
	AAFControlPoint \
	AAFDataDef \
	AAFDefObject \
	AAFEdgecode \
	AAFEffectDef \
	AAFEffectInvocation \
	AAFFiller \
	AAFFindSourceInfo \
	AAFIntegerAttribute \
	AAFLocator \
	AAFMacLocator \
	AAFMasterMob \
	AAFMediaFilmDescriptor \
	AAFMediaGroup  \
	AAFMediaTapeDescriptor \
	AAFNestedScope\
	AAFObjectAttribute \
	AAFParameter \
	AAFParameterSlot \
	AAFPulldown \
	AAFPropertyDef \
	AAFReferenceValue \
	AAFScopeReference \
	AAFSequence \
	AAFStringAttribute \
	AAFTimecode \
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
	EnumAAFComponents \
	EnumAAFControlCodeDefs \
	EnumAAFControlPoints \
	EnumAAFDataDefs \
	EnumAAFDefs \
	EnumAAFEffectDefs \
	EnumAAFIdentifications \
	EnumAAFLocators \
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
	AAFFile \
	AAFHeader \
	AAFIdentification \
	AAFObject \
	AAFComponent \
	AAFEssenceDescriptor \
	AAFFileDescriptor \
	AAFMob  \
	AAFSegment \
	AAFSourceMob \
	AAFMobSlot \
	AAFTimelineMobSlot \
	AAFSourceReference \
	AAFSourceClip \
	AAFSession \
	EnumAAFMobs \
	EnumAAFMobSlots

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

# These are the Com Test files that are typed by humans.  Be sure to
# remove them from the other list.  As you check each one in, just add
# its name here and remove it from AUTO_GEN_COMTEST (below).
# Do not add the 'Test' suffix here.
HUMAN_TYPED_COMTEST = \
	AAFMob

# Build the list of all objects that have automatically-generated
# COM Test classes.
#This can go away when we institute a requirement that a module test
# exist for every class
AUTO_GEN_COMTEST = \
	AAFAttribute \
	AAFClassDef \
	AAFDictionary \
	AAFCompositionMob \
	AAFConstValue \
	AAFControlCodeDef \
	AAFControlPoint \
	AAFDataDef \
	AAFDefObject \
	AAFEdgecode \
	AAFEffectDef \
	AAFEffectInvocation \
	AAFFiller \
	AAFFindSourceInfo \
	AAFIntegerAttribute \
	AAFLocator \
	AAFMacLocator \
	AAFMasterMob \
	AAFMediaFilmDescriptor \
	AAFMediaGroup \
	AAFMediaTapeDescriptor \
	AAFNestedScope \
	AAFObjectAttribute \
	AAFParameter \
	AAFParameterSlot \
	AAFPulldown \
	AAFPropertyDef \
	AAFReferenceValue \
	AAFScopeReference \
	AAFSequence \
	AAFStringAttribute \
	AAFTimecode \
	AAFTimecodeStream \
	AAFTimecodeStream12M \
	AAFTransition \
	AAFTypeDef \
	AAFUnixLocator \
	AAFVaryingValue \
	AAFWindowsLocator \
	AAFRoot \
	AAFMedia \
	EnumAAFAttributes \
	EnumAAFClassDefs \
	EnumAAFComponents \
	EnumAAFControlCodeDefs \
	EnumAAFControlPoints \
	EnumAAFDataDefs \
	EnumAAFDefs \
	EnumAAFEffectDefs \
	EnumAAFIdentifications \
	EnumAAFLocators \
	EnumAAFMedia \
	EnumAAFMobComments \
	EnumAAFParameterSlots \
	EnumAAFPropertyDefs \
	EnumAAFReferenceValues \
	EnumAAFSegments \
	EnumAAFTypeDefs \
	AAFFile \
	AAFHeader \
	AAFIdentification \
	AAFObject \
	AAFComponent \
	AAFEssenceDescriptor \
	AAFFileDescriptor \
	AAFSegment \
	AAFSourceMob \
	AAFMobSlot \
	AAFTimelineMobSlot \
	AAFSourceReference \
	AAFSourceClip \
	AAFSession \
	EnumAAFMobs \
	EnumAAFMobSlots
