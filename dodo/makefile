############################################
#                                          #
# Copyright (c) 1998 Avid Technology, Inc. #
#                                          #
############################################

all : dodo-app depend targets

BLD_CFG_DIR = ../ref-impl/bld_cfg

include $(BLD_CFG_DIR)/common.mk

DODO = dodo.$(EXE)

dodo-app:
	$(SH_PREFIX) cd tool ;  $(MAKE) $(SH_SUFFIX)

test_only:
	$(SH_PREFIX) cd tool ; $(MAKE) test_only $(SH_SUFFIX)

run:
	 $(SH_PREIX) cd tool ; $(MAKE) run $(SH_SUFFIX)

.SUFFIXES: .cpp .h .comc .comh .dod .exp .idl .implc .implh .comt .cppt
								   
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

.dod.cppt :
	$(SH_PREFIX) $(RM) -f $*.cppt $(SH_SUFFIX)
	$(SH_PREFIX) ./tool/$(DODO) -f macros/cppt.mac < $*.dod > $*.tmp $(SH_SUFFIX)
	$(SH_PREFIX) mv $*.tmp $*.cppt $(SH_SUFFIX)
	$(SH_PREFIX) cp $*.cppt $(SRC_DIR)/cpp-api/test/$*Test.cpp $(SH_SUFFIX)

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

.dod.comt :
	$(SH_PREFIX) $(RM) -f $*.comt $(SH_SUFFIX)
	$(SH_PREFIX) ./tool/$(DODO) -f macros/comt.mac < $*.dod > $*.tmp $(SH_SUFFIX)
	$(SH_PREFIX) mv $*.tmp $*.comt $(SH_SUFFIX)
	$(SH_PREFIX) cp $*.comt $(SRC_DIR)/com-api/test/C$*Test.cpp $(SH_SUFFIX)

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
	$(SH_PREFIX) $(RM) -rf *.cpp *.h *.idl *.exp $(SH_SUFFIX)
	$(SH_PREFIX) $(RM) -rf *.comc *.comh *.comt $(SH_SUFFIX)
	$(SH_PREFIX) $(RM) -rf *.implc *.implh *.list $(SH_SUFFIX)
	$(SH_PREFIX) $(RM) -rf $(SRC_DIR)/cpp-api/*.cpp $(SH_SUFFIX)
	$(SH_PREFIX) $(RM) -rf $(SRC_DIR)/com-api/*.h $(SH_SUFFIX)
	$(SH_PREFIX) $(RM) -rf $(SRC_DIR)/com-api/*.cpp $(SH_SUFFIX)
	$(SH_PREFIX) $(RM) -rf $(SRC_DIR)/impl/*.h $(SH_SUFFIX)
	$(SH_PREFIX) $(RM) -rf $(SRC_DIR)/impl/*.cpp $(SH_SUFFIX)
	$(SH_PREFIX) $(RM) -rf $(INCLUDE_DIR)/com-api/*.h $(SH_SUFFIX)
	$(SH_PREFIX) $(RM) -rf $(INCLUDE_DIR)/com-api/*.idl $(SH_SUFFIX)
	$(SH_PREFIX) $(RM) -rf $(INCLUDE_DIR)/cpp-api/*.h $(SH_SUFFIX)

depend :
	@echo "Creating depend.mk ..."
	@rm -f depend.mk
	@echo "# This file automatically generated by make depend." > depend.mk
	@$(SH_PREFIX) for targ in $(DODO_TARGETS) ; do \
		echo $$targ... ; \
		echo '' >> depend.mk ; \
		base=`echo $$targ | sed -e "s,\.all,,"` ; \
		echo $$targ : $$base.cpp $$base.h $base.cppt >> depend.mk ; \
		echo $$targ : $$base.comc $$base.comh $$base.comt >> depend.mk ; \
		echo $$targ : $$base.implc $$base.implh >> depend.mk ; \
		echo $$targ : $$base.idl $$base.exp >> depend.mk ; \
		echo $$base.cpp : macros/cpp.mac macros/base.mac >> depend.mk ; \
		echo $$base.h : macros/h.mac macros/base.mac >> depend.mk ; \
		echo $$base.cppt : macros/cppt.mac macros/base.mac >> depend.mk ; \
		echo $$base.comc : macros/comc.mac macros/base.mac >> depend.mk ; \
		echo $$base.comh : macros/comh.mac macros/base.mac >> depend.mk ; \
		echo $$base.comt : macros/comt.mac macros/base.mac >> depend.mk ; \
		echo $$base.implc : macros/implc.mac macros/base.mac >> depend.mk ; \
		echo $$base.implh : macros/implh.mac macros/base.mac >> depend.mk ; \
		echo $$base.idl : macros/idl.mac macros/base.mac >> depend.mk ; \
		echo $$base.exp : macros/exp.mac macros/base.mac >> depend.mk ; \
		$(SH_PREFIX) for import in `grep "^#import" $$base.dod | sed -e "s,#import,,"` ; do \
			echo $$base.cpp : $$import >> depend.mk ; \
			echo $$base.h : $$import >> depend.mk ; \
			echo $$base.cppt : $$import >> depend.mk ; \
			echo $$base.comc : $$import >> depend.mk ; \
			echo $$base.comh : $$import >> depend.mk ; \
			echo $$base.comt : $$import >> depend.mk ; \
			echo $$base.implc : $$import >> depend.mk ; \
			echo $$base.implh : $$import >> depend.mk ; \
			echo $$base.idl : $$import >> depend.mk ; \
			echo $$base.exp : $$import >> depend.mk ; \
		done $(SH_SUFFIX) \
	done $(SH_SUFFIX)
	@echo "Done with depend.mk."

include depend.mk
