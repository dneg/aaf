#################################################
#                                               #
# Copyright (c) 1998-1999 Avid Technology, Inc. #
#                                               #
#################################################

all : targets

AAFBASE ?= ..

BLD_CFG_DIR = $(AAFBASE)/ref-impl/bld_cfg
PLUGIN_DIR = $(AAFBASE)/ref-impl/plugins

include $(BLD_CFG_DIR)/common.mk

PACKAGE = dodoTool
include $(AAFBASE)/build/defs.mk

DODO = $(BINDIR)/dodo

dodo-app:
	cd tool ;  $(MAKE)

test_only:
	cd tool ; $(MAKE) test_only

run:
	 cd tool ; $(MAKE) run

.SUFFIXES: .cpp .h .comc .comcx .comh .dod .exp .idl .fidl .implc .implh .comt .cppt .refh .frefh

# This file contains the list of all of the targets to be built...
include targets.mk
include aafobjects_gnu.mk


INCLUDE_DIR = ../ref-impl/include
IMPL_DIR = ../ref-impl/src/impl
UUID_DIR = ../ref-impl/include/ref-api
COMAPI_DIR = ../ref-impl/src/com-api
TEST_DIR = ../test/com/ComModTestAAF/ModuleTests

targets: $(DODO_TARGETS)
targets: $(PLUGIN_TARGETS)
targets: $(INCLUDE_DIR)/com-api/AAF.idl
targets: $(PLUGIN_DIR)/AAFPlugin.idl
targets: $(INCLUDE_DIR)/ref-api/AAF.h
targets: $(UUID_DIR)/AAFRoot_i.c
targets: $(UUID_DIR)/AAF_i.c
targets: $(INCLUDE_DIR)/ref-api/AAFPlugin.h
targets: $(UUID_DIR)/AAFPlugin_i.c
targets: $(COMAPI_DIR)/AAFCLSIDs.h
targets: $(IMPL_DIR)/AAFClassIDs.h
targets: $(COMAPI_DIR)/AAFObjectTable.h
targets: $(COMAPI_DIR)/AAFObjectTable_i.cpp


$(INCLUDE_DIR)/com-api/AAF.idl : $(FIDL_TARGETS)
	@ echo Generating AAF.idl...
	$(RM) -f $(INCLUDE_DIR)/com-api/AAF.idl
	@ ( echo "cpp_quote(\"//=--------------------------------------------------------------------------=\")" ; \
	    echo "cpp_quote(\"// (C) Copyright 1998-1999 Avid Technology.\")" ; \
	    echo "cpp_quote(\"//\")" ; \
	    echo "cpp_quote(\"// This file was GENERATED for the AAF SDK on \")" ; \
	    echo "cpp_quote(\"//  `date`\")" ; \
	    echo "cpp_quote(\"//\")" ; \
	    echo "cpp_quote(\"// Permission to use, copy and modify this software and accompanying \")" ; \
	    echo "cpp_quote(\"// documentation, and to distribute and sublicense application software \")" ; \
	    echo "cpp_quote(\"// incorporating this software for any purpose is hereby granted, \")" ; \
	    echo "cpp_quote(\"// provided that (i) the above copyright notice and this permission \")" ; \
	    echo "cpp_quote(\"// notice appear in all copies of the software and related documentation, \")" ; \
	    echo "cpp_quote(\"// and (ii) the name Avid Technology, Inc. may not be used in any \")" ; \
	    echo "cpp_quote(\"// advertising or publicity relating to the software without the specific, \")" ; \
	    echo "cpp_quote(\"// prior written permission of Avid Technology, Inc. \")" ; \
	    echo "cpp_quote(\"//\")" ; \
	    echo "cpp_quote(\"// THE SOFTWARE IS PROVIDED AS-IS AND WITHOUT WARRANTY OF ANY KIND, \")" ; \
	    echo "cpp_quote(\"// EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY \")" ; \
	    echo "cpp_quote(\"// WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE. \")" ; \
	    echo "cpp_quote(\"// IN NO EVENT SHALL AVID TECHNOLOGY, INC. BE LIABLE FOR ANY DIRECT, \")" ; \
	    echo "cpp_quote(\"// SPECIAL, INCIDENTAL, PUNITIVE, INDIRECT, ECONOMIC, CONSEQUENTIAL OR \")" ; \
	    echo "cpp_quote(\"// OTHER DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER ARISING OUT OF \")" ; \
	    echo "cpp_quote(\"// OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE AND \")" ; \
	    echo "cpp_quote(\"// ACCOMPANYING DOCUMENTATION, INCLUDING, WITHOUT LIMITATION, DAMAGES \")" ; \
	    echo "cpp_quote(\"// RESULTING FROM LOSS OF USE, DATA OR PROFITS, AND WHETHER OR NOT \")" ; \
	    echo "cpp_quote(\"// ADVISED OF THE POSSIBILITY OF DAMAGE, REGARDLESS OF THE THEORY OF \")" ; \
	    echo "cpp_quote(\"// LIABILITY. \")" ; \
	    echo "cpp_quote(\"// \")" ; \
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
	    echo "cpp_quote(\"//\")" ; \
	    echo "cpp_quote(\"// This file was GENERATED for the AAF SDK on \")" ; \
	    echo "cpp_quote(\"//  `date`\")" ; \
	    echo "cpp_quote(\"//\")" ; \
	    echo "cpp_quote(\"// Permission to use, copy and modify this software and accompanying \")" ; \
	    echo "cpp_quote(\"// documentation, and to distribute and sublicense application software \")" ; \
	    echo "cpp_quote(\"// incorporating this software for any purpose is hereby granted, \")" ; \
	    echo "cpp_quote(\"// provided that (i) the above copyright notice and this permission \")" ; \
	    echo "cpp_quote(\"// notice appear in all copies of the software and related documentation, \")" ; \
	    echo "cpp_quote(\"// and (ii) the name Avid Technology, Inc. may not be used in any \")" ; \
	    echo "cpp_quote(\"// advertising or publicity relating to the software without the specific, \")" ; \
	    echo "cpp_quote(\"// prior written permission of Avid Technology, Inc. \")" ; \
	    echo "cpp_quote(\"//\")" ; \
	    echo "cpp_quote(\"// THE SOFTWARE IS PROVIDED AS-IS AND WITHOUT WARRANTY OF ANY KIND, \")" ; \
	    echo "cpp_quote(\"// EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY \")" ; \
	    echo "cpp_quote(\"// WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE. \")" ; \
	    echo "cpp_quote(\"// IN NO EVENT SHALL AVID TECHNOLOGY, INC. BE LIABLE FOR ANY DIRECT, \")" ; \
	    echo "cpp_quote(\"// SPECIAL, INCIDENTAL, PUNITIVE, INDIRECT, ECONOMIC, CONSEQUENTIAL OR \")" ; \
	    echo "cpp_quote(\"// OTHER DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER ARISING OUT OF \")" ; \
	    echo "cpp_quote(\"// OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE AND \")" ; \
	    echo "cpp_quote(\"// ACCOMPANYING DOCUMENTATION, INCLUDING, WITHOUT LIMITATION, DAMAGES \")" ; \
	    echo "cpp_quote(\"// RESULTING FROM LOSS OF USE, DATA OR PROFITS, AND WHETHER OR NOT \")" ; \
	    echo "cpp_quote(\"// ADVISED OF THE POSSIBILITY OF DAMAGE, REGARDLESS OF THE THEORY OF \")" ; \
	    echo "cpp_quote(\"// LIABILITY. \")" ; \
	    echo "cpp_quote(\"// \")" ; \
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
	    echo "//" ; \
	    echo "// This file was GENERATED for the AAF SDK on " ; \
	    echo "//  `date`" ; \
	    echo "//" ; \
	    echo "// Permission to use, copy and modify this software and accompanying " ; \
	    echo "// documentation, and to distribute and sublicense application software " ; \
	    echo "// incorporating this software for any purpose is hereby granted, " ; \
	    echo "// provided that (i) the above copyright notice and this permission " ; \
	    echo "// notice appear in all copies of the software and related documentation, " ; \
	    echo "// and (ii) the name Avid Technology, Inc. may not be used in any " ; \
	    echo "// advertising or publicity relating to the software without the specific, " ; \
	    echo "// prior written permission of Avid Technology, Inc. " ; \
	    echo "//" ; \
	    echo "// THE SOFTWARE IS PROVIDED AS-IS AND WITHOUT WARRANTY OF ANY KIND, " ; \
	    echo "// EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY " ; \
	    echo "// WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE. " ; \
	    echo "// IN NO EVENT SHALL AVID TECHNOLOGY, INC. BE LIABLE FOR ANY DIRECT, " ; \
	    echo "// SPECIAL, INCIDENTAL, PUNITIVE, INDIRECT, ECONOMIC, CONSEQUENTIAL OR " ; \
	    echo "// OTHER DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER ARISING OUT OF " ; \
	    echo "// OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE AND " ; \
	    echo "// ACCOMPANYING DOCUMENTATION, INCLUDING, WITHOUT LIMITATION, DAMAGES " ; \
	    echo "// RESULTING FROM LOSS OF USE, DATA OR PROFITS, AND WHETHER OR NOT " ; \
	    echo "// ADVISED OF THE POSSIBILITY OF DAMAGE, REGARDLESS OF THE THEORY OF " ; \
	    echo "// LIABILITY. " ; \
	    echo "// " ; \
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
	    echo "//" ; \
	    echo "// This file was GENERATED for the AAF SDK on " ; \
	    echo "//  "`date` ; \
	    echo "//" ; \
	    echo "// Permission to use, copy and modify this software and accompanying " ; \
	    echo "// documentation, and to distribute and sublicense application software " ; \
	    echo "// incorporating this software for any purpose is hereby granted, " ; \
	    echo "// provided that (i) the above copyright notice and this permission " ; \
	    echo "// notice appear in all copies of the software and related documentation, " ; \
	    echo "// and (ii) the name Avid Technology, Inc. may not be used in any " ; \
	    echo "// advertising or publicity relating to the software without the specific, " ; \
	    echo "// prior written permission of Avid Technology, Inc. " ; \
	    echo "//" ; \
	    echo "// THE SOFTWARE IS PROVIDED AS-IS AND WITHOUT WARRANTY OF ANY KIND, " ; \
	    echo "// EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY " ; \
	    echo "// WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE. " ; \
	    echo "// IN NO EVENT SHALL AVID TECHNOLOGY, INC. BE LIABLE FOR ANY DIRECT, " ; \
	    echo "// SPECIAL, INCIDENTAL, PUNITIVE, INDIRECT, ECONOMIC, CONSEQUENTIAL OR " ; \
	    echo "// OTHER DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER ARISING OUT OF " ; \
	    echo "// OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE AND " ; \
	    echo "// ACCOMPANYING DOCUMENTATION, INCLUDING, WITHOUT LIMITATION, DAMAGES " ; \
	    echo "// RESULTING FROM LOSS OF USE, DATA OR PROFITS, AND WHETHER OR NOT " ; \
	    echo "// ADVISED OF THE POSSIBILITY OF DAMAGE, REGARDLESS OF THE THEORY OF " ; \
	    echo "// LIABILITY. " ; \
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


$(UUID_DIR)/AAF_i.c : aafobjects_gnu.mk dod2iid.awk
	@ echo Generating reference AAF_i.c...
	$(RM) -f $(UUID_DIR)/AAF_i.c
	@ ( echo "/* this file contains the actual definitions of */" ; \
	    echo "/* the IIDs and CLSIDs */" ; \
	    echo "" ; \
	    echo "/* link this file in with the server and any clients */" ; \
	    echo "//=--------------------------------------------------------------------------=" ; \
	    echo "// (C) Copyright 1998-1999 Avid Technology." ; \
	    echo "//" ; \
	    echo "// This file was GENERATED for the AAF SDK on " ; \
	    echo "//  "`date` ; \
	    echo "//" ; \
	    echo "// Permission to use, copy and modify this software and accompanying " ; \
	    echo "// documentation, and to distribute and sublicense application software " ; \
	    echo "// incorporating this software for any purpose is hereby granted, " ; \
	    echo "// provided that (i) the above copyright notice and this permission " ; \
	    echo "// notice appear in all copies of the software and related documentation, " ; \
	    echo "// and (ii) the name Avid Technology, Inc. may not be used in any " ; \
	    echo "// advertising or publicity relating to the software without the specific, " ; \
	    echo "// prior written permission of Avid Technology, Inc. " ; \
	    echo "//" ; \
	    echo "// THE SOFTWARE IS PROVIDED AS-IS AND WITHOUT WARRANTY OF ANY KIND, " ; \
	    echo "// EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY " ; \
	    echo "// WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE. " ; \
	    echo "// IN NO EVENT SHALL AVID TECHNOLOGY, INC. BE LIABLE FOR ANY DIRECT, " ; \
	    echo "// SPECIAL, INCIDENTAL, PUNITIVE, INDIRECT, ECONOMIC, CONSEQUENTIAL OR " ; \
	    echo "// OTHER DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER ARISING OUT OF " ; \
	    echo "// OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE AND " ; \
	    echo "// ACCOMPANYING DOCUMENTATION, INCLUDING, WITHOUT LIMITATION, DAMAGES " ; \
	    echo "// RESULTING FROM LOSS OF USE, DATA OR PROFITS, AND WHETHER OR NOT " ; \
	    echo "// ADVISED OF THE POSSIBILITY OF DAMAGE, REGARDLESS OF THE THEORY OF " ; \
	    echo "// LIABILITY. " ; \
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


$(UUID_DIR)/AAFPlugin_i.c : aafobjects_gnu.mk dod2iid.awk
	@ echo Generating reference AAFPlugin_i.c...
	$(RM) -f $(UUID_DIR)/AAFPlugin_i.c
	@ ( echo "/* this file contains the actual definitions of */" ; \
	    echo "/* the IIDs and CLSIDs */" ; \
	    echo "" ; \
	    echo "/* link this file in with the server and any clients */" ; \
	    echo "//=--------------------------------------------------------------------------=" ; \
	    echo "// (C) Copyright 1998-1999 Avid Technology." ; \
	    echo "//" ; \
	    echo "// This file was GENERATED for the AAF SDK on " ; \
	    echo "//  "`date` ; \
	    echo "//" ; \
	    echo "// Permission to use, copy and modify this software and accompanying " ; \
	    echo "// documentation, and to distribute and sublicense application software " ; \
	    echo "// incorporating this software for any purpose is hereby granted, " ; \
	    echo "// provided that (i) the above copyright notice and this permission " ; \
	    echo "// notice appear in all copies of the software and related documentation, " ; \
	    echo "// and (ii) the name Avid Technology, Inc. may not be used in any " ; \
	    echo "// advertising or publicity relating to the software without the specific, " ; \
	    echo "// prior written permission of Avid Technology, Inc. " ; \
	    echo "//" ; \
	    echo "// THE SOFTWARE IS PROVIDED AS-IS AND WITHOUT WARRANTY OF ANY KIND, " ; \
	    echo "// EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY " ; \
	    echo "// WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE. " ; \
	    echo "// IN NO EVENT SHALL AVID TECHNOLOGY, INC. BE LIABLE FOR ANY DIRECT, " ; \
	    echo "// SPECIAL, INCIDENTAL, PUNITIVE, INDIRECT, ECONOMIC, CONSEQUENTIAL OR " ; \
	    echo "// OTHER DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER ARISING OUT OF " ; \
	    echo "// OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE AND " ; \
	    echo "// ACCOMPANYING DOCUMENTATION, INCLUDING, WITHOUT LIMITATION, DAMAGES " ; \
	    echo "// RESULTING FROM LOSS OF USE, DATA OR PROFITS, AND WHETHER OR NOT " ; \
	    echo "// ADVISED OF THE POSSIBILITY OF DAMAGE, REGARDLESS OF THE THEORY OF " ; \
	    echo "// LIABILITY. " ; \
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


$(UUID_DIR)/AAFRoot_i.c : aafobjects_gnu.mk dod2iid.awk
	@ echo Generating reference AAFRoot_i.c...
	$(RM) -f $(UUID_DIR)/AAFRoot_i.c
	@ ( echo "/* this file contains the actual definitions of */" ; \
	    echo "/* the IIDs and CLSIDs */" ; \
	    echo "" ; \
	    echo "/* link this file in with the server and any clients */" ; \
	    echo "//=--------------------------------------------------------------------------=" ; \
	    echo "// (C) Copyright 1998-1999 Avid Technology." ; \
	    echo "//" ; \
	    echo "// This file was GENERATED for the AAF SDK on " ; \
	    echo "//  "`date` ; \
	    echo "//" ; \
	    echo "// Permission to use, copy and modify this software and accompanying " ; \
	    echo "// documentation, and to distribute and sublicense application software " ; \
	    echo "// incorporating this software for any purpose is hereby granted, " ; \
	    echo "// provided that (i) the above copyright notice and this permission " ; \
	    echo "// notice appear in all copies of the software and related documentation, " ; \
	    echo "// and (ii) the name Avid Technology, Inc. may not be used in any " ; \
	    echo "// advertising or publicity relating to the software without the specific, " ; \
	    echo "// prior written permission of Avid Technology, Inc. " ; \
	    echo "//" ; \
	    echo "// THE SOFTWARE IS PROVIDED AS-IS AND WITHOUT WARRANTY OF ANY KIND, " ; \
	    echo "// EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY " ; \
	    echo "// WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE. " ; \
	    echo "// IN NO EVENT SHALL AVID TECHNOLOGY, INC. BE LIABLE FOR ANY DIRECT, " ; \
	    echo "// SPECIAL, INCIDENTAL, PUNITIVE, INDIRECT, ECONOMIC, CONSEQUENTIAL OR " ; \
	    echo "// OTHER DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER ARISING OUT OF " ; \
	    echo "// OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE AND " ; \
	    echo "// ACCOMPANYING DOCUMENTATION, INCLUDING, WITHOUT LIMITATION, DAMAGES " ; \
	    echo "// RESULTING FROM LOSS OF USE, DATA OR PROFITS, AND WHETHER OR NOT " ; \
	    echo "// ADVISED OF THE POSSIBILITY OF DAMAGE, REGARDLESS OF THE THEORY OF " ; \
	    echo "// LIABILITY. " ; \
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
	    for class in AAFRoot ; do \
	    	awk -f dod2iid.awk C=$$class $$class.dod ; \
	    done ; \
	    echo \#ifdef __cplusplus ; \
	    echo "}" ; \
	    echo \#endif ; \
	    echo "" ; \
	) > $(UUID_DIR)/AAFRoot_i.c
	chmod -w $(UUID_DIR)/AAFRoot_i.c


$(IMPL_DIR)/AAFClassIDs.h : aafobjects_gnu.mk
	@ echo Generating reference AAFClassIDs.h...
	$(RM) -f $(IMPL_DIR)/AAFClassIDs.h
	@ ( echo "//=--------------------------------------------------------------------------=" ; \
	    echo "// (C) Copyright 1998-1999 Avid Technology." ; \
	    echo "//" ; \
	    echo "// This file was GENERATED for the AAF SDK on " ; \
	    echo "//  "`date` ; \
	    echo "//" ; \
	    echo "// Permission to use, copy and modify this software and accompanying " ; \
	    echo "// documentation, and to distribute and sublicense application software " ; \
	    echo "// incorporating this software for any purpose is hereby granted, " ; \
	    echo "// provided that (i) the above copyright notice and this permission " ; \
	    echo "// notice appear in all copies of the software and related documentation, " ; \
	    echo "// and (ii) the name Avid Technology, Inc. may not be used in any " ; \
	    echo "// advertising or publicity relating to the software without the specific, " ; \
	    echo "// prior written permission of Avid Technology, Inc. " ; \
	    echo "//" ; \
	    echo "// THE SOFTWARE IS PROVIDED AS-IS AND WITHOUT WARRANTY OF ANY KIND, " ; \
	    echo "// EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY " ; \
	    echo "// WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE. " ; \
	    echo "// IN NO EVENT SHALL AVID TECHNOLOGY, INC. BE LIABLE FOR ANY DIRECT, " ; \
	    echo "// SPECIAL, INCIDENTAL, PUNITIVE, INDIRECT, ECONOMIC, CONSEQUENTIAL OR " ; \
	    echo "// OTHER DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER ARISING OUT OF " ; \
	    echo "// OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE AND " ; \
	    echo "// ACCOMPANYING DOCUMENTATION, INCLUDING, WITHOUT LIMITATION, DAMAGES " ; \
	    echo "// RESULTING FROM LOSS OF USE, DATA OR PROFITS, AND WHETHER OR NOT " ; \
	    echo "// ADVISED OF THE POSSIBILITY OF DAMAGE, REGARDLESS OF THE THEORY OF " ; \
	    echo "// LIABILITY. " ; \
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
	    for class in $(AAFOBJECTS) ; do \
	    	echo "extern \"C\" const aafClassID_t CLSID_$$class;"; \
	    done ; \
	    echo "" ; \
	    echo \#endif // __AAFClassIDs_h__ ; \
	) > $(IMPL_DIR)/AAFClassIDs.h
	chmod -w $(IMPL_DIR)/AAFClassIDs.h


$(COMAPI_DIR)/AAFCLSIDs.h : aafobjects_gnu.mk
	@ echo Generating reference AAFCLSIDs.h...
	$(RM) -f $(COMAPI_DIR)/AAFCLSIDs.h
	@ ( echo "//=--------------------------------------------------------------------------=" ; \
	    echo "// (C) Copyright 1998-1999 Avid Technology." ; \
	    echo "//" ; \
	    echo "// This file was GENERATED for the AAF SDK on " ; \
	    echo "//  "`date` ; \
	    echo "//" ; \
	    echo "// Permission to use, copy and modify this software and accompanying " ; \
	    echo "// documentation, and to distribute and sublicense application software " ; \
	    echo "// incorporating this software for any purpose is hereby granted, " ; \
	    echo "// provided that (i) the above copyright notice and this permission " ; \
	    echo "// notice appear in all copies of the software and related documentation, " ; \
	    echo "// and (ii) the name Avid Technology, Inc. may not be used in any " ; \
	    echo "// advertising or publicity relating to the software without the specific, " ; \
	    echo "// prior written permission of Avid Technology, Inc. " ; \
	    echo "//" ; \
	    echo "// THE SOFTWARE IS PROVIDED AS-IS AND WITHOUT WARRANTY OF ANY KIND, " ; \
	    echo "// EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY " ; \
	    echo "// WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE. " ; \
	    echo "// IN NO EVENT SHALL AVID TECHNOLOGY, INC. BE LIABLE FOR ANY DIRECT, " ; \
	    echo "// SPECIAL, INCIDENTAL, PUNITIVE, INDIRECT, ECONOMIC, CONSEQUENTIAL OR " ; \
	    echo "// OTHER DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER ARISING OUT OF " ; \
	    echo "// OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE AND " ; \
	    echo "// ACCOMPANYING DOCUMENTATION, INCLUDING, WITHOUT LIMITATION, DAMAGES " ; \
	    echo "// RESULTING FROM LOSS OF USE, DATA OR PROFITS, AND WHETHER OR NOT " ; \
	    echo "// ADVISED OF THE POSSIBILITY OF DAMAGE, REGARDLESS OF THE THEORY OF " ; \
	    echo "// LIABILITY. " ; \
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
	    for class in $(AAFOBJECTS) ; do \
	    	echo "EXTERN_C const CLSID CLSID_$$class;"; \
	    done ; \
	    echo "" ; \
	    echo \#endif // __AAFCLSIDs_h__ ; \
	) > $(COMAPI_DIR)/AAFCLSIDs.h
	chmod -w $(COMAPI_DIR)/AAFCLSIDs.h


$(COMAPI_DIR)/AAFObjectTable.h : aafobjects_gnu.mk
	@ echo Generating reference AAFObjectTable.h...
	@ $(RM) -f $(COMAPI_DIR)/AAFObjectTable.tmp
	@ ( echo "//=--------------------------------------------------------------------------=" ; \
	    echo "// (C) Copyright 1998-1999 Avid Technology." ; \
	    echo "//" ; \
	    echo "// This file was GENERATED for the AAF SDK on " ; \
	    echo "//  "`date` ; \
	    echo "//" ; \
	    echo "// Permission to use, copy and modify this software and accompanying " ; \
	    echo "// documentation, and to distribute and sublicense application software " ; \
	    echo "// incorporating this software for any purpose is hereby granted, " ; \
	    echo "// provided that (i) the above copyright notice and this permission " ; \
	    echo "// notice appear in all copies of the software and related documentation, " ; \
	    echo "// and (ii) the name Avid Technology, Inc. may not be used in any " ; \
	    echo "// advertising or publicity relating to the software without the specific, " ; \
	    echo "// prior written permission of Avid Technology, Inc. " ; \
	    echo "//" ; \
	    echo "// THE SOFTWARE IS PROVIDED AS-IS AND WITHOUT WARRANTY OF ANY KIND, " ; \
	    echo "// EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY " ; \
	    echo "// WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE. " ; \
	    echo "// IN NO EVENT SHALL AVID TECHNOLOGY, INC. BE LIABLE FOR ANY DIRECT, " ; \
	    echo "// SPECIAL, INCIDENTAL, PUNITIVE, INDIRECT, ECONOMIC, CONSEQUENTIAL OR " ; \
	    echo "// OTHER DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER ARISING OUT OF " ; \
	    echo "// OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE AND " ; \
	    echo "// ACCOMPANYING DOCUMENTATION, INCLUDING, WITHOUT LIMITATION, DAMAGES " ; \
	    echo "// RESULTING FROM LOSS OF USE, DATA OR PROFITS, AND WHETHER OR NOT " ; \
	    echo "// ADVISED OF THE POSSIBILITY OF DAMAGE, REGARDLESS OF THE THEORY OF " ; \
	    echo "// LIABILITY. " ; \
	    echo "//=--------------------------------------------------------------------------=" ; \
	    echo "// This file contains invocations of the macros described below." ; \
	    echo "//" ; \
	    echo "// To use this file -" ; \
	    echo "//   1) #define the macros to suit your usage" ; \
	    echo "//   2) #include this file" ; \
	    echo "//   3) #undef the macros" ; \
	    echo "" ; \
	    echo "// Default empty definitions so that you only have to define" ; \
	    echo "// those macros you actually want to use." ; \
	    echo "//" ; \
	    echo \#ifndef AAF_BEGIN_OBJECT_MAP ; \
	    echo \#define AAF_BEGIN_OBJECT_MAP\(x\) ; \
	    echo \#endif ; \
	    echo "" ; \
	    echo \#ifndef AAF_END_OBJECT_MAP ; \
	    echo \#define AAF_END_OBJECT_MAP\(\) ; \
	    echo \#endif ; \
	    echo "" ; \
	    echo \#ifndef AAF_END_OBJECT_MAP ; \
	    echo \#define AAF_OBJECT_ENTRY\(name\) ; \
	    echo \#endif ; \
	    echo "" ; \
	    echo "" ; \
	    echo "//" ; \
	    echo "// Include all objects in the following table:" ; \
	    echo "//" ; \
	    echo AAF_BEGIN_OBJECT_MAP\(AAFObjectMap\) ; \
	    for AAF in ${AAFOBJECTS} ; do \
		echo "	AAF_OBJECT_ENTRY("$$AAF")" ; \
	    done ; \
	    echo AAF_END_OBJECT_MAP\(\) ; \
	) > $(COMAPI_DIR)/AAFObjectTable.tmp
	$(RM) -f $(COMAPI_DIR)/AAFObjectTable.h
	mv $(COMAPI_DIR)/AAFObjectTable.tmp $(COMAPI_DIR)/AAFObjectTable.h
	chmod -w $(COMAPI_DIR)/AAFObjectTable.h


$(COMAPI_DIR)/AAFObjectTable_i.cpp : aafobjects_gnu.mk
	@ echo Generating reference AAFObjectTable_i.cpp...
	@ $(RM) -f $(COMAPI_DIR)/AAFObjectTable_i.tmp
	@ ( echo "//=--------------------------------------------------------------------------=" ; \
	    echo "// (C) Copyright 1998-1999 Avid Technology." ; \
	    echo "//" ; \
	    echo "// This file was GENERATED for the AAF SDK on " ; \
	    echo "//  "`date` ; \
	    echo "//" ; \
	    echo "// Permission to use, copy and modify this software and accompanying " ; \
	    echo "// documentation, and to distribute and sublicense application software " ; \
	    echo "// incorporating this software for any purpose is hereby granted, " ; \
	    echo "// provided that (i) the above copyright notice and this permission " ; \
	    echo "// notice appear in all copies of the software and related documentation, " ; \
	    echo "// and (ii) the name Avid Technology, Inc. may not be used in any " ; \
	    echo "// advertising or publicity relating to the software without the specific, " ; \
	    echo "// prior written permission of Avid Technology, Inc. " ; \
	    echo "//" ; \
	    echo "// THE SOFTWARE IS PROVIDED AS-IS AND WITHOUT WARRANTY OF ANY KIND, " ; \
	    echo "// EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY " ; \
	    echo "// WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE. " ; \
	    echo "// IN NO EVENT SHALL AVID TECHNOLOGY, INC. BE LIABLE FOR ANY DIRECT, " ; \
	    echo "// SPECIAL, INCIDENTAL, PUNITIVE, INDIRECT, ECONOMIC, CONSEQUENTIAL OR " ; \
	    echo "// OTHER DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER ARISING OUT OF " ; \
	    echo "// OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE AND " ; \
	    echo "// ACCOMPANYING DOCUMENTATION, INCLUDING, WITHOUT LIMITATION, DAMAGES " ; \
	    echo "// RESULTING FROM LOSS OF USE, DATA OR PROFITS, AND WHETHER OR NOT " ; \
	    echo "// ADVISED OF THE POSSIBILITY OF DAMAGE, REGARDLESS OF THE THEORY OF " ; \
	    echo "// LIABILITY. " ; \
	    echo "//=--------------------------------------------------------------------------=" ; \
	    echo "" ; \
	    echo "//" ; \
	    echo "// Include the CLSID declarations..." ; \
	    echo "//" ; \
	    echo \#include \"AAFCLSIDs.h\" ; \
	    echo "" ; \
	    echo "//" ; \
	    echo "// Include the appropriate header files:" ; \
	    echo "//" ; \
	    for AAF in ${AAFOBJECTS} ; do \
	        echo \#include \"C$$AAF.h\" ; \
	    done ; \
	    echo "" ; \
	    echo "//" ; \
	    echo "// Include AAF Object Table:" ; \
	    echo "//" ; \
	    echo \#include \"AAFObjectTable.h\" ; \
	) > $(COMAPI_DIR)/AAFObjectTable_i.tmp
	$(RM) -f $(COMAPI_DIR)/AAFObjectTable_i.cpp
	mv $(COMAPI_DIR)/AAFObjectTable_i.tmp $(COMAPI_DIR)/AAFObjectTable_i.cpp
	chmod -w $(COMAPI_DIR)/AAFObjectTable_i.cpp



SRC_DIR = ../ref-impl/src

.dod.exp :
	$(RM) -f $*.exp
	$(DODO) -f macros/exp.mac < $*.dod > $*.tmp
	mv $*.tmp $*.exp
	chmod -w $*.exp

.dod.h :
	$(RM) -f $*.h
	$(DODO) -f macros/h.mac < $*.dod > $*.tmp
	mv $*.tmp $*.h
	chmod -w $*.h
	cp -f $*.h $(INCLUDE_DIR)/cpp-api/
	chmod -w $(INCLUDE_DIR)/cpp-api/$*.h

.dod.cppt :
	$(RM) -f $*.cppt
	$(DODO) -f macros/cppt.mac < $*.dod > $*.tmp
	mv $*.tmp $*.cppt
	chmod -w $*.cppt
	cp -f $*.cppt $(TEST_DIR)/$*Test.cpp
	chmod -w $(TEST_DIR)//$*Test.cpp

.dod.comh :
	$(RM) -f $*.comh
	$(DODO) -f macros/comh.mac < $*.dod > $*.tmp
	mv $*.tmp $*.comh
	chmod -w $*.comh
	cp -f $*.comh $(SRC_DIR)/com-api/C$*.h
	chmod -w $(SRC_DIR)/com-api/C$*.h

.dod.comc :
	$(RM) -f $*.comc
	$(DODO) -f macros/comc.mac < $*.dod > $*.tmp
	mv $*.tmp $*.comc
	chmod -w $*.comc
	cp -f $*.comc $(SRC_DIR)/com-api/C$*.cpp
	chmod -w $(SRC_DIR)/com-api/C$*.cpp

.dod.comcx :
	$(RM) -f $*.comcx
	$(DODO) -f macros/comcx.mac < $*.dod > $*.tmp
	mv $*.tmp $*.comcx
	chmod -w $*.comcx
	cp -f $*.comcx $(SRC_DIR)/com-api/C$*.cpp
	chmod -w $(SRC_DIR)/com-api/C$*.cpp

.dod.comt :
	$(RM) -f $*.comt
	$(DODO) -f macros/comt.mac < $*.dod > $*.tmp
	mv $*.tmp $*.comt
	chmod -w $*.comt
	cp -f $*.comt $(TEST_DIR)/C$*Test.cpp
	chmod -w $(TEST_DIR)/C$*Test.cpp

.dod.implh :
	$(RM) -f $*.implh ;
	$(DODO) -f macros/implh.mac < $*.dod > $*.tmp ;
	mv $*.tmp $*.implh ;
	chmod -w $*.implh ;
	cp -f $*.implh $(SRC_DIR)/impl/Impl$*.h ;
	chmod -w $(SRC_DIR)/impl/Impl$*.h ;

.dod.implc :
	$(RM) -f $*.implc ;
	$(DODO) -f macros/implc.mac < $*.dod > $*.tmp ;
	mv $*.tmp $*.implc ;
	chmod -w $*.implc ;
	cp -f $*.implc $(SRC_DIR)/impl/Impl$*.cpp ;
	chmod -w $(SRC_DIR)/impl/Impl$*.cpp ;

.dod.cpp :
	$(RM) -f $*.cpp
	$(DODO) -f macros/cpp.mac < $*.dod > $*.tmp
	mv $*.tmp $*.cpp
	chmod -w $*.cpp
	cp -f $*.cpp $(SRC_DIR)/cpp-api
	chmod -w $(SRC_DIR)/cpp-api/$*.cpp

.dod.idl :
	$(RM) -f $*.idl
	$(DODO) -f macros/idl.mac < $*.dod > $*.tmp
	mv $*.tmp $*.idl
	chmod -w $*.idl
	cp -f $*.idl $(INCLUDE_DIR)/com-api/
	chmod -w $(INCLUDE_DIR)/com-api/$*.idl

.dod.fidl :
	$(RM) -f $*.fidl
	$(DODO) -f macros/fidl.mac < $*.dod > $*.tmp
	mv $*.tmp $*.fidl
	chmod -w $*.fidl

.dod.refh :
	$(RM) -f $*.refh
	$(DODO) -f macros/refh.mac < $*.dod > $*.tmp
	mv $*.tmp $*.refh
	chmod -w $*.refh
	cp -f $*.refh $(INCLUDE_DIR)/ref-api/$*.h
	chmod -w $(INCLUDE_DIR)/ref-api/$*.h

.dod.frefh :
	$(RM) -f $*.frefh
	$(DODO) -f macros/frefh.mac < $*.dod > $*.tmp
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
	$(RM) -f $(INCLUDE_DIR)/com-api/AAF.h
	$(RM) -f $(INCLUDE_DIR)/com-api/AAFTypes.h
	$(RM) -f $(INCLUDE_DIR)/com-api/AAFPluginTypes.h
	$(RM) -f $(INCLUDE_DIR)/com-api/AAF.idl
	$(RM) -f $(INCLUDE_DIR)/com-api/AAFTypes.idl
	$(RM) -f $(INCLUDE_DIR)/com-api/AAFRoot.idl
	$(RM) -f $(INCLUDE_DIR)/com-api/AAFPluginTypes.idl
	$(RM) -f $(INCLUDE_DIR)/ref-api/AAF.h
	$(RM) -f $(INCLUDE_DIR)/ref-api/AAFTypes.h
	$(RM) -f $(INCLUDE_DIR)/ref-api/AAFRoot.h
	$(RM) -f $(INCLUDE_DIR)/ref-api/AAFPluginTypes.h
	$(RM) -f $(INCLUDE_DIR)/ref-api/AAFPlugin.h
	$(RM) -f $(IMPL_DIR)/AAFClassIDs.h
	$(RM) -f $(UUID_DIR)/AAFRoot_i.c
	$(RM) -f $(UUID_DIR)/AAF_i.c
	$(RM) -f $(UUID_DIR)/AAFPlugin_i.c
	$(RM) -f $(COMAPI_DIR)/AAFCLSIDs.h
	$(RM) -f $(COMAPI_DIR)/AAFObjectTable_i.tmp
	$(RM) -f $(COMAPI_DIR)/AAFObjectTable.h
	$(RM) -f $(COMAPI_DIR)/AAFObjectTable_i.cpp
#	AUTO_GEN_IMPL is empty now
#	@for file in $(AUTO_GEN_IMPL) ; do \
#		echo $(RM) -f $(SRC_DIR)/impl/Impl$$file.cpp ; \
#		$(RM) -f $(SRC_DIR)/impl/Impl$$file.cpp ; \
#		echo $(RM) -f $(SRC_DIR)/impl/Impl$$file.h ; \
#		$(RM) -f $(SRC_DIR)/impl/Impl$$file.h ; \
#		echo $(RM) -f $(TEST_DIR)/C$${file}Test.cpp ; \
#		$(RM) -f $(TEST_DIR)/C$${file}Test.cpp ; \
#	done

# This file contains the list of all dependents...
include depend.mk

