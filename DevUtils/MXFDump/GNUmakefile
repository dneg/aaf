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

PACKAGE = DevUtils

AAFBASE ?= ../..

# Common definitions
include $(AAFBASE)/build/common.mk

# Add -framework linker arg for all Darwin platforms (intel, ppc, universal)
ifneq (,$(findstring Darwin,$(AAFPLATFORM)))
PLATFORMLIBS += -framework CoreServices
endif

# For MSVC build turn off UNICODE define so that CreateFile() can be passed
# the char * command line MXF filename (otherwise CreateFileW() is used).
ifeq ($(COMPILER),cl)
  CFLAGS:=$(filter-out %UNICODE,$(CFLAGS))
endif

# Include directories
INCLUDES = -I$(AAFSDKINCLUDEDIR)


all : $(OBJDIR) $(AAFSDKBINDIR)/MXFDump$(EXE)

$(AAFSDKBINDIR)/MXFDump$(EXE) : $(CXXOBJS)
	$(LD) $(CXXOBJS) $(PLATFORMLIBS) $(LIBCIO) $(LD_DYN_OUTPUT)

.PHONY : clean
clean :
	@$(RM) $(CXXOBJS)
	@$(RM) $(DEPS)
	@$(RM) $(AAFSDKBINDIR)/MXFDump$(EXE)
	@$(RMDIR) $(OBJDIR)/ii_files


.PHONY : realclean
realclean : clean


include $(AAFBASE)/build/rules.mk
