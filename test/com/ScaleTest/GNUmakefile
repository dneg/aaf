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

PACKAGE = Test

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


.PHONY : all
all : $(OBJDIR) $(AAFSDKBINDIR)/ScaleTest$(EXE)


ifeq ($(AAFTARGET),Debug-static)
$(AAFSDKBINDIR)/ScaleTest$(EXE) : $(CXXOBJS)
	$(LD) $(CXXOBJS) $(STATIC_LINK_LINE) -o $@
else
$(AAFSDKBINDIR)/ScaleTest$(EXE) : $(CXXOBJS)
	$(LD) $(CXXOBJS) -L$(AAFSDKLIBDIR) -laaflib -laafiid $(PLATFORMLIBS) $(LIBCIO) -o $@
endif


# Create a 30 minute DV clip (requires 6GB disk space!)
.PHONY : check
check : $(AAFSDKBINDIR)/ScaleTest$(EXE)
	@echo "*** Testing EssenceData API ***"
	env LD_LIBRARY_PATH=$(AAFSDKBINDIR):$(LD_LIBRARY_PATH) $(AAFSDKBINDIR)/ScaleTest$(EXE) -q -d
	@echo "*** Testing EssenceAccess API (Plugin codecs) ***"
	env LD_LIBRARY_PATH=$(AAFSDKBINDIR):$(LD_LIBRARY_PATH) $(AAFSDKBINDIR)/ScaleTest$(EXE) -q -c -d


.PHONY : clean
clean :
	@$(RM) $(CXXOBJS)
	@$(RM) $(DEPS)
	@$(RM) $(AAFSDKBINDIR)/ScaleTest$(EXE)
	@$(RMDIR) $(OBJDIR)/ii_files


.PHONY : realclean
realclean : clean


include $(AAFBASE)/build/rules.mk

