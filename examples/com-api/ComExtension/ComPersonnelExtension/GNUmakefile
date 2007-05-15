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

PACKAGE = Examples

AAFBASE ?= ../../../..


# Common definitions
include $(AAFBASE)/build/common.mk


# Include directories
INCLUDES = -I$(AAFSDKINCLUDEDIR) \
		-I../../../../ref-impl/src/com-api/com-dll \
		-I..


.PHONY : all

# This example not relevant for static builds
ifeq ($(AAFTARGET),Debug-static)
all :

.PHONY : clean
clean :

else

PERSONNEL_EXTENSION_LIB = libAAFPersonnelExtension$(DLL)
PERSONNEL_EXTENSION_DEF =
ifeq ($(AAFPLATFORM),Win)
  # The library has a different name under Win for historical reasons
  PERSONNEL_EXTENSION_LIB = AAFPersonnelExtension$(DLL)
  PERSONNEL_EXTENSION_DEF = /DEF:AAFPersonnelExtension.def
endif

all : $(OBJDIR) \
		$(AAFSDKBINDIR)/$(PERSONNEL_EXTENSION_LIB) \
		$(AAFSDKBINDIR)/ComPersonnelPluginWrite$(EXE)


CXXFILES = AAFPersonnelExtension.cpp \
		CAAFAdminMob.cpp \
		CAAFPersonnelResource.cpp


SUPOBJS = $(AAFBUILDDIR)/com-api/$(AAFTARGETDIR)/CAAFClassFactory$(OBJ) \
		$(AAFBUILDDIR)/com-api/$(AAFTARGETDIR)/CAAFServer$(OBJ) \
		$(AAFBUILDDIR)/com-api/$(AAFTARGETDIR)/CAAFUnknown$(OBJ) \
		$(OBJDIR)/extensionUtils$(OBJ)

ifeq ($(COMPILER),cl)
  # Avoid "already defined" errors using MSVC
  CXXOBJS:=$(filter-out %/AAFPersonnelExtension_i.obj,$(CXXOBJS))
endif

$(AAFSDKBINDIR)/$(PERSONNEL_EXTENSION_LIB) : $(CXXOBJS)
	$(LD_DYN_LIB) $(SUPOBJS) $(CXXOBJS) $(PERSONNEL_EXTENSION_DEF) $(LINK_AAF_APP)


CXXFILES2 = extensionReadPlugin.cpp \
		extensionWritePlugin.cpp \
		writePlugin.cpp


$(AAFSDKBINDIR)/ComPersonnelPluginWrite$(EXE) : $(CXXOBJS2)
	$(LD) $(CXXOBJS2) $(OBJDIR)/extensionUtils$(OBJ) \
	$(LINK_AAF_APP)


.PHONY : clean
clean :
	@$(RM) $(CXXOBJS)
	@$(RM) $(CXXOBJS2)
	@$(RM) $(DEPS)
	@$(RM) $(AAFSDKBINDIR)/$(PERSONNEL_EXTENSION_LIB)
	@$(RM) $(AAFSDKBINDIR)/ComPersonnelPluginWrite$(EXE)
	@$(RMDIR) $(OBJDIR)/ii_files
endif

.PHONY : realclean
realclean : clean


include $(AAFBASE)/build/rules.mk
