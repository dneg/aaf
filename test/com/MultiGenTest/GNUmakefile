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

PACKAGE = MultiGenTest

AAFBASE ?= ../../..


# Common definitions
include $(AAFBASE)/build/common.mk


# Include directories
INCLUDES = -I$(AAFSDKINCLUDEDIR) \
	   -I../../../ref-impl/src/com-api


# CXXFILES2 is declared just to get the list of object files
# from MultiGenTests directory. These object files are created 
# when building target MultiGenTests
CXXFILES_TMP = $(wildcard MultiGenTests/*.$(CPP_EXTENSION)) $(wildcard MultiGenTests/*.c)
CXXFILES2 = $(CXXFILES_TMP:MultiGenTests/%=%)


.PHONY : all
#all : $(OBJDIR) $(AAFSDKBINDIR)/MultiGenTest$(EXE)
all : $(AAFSDKBINDIR)/MultiGenTest$(EXE)


# $(AAFSDKBINDIR)/MultiGenTest$(EXE) : $(CXXOBJS) MultiGenTests
$(AAFSDKBINDIR)/MultiGenTest$(EXE) : $(CXXOBJS) $(CXXOBJS2)
	$(LD) $(CXXOBJS) $(CXXOBJS2) \
	-L$(AAFSDKLIBDIR) -laaflib -laafiid -o $@


#.PHONY : MultiGenTests
#MultiGenTests :
$(CXXOBJS2):
	@echo "\nBuilding MultiGenTests\n"
	cd MultiGenTests && $(MAKE)

.PHONY : clean
clean :
	cd MultiGenTests && $(MAKE) GENDEPS=0 $@
	@$(RM) $(CXXOBJS)
	@$(RM) $(DEPS)
	@$(RM) $(AAFSDKBINDIR)/MultiGenTest$(EXE)
	@$(RMDIR) $(OBJDIR)/ii_files


.PHONY : realclean
realclean : clean
	cd MultiGenTests && $(MAKE) GENDEPS=0 $@


.PHONY : check
check : $(AAFSDKBINDIR)/MultiGenTest$(EXE)
	echo MultiGenTest not implemented yet.


include $(AAFBASE)/build/rules.mk

