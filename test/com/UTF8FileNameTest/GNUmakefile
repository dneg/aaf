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
INCLUDES = -I../../../ref-impl/include \
	   -I../../../ref-impl/include/ref-api \
	   -I../../../ref-impl/include/com-api \
	   -I../../../ref-impl/src/com-api


.PHONY : all
all : $(OBJDIR) $(AAFSDKBINDIR)/UTF8FileNameTest$(EXE)


ifeq ($(AAFTARGET),Debug-static)
$(AAFSDKBINDIR)/UTF8FileNameTest$(EXE) : $(CXXOBJS)
	$(LD) $(CXXOBJS) $(STATIC_LINK_LINE) -o $@
else
$(AAFSDKBINDIR)/UTF8FileNameTest$(EXE) : $(CXXOBJS)
	$(LD) $(CXXOBJS) -L$(AAFSDKLIBDIR) -laaflib -laafiid $(PLATFORMLIBS) $(LIBCIO) -o $@
endif


.PHONY : clean
clean :
	@$(RM) $(CXXOBJS)
	@$(RM) $(DEPS)
	@$(RM) $(AAFSDKBINDIR)/ComFileKindTest$(EXE)
	@$(RMDIR) $(OBJDIR)/ii_files


.PHONY : realclean
realclean : clean


include $(AAFBASE)/build/rules.mk
