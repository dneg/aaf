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

.SUFFIXES: .cpp .h .comc .comh .dod .exp .idl .fidl .implc .implh .comt .cppt .refh .frefh

# This file contains the list of all of the targets to be built...								   
include targets.mk
include aafobjects.mk


INCLUDE_DIR = ../ref-impl/include

targets: $(DODO_TARGETS) $(INCLUDE_DIR)/com-api/AAF.idl $(INCLUDE_DIR)/ref-api/AAF.h

$(INCLUDE_DIR)/com-api/AAF.idl : $(FIDL_TARGETS)
	@ echo Generating AAF.idl...
	$(RM) -f $(INCLUDE_DIR)/com-api/AAF.idl
	@ ( echo "cpp_quote(\"//=--------------------------------------------------------------------------=\")" ; \
	    echo "cpp_quote(\"// (C) Copyright 1998 Avid Technology.\")" ; \
	    echo "cpp_quote(\"// (C) Copyright 1998 Microsoft Corporation.\")" ; \
	    echo "cpp_quote(\"//\")" ; \
	    echo "cpp_quote(\"// THIS CODE AND INFORMATION IS PROVIDED 'AS IS' WITHOUT WARRANTY OF\")" ; \
	    echo "cpp_quote(\"// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO\")" ; \
	    echo "cpp_quote(\"// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A\")" ; \
	    echo "cpp_quote(\"// PARTICULAR PURPOSE.\")" ; \
	    echo "cpp_quote(\"//=--------------------------------------------------------------------------=\")" ; \
	    echo "cpp_quote(\"// AAF Interfaces.\")" ; \
	    echo "cpp_quote(\"//=--------------------------------------------------------------------------=\")" ; \
	    echo "cpp_quote(\"//\")" ; \
	    echo "" ; \
	    echo \#ifndef DO_NO_IMPORTS ; \
	    echo import \"unknwn.idl\"\; ; \
	    echo \#endif ; \
	    echo "" ; \
	    echo \#ifndef DO_NO_IMPORTS ; \
	    echo import \"objidl.idl\"\; ; \
	    echo \#endif ; \
	    echo "" ; \
	    echo \#ifndef DO_NO_IMPORTS ; \
	    echo import \"AAFTypes.idl\"\; ; \
	    echo \#endif ; \
	    echo "" ; \
	    for class in $(DODO_TARGET_NAMES) $(AAFCOMINTERFACESONLY) ; do \
	    	echo interface I$$class\;; \
	    done ; \
	    for class in $(DODO_TARGET_NAMES) $(AAFCOMINTERFACESONLY) ; do \
	    	echo ""; \
	    	echo "// I$$class"; \
	    	echo ""; \
	    	cat $$class.fidl; \
	    done ; \
	) > $(INCLUDE_DIR)/com-api/AAF.idl
	chmod -w $(INCLUDE_DIR)/com-api/AAF.idl

$(INCLUDE_DIR)/ref-api/AAF.h : $(FREFH_TARGETS)
	@ echo Generating reference AAF.h...
	$(RM) -f $(INCLUDE_DIR)/ref-api/AAF.h
	@ ( echo "//=--------------------------------------------------------------------------=" ; \
	    echo "// (C) Copyright 1998 Avid Technology." ; \
	    echo "// (C) Copyright 1998 Microsoft Corporation." ; \
	    echo "//" ; \
	    echo "// THIS CODE AND INFORMATION IS PROVIDED 'AS IS' WITHOUT WARRANTY OF" ; \
	    echo "// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO" ; \
	    echo "// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A" ; \
	    echo "// PARTICULAR PURPOSE." ; \
	    echo "//=--------------------------------------------------------------------------=" ; \
	    echo "// AAF Interfaces." ; \
	    echo "//=--------------------------------------------------------------------------=" ; \
	    echo "//" ; \
	    echo \#ifndef __AAF_h__ ; \
	    echo \#define __AAF_h__ ; \
	    echo "" ; \
	    echo \#ifndef __AAFCOMPlatform_h__ ; \
	    echo \#include \"AAFCOMPlatform.h\" ; \
	    echo \#endif ; \
	    echo "" ; \
	    echo \#ifndef __AAFTypes_h__ ; \
	    echo \#include \"AAFTypes.h\" ; \
	    echo \#endif ; \
	    echo "" ; \
	    echo \#ifdef __cplusplus ; \
	    for class in $(DODO_TARGET_NAMES) $(AAFCOMINTERFACESONLY) ; do \
	    	echo interface I$$class\;; \
	    done ; \
	    echo \#else ; \
	    for class in $(DODO_TARGET_NAMES) $(AAFCOMINTERFACESONLY) ; do \
	    	echo typedef interface I$$class I$$class\;;  \
	    done ; \
	    echo \#endif ; \
	    for class in $(DODO_TARGET_NAMES) $(AAFCOMINTERFACESONLY) ; do \
	    	echo ""; \
	    	echo "// I$$class"; \
	    	echo ""; \
	    	cat $$class.frefh; \
	    done ; \
	    echo "" ; \
	    echo \#endif // __AAF_h__ ; \
	) > $(INCLUDE_DIR)/ref-api/AAF.h
	chmod -w $(INCLUDE_DIR)/ref-api/AAF.h

SRC_DIR = ../ref-impl/src

.dod.exp :
	$(RM) -f $*.exp
	./tool/$(DODO) -f macros/exp.mac < $*.dod > $*.tmp
	mv $*.tmp $*.exp
	chmod -w $*.exp

.dod.h :
	$(RM) -f $*.h
	./tool/$(DODO) -f macros/h.mac < $*.dod > $*.tmp
	mv $*.tmp $*.h
	chmod -w $*.h
	cp -f $*.h $(INCLUDE_DIR)/cpp-api/
	chmod -w $(INCLUDE_DIR)/cpp-api/$*.h

.dod.cppt :
	$(RM) -f $*.cppt
	./tool/$(DODO) -f macros/cppt.mac < $*.dod > $*.tmp
	mv $*.tmp $*.cppt
	chmod -w $*.cppt
	cp -f $*.cppt $(SRC_DIR)/cpp-api/test/$*Test.cpp
	chmod -w $(SRC_DIR)/cpp-api/test/$*Test.cpp

.dod.comh :
	$(RM) -f $*.comh
	./tool/$(DODO) -f macros/comh.mac < $*.dod > $*.tmp
	mv $*.tmp $*.comh
	chmod -w $*.comh
	cp -f $*.comh $(SRC_DIR)/com-api/C$*.h
	chmod -w $(SRC_DIR)/com-api/C$*.h

.dod.comc :
	$(RM) -f $*.comc
	./tool/$(DODO) -f macros/comc.mac < $*.dod > $*.tmp
	mv $*.tmp $*.comc
	chmod -w $*.comc
	cp -f $*.comc $(SRC_DIR)/com-api/C$*.cpp
	chmod -w $(SRC_DIR)/com-api/C$*.cpp

.dod.comt :
	$(RM) -f $*.comt
	./tool/$(DODO) -f macros/comt.mac < $*.dod > $*.tmp
	mv $*.tmp $*.comt
	chmod -w $*.comt
	cp -f $*.comt $(SRC_DIR)/com-api/test/C$*Test.cpp
	chmod -w $(SRC_DIR)/com-api/test/C$*Test.cpp

.dod.implh :
	$(RM) -f $*.implh ;
	./tool/$(DODO) -f macros/implh.mac < $*.dod > $*.tmp ;
	mv $*.tmp $*.implh ;
	chmod -w $*.implh ;
	cp -f $*.implh $(SRC_DIR)/impl/Impl$*.h ;
	chmod -w $(SRC_DIR)/impl/Impl$*.h ;

.dod.implc :
	$(RM) -f $*.implc ;
	./tool/$(DODO) -f macros/implc.mac < $*.dod > $*.tmp ;
	mv $*.tmp $*.implc ;
	chmod -w $*.implc ;
	cp -f $*.implc $(SRC_DIR)/impl/Impl$*.cpp ;
	chmod -w $(SRC_DIR)/impl/Impl$*.cpp ;

.dod.cpp :
	$(RM) -f $*.cpp
	./tool/$(DODO) -f macros/cpp.mac < $*.dod > $*.tmp
	mv $*.tmp $*.cpp
	chmod -w $*.cpp
	cp -f $*.cpp $(SRC_DIR)/cpp-api
	chmod -w $(SRC_DIR)/cpp-api/$*.cpp

.dod.idl :
	$(RM) -f $*.idl
	./tool/$(DODO) -f macros/idl.mac < $*.dod > $*.tmp
	mv $*.tmp $*.idl
	chmod -w $*.idl
	cp -f $*.idl $(INCLUDE_DIR)/com-api/
	chmod -w $(INCLUDE_DIR)/com-api/$*.idl

.dod.fidl :
	$(RM) -f $*.fidl
	./tool/$(DODO) -f macros/fidl.mac < $*.dod > $*.tmp
	mv $*.tmp $*.fidl
	chmod -w $*.fidl

.dod.refh :
	$(RM) -f $*.refh
	./tool/$(DODO) -f macros/refh.mac < $*.dod > $*.tmp
	mv $*.tmp $*.refh
	chmod -w $*.refh
	cp -f $*.refh $(INCLUDE_DIR)/ref-api/$*.h
	chmod -w $(INCLUDE_DIR)/ref-api/$*.h

.dod.frefh :
	$(RM) -f $*.frefh
	./tool/$(DODO) -f macros/frefh.mac < $*.dod > $*.tmp
	mv $*.tmp $*.frefh
	chmod -w $*.frefh



clean:
	cd tool ; $(MAKE) clean
	$(RM) -f *.cpp *.cppt *.h *.idl *.fidl *.exp
	$(RM) -f *.comc *.comh *.comt *.refh *.frefh
	$(RM) -f *.implc *.implh
	$(RM) -f core
	@for file in $(AAFOBJECTS) ; do \
		echo $(RM) -f $(SRC_DIR)/cpp-api/$$file.cpp ; \
		$(RM) -f $(SRC_DIR)/cpp-api/$$file.cpp ; \
		echo $(RM) -f $(SRC_DIR)/com-api/C$$file.h ; \
		$(RM) -f $(SRC_DIR)/com-api/C$$file.h ; \
		echo $(RM) -f $(SRC_DIR)/com-api/C$$file.cpp ; \
		$(RM) -f $(SRC_DIR)/com-api/C$$file.cpp ; \
		echo $(RM) -f $(INCLUDE_DIR)/cpp-api/$$file.h ; \
		$(RM) -f $(INCLUDE_DIR)/cpp-api/$$file.h ; \
		echo $(RM) -f $(INCLUDE_DIR)/ref-api/$$file.h ; \
		$(RM) -f $(INCLUDE_DIR)/ref-api/$$file.h ; \
	done
	$(RM) -f $(SRC_DIR)/cpp-api/test/AAF*Test.cpp
	$(RM) -f $(SRC_DIR)/cpp-api/test/EnumAAF*Test.cpp
	$(RM) -f $(SRC_DIR)/com-api/test/CAAF*Test.cpp
	$(RM) -f $(SRC_DIR)/com-api/test/CEnumAAF*Test.cpp
	$(RM) -f $(INCLUDE_DIR)/com-api/AAF.h
	$(RM) -f $(INCLUDE_DIR)/com-api/AAFTypes.h
	$(RM) -f $(INCLUDE_DIR)/com-api/AAFModuleTest.h
	$(RM) -f $(INCLUDE_DIR)/com-api/AAF.idl
	$(RM) -f $(INCLUDE_DIR)/com-api/AAFTypes.idl
	$(RM) -f $(INCLUDE_DIR)/com-api/AAFModuleTest.idl
	$(RM) -f $(INCLUDE_DIR)/ref-api/AAF.h
	$(RM) -f $(INCLUDE_DIR)/ref-api/AAFTypes.h
	$(RM) -f $(INCLUDE_DIR)/ref-api/AAFModuleTest.h
	@for file in $(AUTO_GEN_IMPL) ; do \
		echo $(RM) -f $(SRC_DIR)/impl/Impl$$file.cpp ; \
		$(RM) -f $(SRC_DIR)/impl/Impl$$file.cpp ; \
		echo $(RM) -f $(SRC_DIR)/impl/Impl$$file.h ; \
		$(RM) -f $(SRC_DIR)/impl/Impl$$file.h ; \
	done


# This file contains the list of all dependents...
include depend.mk
