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
# The Original Code of this file is Copyright 1998-2005, Licensor of the
# AAF Association.
# 
# The Initial Developer of the Original Code of this file and the
# Licensor of the AAF Association is British Broadcasting Corporation.
# All rights reserved.
#
###############################################################################

PACKAGE = Test

AAFBASE ?= ../../..


# Common definitions
include $(AAFBASE)/build/common.mk


# Include directories
INCLUDES = -I$(AAFSDKINCLUDEDIR)


.PHONY : all
all : $(OBJDIR) $(AAFSDKBINDIR)/XMLStoredFormatTest$(EXE)


ifeq ($(AAFTARGET),Debug-static)
$(AAFSDKBINDIR)/XMLStoredFormatTest$(EXE) : $(CXXOBJS)
	$(LD) $(CXXOBJS) $(STATIC_LINK_LINE) -o $@
else
$(AAFSDKBINDIR)/XMLStoredFormatTest$(EXE) : $(CXXOBJS)
	$(LD) $(CXXOBJS) -L$(AAFSDKLIBDIR) -laaflib -laafiid $(PLATFORMLIBS) -o $@ 
endif

.PHONY : check
check : $(AAFSDKBINDIR)/XMLStoredFormatTest$(EXE)
	env LD_LIBRARY_PATH=$(AAFSDKBINDIR):$(LD_LIBRARY_PATH) $(AAFSDKBINDIR)/XMLStoredFormatTest$(EXE)


.PHONY : clean
clean :
	@$(RM) $(CXXOBJS)
	@$(RM) $(DEPS)
	@$(RM) $(AAFSDKBINDIR)/XMLStoredFormatTest$(EXE)
	@$(RMDIR) $(OBJDIR)/ii_files
	@$(RM) tmp.xml
	@$(RM) tmp_streams/*
	@$(RMDIR) tmp_streams


.PHONY : realclean
realclean : clean


include $(AAFBASE)/build/rules.mk

