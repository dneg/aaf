############################################
#                                          #
# Copyright (c) 1998 Avid Technology, Inc. #
#                                          #
############################################

all : targets

BLD_CFG_DIR = ../ref-impl/bld_cfg
PLUGIN_DIR = ../ref-impl/plugins

include $(BLD_CFG_DIR)/common.mk

DODO = dodo.$(EXE)

dodo-app:
	cd tool ;  $(MAKE)

test_only:
	cd tool ; $(MAKE) test_only

run:
	 cd tool ; $(MAKE) run

.SUFFIXES: .cpp .h .comc .comcx .comh .dod .exp .idl .fidl .implc .implh .comt .cppt .refh .frefh

# This file contains the list of all of the targets to be built...
include targets.mk
include aafobjects.mk


INCLUDE_DIR = ../ref-impl/include
IMPL_DIR = ../ref-impl/src/impl
UUID_DIR = ../ref-impl/include/ref-api
COMAPI_DIR = ../ref-impl/src/com-api

targets: $(DODO_TARGETS)
targets: $(PLUGIN_TARGETS)
targets: $(INCLUDE_DIR)/com-api/AAF.idl
targets: $(PLUGIN_DIR)/AAFPlugin.idl
targets: $(INCLUDE_DIR)/ref-api/AAF.h
targets: $(UUID_DIR)/AAF_i.c
targets: $(INCLUDE_DIR)/ref-api/AAFPlugin.h
targets: $(UUID_DIR)/AAFPlugin_i.c
targets: $(COMAPI_DIR)/AAFCLSIDs.h
targets: $(IMPL_DIR)/AAFClassIDs.h


$(INCLUDE_DIR)/com-api/AAF.idl : $(FIDL_TARGETS)
	@ echo Generating AAF.idl...
	$(RM) -f $(INCLUDE_DIR)/com-api/AAF.idl
	@ ( echo "cpp_quote(\"//=--------------------------------------------------------------------------=\")" ; \
	    echo "cpp_quote(\"// (C) Copyright 1998-1999 Avid Technology.\")" ; \
	    echo "cpp_quote(\"// (C) Copyright 1998-1999 Microsoft Corporation.\")" ; \
	    echo "cpp_quote(\"//\")" ; \
	    echo "cpp_quote(\"// This file was GENERATED for the AAF SDK on \")" ; \
	    echo "cpp_quote(\"//  `date`\")" ; \
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
	    	echo ""; \
	    	cat $$class.fidl; \
	    done ; \
	    echo "" ; \
	    echo "" ; \
	    echo "" ; \
	    cat AAFModule.fidl; \
	    echo "" ; \
	) > $(INCLUDE_DIR)/com-api/AAF.idl
	chmod -w $(INCLUDE_DIR)/com-api/AAF.idl

$(PLUGIN_DIR)/AAFPlugin.idl : $(PLUGIN_FIDL_TARGETS)
	@ echo Generating Plugin.idl...
	$(RM) -f $(PLUGIN_DIR)/AAFPlugin.idl
	@ ( echo "cpp_quote(\"//=--------------------------------------------------------------------------=\")" ; \
	    echo "cpp_quote(\"// (C) Copyright 1998-1999 Avid Technology.\")" ; \
	    echo "cpp_quote(\"// (C) Copyright 1998-1999 Microsoft Corporation.\")" ; \
	    echo "cpp_quote(\"//\")" ; \
	    echo "cpp_quote(\"// This file was GENERATED for the AAF SDK on \")" ; \
	    echo "cpp_quote(\"//  `date`\")" ; \
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
	    echo import \"AAF.idl\"\; ; \
	    echo \#endif ; \
	    echo "" ; \
	    echo \#ifndef DO_NO_IMPORTS ; \
	    echo import \"AAFPluginTypes.idl\"\; ; \
	    echo \#endif ; \
	    echo "" ; \
	    for class in $(PLUGIN_OBJECTS) ; do \
	    	echo interface I$$class\;; \
	    done ; \
	    for class in $(PLUGIN_OBJECTS) ; do \
	    	echo ""; \
	    	echo ""; \
	    	cat $$class.fidl; \
	    done ; \
	) > $(PLUGIN_DIR)/AAFPlugin.idl
	chmod -w $(PLUGIN_DIR)/AAFPlugin.idl

$(INCLUDE_DIR)/ref-api/AAF.h : $(FREFH_TARGETS)
	@ echo Generating reference AAF.h...
	$(RM) -f $(INCLUDE_DIR)/ref-api/AAF.h
	@ ( echo "//=--------------------------------------------------------------------------=" ; \
	    echo "// (C) Copyright 1998-1999 Avid Technology." ; \
	    echo "// (C) Copyright 1998-1999 Microsoft Corporation." ; \
	    echo "//" ; \
	    echo "// This file was GENERATED for the AAF SDK on " ; \
	    echo "//  "`date` ; \
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
	    echo "" ; \
	    echo "" ; \
	    cat AAFModule.frefh; \
	    echo "" ; \
	    echo \#endif // __AAF_h__ ; \
	) > $(INCLUDE_DIR)/ref-api/AAF.h
	chmod -w $(INCLUDE_DIR)/ref-api/AAF.h


$(INCLUDE_DIR)/ref-api/AAFPlugin.h : $(PLUGIN_FREFH_TARGETS)
	@ echo Generating reference AAFPlugin.h...
	$(RM) -f $(INCLUDE_DIR)/ref-api/AAFPlugin.h
	@ ( echo "//=--------------------------------------------------------------------------=" ; \
	    echo "// (C) Copyright 1998-1999 Avid Technology." ; \
	    echo "// (C) Copyright 1998-1999 Microsoft Corporation." ; \
	    echo "//" ; \
	    echo "// This file was GENERATED for the AAF SDK on " ; \
	    echo "//  "`date` ; \
	    echo "//" ; \
	    echo "// THIS CODE AND INFORMATION IS PROVIDED 'AS IS' WITHOUT WARRANTY OF" ; \
	    echo "// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO" ; \
	    echo "// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A" ; \
	    echo "// PARTICULAR PURPOSE." ; \
	    echo "//=--------------------------------------------------------------------------=" ; \
	    echo "// AAF Plugin Interfaces." ; \
	    echo "//=--------------------------------------------------------------------------=" ; \
	    echo "//" ; \
	    echo \#ifndef __AAFPlugin_h__ ; \
	    echo \#define __AAFPlugin_h__ ; \
	    echo "" ; \
	    echo \#ifndef __AAF_h__ ; \
	    echo \#include \"AAF.h\" ; \
	    echo \#endif ; \
	    echo "" ; \
	    echo \#ifndef __AAFPluginTypes_h__ ; \
	    echo \#include \"AAFPluginTypes.h\" ; \
	    echo \#endif ; \
	    echo "" ; \
	    echo \#ifdef __cplusplus ; \
	    for class in $(PLUGIN_OBJECTS) ; do \
	    	echo interface I$$class\;; \
	    done ; \
	    echo \#else ; \
	    for class in $(PLUGIN_OBJECTS) ; do \
	    	echo typedef interface I$$class I$$class\;;  \
	    done ; \
	    echo \#endif ; \
	    for class in $(PLUGIN_OBJECTS) ; do \
	    	echo ""; \
	    	echo "// I$$class"; \
	    	echo ""; \
	    	cat $$class.frefh; \
	    done ; \
	    echo "" ; \
	    echo \#endif // __AAFPlugin_h__ ; \
	) > $(INCLUDE_DIR)/ref-api/AAFPlugin.h
	chmod -w $(INCLUDE_DIR)/ref-api/AAFPlugin.h


$(UUID_DIR)/AAF_i.c : aafobjects.mk dod2iid.awk
	@ echo Generating reference AAF_i.c...
	$(RM) -f $(UUID_DIR)/AAF_i.c
	@ ( echo "/* this file contains the actual definitions of */" ; \
	    echo "/* the IIDs and CLSIDs */" ; \
	    echo "" ; \
	    echo "/* link this file in with the server and any clients */" ; \
	    echo "//=--------------------------------------------------------------------------=" ; \
	    echo "// (C) Copyright 1998-1999 Avid Technology." ; \
	    echo "// (C) Copyright 1998-1999 Microsoft Corporation." ; \
	    echo "//" ; \
	    echo "// This file was GENERATED for the AAF SDK on " ; \
	    echo "//  "`date` ; \
	    echo "//" ; \
	    echo "// THIS CODE AND INFORMATION IS PROVIDED 'AS IS' WITHOUT WARRANTY OF" ; \
	    echo "// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO" ; \
	    echo "// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A" ; \
	    echo "// PARTICULAR PURPOSE." ; \
	    echo "//=--------------------------------------------------------------------------=" ; \
	    echo "// Definitions for all of the public IID needed by an AAF SDK client" ; \
	    echo "//=--------------------------------------------------------------------------=" ; \
	    echo "//" ; \
	    echo "" ; \
	    echo \#ifdef __cplusplus ; \
	    echo "extern \"C\"{" ; \
	    echo \#endif ; \
	    echo "" ; \
	    echo "" ; \
	    echo \#ifndef __IID_DEFINED__ ; \
	    echo \#define __IID_DEFINED__ ; \
	    echo "" ; \
	    echo "typedef struct _IID" ; \
	    echo "{" ; \
	    echo "    unsigned long x;" ; \
	    echo "    unsigned short s1;" ; \
	    echo "    unsigned short s2;" ; \
	    echo "    unsigned char  c[8];" ; \
	    echo "} IID;" ; \
	    echo "" ; \
	    echo \#endif "// __IID_DEFINED__" ; \
	    echo "" ; \
	    echo \#ifndef CLSID_DEFINED ; \
	    echo \#define CLSID_DEFINED ; \
	    echo "typedef IID CLSID;" ; \
	    echo \#endif "// CLSID_DEFINED" ; \
	    echo "" ; \
	    for class in $(AAFOBJECTS) $(AAFCOMINTERFACESONLY) ; do \
	    	awk -f dod2iid.awk C=$$class $$class.dod ; \
	    done ; \
	    echo \#ifdef __cplusplus ; \
	    echo "}" ; \
	    echo \#endif ; \
	    echo "" ; \
	) > $(UUID_DIR)/AAF_i.c
	chmod -w $(UUID_DIR)/AAF_i.c


$(UUID_DIR)/AAFPlugin_i.c : aafobjects.mk dod2iid.awk
	@ echo Generating reference AAFPlugin_i.c...
	$(RM) -f $(UUID_DIR)/AAFPlugin_i.c
	@ ( echo "/* this file contains the actual definitions of */" ; \
	    echo "/* the IIDs and CLSIDs */" ; \
	    echo "" ; \
	    echo "/* link this file in with the server and any clients */" ; \
	    echo "//=--------------------------------------------------------------------------=" ; \
	    echo "// (C) Copyright 1998-1999 Avid Technology." ; \
	    echo "// (C) Copyright 1998-1999 Microsoft Corporation." ; \
	    echo "//" ; \
	    echo "// This file was GENERATED for the AAF SDK on " ; \
	    echo "//  "`date` ; \
	    echo "//" ; \
	    echo "// THIS CODE AND INFORMATION IS PROVIDED 'AS IS' WITHOUT WARRANTY OF" ; \
	    echo "// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO" ; \
	    echo "// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A" ; \
	    echo "// PARTICULAR PURPOSE." ; \
	    echo "//=--------------------------------------------------------------------------=" ; \
	    echo "// Definitions for all public IID's needed by an AAF SDK Plugin author" ; \
	    echo "//=--------------------------------------------------------------------------=" ; \
	    echo "//" ; \
	    echo "" ; \
	    echo \#ifdef __cplusplus ; \
	    echo "extern \"C\"{" ; \
	    echo \#endif ; \
	    echo "" ; \
	    echo "" ; \
	    echo \#ifndef __IID_DEFINED__ ; \
	    echo \#define __IID_DEFINED__ ; \
	    echo "" ; \
	    echo "typedef struct _IID" ; \
	    echo "{" ; \
	    echo "    unsigned long x;" ; \
	    echo "    unsigned short s1;" ; \
	    echo "    unsigned short s2;" ; \
	    echo "    unsigned char  c[8];" ; \
	    echo "} IID;" ; \
	    echo "" ; \
	    echo \#endif "// __IID_DEFINED__" ; \
	    echo "" ; \
	    echo \#ifndef CLSID_DEFINED ; \
	    echo \#define CLSID_DEFINED ; \
	    echo "typedef IID CLSID;" ; \
	    echo \#endif "// CLSID_DEFINED" ; \
	    echo "" ; \
	    for class in $(PLUGIN_OBJECTS) ; do \
	    	awk -f dod2iid.awk C=$$class $$class.dod ; \
	    done ; \
	    echo \#ifdef __cplusplus ; \
	    echo "}" ; \
	    echo \#endif ; \
	    echo "" ; \
	) > $(UUID_DIR)/AAFPlugin_i.c
	chmod -w $(UUID_DIR)/AAFPlugin_i.c


$(IMPL_DIR)/AAFClassIDs.h : aafobjects.mk
	@ echo Generating reference AAFClassIDs.h...
	$(RM) -f $(IMPL_DIR)/AAFClassIDs.h
	@ ( echo "//=--------------------------------------------------------------------------=" ; \
	    echo "// (C) Copyright 1998-1999 Avid Technology." ; \
	    echo "// (C) Copyright 1998-1999 Microsoft Corporation." ; \
	    echo "//" ; \
	    echo "// This file was GENERATED for the AAF SDK on " ; \
	    echo "//  "`date` ; \
	    echo "//" ; \
	    echo "// THIS CODE AND INFORMATION IS PROVIDED 'AS IS' WITHOUT WARRANTY OF" ; \
	    echo "// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO" ; \
	    echo "// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A" ; \
	    echo "// PARTICULAR PURPOSE." ; \
	    echo "//=--------------------------------------------------------------------------=" ; \
	    echo "// Declarations for all of the private AAF code class ids" ; \
	    echo "// This file is private to the AAF Reference Implementation." ; \
	    echo "//=--------------------------------------------------------------------------=" ; \
	    echo "//" ; \
	    echo \#ifndef __AAFClassIDs_h__ ; \
	    echo \#define __AAFClassIDs_h__ ; \
	    echo "" ; \
	    echo \#ifndef __AAFTypes_h__ ; \
	    echo \#include \"AAFTypes.h\" ; \
	    echo \#endif ; \
	    echo "" ; \
	    echo "" ; \
	    for class in $(AAFOBJECTS) AAFModuleTest ; do \
	    	echo "extern \"C\" const aafClassID_t CLSID_$$class;"; \
	    done ; \
	    echo "" ; \
	    echo \#endif // __AAFClassIDs_h__ ; \
	) > $(IMPL_DIR)/AAFClassIDs.h
	chmod -w $(IMPL_DIR)/AAFClassIDs.h


$(COMAPI_DIR)/AAFCLSIDs.h : aafobjects.mk
	@ echo Generating reference AAFCLSIDs.h...
	$(RM) -f $(COMAPI_DIR)/AAFCLSIDs.h
	@ ( echo "//=--------------------------------------------------------------------------=" ; \
	    echo "// (C) Copyright 1998-1999 Avid Technology." ; \
	    echo "// (C) Copyright 1998-1999 Microsoft Corporation." ; \
	    echo "//" ; \
	    echo "// This file was GENERATED for the AAF SDK on " ; \
	    echo "//  "`date` ; \
	    echo "//" ; \
	    echo "// THIS CODE AND INFORMATION IS PROVIDED 'AS IS' WITHOUT WARRANTY OF" ; \
	    echo "// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO" ; \
	    echo "// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A" ; \
	    echo "// PARTICULAR PURPOSE." ; \
	    echo "//=--------------------------------------------------------------------------=" ; \
	    echo "// Declarations for all of the private AAF code class ids" ; \
	    echo "// This file is private to the AAF Reference Implementation." ; \
	    echo "//=--------------------------------------------------------------------------=" ; \
	    echo "//" ; \
	    echo \#ifndef __AAFCLSIDs_h__ ; \
	    echo \#define __AAFCLSIDs_h__ ; \
	    echo "" ; \
	    echo \#ifndef __AAFCOMPlatformTypes_h__ ; \
	    echo \#include \"AAFCOMPlatformTypes.h\" ; \
	    echo \#endif ; \
	    echo "" ; \
	    for class in $(AAFOBJECTS) AAFModuleTest ; do \
	    	echo "EXTERN_C const CLSID CLSID_$$class;"; \
	    done ; \
	    echo "" ; \
	    echo \#endif // __AAFCLSIDs_h__ ; \
	) > $(COMAPI_DIR)/AAFCLSIDs.h
	chmod -w $(COMAPI_DIR)/AAFCLSIDs.h


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

.dod.comcx :
	$(RM) -f $*.comcx
	./tool/$(DODO) -f macros/comcx.mac < $*.dod > $*.tmp
	mv $*.tmp $*.comcx
	chmod -w $*.comcx
	cp -f $*.comcx $(SRC_DIR)/com-api/C$*.cpp
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
	$(RM) -f *.idl *.fidl *.exp
	$(RM) -f *.comc *.comcx *.comh *.comt *.refh *.frefh
	$(RM) -f *.implc *.implh
	$(RM) -f core
	@for file in $(AAFOBJECTS) ; do \
		echo $(RM) -f $(SRC_DIR)/com-api/C$$file.h ; \
		$(RM) -f $(SRC_DIR)/com-api/C$$file.h ; \
		echo $(RM) -f $(SRC_DIR)/com-api/C$$file.cpp ; \
		$(RM) -f $(SRC_DIR)/com-api/C$$file.cpp ; \
		echo $(RM) -f $(INCLUDE_DIR)/ref-api/$$file.h ; \
		$(RM) -f $(INCLUDE_DIR)/ref-api/$$file.h ; \
	done
#	$(RM) -f $(SRC_DIR)/com-api/test/CAAF*Test.cpp
#	$(RM) -f $(SRC_DIR)/com-api/test/CEnumAAF*Test.cpp
	$(RM) -f $(INCLUDE_DIR)/com-api/AAF.h
	$(RM) -f $(INCLUDE_DIR)/com-api/AAFTypes.h
	$(RM) -f $(INCLUDE_DIR)/com-api/AAFPluginTypes.h
	$(RM) -f $(INCLUDE_DIR)/com-api/AAFModuleTest.h
	$(RM) -f $(INCLUDE_DIR)/com-api/AAF.idl
	$(RM) -f $(INCLUDE_DIR)/com-api/AAFTypes.idl
	$(RM) -f $(INCLUDE_DIR)/com-api/AAFPluginTypes.idl
	$(RM) -f $(INCLUDE_DIR)/com-api/AAFModuleTest.idl
	$(RM) -f $(INCLUDE_DIR)/ref-api/AAF.h
	$(RM) -f $(INCLUDE_DIR)/ref-api/AAFTypes.h
	$(RM) -f $(INCLUDE_DIR)/ref-api/AAFModuleTest.h
	$(RM) -f $(INCLUDE_DIR)/ref-api/AAFPluginTypes.h
	$(RM) -f $(INCLUDE_DIR)/ref-api/AAFPlugin.h
	$(RM) -f $(IMPL_DIR)/AAFClassIDs.h
	$(RM) -f $(UUID_DIR)/AAF_i.c
	$(RM) -f $(UUID_DIR)/AAFPlugin_i.c
	$(RM) -f $(COMAPI_DIR)/AAFCLSIDs.h
	@for file in $(AUTO_GEN_IMPL) ; do \
		echo $(RM) -f $(SRC_DIR)/impl/Impl$$file.cpp ; \
		$(RM) -f $(SRC_DIR)/impl/Impl$$file.cpp ; \
		echo $(RM) -f $(SRC_DIR)/impl/Impl$$file.h ; \
		$(RM) -f $(SRC_DIR)/impl/Impl$$file.h ; \
		echo $(RM) -f $(SRC_DIR)/com-api/test/C$${file}Test.cpp ; \
		$(RM) -f $(SRC_DIR)/com-api/test/C$${file}Test.cpp ; \
	done

# This file contains the list of all dependents...
include depend.mk

