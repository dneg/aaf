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

PACKAGE = ref-impl
export PACKAGE

AAFBASE ?= ../../..


# Common definitions
include $(AAFBASE)/build/common.mk

# Turn off error-on-warning behaviour for this directory for AAFTARGET=Release
# Workaround for Release build gcc warnings of "unused variable ..."
# FIXME: remove this once OMStrongRefSetPropertyT.h, ImplAAFBuiltinTypes.cpp,
# FIXME: ImplAAFClassDef.cpp, ImplAAFDataDef.cpp, ImplAAFDictionary.cpp etc
# FIXME: are fixed to avoid the warning
ifeq ($(CC),g++)
ifeq ($(AAFTARGET), Release)
  ADD_CFLAGS += -Wno-error
endif
endif

# Include directories
INCLUDES = -I. \
	   -I../../include/ref-api \
	   -I../../include \
	   -I../../include/platform \
	   -I../../include/OM \
	   -I../../src/OM \
	   -I../../aaflib


.PHONY : all
all : impl


.PHONY : impl
impl : $(OBJDIR) $(LIBDIR) $(LIBDIR)/libimpl$(LIBEXT)


$(LIBDIR)/libimpl$(LIBEXT) : $(CXXOBJS)
	$(LD_STAT_LIB) $(LD_STAT_OUTPUT) $(CXXOBJS)


.PHONY : clean
clean :
	@$(RM) $(CXXOBJS)
	@$(RM) $(DEPS)
	@$(RM) $(LIBDIR)/libimpl$(LIBEXT)
	@$(RMDIR) $(OBJDIR)/ii_files


.PHONY : realclean
realclean : clean


include $(AAFBASE)/build/rules.mk

