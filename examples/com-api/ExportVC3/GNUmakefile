###############################################################################
# $Id$ $Name$
# The contents of this file are subject to the AAF SDK Public
# Source License Agreement (the "License"); You may not use this file
# except in compliance with the License.  The License is available in
# AAFSDKPSL.TXT, or you may obtain a copy of the License from the AAF
# Association or its successor.
# Software distributed under the License is distributed on an "AS IS"
# basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
# the License for the specific language governing rights and limitations
# under the License.
# The Original Code of this file is Copyright 2008, Licensor of the
# AAF Association.
# The Initial Developer of the Original Code of this file and the
# Licensor of the AAF Association is Avid Technology.
# All rights reserved.
###############################################################################

PACKAGE = Examples

AAFBASE ?= ../../..


# Common definitions
include $(AAFBASE)/build/common.mk


# Include directories
INCLUDES = -I$(AAFSDKINCLUDEDIR)


.PHONY : all
all : $(OBJDIR) $(AAFSDKBINDIR)/ExportVC3$(EXE)


CXXFILES = framebuffer.cpp ExportVC3.cpp

$(AAFSDKBINDIR)/ExportVC3$(EXE) : $(CXXOBJS)
	$(LD) $(CXXOBJS) $(LINK_AAF_APP)

.PHONY : check
check : $(AAFSDKBINDIR)/ExportVC3$(EXE)
	$(APP_RUN_ENV) $(AAFSDKBINDIR)/ExportVC3$(EXE) -vc3=1235 -raw=bgrai.1080p.1235.dnx -o=ExportVC3.aaf && test -f ExportVC3.aaf

.PHONY : clean
clean :
	@$(RM) $(CXXOBJS)
	@$(RM) $(DEPS)
	@$(RM) $(AAFSDKBINDIR)/ExportVC3$(EXE)
	@$(RMDIR) $(OBJDIR)/ii_files


.PHONY : realclean
realclean : clean


include $(AAFBASE)/build/rules.mk
