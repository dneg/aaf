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

PACKAGE = OM
export PACKAGE

AAFBASE ?= ../../..

ifeq ($(shell uname), IRIX)
# Warnings turned off:
# 1107 - signed bit field has length of 1 bit in various stl files
MODULE_PLATFORM_CFLAGS = -LANG:std -woff 1107
MODULE_PLATFORM_LDFLAGS = $(MODULE_PLATFORM_CFLAGS)
endif

# Common definitions
include $(AAFBASE)/build/common.mk

INCLUDES = -I. \
           -I../../include/OM \
           -I$(AAFBASE)/ss-impl/ref \
           -I$(AAFBASE)/ref-impl/include \
           -I$(AAFBASE)/sss-impl/include

# Optional libgsf support is enabled by LIBGSF_PATH variable
# E.g. "make LIBGSF_PATH=/usr/local"
ifndef LIBGSF_PATH
DEPS:=$(filter-out %/OMGSFStructuredStorage.d,$(DEPS))
DEPS:=$(filter-out %/OMGSF_SSStoredObjectFactory.d,$(DEPS))
CXXOBJS:=$(filter-out %/OMGSFStructuredStorage.o,$(CXXOBJS))
CXXOBJS:=$(filter-out %/OMGSF_SSStoredObjectFactory.o,$(CXXOBJS))
endif

CFLAGS += -DOM_STACK_TRACE_ON_ASSERT

# AAF-XML stored format support requires the expat library which is
# bundled with the AAF SDK.
INCLUDES += -I../../expat

.PHONY : all
all : om


.PHONY : om
om : $(OBJDIR) $(LIBDIR) $(LIBDIR)/libom$(LIBEXT)


$(LIBDIR)/libom$(LIBEXT) : $(CXXOBJS)
	$(LD_STAT_LIB) $@ $(CXXOBJS)


.PHONY : clean
clean :
	@$(RM) $(CXXOBJS)
	@$(RM) $(DEPS)
	@$(RM) $(LIBDIR)/libom$(LIBEXT)
	@$(RMDIR) $(OBJDIR)/ii_files


.PHONY : realclean
realclean : clean


include $(AAFBASE)/build/rules.mk

