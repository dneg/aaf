###############################################################################
#
# $Id$ $Name$
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
# The Original Code of this file is Copyright 1998-2004, Licensor of the
# AAF Association.
#
# The Initial Developer of the Original Code of this file and the
# Licensor of the AAF Association is Avid Technology.
# All rights reserved.
#
###############################################################################
AAFPERSISTENTOBJECTS = \
	AAFAttribute \
	AAFAttributeArray \
	AAFClassDefinition \
	AAFDictionary \
	AAFComponent \
	AAFCompositionMob \
	AAFConstValue \
	AAFParameterDefinition \
	AAFControlPoint \
	AAFDataDefinition \
	AAFDefinitionObject \
	AAFEdgecode \
	AAFEffectDefinition \
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
	AAFPropertyDefinition \
	AAFReferenceValue \
	AAFScopeReference \
	AAFSegment \
	AAFSequence \
	AAFSourceClip \
	AAFSourceMob \
	AAFSourceReference \
	AAFStringAttribute \
	AAFTimecode \
	AAFTimecodeStream \
	AAFTimecodeStream12M \
	AAFTimelineMobSlot \
	AAFTransition \
	AAFTypeDefinition \
	AAFUnixLocator \
	AAFVaryingValue \
	AAFWindowsLocator



# These are all of the other non-persistent objects:
AAFNONPERSISTENTOBJECTS = \
	AAFFile \
	AAFMedia \
	EnumAAFAttributes \
	EnumAAFClassDefinitions \
	EnumAAFComponents \
	EnumAAFParameterDefinitions \
	EnumAAFControlPoints \
	EnumAAFDataDefinitions \
	EnumAAFDefinitions \
	EnumAAFEffectDefinitions \
	EnumAAFIdentifications \
	EnumAAFLocators \
	EnumAAFMedia \
	EnumAAFMobComments \
	EnumAAFMobSlots \
	EnumAAFMobs \
	EnumAAFParameterSlots \
	EnumAAFPropertyDefinitions \
	EnumAAFReferenceValues \
	EnumAAFSegments \
	EnumAAFTypeDefinitions 






# Why are these objects being dodo'd?
AAFUTILS = \
	AAFUInt64 \
	AAFString \
	AAFInt64



# The list of standard dodo targets.
# AAFTypes have to be handled separately since no object is to be created.
DODO_TARGET_NAMES = \
	$(AAFPERSISTENTOBJECTS) \
	$(AAFNONPERSISTENTOBJECTS) \
	AAFUnitTest
		 


# Build the list of all objects that need to be built into the COM DLL
# 
# BobT 3-Aug-1998: Instead of taking these automatically
# dodo-makefile-generated dependencies directly, we want more control
# of which targets get built.  We'll add them manually as we see fit.
# 
# Was:
# 
# AAFOBJECTS = \
# 	$(AAFPERSISTENTOBJECTS) \
# 	$(AAFNONPERSISTENTOBJECTS)
# 
AAFOBJECTS = \
	AAFAttribute \
	AAFClassDefinition \
	AAFComponent \
	AAFConstValue \
	AAFControlCodeDefinition \
	AAFControlPoint \
	AAFDataDefinition \
	AAFDefinitionObject \
	AAFDictionary \
	AAFEdgecode \
	AAFEffectDefinition \
	AAFEffectInvocation \
	AAFFiller \
	AAFFile \
	AAFFindSourceInfo \
	AAFIdentification \
	AAFIntegerAttribute \
	AAFLocator \
	AAFMacLocator \
	AAFMediaDescriptor \
	AAFMediaFileDescriptor \
	AAFMediaFilmDescriptor \
	AAFMediaTapeDescriptor \
	AAFMob  \
	AAFMobSlot \
	AAFObject \
	AAFObjectAttribute \
	AAFParameter \
	AAFParameterSlot \
	AAFPropertyDefinition \
	AAFPulldown \
	AAFSegment \
	AAFSequence \
	AAFSourceClip \
	AAFSourceReference \
	AAFStringAttribute \
	AAFTimecode \
	AAFTimelineMobSlot \
	AAFTypeDefinition \
	AAFUnixLocator \
	AAFVaryingValue \
	AAFWindowsLocator \
	EnumAAFAttributes \
	EnumAAFClassDefinitions \
	EnumAAFParameterDefinitions \
	EnumAAFComponents \
	EnumAAFControlPoints \
	EnumAAFDataDefinitions \
	EnumAAFEffectDefinitions \
	EnumAAFIdentifications \
	EnumAAFLocators \
	EnumAAFMobComments \
	EnumAAFParameterSlots \
	EnumAAFPropertyDefinitions \
	EnumAAFReferenceValues \
	EnumAAFTypeDefinitions
