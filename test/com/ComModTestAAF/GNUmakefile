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

PACKAGE = ComModAAF

AAFBASE ?= ../../..


# Common definitions
include $(AAFBASE)/build/common.mk


# Include directories
# NOTE: Client code would never include implementation headers.
# However, to ensure that all classes are tested, a macro from
# ref-impl/src/com-api/AAFObjectTable.h is used by ComModAAF.
# Also, utf8.h is temporarily included from ref-impl.
INCLUDES = -I$(AAFSDKINCLUDEDIR) \
			-I../../../ref-impl/src/com-api \
			-I../../../ref-impl/include


# CXXFILES2 is declared just to get the list of object files
# from ModuleTests directory. These object files are created 
# when building target ModuleTests
CXXFILES_TMP = $(wildcard ModuleTests/*.$(CPP_EXTENSION)) $(wildcard ModuleTests/*.c)
CXXFILES2 = $(CXXFILES_TMP:ModuleTests/%=%)


.PHONY : all
all : ModuleTests $(AAFSDKBINDIR)/ComModAAF$(EXE)


ifeq ($(AAFTARGET),Debug-static)
$(AAFSDKBINDIR)/ComModAAF$(EXE) : $(CXXOBJS) $(CXXOBJS2)
	$(LD) $(CXXOBJS) $(CXXOBJS2) $(STATIC_LINK_LINE) -o $@
else
$(AAFSDKBINDIR)/ComModAAF$(EXE) : $(CXXOBJS) $(CXXOBJS2)
	$(LD) $(CXXOBJS) $(CXXOBJS2) \
	-L$(AAFSDKLIBDIR) -laaflib -laafiid $(PLATFORMLIBS) $(LIBCIO) -o $@
endif

.PHONY : ModuleTests
ModuleTests :
	@echo "\nBuilding ModuleTests\n"
	cd ModuleTests && $(MAKE)


.PHONY : clean
clean :
	cd ModuleTests && $(MAKE) GENDEPS=0 $@
	@$(RM) $(CXXOBJS)
	@$(RM) $(DEPS)
	@$(RM) $(AAFSDKBINDIR)/ComModAAF$(EXE)
	@$(RMDIR) $(OBJDIR)/ii_files


.PHONY : realclean
realclean : clean
	cd ModuleTests && $(MAKE) GENDEPS=0 $@


.PHONY : check
check : $(AAFSDKBINDIR)/ComModAAF$(EXE)
	env LD_LIBRARY_PATH=$(AAFSDKBINDIR):$(LD_LIBRARY_PATH) $(AAFSDKBINDIR)/ComModAAF$(EXE)


include $(AAFBASE)/build/rules.mk
