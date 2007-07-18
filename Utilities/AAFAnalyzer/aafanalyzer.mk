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

PACKAGE = AAFAnalyzer

AAFBASE ?= ../../..

ifeq ($(shell uname), IRIX)
# Warnings turned off:
# 1107 - signed bit field has length of 1 bit in various stl files
# 1682 - partial overide of AxPropertyValuePrtcl::process
MODULE_PLATFORM_CFLAGS = -LANG:std -woff 1107,1682
endif

# Common definitions
include $(AAFBASE)/build/common.mk

# Include directories
INCLUDES ?= -I$(AAFSDKINCLUDEDIR) \
	   -I../AnalyzerBase \
	   -I../AAFAnalyzerBase \
	   -I../EPAnalyzerBase \
	   -I../RequirementLib \
	   -I../TestPhaseLib \
	   -I../BaseTestImpl \
	   -I../EditProtocolTestImpl \
	   -I$(AAFBASE)/ref-impl/expat \
	   -I$(AAFBASE)/examples2/axLib


BINTARGET = $(AAFSDKBINDIR)/$(PROGNAME)$(EXE)

# AxLib lib directory
AXLIBPACKAGE = axExamples
AXLIBDIR = $(AAFBUILDDIR)/$(AXLIBPACKAGE)/$(AAFTARGETDIR)

# expat static library location
EXPATLIBDIR = $(AAFBUILDDIR)/ref-impl/$(AAFTARGETDIR)

.PHONY : all
all : $(OBJDIR) $(BINTARGET) $(LIBDIR)

.PHONY : gcov
gcov : CFLAGS+= -fprofile-arcs -ftest-coverage
gcov : $(OBJDIR) $(BINTARGET) $(LIBDIR)

$(BINTARGET) : $(CXXOBJS) $(AXPROGRAM_ADDITIONAL_DEPENDS) $(LIBDIR)
	$(LD) $(CXXOBJS) \
	-L$(OBJDIR) -L$(EXPATLIBDIR) -L$(AXLIBDIR) $(AXPROGRAM_LD_OPTIONS) $(PROGLIBS) -laxLib $(LINK_AAF_APP)

.PHONY : clean
clean ::
	@$(RM) $(CXXOBJS)
	@$(RM) $(DEPS)
	@$(RM) $(BINTARGET)
	@$(RMDIR) $(OBJDIR)/ii_files


.PHONY : realclean
realclean :: clean


include $(AAFBASE)/build/rules.mk

