############################################
#                                          #
# Copyright (c) 1998 Avid Technology, Inc. #
#                                          #
############################################

all : dodo-app targets

BLD_CFG_DIR = ../ref-impl/bld_cfg

include $(BLD_CFG_DIR)/common.mk

DODO = dodo.$(EXE)

dodo-app:
	$(SH_PREFIX) cd tool ;  $(MAKE) $(SH_SUFFIX)

test_only:
	$(SH_PREFIX) cd tool ; $(MAKE) test_only $(SH_SUFFIX)

run:
	 $(SH_PREIX) cd tool ; $(MAKE) run $(SH_SUFFIX)

.SUFFIXES: .cpp .h .comc .comh .dod .exp .idl .implc .implh
								   
DODO_TARGETS = \
	aafobjects.all \
	AAFAttribute.all \
	AAFAttributeArray.all \
	AAFClassDefinition.all \
	AAFDictionary.all \
	AAFComponent.all \
	AAFCompositionMob.all \
	AAFConstValue.all \
	AAFControlCodeDefinition.all \
	AAFControlPoint.all \
	AAFDataDefinition.all \
	AAFDefinitionObject.all \
	AAFEdgecode.all \
	AAFEffectDefinition.all \
	AAFEffectInvocation.all \
	AAFFile.all \
	AAFFiller.all \
	AAFFindSourceInfo.all \
	AAFHeader.all \
	AAFIdentification.all \
	AAFInt64.all \
	AAFIntegerAttribute.all \
	AAFLocator.all \
	AAFMacLocator.all \
	AAFMasterMob.all \
	AAFMedia.all \
	AAFMediaDescriptor.all \
	AAFMediaFileDescriptor.all \
	AAFMediaFilmDescriptor.all \
	AAFMediaGroup.all  \
	AAFMediaTapeDescriptor.all \
	AAFMob.all  \
	AAFMobSlot.all \
	AAFNestedScope.all\
	AAFObject.all \
	AAFObjectAttribute.all \
	AAFParameter.all \
	AAFParameterSlot.all \
	AAFPulldown.all \
	AAFPropertyDefinition.all \
	AAFReferenceValue.all \
	AAFScopeReference.all \
	AAFSegment.all \
	AAFSequence.all \
	AAFSourceClip.all \
	AAFSourceMob.all \
	AAFSourceReference.all \
	AAFString.all \
	AAFStringAttribute.all \
	AAFTimecode.all \
	AAFTimecodeStream.all \
	AAFTimecodeStream12M.all \
	AAFTimelineMobSlot.all \
	AAFTransition.all \
	AAFTypeDefinition.all \
	AAFTypes.all \
	AAFUInt64.all \
	AAFUnitTest.all \
	AAFUnixLocator.all \
	AAFVaryingValue.all \
	AAFWindowsLocator.all \
	EnumAAFAttributes.all \
	EnumAAFClassDefinitions.all \
	EnumAAFComponents.all \
	EnumAAFControlCodeDefinitions.all \
	EnumAAFControlPoints.all \
	EnumAAFDataDefinitions.all \
	EnumAAFDefinitions.all \
	EnumAAFEffectDefinitions.all \
	EnumAAFIdentifications.all \
	EnumAAFLocators.all \
	EnumAAFMedia.all \
	EnumAAFMobComments.all \
	EnumAAFMobSlots.all \
	EnumAAFMobs.all \
	EnumAAFParameterSlots.all \
	EnumAAFPropertyDefinitions.all \
	EnumAAFReferenceValues.all \
	EnumAAFSegments.all \
	EnumAAFTypeDefinitions.all


targets: $(DODO_TARGETS)

SRC_DIR = ../ref-impl/src

INCLUDE_DIR = ../ref-impl/include

.dod.exp :
	$(SH_PREFIX) $(RM) -f $*.exp $(SH_SUFFIX)
	$(SH_PREFIX) ./tool/$(DODO) -f macros/exp.mac < $*.dod > $*.tmp $(SH_SUFFIX)
	$(SH_PREFIX) mv $*.tmp $*.exp $(SH_SUFFIX)

.dod.h :
	$(SH_PREFIX) $(RM) -f $*.h $(SH_SUFFIX)
	$(SH_PREFIX) ./tool/$(DODO) -f macros/h.mac < $*.dod > $*.tmp $(SH_SUFFIX)
	$(SH_PREFIX) mv $*.tmp $*.h $(SH_SUFFIX)
	$(SH_PREFIX) cp $*.h $(INCLUDE_DIR)/cpp-api/ $(SH_SUFFIX)

.dod.comh :
	$(SH_PREFIX) $(RM) -f $*.comh $(SH_SUFFIX)
	$(SH_PREFIX) ./tool/$(DODO) -f macros/comh.mac < $*.dod > $*.tmp $(SH_SUFFIX)
	$(SH_PREFIX) mv $*.tmp $*.comh $(SH_SUFFIX)
	$(SH_PREFIX) cp $*.comh $(SRC_DIR)/com-api/C$*.h $(SH_SUFFIX)

.dod.comc :
	$(SH_PREFIX) $(RM) -f $*.comc $(SH_SUFFIX)
	$(SH_PREFIX) ./tool/$(DODO) -f macros/comc.mac < $*.dod > $*.tmp $(SH_SUFFIX)
	$(SH_PREFIX) mv $*.tmp $*.comc $(SH_SUFFIX)
	$(SH_PREFIX) cp $*.comc $(SRC_DIR)/com-api/C$*.cpp $(SH_SUFFIX)

.dod.implh :
	$(SH_PREFIX) $(RM) -f $*.implh $(SH_SUFFIX)
	$(SH_PREFIX) ./tool/$(DODO) -f macros/implh.mac < $*.dod > $*.tmp $(SH_SUFFIX)
	$(SH_PREFIX) mv $*.tmp $*.implh $(SH_SUFFIX)
	$(SH_PREFIX) cp $*.implh $(SRC_DIR)/impl/Impl$*.h $(SH_SUFFIX)

.dod.implc :
	$(SH_PREFIX) $(RM) -f $*.implc $(SH_SUFFIX)
	$(SH_PREFIX) ./tool/$(DODO) -f macros/implc.mac < $*.dod > $*.tmp $(SH_SUFFIX)
	$(SH_PREFIX) mv $*.tmp $*.implc $(SH_SUFFIX)
	$(SH_PREFIX) cp $*.implc $(SRC_DIR)/impl/Impl$*.cpp $(SH_SUFFIX)

.dod.cpp :
	$(SH_PREFIX) $(RM) -f $*.cpp $(SH_SUFFIX)
	$(SH_PREFIX) ./tool/$(DODO) -f macros/cpp.mac < $*.dod > $*.tmp $(SH_SUFFIX)
	$(SH_PREFIX) mv $*.tmp $*.cpp $(SH_SUFFIX)
	$(SH_PREFIX) cp $*.cpp $(SRC_DIR)/cpp-api $(SH_SUFFIX)

.dod.idl :
	$(SH_PREFIX) $(RM) -f $*.idl $(SH_SUFFIX)
	$(SH_PREFIX) ./tool/$(DODO) -f macros/idl.mac < $*.dod > $*.tmp $(SH_SUFFIX)
	$(SH_PREFIX) mv $*.tmp $*.idl $(SH_SUFFIX)
	$(SH_PREFIX) cp $*.idl $(INCLUDE_DIR)/com-api/ $(SH_SUFFIX)

#.idl.idlh :
#	$(SH_PREFIX) $(RM) -f $*.idlh $(SH_SUFFIX)
#	$(SH_PREFIX) $(IDL) /out $(INCLUDE_DIR)/com-api $*.idl $(SH_SUFFIX)
#	$(SH_PREFIX) cp $(INCLUDE_DIR)/com-api/$*.h $*.idlh $(SH_SUFFIX)

# Make sure our list of objects in the src directories is up-to-date.
aafobjects.all : aafobjects.list
aafobjects.list : aafobjects.mk
	$(SH_PREFIX) $(RM) -f aafobjects.list $(SH_SUFFIX)
	$(SH_PREFIX) cp aafobjects.mk aafobjects.list $(SH_SUFFIX)
	$(SH_PREFIX) cp aafobjects.mk $(SRC_DIR)/com-api $(SH_SUFFIX)

clean:
	$(SH_PREFIX) cd tool ; $(MAKE) clean $(SH_SUFFIX)
	$(SH_PREFIX) $(RM) -rf *.cpp *.h *.idl *.exp *.comc *.comh *.implc *.implh *.list $(SH_SUFFIX)
	$(SH_PREFIX) $(RM) -rf $(SRC_DIR)/cpp-api/*.cpp $(SH_SUFFIX)
	$(SH_PREFIX) $(RM) -rf $(SRC_DIR)/com-api/*.h $(SH_SUFFIX)
	$(SH_PREFIX) $(RM) -rf $(SRC_DIR)/com-api/*.cpp $(SH_SUFFIX)
	$(SH_PREFIX) $(RM) -rf $(SRC_DIR)/impl/*.h $(SH_SUFFIX)
	$(SH_PREFIX) $(RM) -rf $(SRC_DIR)/impl/*.cpp $(SH_SUFFIX)
	$(SH_PREFIX) $(RM) -rf $(INCLUDE_DIR)/com-api/*.h $(SH_SUFFIX)
	$(SH_PREFIX) $(RM) -rf $(INCLUDE_DIR)/com-api/*.idl $(SH_SUFFIX)
	$(SH_PREFIX) $(RM) -rf $(INCLUDE_DIR)/cpp-api/*.h $(SH_SUFFIX)

AAFTypes.all : AAFTypes.h
AAFTypes.all : AAFTypes.exp AAFTypes.idl
AAFTypes.all : AAFTypes.implh
AAFTypes.exp : macros/exp.mac macros/base.mac
AAFTypes.h : AAFTypes.exp macros/h.mac macros/base.mac
AAFTypes.idl : AAFTypes.exp macros/idl.mac
AAFTypes.implh : AAFTypes.exp macros/implh.mac macros/base.mac

AAFObject.all : AAFTypes.all
AAFObject.all : AAFObject.cpp AAFObject.h
AAFObject.all : AAFObject.comc AAFObject.comh
AAFObject.all : AAFObject.exp AAFObject.idl
AAFObject.all : AAFObject.implc AAFObject.implh
AAFObject.exp : macros/exp.mac macros/base.mac
AAFObject.h : AAFObject.exp macros/h.mac macros/base.mac
AAFObject.cpp : AAFObject.exp macros/cpp.mac macros/base.mac
AAFObject.idl : AAFObject.exp macros/idl.mac macros/base.mac
AAFObject.comc : AAFObject.comh AAFObject.exp macros/comc.mac macros/base.mac
AAFObject.comh : AAFObject.exp macros/comh.mac macros/base.mac
AAFObject.implc : AAFObject.implh AAFObject.exp macros/implc.mac macros/base.mac
AAFObject.implh : AAFObject.exp macros/implh.mac macros/base.mac

AAFAttribute.all : AAFAttribute.cpp AAFAttribute.h
AAFAttribute.all : AAFAttribute.comc AAFAttribute.comh
AAFAttribute.all : AAFAttribute.exp AAFAttribute.idl
AAFAttribute.all : AAFAttribute.implc AAFAttribute.implh
AAFAttribute.exp : AAFUnitTest.exp AAFObject.exp macros/exp.mac macros/base.mac
AAFAttribute.h : AAFAttribute.exp macros/h.mac macros/base.mac
AAFAttribute.cpp : AAFAttribute.exp macros/cpp.mac macros/base.mac
AAFAttribute.idl : AAFAttribute.exp macros/idl.mac macros/base.mac
AAFAttribute.comc : AAFAttribute.exp macros/comc.mac macros/base.mac
AAFAttribute.comh : AAFAttribute.exp macros/comh.mac macros/base.mac
AAFAttribute.implc : AAFAttribute.exp macros/implc.mac macros/base.mac
AAFAttribute.implh : AAFAttribute.exp macros/implh.mac macros/base.mac

AAFAttributeArray.all : AAFAttributeArray.cpp AAFAttributeArray.h
AAFAttributeArray.all : AAFAttributeArray.comc AAFAttributeArray.comh
AAFAttributeArray.all : AAFAttributeArray.exp AAFAttributeArray.idl
AAFAttributeArray.all : AAFAttributeArray.implc AAFAttributeArray.implh
AAFAttributeArray.exp : AAFUnitTest.exp AAFObject.exp macros/exp.mac macros/base.mac
AAFAttributeArray.h : AAFAttributeArray.exp macros/h.mac macros/base.mac
AAFAttributeArray.cpp : AAFAttributeArray.exp macros/cpp.mac macros/base.mac
AAFAttributeArray.idl : AAFAttributeArray.exp macros/idl.mac macros/base.mac
AAFAttributeArray.comc : AAFAttributeArray.exp macros/comc.mac macros/base.mac
AAFAttributeArray.comh : AAFAttributeArray.exp macros/comh.mac macros/base.mac
AAFAttributeArray.implc : AAFAttributeArray.exp macros/implc.mac macros/base.mac
AAFAttributeArray.implh : AAFAttributeArray.exp macros/implh.mac macros/base.mac

EnumAAFPropertyDefinitions.all : EnumAAFPropertyDefinitions.cpp EnumAAFPropertyDefinitions.h
EnumAAFPropertyDefinitions.all : EnumAAFPropertyDefinitions.comc EnumAAFPropertyDefinitions.comh
EnumAAFPropertyDefinitions.all : EnumAAFPropertyDefinitions.exp EnumAAFPropertyDefinitions.idl
EnumAAFPropertyDefinitions.all : EnumAAFPropertyDefinitions.implc EnumAAFPropertyDefinitions.implh
EnumAAFPropertyDefinitions.exp : AAFUnitTest.exp macros/exp.mac macros/base.mac
EnumAAFPropertyDefinitions.h : EnumAAFPropertyDefinitions.exp macros/h.mac macros/base.mac
EnumAAFPropertyDefinitions.cpp : EnumAAFPropertyDefinitions.exp macros/cpp.mac macros/base.mac
EnumAAFPropertyDefinitions.idl : EnumAAFPropertyDefinitions.exp macros/idl.mac macros/base.mac
EnumAAFPropertyDefinitions.comc : EnumAAFPropertyDefinitions.exp macros/comc.mac macros/base.mac
EnumAAFPropertyDefinitions.comh : EnumAAFPropertyDefinitions.exp macros/comh.mac macros/base.mac
EnumAAFPropertyDefinitions.implc : EnumAAFPropertyDefinitions.exp macros/implc.mac macros/base.mac
EnumAAFPropertyDefinitions.implh : EnumAAFPropertyDefinitions.exp macros/implh.mac macros/base.mac

AAFClassDefinition.all : AAFClassDefinition.cpp AAFClassDefinition.h
AAFClassDefinition.all : AAFClassDefinition.comc AAFClassDefinition.comh
AAFClassDefinition.all : AAFClassDefinition.exp AAFClassDefinition.idl
AAFClassDefinition.all : AAFClassDefinition.implc AAFClassDefinition.implh
AAFClassDefinition.exp : AAFUnitTest.exp AAFSegment.exp macros/exp.mac macros/base.mac
AAFClassDefinition.h : AAFClassDefinition.exp macros/h.mac macros/base.mac
AAFClassDefinition.cpp : AAFClassDefinition.exp macros/cpp.mac macros/base.mac
AAFClassDefinition.idl : AAFClassDefinition.exp macros/idl.mac macros/base.mac
AAFClassDefinition.comc : AAFClassDefinition.exp macros/comc.mac macros/base.mac
AAFClassDefinition.comh : AAFClassDefinition.exp macros/comh.mac macros/base.mac
AAFClassDefinition.implc : AAFClassDefinition.exp macros/implc.mac macros/base.mac
AAFClassDefinition.implh : AAFClassDefinition.exp macros/implh.mac macros/base.mac

# Build dependency
AAFDictionary.all : \
	AAFObject.all \
	AAFClassDefinition.all EnumAAFClassDefinitions.all \
	AAFTypeDefinition.all EnumAAFTypeDefinitions.all \
	AAFDataDefinition.all EnumAAFDataDefinitions.all \
	AAFEffectDefinition.all EnumAAFEffectDefinitions.all \
	EnumAAFDefinitions.all
AAFDictionary.all : AAFDictionary.cpp AAFDictionary.h
AAFDictionary.all : AAFDictionary.comc AAFDictionary.comh
AAFDictionary.all : AAFDictionary.exp AAFDictionary.idl
AAFDictionary.all : AAFDictionary.implc AAFDictionary.implh
AAFDictionary.exp : AAFObject.exp macros/exp.mac macros/base.mac
AAFDictionary.h : AAFDictionary.exp macros/h.mac macros/base.mac
AAFDictionary.cpp : AAFDictionary.exp macros/cpp.mac macros/base.mac
AAFDictionary.idl : AAFDictionary.exp macros/idl.mac macros/base.mac
AAFDictionary.comc : AAFDictionary.comh AAFDictionary.exp macros/comc.mac macros/base.mac
AAFDictionary.comh : AAFDictionary.exp macros/comh.mac macros/base.mac
AAFDictionary.implc : AAFDictionary.implh AAFDictionary.exp macros/implc.mac macros/base.mac
AAFDictionary.implh : AAFDictionary.exp macros/implh.mac macros/base.mac 

AAFComponent.all : AAFComponent.cpp AAFComponent.h
AAFComponent.all : AAFComponent.comc AAFComponent.comh
AAFComponent.all : AAFComponent.exp AAFComponent.idl
AAFComponent.all : AAFComponent.implc AAFComponent.implh
AAFComponent.exp : AAFUnitTest.exp AAFObject.exp macros/exp.mac macros/base.mac
AAFComponent.h : AAFComponent.exp macros/h.mac macros/base.mac
AAFComponent.cpp : AAFComponent.exp macros/cpp.mac macros/base.mac
AAFComponent.idl : AAFComponent.exp macros/idl.mac macros/base.mac
AAFComponent.comc : AAFComponent.exp macros/comc.mac macros/base.mac
AAFComponent.comh : AAFComponent.exp macros/comh.mac macros/base.mac
AAFComponent.implc : AAFComponent.exp macros/implc.mac macros/base.mac
AAFComponent.implh : AAFComponent.exp macros/implh.mac macros/base.mac

AAFCompositionMob.all : AAFCompositionMob.cpp AAFCompositionMob.h
AAFCompositionMob.all : AAFCompositionMob.comc AAFCompositionMob.comh
AAFCompositionMob.all : AAFCompositionMob.exp AAFCompositionMob.idl
AAFCompositionMob.all : AAFCompositionMob.implc AAFCompositionMob.implh
AAFCompositionMob.exp : AAFUnitTest.exp macros/exp.mac macros/base.mac
AAFCompositionMob.h : AAFCompositionMob.exp macros/h.mac macros/base.mac
AAFCompositionMob.cpp : AAFCompositionMob.exp macros/cpp.mac macros/base.mac
AAFCompositionMob.idl : AAFCompositionMob.exp macros/idl.mac macros/base.mac
AAFCompositionMob.comc : AAFCompositionMob.exp macros/comc.mac macros/base.mac
AAFCompositionMob.comh : AAFCompositionMob.exp macros/comh.mac macros/base.mac
AAFCompositionMob.implc : AAFCompositionMob.exp macros/implc.mac macros/base.mac
AAFCompositionMob.implh : AAFCompositionMob.exp macros/implh.mac macros/base.mac

AAFConstValue.all : AAFConstValue.cpp AAFConstValue.h
AAFConstValue.all : AAFConstValue.comc AAFConstValue.comh
AAFConstValue.all : AAFConstValue.exp AAFConstValue.idl
AAFConstValue.all : AAFConstValue.implc AAFConstValue.implh
AAFConstValue.exp : AAFUnitTest.exp macros/exp.mac macros/base.mac
AAFConstValue.h : AAFConstValue.exp macros/h.mac macros/base.mac
AAFConstValue.cpp : AAFConstValue.exp macros/cpp.mac macros/base.mac
AAFConstValue.idl : AAFConstValue.exp macros/idl.mac macros/base.mac
AAFConstValue.comc : AAFConstValue.exp macros/comc.mac macros/base.mac
AAFConstValue.comh : AAFConstValue.exp macros/comh.mac macros/base.mac
AAFConstValue.implc : AAFConstValue.exp macros/implc.mac macros/base.mac
AAFConstValue.implh : AAFConstValue.exp macros/implh.mac macros/base.mac

AAFControlCodeDefinition.all : AAFControlCodeDefinition.cpp AAFControlCodeDefinition.h
AAFControlCodeDefinition.all : AAFControlCodeDefinition.comc AAFControlCodeDefinition.comh
AAFControlCodeDefinition.all : AAFControlCodeDefinition.exp AAFControlCodeDefinition.idl
AAFControlCodeDefinition.all : AAFControlCodeDefinition.implc AAFControlCodeDefinition.implh
AAFControlCodeDefinition.exp : AAFUnitTest.exp AAFSegment.exp macros/exp.mac macros/base.mac
AAFControlCodeDefinition.h : AAFControlCodeDefinition.exp macros/h.mac macros/base.mac
AAFControlCodeDefinition.cpp : AAFControlCodeDefinition.exp macros/cpp.mac macros/base.mac
AAFControlCodeDefinition.idl : AAFControlCodeDefinition.exp macros/idl.mac macros/base.mac
AAFControlCodeDefinition.comc : AAFControlCodeDefinition.exp macros/comc.mac macros/base.mac
AAFControlCodeDefinition.comh : AAFControlCodeDefinition.exp macros/comh.mac macros/base.mac
AAFControlCodeDefinition.implc : AAFControlCodeDefinition.exp macros/implc.mac macros/base.mac
AAFControlCodeDefinition.implh : AAFControlCodeDefinition.exp macros/implh.mac macros/base.mac

AAFControlPoint.all : AAFControlPoint.cpp AAFControlPoint.h
AAFControlPoint.all : AAFControlPoint.comc AAFControlPoint.comh
AAFControlPoint.all : AAFControlPoint.exp AAFControlPoint.idl
AAFControlPoint.all : AAFControlPoint.implc AAFControlPoint.implh
AAFControlPoint.exp : AAFUnitTest.exp macros/exp.mac macros/base.mac
AAFControlPoint.h : AAFControlPoint.exp macros/h.mac macros/base.mac
AAFControlPoint.cpp : AAFControlPoint.exp macros/cpp.mac macros/base.mac
AAFControlPoint.idl : AAFControlPoint.exp macros/idl.mac macros/base.mac
AAFControlPoint.comc : AAFControlPoint.exp macros/comc.mac macros/base.mac
AAFControlPoint.comh : AAFControlPoint.exp macros/comh.mac macros/base.mac
AAFControlPoint.implc : AAFControlPoint.exp macros/implc.mac macros/base.mac
AAFControlPoint.implh : AAFControlPoint.exp macros/implh.mac macros/base.mac

AAFDataDefinition.all : AAFTypes.all AAFObject.all AAFDefinitionObject.all
AAFDataDefinition.all : AAFDataDefinition.cpp AAFDataDefinition.h
AAFDataDefinition.all : AAFDataDefinition.comc AAFDataDefinition.comh
AAFDataDefinition.all : AAFDataDefinition.exp AAFDataDefinition.idl
AAFDataDefinition.all : AAFDataDefinition.implc AAFDataDefinition.implh
AAFDataDefinition.exp : AAFDefinitionObject.exp macros/exp.mac macros/base.mac
AAFDataDefinition.h : AAFDataDefinition.exp macros/h.mac macros/base.mac
AAFDataDefinition.cpp : AAFDataDefinition.exp macros/cpp.mac macros/base.mac
AAFDataDefinition.idl : AAFDataDefinition.exp macros/idl.mac macros/base.mac
AAFDataDefinition.implh : AAFTypes.idl AAFDataDefinition.idl
AAFDataDefinition.comc : AAFDataDefinition.comh AAFDataDefinition.exp macros/comc.mac macros/base.mac
AAFDataDefinition.comh : AAFDataDefinition.exp macros/comh.mac macros/base.mac
AAFDataDefinition.implc : AAFDataDefinition.implh AAFDataDefinition.exp macros/implc.mac macros/base.mac
AAFDataDefinition.implh : AAFDataDefinition.exp macros/implh.mac macros/base.mac

AAFDefinitionObject.all : AAFDefinitionObject.cpp AAFDefinitionObject.h
AAFDefinitionObject.all : AAFDefinitionObject.comc AAFDefinitionObject.comh
AAFDefinitionObject.all : AAFDefinitionObject.exp AAFDefinitionObject.idl
AAFDefinitionObject.all : AAFDefinitionObject.implc AAFDefinitionObject.implh
AAFDefinitionObject.exp : AAFObject.exp macros/exp.mac macros/base.mac
AAFDefinitionObject.h : AAFDefinitionObject.exp macros/h.mac macros/base.mac
AAFDefinitionObject.cpp : AAFDefinitionObject.exp macros/cpp.mac macros/base.mac
AAFDefinitionObject.idl : AAFTypes.idl AAFDefinitionObject.exp macros/idl.mac macros/base.mac
AAFDefinitionObject.comc : AAFDefinitionObject.comh AAFDefinitionObject.exp macros/comc.mac macros/base.mac AAFDefinitionObject.comh
AAFDefinitionObject.comh : AAFDefinitionObject.exp macros/comh.mac macros/base.mac
AAFDefinitionObject.implc : AAFDefinitionObject.implh AAFDefinitionObject.exp macros/implc.mac macros/base.mac
AAFDefinitionObject.implh : AAFDefinitionObject.exp macros/implh.mac macros/base.mac

AAFEdgecode.all : AAFEdgecode.cpp AAFEdgecode.h
AAFEdgecode.all : AAFEdgecode.comc AAFEdgecode.comh
AAFEdgecode.all : AAFEdgecode.exp AAFEdgecode.idl
AAFEdgecode.all : AAFEdgecode.implc AAFEdgecode.implh
AAFEdgecode.exp : AAFSegment.exp AAFUnitTest.exp macros/exp.mac macros/base.mac
AAFEdgecode.h : AAFEdgecode.exp macros/h.mac macros/base.mac
AAFEdgecode.cpp : AAFEdgecode.exp macros/cpp.mac macros/base.mac
AAFEdgecode.idl : AAFEdgecode.exp macros/idl.mac macros/base.mac
AAFEdgecode.comc : AAFEdgecode.exp macros/comc.mac macros/base.mac
AAFEdgecode.comh : AAFEdgecode.exp macros/comh.mac macros/base.mac
AAFEdgecode.implc : AAFEdgecode.exp macros/implc.mac macros/base.mac
AAFEdgecode.implh : AAFEdgecode.exp macros/implh.mac macros/base.mac

AAFEffectDefinition.all : AAFEffectDefinition.cpp AAFEffectDefinition.h
AAFEffectDefinition.all : AAFEffectDefinition.comc AAFEffectDefinition.comh
AAFEffectDefinition.all : AAFEffectDefinition.exp AAFEffectDefinition.idl
AAFEffectDefinition.all : AAFEffectDefinition.implc AAFEffectDefinition.implh
AAFEffectDefinition.exp : AAFUnitTest.exp AAFSegment.exp macros/exp.mac macros/base.mac
AAFEffectDefinition.h : AAFEffectDefinition.exp macros/h.mac macros/base.mac
AAFEffectDefinition.cpp : AAFEffectDefinition.exp macros/cpp.mac macros/base.mac
AAFEffectDefinition.idl : AAFEffectDefinition.exp macros/idl.mac macros/base.mac
AAFEffectDefinition.comc : AAFEffectDefinition.exp macros/comc.mac macros/base.mac
AAFEffectDefinition.comh : AAFEffectDefinition.exp macros/comh.mac macros/base.mac
AAFEffectDefinition.implc : AAFEffectDefinition.exp macros/implc.mac macros/base.mac
AAFEffectDefinition.implh : AAFEffectDefinition.exp macros/implh.mac macros/base.mac

AAFEffectInvocation.all : AAFEffectInvocation.cpp AAFEffectInvocation.h
AAFEffectInvocation.all : AAFEffectInvocation.comc AAFEffectInvocation.comh
AAFEffectInvocation.all : AAFEffectInvocation.exp AAFEffectInvocation.idl
AAFEffectInvocation.all : AAFEffectInvocation.implc AAFEffectInvocation.implh
AAFEffectInvocation.exp : AAFEffectInvocation.dod AAFUnitTest.exp macros/exp.mac macros/base.mac
AAFEffectInvocation.h : AAFEffectInvocation.exp AAFSegment.exp macros/h.mac macros/base.mac
AAFEffectInvocation.cpp : AAFEffectInvocation.exp macros/cpp.mac macros/base.mac
AAFEffectInvocation.idl : AAFEffectInvocation.exp macros/idl.mac macros/base.mac
AAFEffectInvocation.comc : AAFEffectInvocation.exp macros/comc.mac macros/base.mac
AAFEffectInvocation.comh : AAFEffectInvocation.exp macros/comh.mac macros/base.mac
AAFEffectInvocation.implc : AAFEffectInvocation.exp macros/implc.mac macros/base.mac
AAFEffectInvocation.implh : AAFEffectInvocation.exp macros/implh.mac macros/base.mac

AAFFile.all : AAFFile.cpp AAFFile.h
AAFFile.all : AAFFile.comc AAFFile.comh
AAFFile.all : AAFFile.exp AAFFile.idl
AAFFile.all : AAFFile.implc AAFFile.implh
AAFFile.exp : macros/exp.mac macros/base.mac
AAFFile.h : AAFFile.exp macros/h.mac macros/base.mac
AAFFile.cpp : AAFFile.exp macros/cpp.mac macros/base.mac
AAFFile.idl : AAFFile.exp macros/idl.mac macros/base.mac
AAFFile.comc : AAFFile.exp macros/comc.mac macros/base.mac
AAFFile.comh : AAFFile.exp macros/comh.mac macros/base.mac
AAFFile.implc : AAFFile.exp macros/implc.mac macros/base.mac
AAFFile.implh : AAFFile.exp macros/implh.mac macros/base.mac

AAFFiller.all : AAFFiller.cpp AAFFiller.h
AAFFiller.all : AAFFiller.comc AAFFiller.comh
AAFFiller.all : AAFFiller.exp AAFFiller.idl
AAFFiller.all : AAFFiller.implc AAFFiller.implh
AAFFiller.exp : AAFSegment.exp AAFUnitTest.exp macros/exp.mac macros/base.mac
AAFFiller.h : AAFFiller.exp macros/h.mac macros/base.mac
AAFFiller.cpp : AAFFiller.exp macros/cpp.mac macros/base.mac
AAFFiller.idl : AAFFiller.exp macros/idl.mac macros/base.mac
AAFFiller.comc : AAFFiller.exp macros/comc.mac macros/base.mac
AAFFiller.comh : AAFFiller.exp macros/comh.mac macros/base.mac
AAFFiller.implc : AAFFiller.exp macros/implc.mac macros/base.mac
AAFFiller.implh : AAFFiller.exp macros/implh.mac macros/base.mac

AAFFindSourceInfo.all : AAFFindSourceInfo.cpp AAFFindSourceInfo.h
AAFFindSourceInfo.all : AAFFindSourceInfo.comc AAFFindSourceInfo.comh
AAFFindSourceInfo.all : AAFFindSourceInfo.exp AAFFindSourceInfo.idl
AAFFindSourceInfo.all : AAFFindSourceInfo.implc AAFFindSourceInfo.implh
AAFFindSourceInfo.exp : macros/exp.mac macros/base.mac
AAFFindSourceInfo.h : AAFFindSourceInfo.exp macros/h.mac macros/base.mac
AAFFindSourceInfo.cpp : AAFFindSourceInfo.exp macros/cpp.mac macros/base.mac
AAFFindSourceInfo.idl : AAFFindSourceInfo.exp macros/idl.mac macros/base.mac
AAFFindSourceInfo.comc : AAFFindSourceInfo.exp macros/comc.mac macros/base.mac
AAFFindSourceInfo.comh : AAFFindSourceInfo.exp macros/comh.mac macros/base.mac
AAFFindSourceInfo.implc : AAFFindSourceInfo.exp macros/implc.mac macros/base.mac
AAFFindSourceInfo.implh : AAFFindSourceInfo.exp macros/implh.mac macros/base.mac

AAFInt64.all : AAFInt64.cpp AAFInt64.h
AAFInt64.all : AAFInt64.comc AAFInt64.comh
AAFInt64.all : AAFInt64.exp AAFInt64.idl
AAFInt64.all : AAFInt64.implc AAFInt64.implh
AAFInt64.exp : AAFUnitTest.exp macros/exp.mac macros/base.mac
AAFInt64.h : AAFInt64.exp macros/h.mac macros/base.mac
AAFInt64.cpp : AAFInt64.exp macros/cpp.mac macros/base.mac
AAFInt64.idl : AAFInt64.exp macros/idl.mac macros/base.mac
AAFInt64.comc : AAFInt64.exp macros/comc.mac macros/base.mac
AAFInt64.comh : AAFInt64.exp macros/comh.mac macros/base.mac
AAFInt64.implc : AAFInt64.exp macros/implc.mac macros/base.mac
AAFInt64.implh : AAFInt64.exp macros/implh.mac macros/base.mac

AAFIntegerAttribute.all : AAFIntegerAttribute.cpp AAFIntegerAttribute.h
AAFIntegerAttribute.all : AAFIntegerAttribute.comc AAFIntegerAttribute.comh
AAFIntegerAttribute.all : AAFIntegerAttribute.exp AAFIntegerAttribute.idl
AAFIntegerAttribute.all : AAFIntegerAttribute.implc AAFIntegerAttribute.implh
AAFIntegerAttribute.exp : AAFUnitTest.exp AAFObject.exp macros/exp.mac macros/base.mac
AAFIntegerAttribute.h : AAFIntegerAttribute.exp macros/h.mac macros/base.mac
AAFIntegerAttribute.cpp : AAFIntegerAttribute.exp macros/cpp.mac macros/base.mac
AAFIntegerAttribute.idl : AAFIntegerAttribute.exp macros/idl.mac macros/base.mac
AAFIntegerAttribute.comc : AAFIntegerAttribute.exp macros/comc.mac macros/base.mac
AAFIntegerAttribute.comh : AAFIntegerAttribute.exp macros/comh.mac macros/base.mac
AAFIntegerAttribute.implc : AAFIntegerAttribute.exp macros/implc.mac macros/base.mac
AAFIntegerAttribute.implh : AAFIntegerAttribute.exp macros/implh.mac macros/base.mac

AAFLocator.all : AAFLocator.cpp AAFLocator.h
AAFLocator.all : AAFLocator.comc AAFLocator.comh
AAFLocator.all : AAFLocator.exp AAFLocator.idl
AAFLocator.all : AAFLocator.implc AAFLocator.implh
AAFLocator.exp : AAFUnitTest.exp AAFObject.exp macros/exp.mac macros/base.mac
AAFLocator.h : AAFLocator.exp macros/h.mac macros/base.mac
AAFLocator.cpp : AAFLocator.exp macros/cpp.mac macros/base.mac
AAFLocator.idl : AAFLocator.exp macros/idl.mac macros/base.mac
AAFLocator.implc : AAFLocator.exp macros/implc.mac macros/base.mac
AAFLocator.implh : AAFLocator.exp macros/implh.mac macros/base.mac

AAFMacLocator.all : AAFMacLocator.cpp AAFMacLocator.h
AAFMacLocator.all : AAFMacLocator.comc AAFMacLocator.comh
AAFMacLocator.all : AAFMacLocator.exp AAFMacLocator.idl
AAFMacLocator.all : AAFMacLocator.implc AAFMacLocator.implh
AAFMacLocator.exp : AAFUnitTest.exp AAFLocator.exp macros/exp.mac macros/base.mac
AAFMacLocator.h : AAFMacLocator.exp macros/h.mac macros/base.mac
AAFMacLocator.cpp : AAFMacLocator.exp macros/cpp.mac macros/base.mac
AAFMacLocator.idl : AAFMacLocator.exp macros/idl.mac macros/base.mac
AAFMacLocator.implc : AAFMacLocator.exp macros/implc.mac macros/base.mac
AAFMacLocator.implh : AAFMacLocator.exp macros/implh.mac macros/base.mac

AAFMedia.all : AAFMedia.cpp AAFMedia.h
AAFMedia.all : AAFMedia.comc AAFMedia.comh
AAFMedia.all : AAFMedia.exp AAFMedia.idl
AAFMedia.all : AAFMedia.implc AAFMedia.implh
AAFMedia.exp : AAFUnitTest.exp macros/exp.mac macros/base.mac
AAFMedia.h : AAFMedia.exp macros/h.mac macros/base.mac
AAFMedia.cpp : AAFMedia.exp macros/cpp.mac macros/base.mac
AAFMedia.idl : AAFMedia.exp macros/idl.mac macros/base.mac
AAFMedia.comc : AAFMedia.exp macros/comc.mac macros/base.mac
AAFMedia.comh : AAFMedia.exp macros/comh.mac macros/base.mac
AAFMedia.implc : AAFMedia.exp macros/implc.mac macros/base.mac
AAFMedia.implh : AAFMedia.exp macros/implh.mac macros/base.mac

AAFMediaDescriptor.all : AAFMediaDescriptor.cpp AAFMediaDescriptor.h
AAFMediaDescriptor.all : AAFMediaDescriptor.comc AAFMediaDescriptor.comh
AAFMediaDescriptor.all : AAFMediaDescriptor.exp AAFMediaDescriptor.idl
AAFMediaDescriptor.all : AAFMediaDescriptor.implc AAFMediaDescriptor.implh
AAFMediaDescriptor.exp : AAFUnitTest.exp macros/exp.mac macros/base.mac
AAFMediaDescriptor.h : AAFMediaDescriptor.exp macros/h.mac macros/base.mac
AAFMediaDescriptor.cpp : AAFMediaDescriptor.exp macros/cpp.mac macros/base.mac
AAFMediaDescriptor.idl : AAFMediaDescriptor.exp macros/idl.mac macros/base.mac
AAFMediaDescriptor.comc : AAFMediaDescriptor.exp macros/comc.mac macros/base.mac
AAFMediaDescriptor.comh : AAFMediaDescriptor.exp macros/comh.mac macros/base.mac
AAFMediaDescriptor.implc : AAFMediaDescriptor.exp macros/implc.mac macros/base.mac
AAFMediaDescriptor.implh : AAFMediaDescriptor.exp macros/implh.mac macros/base.mac

AAFMediaFileDescriptor.all : AAFMediaFileDescriptor.cpp AAFMediaFileDescriptor.h
AAFMediaFileDescriptor.all : AAFMediaFileDescriptor.comc AAFMediaFileDescriptor.comh
AAFMediaFileDescriptor.all : AAFMediaFileDescriptor.exp AAFMediaFileDescriptor.idl
AAFMediaFileDescriptor.all : AAFMediaFileDescriptor.implc AAFMediaFileDescriptor.implh
AAFMediaFileDescriptor.exp : AAFUnitTest.exp AAFMediaDescriptor.exp macros/exp.mac macros/base.mac
AAFMediaFileDescriptor.h : AAFMediaFileDescriptor.exp macros/h.mac macros/base.mac
AAFMediaFileDescriptor.cpp : AAFMediaFileDescriptor.exp macros/cpp.mac macros/base.mac
AAFMediaFileDescriptor.idl : AAFMediaFileDescriptor.exp macros/idl.mac macros/base.mac
AAFMediaFileDescriptor.comc : AAFMediaFileDescriptor.exp macros/comc.mac macros/base.mac
AAFMediaFileDescriptor.comh : AAFMediaFileDescriptor.exp macros/comh.mac macros/base.mac
AAFMediaFileDescriptor.implc : AAFMediaFileDescriptor.exp macros/implc.mac macros/base.mac
AAFMediaFileDescriptor.implh : AAFMediaFileDescriptor.exp macros/implh.mac macros/base.mac

AAFMediaFilmDescriptor.all : AAFMediaFilmDescriptor.cpp AAFMediaFilmDescriptor.h
AAFMediaFilmDescriptor.all : AAFMediaFilmDescriptor.comc AAFMediaFilmDescriptor.comh
AAFMediaFilmDescriptor.all : AAFMediaFilmDescriptor.exp AAFMediaFilmDescriptor.idl
AAFMediaFilmDescriptor.all : AAFMediaFilmDescriptor.implc AAFMediaFilmDescriptor.implh
AAFMediaFilmDescriptor.exp : AAFUnitTest.exp AAFMediaDescriptor.exp macros/exp.mac macros/base.mac
AAFMediaFilmDescriptor.h : AAFMediaFilmDescriptor.exp macros/h.mac macros/base.mac
AAFMediaFilmDescriptor.cpp : AAFMediaFilmDescriptor.exp macros/cpp.mac macros/base.mac
AAFMediaFilmDescriptor.idl : AAFMediaFilmDescriptor.exp macros/idl.mac macros/base.mac
AAFMediaFilmDescriptor.comc : AAFMediaFilmDescriptor.exp macros/comc.mac macros/base.mac
AAFMediaFilmDescriptor.comh : AAFMediaFilmDescriptor.exp macros/comh.mac macros/base.mac
AAFMediaFilmDescriptor.implc : AAFMediaFilmDescriptor.exp macros/implc.mac macros/base.mac
AAFMediaFilmDescriptor.implh : AAFMediaFilmDescriptor.exp macros/implh.mac macros/base.mac

AAFMediaTapeDescriptor.all : AAFMediaTapeDescriptor.cpp AAFMediaTapeDescriptor.h
AAFMediaTapeDescriptor.all : AAFMediaTapeDescriptor.comc AAFMediaTapeDescriptor.comh
AAFMediaTapeDescriptor.all : AAFMediaTapeDescriptor.exp AAFMediaTapeDescriptor.idl
AAFMediaTapeDescriptor.all : AAFMediaTapeDescriptor.implc AAFMediaTapeDescriptor.implh
AAFMediaTapeDescriptor.exp : AAFUnitTest.exp AAFMediaDescriptor.exp macros/exp.mac macros/base.mac
AAFMediaTapeDescriptor.h : AAFMediaTapeDescriptor.exp macros/h.mac macros/base.mac
AAFMediaTapeDescriptor.cpp : AAFMediaTapeDescriptor.exp macros/cpp.mac macros/base.mac
AAFMediaTapeDescriptor.idl : AAFMediaTapeDescriptor.exp macros/idl.mac macros/base.mac
AAFMediaTapeDescriptor.comc : AAFMediaTapeDescriptor.exp macros/comc.mac macros/base.mac
AAFMediaTapeDescriptor.comh : AAFMediaTapeDescriptor.exp macros/comh.mac macros/base.mac
AAFMediaTapeDescriptor.implc : AAFMediaTapeDescriptor.exp macros/implc.mac macros/base.mac
AAFMediaTapeDescriptor.implh : AAFMediaTapeDescriptor.exp macros/implh.mac macros/base.mac

AAFMediaTapeDescriptor.all : AAFMediaTapeDescriptor.cpp AAFMediaTapeDescriptor.h
AAFMediaTapeDescriptor.all : AAFMediaTapeDescriptor.comc AAFMediaTapeDescriptor.comh
AAFMediaTapeDescriptor.all : AAFMediaTapeDescriptor.exp AAFMediaTapeDescriptor.idl
AAFMediaTapeDescriptor.all : AAFMediaTapeDescriptor.implc AAFMediaTapeDescriptor.implh
AAFMediaTapeDescriptor.exp : AAFUnitTest.exp AAFMediaDescriptor.exp macros/exp.mac macros/base.mac
AAFMediaTapeDescriptor.h : AAFMediaTapeDescriptor.exp macros/h.mac macros/base.mac
AAFMediaTapeDescriptor.cpp : AAFMediaTapeDescriptor.exp macros/cpp.mac macros/base.mac
AAFMediaTapeDescriptor.idl : AAFMediaTapeDescriptor.exp macros/idl.mac macros/base.mac
AAFMediaTapeDescriptor.comc : AAFMediaTapeDescriptor.exp macros/comc.mac macros/base.mac
AAFMediaTapeDescriptor.comh : AAFMediaTapeDescriptor.exp macros/comh.mac macros/base.mac
AAFMediaTapeDescriptor.implc : AAFMediaTapeDescriptor.exp macros/implc.mac macros/base.mac
AAFMediaTapeDescriptor.implh : AAFMediaTapeDescriptor.exp macros/implh.mac macros/base.mac

AAFMediaGroup.all : AAFMediaGroup.cpp AAFMediaGroup.h
AAFMediaGroup.all : AAFMediaGroup.comc AAFMediaGroup.comh
AAFMediaGroup.all : AAFMediaGroup.exp AAFMediaGroup.idl
AAFMediaGroup.all : AAFMediaGroup.implc AAFMediaGroup.implh
AAFMediaGroup.exp : AAFUnitTest.exp AAFSegment.exp macros/exp.mac macros/base.mac
AAFMediaGroup.h : AAFMediaGroup.exp macros/h.mac macros/base.mac
AAFMediaGroup.cpp : AAFMediaGroup.exp macros/cpp.mac macros/base.mac
AAFMediaGroup.idl : AAFMediaGroup.exp macros/idl.mac macros/base.mac
AAFMediaGroup.comc : AAFMediaGroup.exp macros/comc.mac macros/base.mac
AAFMediaGroup.comh : AAFMediaGroup.exp macros/comh.mac macros/base.mac
AAFMediaGroup.implc : AAFMediaGroup.exp macros/implc.mac macros/base.mac
AAFMediaGroup.implh : AAFMediaGroup.exp macros/implh.mac macros/base.mac

AAFMob.all : AAFMob.cpp AAFMob.h
AAFMob.all : AAFMob.comc AAFMob.comh
AAFMob.all : AAFMob.exp AAFMob.idl
AAFMob.all : AAFMob.implc AAFMob.implh
AAFMob.exp : AAFUnitTest.exp AAFObject.exp macros/exp.mac macros/base.mac
AAFMob.h : AAFMob.exp macros/h.mac macros/base.mac
AAFMob.cpp : AAFMob.exp macros/cpp.mac macros/base.mac
AAFMob.idl : AAFMob.exp macros/idl.mac macros/base.mac
AAFMob.comc : AAFMob.exp macros/comc.mac macros/base.mac
AAFMob.comh : AAFMob.exp macros/comh.mac macros/base.mac
AAFMob.implc : AAFMob.exp macros/implc.mac macros/base.mac
AAFMob.implh : AAFMob.exp macros/implh.mac macros/base.mac

AAFMasterMob.all : AAFMasterMob.cpp AAFMasterMob.h
AAFMasterMob.all : AAFMasterMob.comc AAFMasterMob.comh
AAFMasterMob.all : AAFMasterMob.exp AAFMasterMob.idl
AAFMasterMob.all : AAFMasterMob.implc AAFMasterMob.implh
AAFMasterMob.exp : AAFUnitTest.exp AAFMob.exp macros/exp.mac macros/base.mac
AAFMasterMob.h : AAFMasterMob.exp macros/h.mac macros/base.mac
AAFMasterMob.cpp : AAFMasterMob.exp macros/cpp.mac macros/base.mac
AAFMasterMob.idl : AAFMasterMob.exp macros/idl.mac macros/base.mac
AAFMasterMob.comc : AAFMasterMob.exp macros/comc.mac macros/base.mac
AAFMasterMob.comh : AAFMasterMob.exp macros/comh.mac macros/base.mac
AAFMasterMob.implc : AAFMasterMob.exp macros/implc.mac macros/base.mac
AAFMasterMob.implh : AAFMasterMob.exp macros/implh.mac macros/base.mac

AAFSourceMob.all : AAFSourceMob.cpp AAFSourceMob.h
AAFSourceMob.all : AAFSourceMob.comc AAFSourceMob.comh
AAFSourceMob.all : AAFSourceMob.exp AAFSourceMob.idl
AAFSourceMob.all : AAFSourceMob.implc AAFSourceMob.implh
AAFSourceMob.exp : AAFUnitTest.exp AAFMob.exp macros/exp.mac macros/base.mac
AAFSourceMob.h : AAFSourceMob.exp macros/h.mac macros/base.mac
AAFSourceMob.cpp : AAFSourceMob.exp macros/cpp.mac macros/base.mac
AAFSourceMob.idl : AAFSourceMob.exp macros/idl.mac macros/base.mac
AAFSourceMob.comc : AAFSourceMob.exp macros/comc.mac macros/base.mac
AAFSourceMob.comh : AAFSourceMob.exp macros/comh.mac macros/base.mac
AAFSourceMob.implc : AAFSourceMob.exp macros/implc.mac macros/base.mac
AAFSourceMob.implh : AAFSourceMob.exp macros/implh.mac macros/base.mac

AAFMobSlot.all : AAFMobSlot.cpp AAFMobSlot.h
AAFMobSlot.all : AAFMobSlot.comc AAFMobSlot.comh
AAFMobSlot.all : AAFMobSlot.exp AAFMobSlot.idl
AAFMobSlot.all : AAFMobSlot.implc AAFMobSlot.implh
AAFMobSlot.exp : AAFUnitTest.exp AAFObject.exp macros/exp.mac macros/base.mac
AAFMobSlot.h : AAFMobSlot.exp macros/h.mac macros/base.mac
AAFMobSlot.cpp : AAFMobSlot.exp macros/cpp.mac macros/base.mac
AAFMobSlot.idl : AAFMobSlot.exp macros/idl.mac macros/base.mac
AAFMobSlot.comc : AAFMobSlot.exp macros/comc.mac macros/base.mac
AAFMobSlot.comh : AAFMobSlot.exp macros/comh.mac macros/base.mac
AAFMobSlot.implc : AAFMobSlot.exp macros/implc.mac macros/base.mac
AAFMobSlot.implh : AAFMobSlot.exp macros/implh.mac macros/base.mac

AAFNestedScope.all : AAFNestedScope.cpp AAFNestedScope.h
AAFNestedScope.all : AAFNestedScope.comc AAFNestedScope.comh
AAFNestedScope.all : AAFNestedScope.exp AAFNestedScope.idl
AAFNestedScope.all : AAFNestedScope.implc AAFNestedScope.implh
AAFNestedScope.exp : AAFUnitTest.exp AAFSegment.exp macros/exp.mac macros/base.mac
AAFNestedScope.h : AAFNestedScope.exp macros/h.mac macros/base.mac
AAFNestedScope.cpp : AAFNestedScope.exp macros/cpp.mac macros/base.mac
AAFNestedScope.idl : AAFNestedScope.exp macros/idl.mac macros/base.mac
AAFNestedScope.comc : AAFNestedScope.exp macros/comc.mac macros/base.mac
AAFNestedScope.comh : AAFNestedScope.exp macros/comh.mac macros/base.mac
AAFNestedScope.implc : AAFNestedScope.exp macros/implc.mac macros/base.mac
AAFNestedScope.implh : AAFNestedScope.exp macros/implh.mac macros/base.mac

AAFTimelineMobSlot.all : AAFTimelineMobSlot.cpp AAFTimelineMobSlot.h
AAFTimelineMobSlot.all : AAFTimelineMobSlot.comc AAFTimelineMobSlot.comh
AAFTimelineMobSlot.all : AAFTimelineMobSlot.exp AAFTimelineMobSlot.idl
AAFTimelineMobSlot.all : AAFTimelineMobSlot.implc AAFTimelineMobSlot.implh
AAFTimelineMobSlot.exp : AAFUnitTest.exp AAFMobSlot.exp macros/exp.mac macros/base.mac
AAFTimelineMobSlot.h : AAFTimelineMobSlot.exp macros/h.mac macros/base.mac
AAFTimelineMobSlot.cpp : AAFTimelineMobSlot.exp macros/cpp.mac macros/base.mac
AAFTimelineMobSlot.idl : AAFTimelineMobSlot.exp macros/idl.mac macros/base.mac
AAFTimelineMobSlot.comc : AAFTimelineMobSlot.exp macros/comc.mac macros/base.mac
AAFTimelineMobSlot.comh : AAFTimelineMobSlot.exp macros/comh.mac macros/base.mac
AAFTimelineMobSlot.implc : AAFTimelineMobSlot.exp macros/implc.mac macros/base.mac
AAFTimelineMobSlot.implh : AAFTimelineMobSlot.exp macros/implh.mac macros/base.mac

AAFScopeReference.all : AAFScopeReference.cpp AAFScopeReference.h
AAFScopeReference.all : AAFScopeReference.comc AAFScopeReference.comh
AAFScopeReference.all : AAFScopeReference.exp AAFScopeReference.idl
AAFScopeReference.all : AAFScopeReference.implc AAFScopeReference.implh
AAFScopeReference.exp : AAFUnitTest.exp AAFSegment.exp macros/exp.mac macros/base.mac
AAFScopeReference.h : AAFScopeReference.exp macros/h.mac macros/base.mac
AAFScopeReference.cpp : AAFScopeReference.exp macros/cpp.mac macros/base.mac
AAFScopeReference.idl : AAFScopeReference.exp macros/idl.mac macros/base.mac
AAFScopeReference.comc : AAFScopeReference.exp macros/comc.mac macros/base.mac
AAFScopeReference.comh : AAFScopeReference.exp macros/comh.mac macros/base.mac
AAFScopeReference.implc : AAFScopeReference.exp macros/implc.mac macros/base.mac
AAFScopeReference.implh : AAFScopeReference.exp macros/implh.mac macros/base.mac

AAFObjectAttribute.all : AAFObjectAttribute.cpp AAFObjectAttribute.h
AAFObjectAttribute.all : AAFObjectAttribute.comc AAFObjectAttribute.comh
AAFObjectAttribute.all : AAFObjectAttribute.exp AAFObjectAttribute.idl
AAFObjectAttribute.all : AAFObjectAttribute.implc AAFObjectAttribute.implh
AAFObjectAttribute.exp : AAFUnitTest.exp macros/exp.mac macros/base.mac
AAFObjectAttribute.h : AAFObjectAttribute.exp macros/h.mac macros/base.mac
AAFObjectAttribute.cpp : AAFObjectAttribute.exp macros/cpp.mac macros/base.mac
AAFObjectAttribute.idl : AAFObjectAttribute.exp macros/idl.mac macros/base.mac
AAFObjectAttribute.comc : AAFObjectAttribute.exp macros/comc.mac macros/base.mac
AAFObjectAttribute.comh : AAFObjectAttribute.exp macros/comh.mac macros/base.mac
AAFObjectAttribute.implc : AAFObjectAttribute.exp macros/implc.mac macros/base.mac
AAFObjectAttribute.implh : AAFObjectAttribute.exp macros/implh.mac macros/base.mac

AAFParameter.all : AAFParameter.cpp AAFParameter.h
AAFParameter.all : AAFParameter.comc AAFParameter.comh
AAFParameter.all : AAFParameter.exp AAFParameter.idl
AAFParameter.all : AAFParameter.implc AAFParameter.implh
AAFParameter.exp : AAFUnitTest.exp macros/exp.mac macros/base.mac
AAFParameter.h : AAFParameter.exp macros/h.mac macros/base.mac
AAFParameter.cpp : AAFParameter.exp macros/cpp.mac macros/base.mac
AAFParameter.idl : AAFParameter.exp macros/idl.mac macros/base.mac
AAFParameter.comc : AAFParameter.exp macros/comc.mac macros/base.mac
AAFParameter.comh : AAFParameter.exp macros/comh.mac macros/base.mac
AAFParameter.implc : AAFParameter.exp macros/implc.mac macros/base.mac
AAFParameter.implh : AAFParameter.exp macros/implh.mac macros/base.mac

AAFParameterSlot.all : AAFParameterSlot.cpp AAFParameterSlot.h
AAFParameterSlot.all : AAFParameterSlot.comc AAFParameterSlot.comh
AAFParameterSlot.all : AAFParameterSlot.exp AAFParameterSlot.idl
AAFParameterSlot.all : AAFParameterSlot.implc AAFParameterSlot.implh
AAFParameterSlot.exp : AAFUnitTest.exp macros/exp.mac macros/base.mac
AAFParameterSlot.h : AAFParameterSlot.exp macros/h.mac macros/base.mac
AAFParameterSlot.cpp : AAFParameterSlot.exp macros/cpp.mac macros/base.mac
AAFParameterSlot.idl : AAFParameterSlot.exp macros/idl.mac macros/base.mac
AAFParameterSlot.comc : AAFParameterSlot.exp macros/comc.mac macros/base.mac
AAFParameterSlot.comh : AAFParameterSlot.exp macros/comh.mac macros/base.mac
AAFParameterSlot.implc : AAFParameterSlot.exp macros/implc.mac macros/base.mac
AAFParameterSlot.implh : AAFParameterSlot.exp macros/implh.mac macros/base.mac

AAFPropertyDefinition.all : AAFPropertyDefinition.cpp AAFPropertyDefinition.h
AAFPropertyDefinition.all : AAFPropertyDefinition.comc AAFPropertyDefinition.comh
AAFPropertyDefinition.all : AAFPropertyDefinition.exp AAFPropertyDefinition.idl
AAFPropertyDefinition.all : AAFPropertyDefinition.implc AAFPropertyDefinition.implh
AAFPropertyDefinition.exp : AAFUnitTest.exp AAFSegment.exp macros/exp.mac macros/base.mac
AAFPropertyDefinition.h : AAFPropertyDefinition.exp macros/h.mac macros/base.mac
AAFPropertyDefinition.cpp : AAFPropertyDefinition.exp macros/cpp.mac macros/base.mac
AAFPropertyDefinition.idl : AAFPropertyDefinition.exp macros/idl.mac macros/base.mac
AAFPropertyDefinition.comc : AAFPropertyDefinition.exp macros/comc.mac macros/base.mac
AAFPropertyDefinition.comh : AAFPropertyDefinition.exp macros/comh.mac macros/base.mac
AAFPropertyDefinition.implc : AAFPropertyDefinition.exp macros/implc.mac macros/base.mac
AAFPropertyDefinition.implh : AAFPropertyDefinition.exp macros/implh.mac macros/base.mac

AAFPulldown.all : AAFPulldown.cpp AAFPulldown.h
AAFPulldown.all : AAFPulldown.comc AAFPulldown.comh
AAFPulldown.all : AAFPulldown.exp AAFPulldown.idl
AAFPulldown.all : AAFPulldown.implc AAFPulldown.implh
AAFPulldown.exp : AAFUnitTest.exp AAFSegment.exp macros/exp.mac macros/base.mac
AAFPulldown.h : AAFPulldown.exp macros/h.mac macros/base.mac
AAFPulldown.cpp : AAFPulldown.exp macros/cpp.mac macros/base.mac
AAFPulldown.idl : AAFPulldown.exp macros/idl.mac macros/base.mac
AAFPulldown.comc : AAFPulldown.exp macros/comc.mac macros/base.mac
AAFPulldown.comh : AAFPulldown.exp macros/comh.mac macros/base.mac
AAFPulldown.implc : AAFPulldown.exp macros/implc.mac macros/base.mac
AAFPulldown.implh : AAFPulldown.exp macros/implh.mac macros/base.mac

AAFReferenceValue.all : AAFReferenceValue.cpp AAFReferenceValue.h
AAFReferenceValue.all : AAFReferenceValue.comc AAFReferenceValue.comh
AAFReferenceValue.all : AAFReferenceValue.exp AAFReferenceValue.idl
AAFReferenceValue.all : AAFReferenceValue.implc AAFReferenceValue.implh
AAFReferenceValue.exp : AAFUnitTest.exp macros/exp.mac macros/base.mac
AAFReferenceValue.h : AAFReferenceValue.exp macros/h.mac macros/base.mac
AAFReferenceValue.cpp : AAFReferenceValue.exp macros/cpp.mac macros/base.mac
AAFReferenceValue.idl : AAFReferenceValue.exp macros/idl.mac macros/base.mac
AAFReferenceValue.comc : AAFReferenceValue.exp macros/comc.mac macros/base.mac
AAFReferenceValue.comh : AAFReferenceValue.exp macros/comh.mac macros/base.mac
AAFReferenceValue.implc : AAFReferenceValue.exp macros/implc.mac macros/base.mac
AAFReferenceValue.implh : AAFReferenceValue.exp macros/implh.mac macros/base.mac

AAFSegment.all : AAFSegment.cpp AAFSegment.h
AAFSegment.all : AAFSegment.comc AAFSegment.comh
AAFSegment.all : AAFSegment.exp AAFSegment.idl
AAFSegment.all : AAFSegment.implc AAFSegment.implh
AAFSegment.exp : AAFUnitTest.exp AAFComponent.exp macros/exp.mac macros/base.mac
AAFSegment.h : AAFSegment.exp macros/h.mac macros/base.mac
AAFSegment.cpp : AAFSegment.exp macros/cpp.mac macros/base.mac
AAFSegment.idl : AAFSegment.exp macros/idl.mac macros/base.mac
AAFSegment.comc : AAFSegment.exp macros/comc.mac macros/base.mac
AAFSegment.comh : AAFSegment.exp macros/comh.mac macros/base.mac
AAFSegment.implc : AAFSegment.exp macros/implc.mac macros/base.mac
AAFSegment.implh : AAFSegment.exp macros/implh.mac macros/base.mac

AAFSequence.all : AAFSequence.cpp AAFSequence.h
AAFSequence.all : AAFSequence.comc AAFSequence.comh
AAFSequence.all : AAFSequence.exp AAFSequence.idl
AAFSequence.all : AAFSequence.implc AAFSequence.implh
AAFSequence.exp : AAFUnitTest.exp AAFSegment.exp macros/exp.mac macros/base.mac
AAFSequence.h : AAFSequence.exp macros/h.mac macros/base.mac
AAFSequence.cpp : AAFSequence.exp macros/cpp.mac macros/base.mac
AAFSequence.idl : AAFSequence.exp macros/idl.mac macros/base.mac
AAFSequence.comc : AAFSequence.exp macros/comc.mac macros/base.mac
AAFSequence.comh : AAFSequence.exp macros/comh.mac macros/base.mac
AAFSequence.implc : AAFSequence.exp macros/implc.mac macros/base.mac
AAFSequence.implh : AAFSequence.exp macros/implh.mac macros/base.mac

AAFSourceClip.all : AAFSourceClip.cpp AAFSourceClip.h
AAFSourceClip.all : AAFSourceClip.comc AAFSourceClip.comh
AAFSourceClip.all : AAFSourceClip.exp AAFSourceClip.idl
AAFSourceClip.all : AAFSourceClip.implc AAFSourceClip.implh
AAFSourceClip.exp : AAFUnitTest.exp AAFSourceReference.exp macros/exp.mac macros/base.mac
AAFSourceClip.h : AAFSourceClip.exp macros/h.mac macros/base.mac
AAFSourceClip.cpp : AAFSourceClip.exp macros/cpp.mac macros/base.mac
AAFSourceClip.idl : AAFSourceClip.exp macros/idl.mac macros/base.mac
AAFSourceClip.comc : AAFSourceClip.exp macros/comc.mac macros/base.mac
AAFSourceClip.comh : AAFSourceClip.exp macros/comh.mac macros/base.mac
AAFSourceClip.implc : AAFSourceClip.exp macros/implc.mac macros/base.mac
AAFSourceClip.implh : AAFSourceClip.exp macros/implh.mac macros/base.mac

AAFSourceReference.all : AAFSourceReference.cpp AAFSourceReference.h
AAFSourceReference.all : AAFSourceReference.comc AAFSourceReference.comh
AAFSourceReference.all : AAFSourceReference.exp AAFSourceReference.idl
AAFSourceReference.all : AAFSourceReference.implc AAFSourceReference.implh
AAFSourceReference.exp : AAFSourceReference.dod AAFUnitTest.exp macros/exp.mac macros/base.mac
AAFSourceReference.h : AAFSourceReference.exp AAFSegment.exp macros/h.mac macros/base.mac
AAFSourceReference.cpp : AAFSourceReference.exp macros/cpp.mac macros/base.mac
AAFSourceReference.idl : AAFSourceReference.exp macros/idl.mac macros/base.mac
AAFSourceReference.comc : AAFSourceReference.exp macros/comc.mac macros/base.mac
AAFSourceReference.comh : AAFSourceReference.exp macros/comh.mac macros/base.mac
AAFSourceReference.implc : AAFSourceReference.exp macros/implc.mac macros/base.mac
AAFSourceReference.implh : AAFSourceReference.exp macros/implh.mac macros/base.mac

AAFString.all : AAFString.cpp AAFString.h
AAFString.all : AAFString.comc AAFString.comh
AAFString.all : AAFString.exp AAFString.idl
AAFString.all : AAFString.implc AAFString.implh
AAFString.exp : AAFUnitTest.exp macros/exp.mac macros/base.mac
AAFString.h : AAFString.exp macros/h.mac macros/base.mac
AAFString.cpp : AAFString.exp macros/cpp.mac macros/base.mac
AAFString.idl : AAFString.exp macros/idl.mac macros/base.mac
AAFString.comc : AAFString.exp macros/comc.mac macros/base.mac
AAFString.comh : AAFString.exp macros/comh.mac macros/base.mac
AAFString.implc : AAFString.exp macros/implc.mac macros/base.mac
AAFString.implh : AAFString.exp macros/implh.mac macros/base.mac

AAFStringAttribute.all : AAFStringAttribute.cpp AAFStringAttribute.h
AAFStringAttribute.all : AAFStringAttribute.comc AAFStringAttribute.comh
AAFStringAttribute.all : AAFStringAttribute.exp AAFStringAttribute.idl
AAFStringAttribute.all : AAFStringAttribute.implc AAFStringAttribute.implh
AAFStringAttribute.exp : AAFUnitTest.exp macros/exp.mac macros/base.mac
AAFStringAttribute.h : AAFStringAttribute.exp macros/h.mac macros/base.mac
AAFStringAttribute.cpp : AAFStringAttribute.exp macros/cpp.mac macros/base.mac
AAFStringAttribute.idl : AAFStringAttribute.exp macros/idl.mac macros/base.mac
AAFStringAttribute.comc : AAFStringAttribute.exp macros/comc.mac macros/base.mac
AAFStringAttribute.comh : AAFStringAttribute.exp macros/comh.mac macros/base.mac
AAFStringAttribute.implc : AAFStringAttribute.exp macros/implc.mac macros/base.mac
AAFStringAttribute.implh : AAFStringAttribute.exp macros/implh.mac macros/base.mac

AAFTimecode.all : AAFTimecode.cpp AAFTimecode.h
AAFTimecode.all : AAFTimecode.comc AAFTimecode.comh
AAFTimecode.all : AAFTimecode.exp AAFTimecode.idl
AAFTimecode.all : AAFTimecode.implc AAFTimecode.implh
AAFTimecode.exp : AAFUnitTest.exp AAFSegment.exp macros/exp.mac macros/base.mac
AAFTimecode.h : AAFTimecode.exp macros/h.mac macros/base.mac
AAFTimecode.cpp : AAFTimecode.exp macros/cpp.mac macros/base.mac
AAFTimecode.idl : AAFTimecode.exp macros/idl.mac macros/base.mac
AAFTimecode.comc : AAFTimecode.exp macros/comc.mac macros/base.mac
AAFTimecode.comh : AAFTimecode.exp macros/comh.mac macros/base.mac
AAFTimecode.implc : AAFTimecode.exp macros/implc.mac macros/base.mac
AAFTimecode.implh : AAFTimecode.exp macros/implh.mac macros/base.mac

AAFTimecodeStream.all : AAFTimecodeStream.cpp AAFTimecodeStream.h
AAFTimecodeStream.all : AAFTimecodeStream.comc AAFTimecodeStream.comh
AAFTimecodeStream.all : AAFTimecodeStream.exp AAFTimecodeStream.idl
AAFTimecodeStream.all : AAFTimecodeStream.implc AAFTimecodeStream.implh
AAFTimecodeStream.exp : AAFUnitTest.exp AAFSegment.exp macros/exp.mac macros/base.mac
AAFTimecodeStream.h : AAFTimecodeStream.exp macros/h.mac macros/base.mac
AAFTimecodeStream.cpp : AAFTimecodeStream.exp macros/cpp.mac macros/base.mac
AAFTimecodeStream.idl : AAFTimecodeStream.exp macros/idl.mac macros/base.mac
AAFTimecodeStream.comc : AAFTimecodeStream.exp macros/comc.mac macros/base.mac
AAFTimecodeStream.comh : AAFTimecodeStream.exp macros/comh.mac macros/base.mac
AAFTimecodeStream.implc : AAFTimecodeStream.exp macros/implc.mac macros/base.mac
AAFTimecodeStream.implh : AAFTimecodeStream.exp macros/implh.mac macros/base.mac

AAFTimecodeStream12M.all : AAFTimecodeStream12M.cpp AAFTimecodeStream12M.h
AAFTimecodeStream12M.all : AAFTimecodeStream12M.comc AAFTimecodeStream12M.comh
AAFTimecodeStream12M.all : AAFTimecodeStream12M.exp AAFTimecodeStream12M.idl
AAFTimecodeStream12M.all : AAFTimecodeStream12M.implc AAFTimecodeStream12M.implh
AAFTimecodeStream12M.exp : AAFUnitTest.exp AAFTimecodeStream.exp macros/exp.mac macros/base.mac
AAFTimecodeStream12M.h : AAFTimecodeStream12M.exp macros/h.mac macros/base.mac
AAFTimecodeStream12M.cpp : AAFTimecodeStream12M.exp macros/cpp.mac macros/base.mac
AAFTimecodeStream12M.idl : AAFTimecodeStream12M.exp macros/idl.mac macros/base.mac
AAFTimecodeStream12M.comc : AAFTimecodeStream12M.exp macros/comc.mac macros/base.mac
AAFTimecodeStream12M.comh : AAFTimecodeStream12M.exp macros/comh.mac macros/base.mac
AAFTimecodeStream12M.implc : AAFTimecodeStream12M.exp macros/implc.mac macros/base.mac
AAFTimecodeStream12M.implh : AAFTimecodeStream12M.exp macros/implh.mac macros/base.mac

AAFTransition.all : AAFTransition.cpp AAFTransition.h
AAFTransition.all : AAFTransition.comc AAFTransition.comh
AAFTransition.all : AAFTransition.exp AAFTransition.idl
AAFTransition.all : AAFTransition.implc AAFTransition.implh
AAFTransition.exp : AAFUnitTest.exp AAFTimecodeStream.exp macros/exp.mac macros/base.mac
AAFTransition.h : AAFTransition.exp macros/h.mac macros/base.mac
AAFTransition.cpp : AAFTransition.exp macros/cpp.mac macros/base.mac
AAFTransition.idl : AAFTransition.exp macros/idl.mac macros/base.mac
AAFTransition.comc : AAFTransition.exp macros/comc.mac macros/base.mac
AAFTransition.comh : AAFTransition.exp macros/comh.mac macros/base.mac
AAFTransition.implc : AAFTransition.exp macros/implc.mac macros/base.mac
AAFTransition.implh : AAFTransition.exp macros/implh.mac macros/base.mac

AAFTypeDefinition.all : AAFTypeDefinition.cpp AAFTypeDefinition.h
AAFTypeDefinition.all : AAFTypeDefinition.comc AAFTypeDefinition.comh
AAFTypeDefinition.all : AAFTypeDefinition.exp AAFTypeDefinition.idl
AAFTypeDefinition.all : AAFTypeDefinition.implc AAFTypeDefinition.implh
AAFTypeDefinition.exp : AAFUnitTest.exp AAFSegment.exp macros/exp.mac macros/base.mac
AAFTypeDefinition.h : AAFTypeDefinition.exp macros/h.mac macros/base.mac
AAFTypeDefinition.cpp : AAFTypeDefinition.exp macros/cpp.mac macros/base.mac
AAFTypeDefinition.idl : AAFTypeDefinition.exp macros/idl.mac macros/base.mac
AAFTypeDefinition.comc : AAFTypeDefinition.exp macros/comc.mac macros/base.mac
AAFTypeDefinition.comh : AAFTypeDefinition.exp macros/comh.mac macros/base.mac
AAFTypeDefinition.implc : AAFTypeDefinition.exp macros/implc.mac macros/base.mac
AAFTypeDefinition.implh : AAFTypeDefinition.exp macros/implh.mac macros/base.mac

AAFUInt64.all : AAFUInt64.cpp AAFUInt64.h
AAFUInt64.all : AAFUInt64.comc AAFUInt64.comh
AAFUInt64.all : AAFUInt64.exp AAFUInt64.idl
AAFUInt64.all : AAFUInt64.implc AAFUInt64.implh
AAFUInt64.exp : AAFUnitTest.exp macros/exp.mac macros/base.mac
AAFUInt64.h : AAFUInt64.exp macros/h.mac macros/base.mac
AAFUInt64.cpp : AAFUInt64.exp macros/cpp.mac macros/base.mac
AAFUInt64.idl : AAFUInt64.exp macros/idl.mac macros/base.mac
AAFUInt64.comc : AAFUInt64.exp macros/comc.mac macros/base.mac
AAFUInt64.comh : AAFUInt64.exp macros/comh.mac macros/base.mac
AAFUInt64.implc : AAFUInt64.exp macros/implc.mac macros/base.mac
AAFUInt64.implh : AAFUInt64.exp macros/implh.mac macros/base.mac

AAFUnitTest.all : AAFUnitTest.cpp AAFUnitTest.h
AAFUnitTest.all : AAFUnitTest.comc AAFUnitTest.comh
AAFUnitTest.all : AAFUnitTest.exp AAFUnitTest.idl
AAFUnitTest.all : AAFUnitTest.implc AAFUnitTest.implh
AAFUnitTest.exp : AAFUnitTest.dod macros/exp.mac macros/base.mac
AAFUnitTest.h : macros/h.mac macros/base.mac
AAFUnitTest.cpp : AAFUnitTest.exp macros/cpp.mac macros/base.mac
AAFUnitTest.idl : AAFUnitTest.exp macros/idl.mac macros/base.mac
AAFUnitTest.comc : AAFUnitTest.exp macros/comc.mac macros/base.mac
AAFUnitTest.comh : AAFUnitTest.exp macros/comh.mac macros/base.mac
AAFUnitTest.implc : AAFUnitTest.exp macros/implc.mac macros/base.mac
AAFUnitTest.implh : AAFUnitTest.exp macros/implh.mac macros/base.mac

AAFUnixLocator.all : AAFUnixLocator.cpp AAFUnixLocator.h
AAFUnixLocator.all : AAFUnixLocator.comc AAFUnixLocator.comh
AAFUnixLocator.all : AAFUnixLocator.exp AAFUnixLocator.idl
AAFUnixLocator.all : AAFUnixLocator.implc AAFUnixLocator.implh
AAFUnixLocator.exp : AAFUnitTest.exp AAFLocator.exp macros/exp.mac macros/base.mac
AAFUnixLocator.h : AAFUnixLocator.exp macros/h.mac macros/base.mac
AAFUnixLocator.cpp : AAFUnixLocator.exp macros/cpp.mac macros/base.mac
AAFUnixLocator.idl : AAFUnixLocator.exp macros/idl.mac macros/base.mac
AAFUnixLocator.comc : AAFUnixLocator.exp macros/comc.mac macros/base.mac
AAFUnixLocator.comh : AAFUnixLocator.exp macros/comh.mac macros/base.mac
AAFUnixLocator.implc : AAFUnixLocator.exp macros/implc.mac macros/base.mac
AAFUnixLocator.implh : AAFUnixLocator.exp macros/implh.mac macros/base.mac

AAFVaryingValue.all : AAFVaryingValue.cpp AAFVaryingValue.h
AAFVaryingValue.all : AAFVaryingValue.comc AAFVaryingValue.comh
AAFVaryingValue.all : AAFVaryingValue.exp AAFVaryingValue.idl
AAFVaryingValue.all : AAFVaryingValue.implc AAFVaryingValue.implh
AAFVaryingValue.exp : AAFParameter.exp macros/exp.mac macros/base.mac
AAFVaryingValue.h : AAFVaryingValue.exp macros/h.mac macros/base.mac
AAFVaryingValue.cpp : AAFVaryingValue.exp macros/cpp.mac macros/base.mac
AAFVaryingValue.idl : AAFVaryingValue.exp macros/idl.mac macros/base.mac
AAFVaryingValue.comc : AAFVaryingValue.exp macros/comc.mac macros/base.mac
AAFVaryingValue.comh : AAFVaryingValue.exp macros/comh.mac macros/base.mac
AAFVaryingValue.implc : AAFVaryingValue.exp macros/implc.mac macros/base.mac
AAFVaryingValue.implh : AAFVaryingValue.exp macros/implh.mac macros/base.mac

AAFWindowsLocator.all : AAFWindowsLocator.cpp AAFWindowsLocator.h
AAFWindowsLocator.all : AAFWindowsLocator.comc AAFWindowsLocator.comh
AAFWindowsLocator.all : AAFWindowsLocator.exp AAFWindowsLocator.idl
AAFWindowsLocator.all : AAFWindowsLocator.implc AAFWindowsLocator.implh
AAFWindowsLocator.exp : AAFUnitTest.exp AAFLocator.exp macros/exp.mac macros/base.mac
AAFWindowsLocator.h : AAFWindowsLocator.exp macros/h.mac macros/base.mac
AAFWindowsLocator.cpp : AAFWindowsLocator.exp macros/cpp.mac macros/base.mac
AAFWindowsLocator.idl : AAFWindowsLocator.exp macros/idl.mac macros/base.mac
AAFWindowsLocator.comc : AAFWindowsLocator.exp macros/comc.mac macros/base.mac
AAFWindowsLocator.comh : AAFWindowsLocator.exp macros/comh.mac macros/base.mac
AAFWindowsLocator.implc : AAFWindowsLocator.exp macros/implc.mac macros/base.mac
AAFWindowsLocator.implh : AAFWindowsLocator.exp macros/implh.mac macros/base.mac

AAFHeader.all : \
	AAFObject.all \
	AAFIdentification.all EnumAAFIdentifications.all \
	AAFDictionary.all EnumAAFMedia.all AAFMob.all EnumAAFMobs.all
AAFHeader.all : AAFHeader.cpp AAFHeader.h
AAFHeader.all : AAFHeader.comc AAFHeader.comh
AAFHeader.all : AAFHeader.exp AAFHeader.idl
AAFHeader.all : AAFHeader.implc AAFHeader.implh
AAFHeader.exp : AAFUnitTest.exp AAFLocator.exp macros/exp.mac macros/base.mac
AAFHeader.h : AAFHeader.exp macros/h.mac macros/base.mac
AAFHeader.cpp : AAFHeader.h AAFHeader.exp macros/cpp.mac macros/base.mac
AAFHeader.idl : AAFHeader.exp macros/idl.mac macros/base.mac
AAFHeader.comc : AAFHeader.comh AAFHeader.exp macros/comc.mac macros/base.mac
AAFHeader.comh : AAFHeader.exp macros/comh.mac macros/base.mac
AAFHeader.implc : AAFHeader.implh AAFHeader.exp macros/implc.mac macros/base.mac
AAFHeader.implh : AAFHeader.exp macros/implh.mac macros/base.mac

AAFIdentification.all : AAFIdentification.cpp AAFIdentification.h
AAFIdentification.all : AAFIdentification.comc AAFIdentification.comh
AAFIdentification.all : AAFIdentification.exp AAFIdentification.idl
AAFIdentification.all : AAFIdentification.implc AAFIdentification.implh
AAFIdentification.exp : AAFUnitTest.exp AAFObject.exp macros/exp.mac macros/base.mac
AAFIdentification.h : AAFIdentification.exp macros/h.mac macros/base.mac
AAFIdentification.cpp : AAFIdentification.exp macros/cpp.mac macros/base.mac
AAFIdentification.idl : AAFIdentification.exp macros/idl.mac macros/base.mac
AAFIdentification.comc : AAFIdentification.exp macros/comc.mac macros/base.mac
AAFIdentification.comh : AAFIdentification.exp macros/comh.mac macros/base.mac
AAFIdentification.implc : AAFIdentification.exp macros/implc.mac macros/base.mac
AAFIdentification.implh : AAFIdentification.exp macros/implh.mac macros/base.mac

EnumAAFAttributes.all : EnumAAFAttributes.cpp EnumAAFAttributes.h
EnumAAFAttributes.all : EnumAAFAttributes.comc EnumAAFAttributes.comh
EnumAAFAttributes.all : EnumAAFAttributes.exp EnumAAFAttributes.idl
EnumAAFAttributes.all : EnumAAFAttributes.implc EnumAAFAttributes.implh
EnumAAFAttributes.exp : AAFUnitTest.exp macros/exp.mac macros/base.mac
EnumAAFAttributes.h : EnumAAFAttributes.exp macros/h.mac macros/base.mac
EnumAAFAttributes.cpp : EnumAAFAttributes.exp macros/cpp.mac macros/base.mac
EnumAAFAttributes.idl : EnumAAFAttributes.exp macros/idl.mac macros/base.mac
EnumAAFAttributes.comc : EnumAAFAttributes.exp macros/comc.mac macros/base.mac
EnumAAFAttributes.comh : EnumAAFAttributes.exp macros/comh.mac macros/base.mac
EnumAAFAttributes.implc : EnumAAFAttributes.exp macros/implc.mac macros/base.mac
EnumAAFAttributes.implh : EnumAAFAttributes.exp macros/implh.mac macros/base.mac

EnumAAFClassDefinitions.all : EnumAAFClassDefinitions.cpp EnumAAFClassDefinitions.h
EnumAAFClassDefinitions.all : EnumAAFClassDefinitions.comc EnumAAFClassDefinitions.comh
EnumAAFClassDefinitions.all : EnumAAFClassDefinitions.exp EnumAAFClassDefinitions.idl
EnumAAFClassDefinitions.all : EnumAAFClassDefinitions.implc EnumAAFClassDefinitions.implh
EnumAAFClassDefinitions.exp : AAFUnitTest.exp macros/exp.mac macros/base.mac
EnumAAFClassDefinitions.h : EnumAAFClassDefinitions.exp macros/h.mac macros/base.mac
EnumAAFClassDefinitions.cpp : EnumAAFClassDefinitions.exp macros/cpp.mac macros/base.mac
EnumAAFClassDefinitions.idl : EnumAAFClassDefinitions.exp macros/idl.mac macros/base.mac
EnumAAFClassDefinitions.comc : EnumAAFClassDefinitions.exp macros/comc.mac macros/base.mac
EnumAAFClassDefinitions.comh : EnumAAFClassDefinitions.exp macros/comh.mac macros/base.mac
EnumAAFClassDefinitions.implc : EnumAAFClassDefinitions.exp macros/implc.mac macros/base.mac
EnumAAFClassDefinitions.implh : EnumAAFClassDefinitions.exp macros/implh.mac macros/base.mac

EnumAAFComponents.all : EnumAAFComponents.cpp EnumAAFComponents.h
EnumAAFComponents.all : EnumAAFComponents.comc EnumAAFComponents.comh
EnumAAFComponents.all : EnumAAFComponents.exp EnumAAFComponents.idl
EnumAAFComponents.all : EnumAAFComponents.implc EnumAAFComponents.implh
EnumAAFComponents.exp : AAFUnitTest.exp macros/exp.mac macros/base.mac
EnumAAFComponents.h : EnumAAFComponents.exp macros/h.mac macros/base.mac
EnumAAFComponents.cpp : EnumAAFComponents.exp macros/cpp.mac macros/base.mac
EnumAAFComponents.idl : EnumAAFComponents.exp macros/idl.mac macros/base.mac
EnumAAFComponents.comc : EnumAAFComponents.exp macros/comc.mac macros/base.mac
EnumAAFComponents.comh : EnumAAFComponents.exp macros/comh.mac macros/base.mac
EnumAAFComponents.implc : EnumAAFComponents.exp macros/implc.mac macros/base.mac
EnumAAFComponents.implh : EnumAAFComponents.exp macros/implh.mac macros/base.mac

EnumAAFControlCodeDefinitions.all : EnumAAFControlCodeDefinitions.cpp EnumAAFControlCodeDefinitions.h
EnumAAFControlCodeDefinitions.all : EnumAAFControlCodeDefinitions.comc EnumAAFControlCodeDefinitions.comh
EnumAAFControlCodeDefinitions.all : EnumAAFControlCodeDefinitions.exp EnumAAFControlCodeDefinitions.idl
EnumAAFControlCodeDefinitions.all : EnumAAFControlCodeDefinitions.implc EnumAAFControlCodeDefinitions.implh
EnumAAFControlCodeDefinitions.exp : AAFUnitTest.exp macros/exp.mac macros/base.mac
EnumAAFControlCodeDefinitions.h : EnumAAFControlCodeDefinitions.exp macros/h.mac macros/base.mac
EnumAAFControlCodeDefinitions.cpp : EnumAAFControlCodeDefinitions.exp macros/cpp.mac macros/base.mac
EnumAAFControlCodeDefinitions.idl : EnumAAFControlCodeDefinitions.exp macros/idl.mac macros/base.mac
EnumAAFControlCodeDefinitions.comc : EnumAAFControlCodeDefinitions.exp macros/comc.mac macros/base.mac
EnumAAFControlCodeDefinitions.comh : EnumAAFControlCodeDefinitions.exp macros/comh.mac macros/base.mac
EnumAAFControlCodeDefinitions.implc : EnumAAFControlCodeDefinitions.exp macros/implc.mac macros/base.mac
EnumAAFControlCodeDefinitions.implh : EnumAAFControlCodeDefinitions.exp macros/implh.mac macros/base.mac

EnumAAFControlPoints.all : EnumAAFControlPoints.cpp EnumAAFControlPoints.h
EnumAAFControlPoints.all : EnumAAFControlPoints.comc EnumAAFControlPoints.comh
EnumAAFControlPoints.all : EnumAAFControlPoints.exp EnumAAFControlPoints.idl
EnumAAFControlPoints.all : EnumAAFControlPoints.implc EnumAAFControlPoints.implh
EnumAAFControlPoints.exp : AAFUnitTest.exp macros/exp.mac macros/base.mac
EnumAAFControlPoints.h : EnumAAFControlPoints.exp macros/h.mac macros/base.mac
EnumAAFControlPoints.cpp : EnumAAFControlPoints.exp macros/cpp.mac macros/base.mac
EnumAAFControlPoints.idl : EnumAAFControlPoints.exp macros/idl.mac macros/base.mac
EnumAAFControlPoints.comc : EnumAAFControlPoints.exp macros/comc.mac macros/base.mac
EnumAAFControlPoints.comh : EnumAAFControlPoints.exp macros/comh.mac macros/base.mac
EnumAAFControlPoints.implc : EnumAAFControlPoints.exp macros/implc.mac macros/base.mac
EnumAAFControlPoints.implh : EnumAAFControlPoints.exp macros/implh.mac macros/base.mac

EnumAAFDataDefinitions.all : EnumAAFDataDefinitions.cpp EnumAAFDataDefinitions.h
EnumAAFDataDefinitions.all : EnumAAFDataDefinitions.comc EnumAAFDataDefinitions.comh
EnumAAFDataDefinitions.all : EnumAAFDataDefinitions.exp EnumAAFDataDefinitions.idl
EnumAAFDataDefinitions.all : EnumAAFDataDefinitions.implc EnumAAFDataDefinitions.implh
EnumAAFDataDefinitions.exp : AAFUnitTest.exp macros/exp.mac macros/base.mac
EnumAAFDataDefinitions.h : EnumAAFDataDefinitions.exp macros/h.mac macros/base.mac
EnumAAFDataDefinitions.cpp : EnumAAFDataDefinitions.exp macros/cpp.mac macros/base.mac
EnumAAFDataDefinitions.idl : EnumAAFDataDefinitions.exp macros/idl.mac macros/base.mac
EnumAAFDataDefinitions.comc : EnumAAFDataDefinitions.exp macros/comc.mac macros/base.mac
EnumAAFDataDefinitions.comh : EnumAAFDataDefinitions.exp macros/comh.mac macros/base.mac
EnumAAFDataDefinitions.implc : EnumAAFDataDefinitions.exp macros/implc.mac macros/base.mac
EnumAAFDataDefinitions.implh : EnumAAFDataDefinitions.exp macros/implh.mac macros/base.mac

EnumAAFDefinitions.all : EnumAAFDefinitions.cpp EnumAAFDefinitions.h
EnumAAFDefinitions.all : EnumAAFDefinitions.comc EnumAAFDefinitions.comh
EnumAAFDefinitions.all : EnumAAFDefinitions.exp EnumAAFDefinitions.idl
EnumAAFDefinitions.all : EnumAAFDefinitions.implc EnumAAFDefinitions.implh
EnumAAFDefinitions.exp : AAFUnitTest.exp macros/exp.mac macros/base.mac
EnumAAFDefinitions.h : EnumAAFDefinitions.exp macros/h.mac macros/base.mac
EnumAAFDefinitions.cpp : EnumAAFDefinitions.exp macros/cpp.mac macros/base.mac
EnumAAFDefinitions.idl : EnumAAFDefinitions.exp macros/idl.mac macros/base.mac
EnumAAFDefinitions.comc : EnumAAFDefinitions.exp macros/comc.mac macros/base.mac
EnumAAFDefinitions.comh : EnumAAFDefinitions.exp macros/comh.mac macros/base.mac
EnumAAFDefinitions.implc : EnumAAFDefinitions.exp macros/implc.mac macros/base.mac
EnumAAFDefinitions.implh : EnumAAFDefinitions.exp macros/implh.mac macros/base.mac

EnumAAFEffectDefinitions.all : EnumAAFEffectDefinitions.cpp EnumAAFEffectDefinitions.h
EnumAAFEffectDefinitions.all : EnumAAFEffectDefinitions.comc EnumAAFEffectDefinitions.comh
EnumAAFEffectDefinitions.all : EnumAAFEffectDefinitions.exp EnumAAFEffectDefinitions.idl
EnumAAFEffectDefinitions.all : EnumAAFEffectDefinitions.implc EnumAAFEffectDefinitions.implh
EnumAAFEffectDefinitions.exp : AAFUnitTest.exp macros/exp.mac macros/base.mac
EnumAAFEffectDefinitions.h : EnumAAFEffectDefinitions.exp macros/h.mac macros/base.mac
EnumAAFEffectDefinitions.cpp : EnumAAFEffectDefinitions.exp macros/cpp.mac macros/base.mac
EnumAAFEffectDefinitions.idl : EnumAAFEffectDefinitions.exp macros/idl.mac macros/base.mac
EnumAAFEffectDefinitions.comc : EnumAAFEffectDefinitions.exp macros/comc.mac macros/base.mac
EnumAAFEffectDefinitions.comh : EnumAAFEffectDefinitions.exp macros/comh.mac macros/base.mac
EnumAAFEffectDefinitions.implc : EnumAAFEffectDefinitions.exp macros/implc.mac macros/base.mac
EnumAAFEffectDefinitions.implh : EnumAAFEffectDefinitions.exp macros/implh.mac macros/base.mac

EnumAAFIdentifications.all : EnumAAFIdentifications.cpp EnumAAFIdentifications.h
EnumAAFIdentifications.all : EnumAAFIdentifications.comc EnumAAFIdentifications.comh
EnumAAFIdentifications.all : EnumAAFIdentifications.exp EnumAAFIdentifications.idl
EnumAAFIdentifications.all : EnumAAFIdentifications.implc EnumAAFIdentifications.implh
EnumAAFIdentifications.exp : AAFUnitTest.exp macros/exp.mac macros/base.mac
EnumAAFIdentifications.h : EnumAAFIdentifications.exp macros/h.mac macros/base.mac
EnumAAFIdentifications.cpp : EnumAAFIdentifications.exp macros/cpp.mac macros/base.mac
EnumAAFIdentifications.idl : EnumAAFIdentifications.exp macros/idl.mac macros/base.mac
EnumAAFIdentifications.comc : EnumAAFIdentifications.exp macros/comc.mac macros/base.mac
EnumAAFIdentifications.comh : EnumAAFIdentifications.exp macros/comh.mac macros/base.mac
EnumAAFIdentifications.implc : EnumAAFIdentifications.exp macros/implc.mac macros/base.mac
EnumAAFIdentifications.implh : EnumAAFIdentifications.exp macros/implh.mac macros/base.mac

EnumAAFLocators.all : EnumAAFLocators.cpp EnumAAFLocators.h
EnumAAFLocators.all : EnumAAFLocators.comc EnumAAFLocators.comh
EnumAAFLocators.all : EnumAAFLocators.exp EnumAAFLocators.idl
EnumAAFLocators.all : EnumAAFLocators.implc EnumAAFLocators.implh
EnumAAFLocators.exp : AAFUnitTest.exp macros/exp.mac macros/base.mac
EnumAAFLocators.h : EnumAAFLocators.exp macros/h.mac macros/base.mac
EnumAAFLocators.cpp : EnumAAFLocators.exp macros/cpp.mac macros/base.mac
EnumAAFLocators.idl : EnumAAFLocators.exp macros/idl.mac macros/base.mac
EnumAAFLocators.comc : EnumAAFLocators.exp macros/comc.mac macros/base.mac
EnumAAFLocators.comh : EnumAAFLocators.exp macros/comh.mac macros/base.mac
EnumAAFLocators.implc : EnumAAFLocators.exp macros/implc.mac macros/base.mac
EnumAAFLocators.implh : EnumAAFLocators.exp macros/implh.mac macros/base.mac

EnumAAFMedia.all : EnumAAFMedia.cpp EnumAAFMedia.h
EnumAAFMedia.all : EnumAAFMedia.comc EnumAAFMedia.comh
EnumAAFMedia.all : EnumAAFMedia.exp EnumAAFMedia.idl
EnumAAFMedia.all : EnumAAFMedia.implc EnumAAFMedia.implh
EnumAAFMedia.exp : AAFUnitTest.exp macros/exp.mac macros/base.mac
EnumAAFMedia.h : EnumAAFMedia.exp macros/h.mac macros/base.mac
EnumAAFMedia.cpp : EnumAAFMedia.exp macros/cpp.mac macros/base.mac
EnumAAFMedia.idl : EnumAAFMedia.exp macros/idl.mac macros/base.mac
EnumAAFMedia.comc : EnumAAFMedia.exp macros/comc.mac macros/base.mac
EnumAAFMedia.comh : EnumAAFMedia.exp macros/comh.mac macros/base.mac
EnumAAFMedia.implc : EnumAAFMedia.exp macros/implc.mac macros/base.mac
EnumAAFMedia.implh : EnumAAFMedia.exp macros/implh.mac macros/base.mac

EnumAAFMobComments.all : EnumAAFMobComments.cpp EnumAAFMobComments.h
EnumAAFMobComments.all : EnumAAFMobComments.comc EnumAAFMobComments.comh
EnumAAFMobComments.all : EnumAAFMobComments.exp EnumAAFMobComments.idl
EnumAAFMobComments.all : EnumAAFMobComments.implc EnumAAFMobComments.implh
EnumAAFMobComments.exp : AAFUnitTest.exp macros/exp.mac macros/base.mac
EnumAAFMobComments.h : EnumAAFMobComments.exp macros/h.mac macros/base.mac
EnumAAFMobComments.cpp : EnumAAFMobComments.exp macros/cpp.mac macros/base.mac
EnumAAFMobComments.idl : EnumAAFMobComments.exp macros/idl.mac macros/base.mac
EnumAAFMobComments.comc : EnumAAFMobComments.exp macros/comc.mac macros/base.mac
EnumAAFMobComments.comh : EnumAAFMobComments.exp macros/comh.mac macros/base.mac
EnumAAFMobComments.implc : EnumAAFMobComments.exp macros/implc.mac macros/base.mac
EnumAAFMobComments.implh : EnumAAFMobComments.exp macros/implh.mac macros/base.mac

EnumAAFMobs.all : EnumAAFMobs.cpp EnumAAFMobs.h
EnumAAFMobs.all : EnumAAFMobs.comc EnumAAFMobs.comh
EnumAAFMobs.all : EnumAAFMobs.exp EnumAAFMobs.idl
EnumAAFMobs.all : EnumAAFMobs.implc EnumAAFMobs.implh
EnumAAFMobs.exp : AAFUnitTest.exp macros/exp.mac macros/base.mac
EnumAAFMobs.h : EnumAAFMobs.exp macros/h.mac macros/base.mac
EnumAAFMobs.cpp : EnumAAFMobs.exp macros/cpp.mac macros/base.mac
EnumAAFMobs.idl : EnumAAFMobs.exp macros/idl.mac macros/base.mac
EnumAAFMobs.comc : EnumAAFMobs.exp macros/comc.mac macros/base.mac
EnumAAFMobs.comh : EnumAAFMobs.exp macros/comh.mac macros/base.mac
EnumAAFMobs.implc : EnumAAFMobs.exp macros/implc.mac macros/base.mac
EnumAAFMobs.implh : EnumAAFMobs.exp macros/implh.mac macros/base.mac

EnumAAFMobSlots.all : EnumAAFMobSlots.cpp EnumAAFMobSlots.h
EnumAAFMobSlots.all : EnumAAFMobSlots.comc EnumAAFMobSlots.comh
EnumAAFMobSlots.all : EnumAAFMobSlots.exp EnumAAFMobSlots.idl
EnumAAFMobSlots.all : EnumAAFMobSlots.implc EnumAAFMobSlots.implh
EnumAAFMobSlots.exp : AAFUnitTest.exp macros/exp.mac macros/base.mac
EnumAAFMobSlots.h : EnumAAFMobSlots.exp macros/h.mac macros/base.mac
EnumAAFMobSlots.cpp : EnumAAFMobSlots.exp macros/cpp.mac macros/base.mac
EnumAAFMobSlots.idl : EnumAAFMobSlots.exp macros/idl.mac macros/base.mac
EnumAAFMobSlots.comc : EnumAAFMobSlots.exp macros/comc.mac macros/base.mac
EnumAAFMobSlots.comh : EnumAAFMobSlots.exp macros/comh.mac macros/base.mac
EnumAAFMobSlots.implc : EnumAAFMobSlots.exp macros/implc.mac macros/base.mac
EnumAAFMobSlots.implh : EnumAAFMobSlots.exp macros/implh.mac macros/base.mac

EnumAAFParameterSlots.all : EnumAAFParameterSlots.cpp EnumAAFParameterSlots.h
EnumAAFParameterSlots.all : EnumAAFParameterSlots.comc EnumAAFParameterSlots.comh
EnumAAFParameterSlots.all : EnumAAFParameterSlots.exp EnumAAFParameterSlots.idl
EnumAAFParameterSlots.all : EnumAAFParameterSlots.implc EnumAAFParameterSlots.implh
EnumAAFParameterSlots.exp : AAFUnitTest.exp macros/exp.mac macros/base.mac
EnumAAFParameterSlots.h : EnumAAFParameterSlots.exp macros/h.mac macros/base.mac
EnumAAFParameterSlots.cpp : EnumAAFParameterSlots.exp macros/cpp.mac macros/base.mac
EnumAAFParameterSlots.idl : EnumAAFParameterSlots.exp macros/idl.mac macros/base.mac
EnumAAFParameterSlots.comc : EnumAAFParameterSlots.exp macros/comc.mac macros/base.mac
EnumAAFParameterSlots.comh : EnumAAFParameterSlots.exp macros/comh.mac macros/base.mac
EnumAAFParameterSlots.implc : EnumAAFParameterSlots.exp macros/implc.mac macros/base.mac
EnumAAFParameterSlots.implh : EnumAAFParameterSlots.exp macros/implh.mac macros/base.mac

EnumAAFReferenceValues.all : EnumAAFReferenceValues.cpp EnumAAFReferenceValues.h
EnumAAFReferenceValues.all : EnumAAFReferenceValues.comc EnumAAFReferenceValues.comh
EnumAAFReferenceValues.all : EnumAAFReferenceValues.exp EnumAAFReferenceValues.idl
EnumAAFReferenceValues.all : EnumAAFReferenceValues.implc EnumAAFReferenceValues.implh
EnumAAFReferenceValues.exp : AAFUnitTest.exp macros/exp.mac macros/base.mac
EnumAAFReferenceValues.h : EnumAAFReferenceValues.exp macros/h.mac macros/base.mac
EnumAAFReferenceValues.cpp : EnumAAFReferenceValues.exp macros/cpp.mac macros/base.mac
EnumAAFReferenceValues.idl : EnumAAFReferenceValues.exp macros/idl.mac macros/base.mac
EnumAAFReferenceValues.comc : EnumAAFReferenceValues.exp macros/comc.mac macros/base.mac
EnumAAFReferenceValues.comh : EnumAAFReferenceValues.exp macros/comh.mac macros/base.mac
EnumAAFReferenceValues.implc : EnumAAFReferenceValues.exp macros/implc.mac macros/base.mac
EnumAAFReferenceValues.implh : EnumAAFReferenceValues.exp macros/implh.mac macros/base.mac

EnumAAFSegments.all : EnumAAFSegments.cpp EnumAAFSegments.h
EnumAAFSegments.all : EnumAAFSegments.comc EnumAAFSegments.comh
EnumAAFSegments.all : EnumAAFSegments.exp EnumAAFSegments.idl
EnumAAFSegments.all : EnumAAFSegments.implc EnumAAFSegments.implh
EnumAAFSegments.exp : AAFUnitTest.exp macros/exp.mac macros/base.mac
EnumAAFSegments.h : EnumAAFSegments.exp macros/h.mac macros/base.mac
EnumAAFSegments.cpp : EnumAAFSegments.exp macros/cpp.mac macros/base.mac
EnumAAFSegments.idl : EnumAAFSegments.exp macros/idl.mac macros/base.mac
EnumAAFSegments.comc : EnumAAFSegments.exp macros/comc.mac macros/base.mac
EnumAAFSegments.comh : EnumAAFSegments.exp macros/comh.mac macros/base.mac
EnumAAFSegments.implc : EnumAAFSegments.exp macros/implc.mac macros/base.mac
EnumAAFSegments.implh : EnumAAFSegments.exp macros/implh.mac macros/base.mac

EnumAAFTypeDefinitions.all : EnumAAFTypeDefinitions.cpp EnumAAFTypeDefinitions.h
EnumAAFTypeDefinitions.all : EnumAAFTypeDefinitions.comc EnumAAFTypeDefinitions.comh
EnumAAFTypeDefinitions.all : EnumAAFTypeDefinitions.exp EnumAAFTypeDefinitions.idl
EnumAAFTypeDefinitions.all : EnumAAFTypeDefinitions.implc EnumAAFTypeDefinitions.implh
EnumAAFTypeDefinitions.exp : AAFUnitTest.exp macros/exp.mac macros/base.mac
EnumAAFTypeDefinitions.h : EnumAAFTypeDefinitions.exp macros/h.mac macros/base.mac
EnumAAFTypeDefinitions.cpp : EnumAAFTypeDefinitions.exp macros/cpp.mac macros/base.mac
EnumAAFTypeDefinitions.idl : EnumAAFTypeDefinitions.exp macros/idl.mac macros/base.mac
EnumAAFTypeDefinitions.comc : EnumAAFTypeDefinitions.exp macros/comc.mac macros/base.mac
EnumAAFTypeDefinitions.comh : EnumAAFTypeDefinitions.exp macros/comh.mac macros/base.mac
EnumAAFTypeDefinitions.implc : EnumAAFTypeDefinitions.exp macros/implc.mac macros/base.mac
EnumAAFTypeDefinitions.implh : EnumAAFTypeDefinitions.exp macros/implh.mac macros/base.mac
	