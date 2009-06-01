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

PACKAGE = Examples

AAFBASE ?= ../../..


# Common definitions
include $(AAFBASE)/build/common.mk


# Include directories
INCLUDES = -I$(AAFSDKINCLUDEDIR)


.PHONY : all
all : $(OBJDIR) $(AAFSDKBINDIR)/ComAAFInfo$(EXE) $(AAFSDKBINDIR)/ComClientAAF$(EXE)


CXXFILES = AAFInfo.cpp

$(AAFSDKBINDIR)/ComAAFInfo$(EXE) : $(CXXOBJS)
	$(LD) $(CXXOBJS) $(LINK_AAF_APP)

CXXFILES2 = ComClientTestAAF.cpp


$(AAFSDKBINDIR)/ComClientAAF$(EXE) : $(CXXOBJS2)
	$(LD) $(CXXOBJS2) $(LINK_AAF_APP)

.PHONY : check
check : $(AAFSDKBINDIR)/ComClientAAF$(EXE) $(AAFSDKBINDIR)/ComAAFInfo$(EXE)
	$(APP_RUN_ENV) $(AAFSDKBINDIR)/ComClientAAF$(EXE)
	for f in ../*/*.aaf ../../../test/com/*/*.aaf; do if echo $$f | grep UTF8FileNameTest > /dev/null; then echo "Skipping $$f until wctou8() et al are removed from the SDK"; else echo ComAAFInfo $$f; (cd `dirname $$f`; $(APP_RUN_ENV) $(AAFSDKBINDIR)/ComAAFInfo$(EXE) `basename $$f` > /dev/null) || exit $$?; fi; done

.PHONY : clean
clean :
	@$(RM) $(CXXOBJS)
	@$(RM) $(CXXOBJS2)
	@$(RM) $(DEPS)
	@$(RM) $(AAFSDKBINDIR)/ComAAFInfo$(EXE)
	@$(RM) $(AAFSDKBINDIR)/ComClientAAF$(EXE)
	@$(RMDIR) $(OBJDIR)/ii_files


.PHONY : realclean
realclean : clean


include $(AAFBASE)/build/rules.mk
