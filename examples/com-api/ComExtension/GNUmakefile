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

PACKAGE = Examples

AAFBASE ?= ../../..

# Common definitions
include $(AAFBASE)/build/common.mk


# Include directories
INCLUDES = -I$(AAFSDKINCLUDEDIR)


.PHONY : all
all : $(OBJDIR) \
      $(AAFSDKBINDIR)/ComExtensionRead$(EXE) \
      $(AAFSDKBINDIR)/ComExtensionWrite$(EXE) \
      ComPersonnelExtension


.PHONY : ComPersonnelExtension
ComPersonnelExtension :
	cd ComPersonnelExtension && $(MAKE)


CXXFILES = read.cpp \
	   extensionRead.cpp \
	   extensionUtils.cpp


ifeq ($(AAFTARGET),Debug-static)
$(AAFSDKBINDIR)/ComExtensionRead$(EXE) : $(CXXOBJS)
	$(LD) $(CXXOBJS) $(STATIC_LINK_LINE) -o $@
else
$(AAFSDKBINDIR)/ComExtensionRead$(EXE) : $(CXXOBJS)
	$(LD) $(CXXOBJS) -L$(AAFSDKLIBDIR) -laaflib -laafiid $(LIBCIO) -o $@
endif


CXXFILES2 = write.cpp \
	    extensionRead.cpp \
	    extensionUtils.cpp \
	    extensionWrite.cpp


ifeq ($(AAFTARGET),Debug-static)
$(AAFSDKBINDIR)/ComExtensionWrite$(EXE) : $(CXXOBJS2)
	$(LD) $(CXXOBJS2) $(STATIC_LINK_LINE) -o $@
else
$(AAFSDKBINDIR)/ComExtensionWrite$(EXE) : $(CXXOBJS2)
	$(LD) $(CXXOBJS2) -L$(AAFSDKLIBDIR) -laaflib -laafiid $(LIBCIO) -o $@
endif


.PHONY : clean
clean :
	@$(RM) $(CXXOBJS)
	@$(RM) $(CXXOBJS2)
	@$(RM) $(DEPS)
	@$(RM) $(AAFSDKBINDIR)/ComExtensionRead$(EXE)
	@$(RM) $(AAFSDKBINDIR)/ComExtensionWrite$(EXE)
	@$(RMDIR) $(OBJDIR)/ii_files
	cd ComPersonnelExtension && $(MAKE) GENDEPS=0 $@

.PHONY : realclean
realclean : clean


include $(AAFBASE)/build/rules.mk
