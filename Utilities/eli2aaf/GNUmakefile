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


PROGNAME = eli2aaf

PACKAGE = Utilities

AAFBASE ?= ../..

ifeq ($(shell uname), IRIX)
# Warnings turned off:
# 1107 - signed bit field has length of 1 bit in various stl files
# 1682 - partial overide of AxPropertyValuePrtcl::process
MODULE_PLATFORM_CFLAGS = -LANG:std -woff 1107,1682
LIBCIO = -lCio
endif

# Common definitions
include $(AAFBASE)/build/common.mk

# Include directories
INCLUDES = -I../../ref-impl/include -I../../ref-impl/include/ref-api -I../../examples2/axLib

# AxLib lib directory
AXLIBPACKAGE = axExamples
AXLIBDIR = $(AAFBUILDDIR)/$(AXLIBPACKAGE)/$(AAFTARGETDIR)

BINTARGET = $(AAFSDKBINDIR)/$(PROGNAME)$(EXE)

.PHONY : all
all : $(OBJDIR) $(BINTARGET)

ifeq ($(AAFTARGET),Debug-static)
$(BINTARGET) : $(CXXOBJS)
	$(LD) $(CXXOBJS) -L$(AXLIBDIR) -laxLib $(STATIC_LINK_LINE) -o $@
else
$(BINTARGET) : $(CXXOBJS)
	$(LD) $(CXXOBJS) $(RPATH_OPT) \
	-L$(AAFSDKLIBDIR) -L$(OBJDIR) -L$(AXLIBDIR) -laxLib -laaflib -laafiid $(LIBCIO) -o $@
endif

.PHONY : clean
clean ::
	@$(RM) $(CXXOBJS)
	@$(RM) $(DEPS)
	@$(RM) $(BINTARGET)
	@$(RMDIR) $(OBJDIR)/ii_files


.PHONY : realclean
realclean :: clean


include $(AAFBASE)/build/rules.mk

