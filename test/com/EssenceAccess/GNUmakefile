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


# Common definitions
include $(AAFBASE)/build/common.mk


# Include directories
INCLUDES = -I$(AAFSDKINCLUDEDIR)


.PHONY : all
all : $(OBJDIR) $(AAFSDKBINDIR)/EssenceAccess$(EXE)


ifeq ($(AAFTARGET),Debug-static)
$(AAFSDKBINDIR)/EssenceAccess$(EXE) : $(CXXOBJS)
	$(LD) $(CXXOBJS) $(STATIC_LINK_LINE) -o $@
else
$(AAFSDKBINDIR)/EssenceAccess$(EXE) : $(CXXOBJS)
	$(LD) $(CXXOBJS) -L$(AAFSDKLIBDIR) -laaflib -laafiid $(PLATFORMLIBS) -o $@
endif


# Create a simple AAF file with 100 copies of the audio from Laser.wav
# This exercises the EssenceAccess API and the WAVE Codec
.PHONY : check
check : $(AAFSDKBINDIR)/EssenceAccess$(EXE)
	@if [ ! -r Laser.wav ]; then cp ../ComModTestAAF/Laser.wav .; fi
	env LD_LIBRARY_PATH=$(AAFSDKBINDIR):$(LD_LIBRARY_PATH) $(AAFSDKBINDIR)/EssenceAccess$(EXE) 100


.PHONY : clean
clean :
	@$(RM) $(CXXOBJS)
	@$(RM) $(DEPS)
	@$(RM) $(AAFSDKBINDIR)/EssenceAccess$(EXE)
	@$(RMDIR) $(OBJDIR)/ii_files


.PHONY : realclean
realclean : clean


include $(AAFBASE)/build/rules.mk

