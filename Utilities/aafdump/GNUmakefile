###############################################################################
#
# $Id$ $Name$
#
# The contents of this file are subject to the AAF SDK Public Source
# License Agreement Version 2.0 (the "License"); You may not use this
# file except in compliance with the License.  The License is available
# in AAFSDKPSL.TXT, or you may obtain a copy of the License from the
# Advanced Media Workflow Association, Inc., or its successor.
#
# Software distributed under the License is distributed on an "AS IS"
# basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
# the License for the specific language governing rights and limitations
# under the License.  Refer to Section 3.3 of the License for proper use
# of this Exhibit.
#
# WARNING:  Please contact the Advanced Media Workflow Association,
# Inc., for more information about any additional licenses to
# intellectual property covering the AAF Standard that may be required
# to create and distribute AAF compliant products.
# (http://www.amwa.tv/policies).
#
# Copyright Notices:
# The Original Code of this file is Copyright 1998-2009, licensor of the
# Advanced Media Workflow Association.  All rights reserved.
#
# The Initial Developer of the Original Code of this file and the
# licensor of the Advanced Media Workflow Association is
# Avid Technology.
# All rights reserved.
#
###############################################################################

PACKAGE = Utilities

AAFBASE ?= ../..


# Common definitions
include $(AAFBASE)/build/common.mk


# Include directories
INCLUDES = -I. \
           -I$(AAFBASE)/ss-impl/ref


DEPS:=$(filter-out %/MStructuredStorage.d,$(DEPS))
CXXOBJS:=$(filter-out %/MStructuredStorage.o,$(CXXOBJS))

# avoids problem with gutils.h:104:30: "__OPTIMIZE__" is not defined
ADD_CFLAGS += -Wno-undef

# aafdump depends on the LIBGSF library for non-Windows builds.
ifeq "$(LIBGSF_PATH)" ""
GENDEPS = 0

.PHONY : all
all :
	@echo; echo "LIBGSF library not present - aafdump will not be built."; echo;

else
.PHONY : all
all : $(OBJDIR) $(AAFSDKBINDIR)/aafdump$(EXE)

$(AAFSDKBINDIR)/aafdump$(EXE) : $(CXXOBJS)
	$(LD) $(CXXOBJS) $(PLATFORMLIBS) $(LD_DYN_OUTPUT)

endif


.PHONY : clean
clean :
	@$(RM) $(CXXOBJS)
	@$(RM) $(DEPS)
	@$(RM) $(AAFSDKBINDIR)/aafdump$(EXE)
	@$(RMDIR) $(OBJDIR)/ii_files


.PHONY : realclean
realclean : clean


include $(AAFBASE)/build/rules.mk
