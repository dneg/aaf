###############################################################################
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
# The Original Code of this file is Copyright 1998-2001, Licensor of the
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
INCLUDES = -I$(AAFSDKINCLUDEDIR) \
	   -I../../../ref-impl/src/com-api


# CXXFILES2 is declared just to get the list of object files
# from ModuleTests directory. These object files are created 
# when building target ModuleTests
CXXFILES_TMP = $(wildcard ModuleTests/*.$(CPP_EXTENSION)) $(wildcard ModuleTests/*.c)
CXXFILES2 = $(CXXFILES_TMP:ModuleTests/%=%)


.PHONY : all
all : $(OBJDIR) $(AAFSDKBINDIR)/ComModAAF$(EXE)


$(AAFSDKBINDIR)/ComModAAF$(EXE) : $(CXXOBJS) ModuleTests
	$(LD) $(CXXOBJS) $(CXXOBJS2) \
	-L$(AAFSDKLIBDIR) -laaflib -laafiid -o $@


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
	$(AAFSDKBINDIR)/ComModAAF$(EXE)


include $(AAFBASE)/build/rules.mk

