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
	cd tool ;  $(MAKE)

test_only:
	cd tool ; $(MAKE) test_only

run:
	cd tool ; $(MAKE) run

.SUFFIXES: .cpp .h .dod .exp .idl

# This file contains the list of all of the targets to be built...								   
include targets.mk


targets: $(DODO_TARGETS)

SRC_DIR = ../ref-impl/src

INCLUDE_DIR = ../ref-impl/include

.dod.exp :
	./tool/$(DODO) -f macros/exp.mac -o $*.exp < $*.dod

$(INCLUDE_DIR)/cpp-api/%.h : %.dod
	./tool/$(DODO) -f macros/h.mac -o $(INCLUDE_DIR)/cpp-api/$*.h < $*.dod

$(SRC_DIR)/cpp-api/test/%Test.cpp : %.dod
	./tool/$(DODO) -f macros/cppt.mac -o $(SRC_DIR)/cpp-api/test/$*Test.cpp < $*.dod

$(SRC_DIR)/com-api/C%.h : %.dod
	./tool/$(DODO) -f macros/comh.mac -o $(SRC_DIR)/com-api/C$*.h < $*.dod

$(SRC_DIR)/com-api/C%.cpp : %.dod
	./tool/$(DODO) -f macros/comc.mac -o $(SRC_DIR)/com-api/C$*.cpp < $*.dod

$(SRC_DIR)/com-api/test/C%Test.cpp : %.dod
	./tool/$(DODO) -f macros/comt.mac -o $(SRC_DIR)/com-api/test/C$*Test.cpp < $*.dod

$(SRC_DIR)/impl/Impl%.h : %.dod
	-@ result=`cleartool ls -vob $(SRC_DIR)/impl/Impl$*.h` ; \
	if test -z "$$result" ; then \
		echo ./tool/$(DODO) -f macros/implh.mac -o $(SRC_DIR)/impl/Impl$*.h < $*.dod ; \
		./tool/$(DODO) -f macros/implh.mac -o $(SRC_DIR)/impl/Impl$*.h < $*.dod ; \
	else \
		if echo $$result | grep "\[eclipsed\]" ; then \
			echo Renaming old dodo file Impl$*.h to Impl$*.h.dodo ; \
			echo mv $(SRC_DIR)/impl/Impl$*.h $(SRC_DIR)/impl/Impl$*.h.dodo ; \
			mv $(SRC_DIR)/impl/Impl$*.h $(SRC_DIR)/impl/Impl$*.h.dodo ; \
		fi ; \
	fi

$(SRC_DIR)/impl/Impl%.cpp : %.dod
	-@ result=`cleartool ls -vob $(SRC_DIR)/impl/Impl$*.cpp` ; \
	if test -z "$$result" ; then \
		echo ./tool/$(DODO) -f macros/implc.mac -o $(SRC_DIR)/impl/Impl$*.cpp < $*.dod ; \
		./tool/$(DODO) -f macros/implc.mac -o $(SRC_DIR)/impl/Impl$*.cpp < $*.dod ; \
	else \
		if echo $$result | grep "\[eclipsed\]" ; then \
			echo Renaming old dodo file Impl$*.cpp to Impl$*.cpp.dodo ; \
			echo mv $(SRC_DIR)/impl/Impl$*.cpp $(SRC_DIR)/impl/Impl$*.cpp.dodo ; \
			mv $(SRC_DIR)/impl/Impl$*.cpp $(SRC_DIR)/impl/Impl$*.cpp.dodo ; \
		fi ; \
	fi

$(SRC_DIR)/cpp-api/%.cpp : %.dod
	./tool/$(DODO) -f macros/cpp.mac -o $(SRC_DIR)/cpp-api/$*.cpp < $*.dod

$(INCLUDE_DIR)/com-api/%.idl : %.dod
	./tool/$(DODO) -f macros/idl.mac -o $(INCLUDE_DIR)/com-api/$*.idl < $*.dod

$(INCLUDE_DIR)/ref-api/%.h : %.dod
	./tool/$(DODO) -f macros/refh.mac -o $(INCLUDE_DIR)/ref-api/$*.h < $*.dod


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
		- result=`cleartool ls -vob $$file` ; \
		if test -z "$$result" ; then \
			echo $(RM) -f $$file ; \
			$(RM) -f $$file ; \
		fi ; \
	done
	@for file in $(SRC_DIR)/impl/Impl*.cpp ; do \
		- result=`cleartool ls -vob $$file` ; \
		if test -z "$$result" ; then \
			echo $(RM) -f $$file ; \
			$(RM) -f $$file ; \
		fi ; \
	done


# This file contains the list of all dependents...
include depend.mk
