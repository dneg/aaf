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

PACKAGE = MultiGenTest

AAFBASE ?= ../../..

ifeq ($(shell uname), IRIX)
# Warnings turned off:
# 1107 - signed bit field has length of 1 bit in various stl files
MODULE_PLATFORM_CFLAGS = -LANG:std -woff 1107
LIBCIO = -lCio
endif

# Common definitions
include $(AAFBASE)/build/common.mk


# Include directories
INCLUDES = -I$(AAFSDKINCLUDEDIR)


# CXXFILES2 is declared just to get the list of object files
# from MultiGenTests directory. These object files are created 
# when building target MultiGenTests
CXXFILES_TMP = $(wildcard MultiGenTests/*.$(CPP_EXTENSION)) $(wildcard MultiGenTests/*.c)
CXXFILES2 = $(CXXFILES_TMP:MultiGenTests/%=%)


.PHONY : all
#all : $(OBJDIR) $(AAFSDKBINDIR)/MultiGenTest$(EXE)
all : $(AAFSDKBINDIR)/MultiGenTest$(EXE)


# $(AAFSDKBINDIR)/MultiGenTest$(EXE) : $(CXXOBJS) MultiGenTests
ifeq ($(AAFTARGET),Debug-static)
$(AAFSDKBINDIR)/MultiGenTest$(EXE) : $(CXXOBJS) $(CXXOBJS2)
	$(LD) $(CXXOBJS) $(CXXOBJS2) $(STATIC_LINK_LINE) -o $@
else
$(AAFSDKBINDIR)/MultiGenTest$(EXE) : $(CXXOBJS) $(CXXOBJS2)
	$(LD) $(CXXOBJS) $(CXXOBJS2) \
	-L$(AAFSDKLIBDIR) -laaflib -laafiid $(PLATFORMLIBS) $(LIBCIO) -o $@
endif


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


# Check that the basic MultiGenTest operations work.
# NOTE: MultiGenTest is designed to be run across multiple
# platforms and SDK versions - this test merely checks that
# MultiGenTest is in good working order.
TEST_LIB_NAME = libcom-api.so
ifeq ($(AAFPLATFORM),PPCDarwin)
TEST_LIB_NAME = libcom-api.dylib
endif
ifeq ($(AAFPLATFORM),i386Darwin)
TEST_LIB_NAME = libcom-api.dylib
endif
ifeq ($(AAFPLATFORM),UniversalDarwin)
TEST_LIB_NAME = libcom-api.dylib
endif

.PHONY : check
check : $(AAFSDKBINDIR)/MultiGenTest$(EXE)
	rm -f basic.aaf
	env LD_LIBRARY_PATH=$(AAFSDKBINDIR):$(LD_LIBRARY_PATH) $(AAFSDKBINDIR)/MultiGenTest$(EXE) -r LoadLib $(TEST_LIB_NAME) -r FileOp write S4KBinary basic.aaf -r AddMasterMobs A B -r FileOp save_and_close -r UnloadLib || exit 1
	env LD_LIBRARY_PATH=$(AAFSDKBINDIR):$(LD_LIBRARY_PATH) $(AAFSDKBINDIR)/MultiGenTest$(EXE) -r LoadLib $(TEST_LIB_NAME) -r FileOp read S4KBinary basic.aaf -r FindMasterMobsExclusive A B -r FileOp close -r UnloadLib || exit 1


include $(AAFBASE)/build/rules.mk
