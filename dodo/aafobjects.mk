\#
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
	AAFComponent \
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
	AAFHeader \
	AAFIdentification \
	AAFIntegerAttribute \
	AAFLocator \
	AAFMacLocator \
	AAFMasterMob \
	AAFMediaDescriptor \
	AAFMediaFileDescriptor \
	AAFMediaFilmDescriptor \
	AAFMediaGroup  \
	AAFMediaTapeDescriptor \
	AAFMob  \
	AAFMobSlot \
	AAFNestedScope\
	AAFObject \
	AAFObjectAttribute \
	AAFParameter \
	AAFParameterSlot \
	AAFPulldown \
	AAFPropertyDef \
	AAFReferenceValue \
	AAFScopeReference \
	AAFSegment \
	AAFSequence \
	AAFSession \
	AAFSourceClip \
	AAFSourceMob \
	AAFSourceReference \
	AAFStringAttribute \
	AAFTimecode \
	AAFTimecodeStream \
	AAFTimecodeStream12M \
	AAFTimelineMobSlot \
	AAFTransition \
	AAFTypeDef \
	AAFUnixLocator \
	AAFVaryingValue \
	AAFWindowsLocator



# These are all of the other non-persistent objects:
AAFNONPERSISTENTOBJECTS = \
	AAFFile \
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
	EnumAAFMobSlots \
	EnumAAFMobs \
	EnumAAFParameterSlots \
	EnumAAFPropertyDefs \
	EnumAAFReferenceValues \
	EnumAAFSegments \
	EnumAAFTypeDefs 


# The list of standard dodo targets.
# AAFTypes have to be handled separately since no object is to be created.
DODO_TARGET_NAMES = \
	$(AAFPERSISTENTOBJECTS) \
	$(AAFNONPERSISTENTOBJECTS)


# Build the list of all objects that need to be built into the COM DLL
AAFOBJECTS = \
	$(AAFPERSISTENTOBJECTS) \
	$(AAFNONPERSISTENTOBJECTS)

