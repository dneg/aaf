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

PACKAGE = com-api
export PACKAGE

AAFBASE ?= ../../..


# Common definitions
include $(AAFBASE)/build/common.mk


# Include directories
INCLUDES = -I../../include/ref-api \
	   -I../../include/OM \
	   -I../../src/OM \
	   -I../../include \
	   -Icom-dll \
	   -I../../../include/platform \
	   -I../impl \
	   -I.. -I. \
	   -I../../aaflib


LIBTARGET = $(LIBDIR)/libcom-api$(DLL)


# The following two lines initialize CXXFILES2 to contain the list
# of source files from ./com-dll directory.
CXXFILES_TMP = $(wildcard com-dll/*.$(CPP_EXTENSION)) $(wildcard com-dll/*.c)
CXXFILES2 = $(CXXFILES_TMP:com-dll/%=%)


.PHONY : all
all : com-api


.PHONY : com-api
com-api : $(OBJDIR) $(LIBDIR) com-dll $(LIBTARGET)


.PHONY : com-dll
com-dll :
	cd com-dll && $(MAKE)


$(LIBTARGET) : $(CXXOBJS) $(CXXOBJS2) $(AAFBUILDDIR)/ref-impl/$(AAFTARGETDIR)/libimpl$(LIB) $(AAFBUILDDIR)/OM/$(AAFTARGETDIR)/libom$(LIB) $(AAFBUILDDIR)/ss-impl/$(AAFTARGETDIR)/librefstg$(LIB)
	$(LD_DYN_LIB) $(CXXOBJS) $(CXXOBJS2) -o $@ \
	$(AAFBUILDDIR)/ref-impl/$(AAFTARGETDIR)/libimpl$(LIB) \
	$(AAFBUILDDIR)/OM/$(AAFTARGETDIR)/libom$(LIB) \
	$(AAFBUILDDIR)/ss-impl/$(AAFTARGETDIR)/librefstg$(LIB) \
	-lm $(UUIDLIB)


.PHONY : clean
clean:
	cd com-dll && $(MAKE) GENDEPS=0 $@
	@$(RM) $(CXXOBJS) $(CXXOBJS2)
	@$(RM) $(DEPS)
	@$(RM) $(LIBTARGET)
	@$(RMDIR) $(OBJDIR)/ii_files


.PHONY : realclean
realclean : clean
	cd com-dll && $(MAKE) GENDEPS=0 $@


include $(AAFBASE)/build/rules.mk

