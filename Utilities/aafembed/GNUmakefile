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

PACKAGE = Utilities

AAFBASE ?= ../..

# Common definitions
include $(AAFBASE)/build/common.mk

# Include directories
INCLUDES = -I. -I$(AAFSDKINCLUDEDIR)

.PHONY : all
all : $(OBJDIR) $(AAFSDKBINDIR)/aafembed$(EXE)

$(AAFSDKBINDIR)/aafembed$(EXE) : $(CXXOBJS)
	$(LD) $(CXXOBJS) $(LINK_AAF_APP)

.PHONY : clean
clean :
	@$(RM) $(CXXOBJS)
	@$(RM) $(DEPS)
	@$(RM) $(AAFSDKBINDIR)/aafembed$(EXE)


.PHONY : realclean
realclean : clean


include $(AAFBASE)/build/rules.mk
