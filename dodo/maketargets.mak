############################################
#                                          #
# Copyright (c) 1998 Avid Technology, Inc. #
#                                          #
############################################

all : targets

BLD_CFG_DIR = ../ref-impl/bld_cfg

include $(BLD_CFG_DIR)/common.mk

DODO = dodo.$(EXE)

dodo-app:
	$(SH_PREFIX) cd tool ;  $(MAKE) $(SH_SUFFIX)

test_only:
	$(SH_PREFIX) cd tool ; $(MAKE) test_only $(SH_SUFFIX)

run:
	 $(SH_PREFIX) cd tool ; $(MAKE) run $(SH_SUFFIX)

.SUFFIXES: .cpp .h .comc .comh .dod .exp .idl .implc .implh .comt .cppt .refh

# This file contains the list of all of the targets to be built...								   
include targets.mk


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
	@ result=`cleartool ls -vob $(SRC_DIR)/impl/Impl$*.h` ; \
	if test -z "$$result" ; then \
		echo $(RM) -f $*.implh ; \
		$(RM) -f $*.implh ; \
		echo ./tool/$(DODO) -f macros/implh.mac < $*.dod > $*.tmp ; \
		./tool/$(DODO) -f macros/implh.mac < $*.dod > $*.tmp ; \
		echo mv $*.tmp $*.implh ; \
		mv $*.tmp $*.implh ; \
		echo cp $*.implh $(SRC_DIR)/impl/Impl$*.h ; \
		cp $*.implh $(SRC_DIR)/impl/Impl$*.h ; \
	else \
		if echo $$result | grep "\[eclipsed\]" ; then \
			echo Renaming old dodo file Impl$*.h to Impl$*.h.dodo ; \
			echo mv $(SRC_DIR)/impl/Impl$*.h $(SRC_DIR)/impl/Impl$*.h.dodo ; \
			mv $(SRC_DIR)/impl/Impl$*.h $(SRC_DIR)/impl/Impl$*.h.dodo ; \
		fi ; \
	fi

.dod.implc :
	@ result=`cleartool ls -vob $(SRC_DIR)/impl/Impl$*.cpp` ; \
	if test -z "$$result" ; then \
		echo $(RM) -f $*.implc ; \
		$(RM) -f $*.implc ; \
		echo ./tool/$(DODO) -f macros/implc.mac < $*.dod > $*.tmp ; \
		./tool/$(DODO) -f macros/implc.mac < $*.dod > $*.tmp ; \
		echo mv $*.tmp $*.implc ; \
		mv $*.tmp $*.implc ; \
		echo cp $*.implc $(SRC_DIR)/impl/Impl$*.cpp ; \
		cp $*.implc $(SRC_DIR)/impl/Impl$*.cpp ; \
	else \
		if echo $$result | grep "\[eclipsed\]" ; then \
			echo Renaming old dodo file Impl$*.cpp to Impl$*.cpp.dodo ; \
			echo mv $(SRC_DIR)/impl/Impl$*.cpp $(SRC_DIR)/impl/Impl$*.c.dodo ; \
			mv $(SRC_DIR)/impl/Impl$*.cpp $(SRC_DIR)/impl/Impl$*.c.dodo ; \
		fi ; \
	fi

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

.dod.refh :
	$(SH_PREFIX) $(RM) -f $*.refh $(SH_SUFFIX)
	$(SH_PREFIX) ./tool/$(DODO) -f macros/refh.mac < $*.dod > $*.tmp $(SH_SUFFIX)
	$(SH_PREFIX) mv $*.tmp $*.refh $(SH_SUFFIX)
	$(SH_PREFIX) cp $*.refh $(INCLUDE_DIR)/ref-api/$*.h $(SH_SUFFIX)



clean:
	$(SH_PREFIX) cd tool ; $(MAKE) clean $(SH_SUFFIX)
	$(SH_PREFIX) $(RM) -f *.cpp *.cppt *.h *.idl *.exp $(SH_SUFFIX)
	$(SH_PREFIX) $(RM) -f *.comc *.comh *.comt $(SH_SUFFIX)
	$(SH_PREFIX) $(RM) -f *.implc *.implh $(SH_SUFFIX)
	$(SH_PREFIX) $(RM) -f core $(SH_SUFFIX)
	$(SH_PREFIX) $(RM) -f $(SRC_DIR)/cpp-api/AAF*.cpp $(SH_SUFFIX)
	$(SH_PREFIX) $(RM) -f $(SRC_DIR)/com-api/CAAF*.h $(SH_SUFFIX)
	$(SH_PREFIX) $(RM) -f $(SRC_DIR)/com-api/CAAF*.cpp $(SH_SUFFIX)
	$(SH_PREFIX) $(RM) -f $(INCLUDE_DIR)/com-api/AAF*.h $(SH_SUFFIX)
	$(SH_PREFIX) $(RM) -f $(INCLUDE_DIR)/com-api/EnumAAF*.h $(SH_SUFFIX)
	$(SH_PREFIX) $(RM) -f $(INCLUDE_DIR)/com-api/AAF*.idl $(SH_SUFFIX)
	$(SH_PREFIX) $(RM) -f $(INCLUDE_DIR)/com-api/EnumAAF*.idl $(SH_SUFFIX)
	$(SH_PREFIX) $(RM) -f $(INCLUDE_DIR)/cpp-api/AAF*.h $(SH_SUFFIX)
	$(SH_PREFIX) $(RM) -f $(INCLUDE_DIR)/cpp-api/EnumAAF*.h $(SH_SUFFIX)
	$(SH_PREFIX) $(RM) -f $(INCLUDE_DIR)/ref-api/AAF*.h $(SH_SUFFIX)
	$(SH_PREFIX) $(RM) -f $(INCLUDE_DIR)/ref-api/EnumAAF*.h $(SH_SUFFIX)
	@for file in $(SRC_DIR)/impl/Impl*.h ; do \
		result=`cleartool ls -vob $$file` ; \
		if test -z "$$result" ; then \
			echo $(RM) -f $$file ; \
			$(RM) -f $$file ; \
		fi ; \
	done
	@for file in $(SRC_DIR)/impl/Impl*.cpp ; do \
		result=`cleartool ls -vob $$file` ; \
		if test -z "$$result" ; then \
			echo $(RM) -f $$file ; \
			$(RM) -f $$file ; \
		fi ; \
	done


# This file contains the list of all dependents...
include depend.mk
