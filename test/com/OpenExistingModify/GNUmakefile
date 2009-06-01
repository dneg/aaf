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

PACKAGE = Test

AAFBASE ?= ../../..


# Common definitions
include $(AAFBASE)/build/common.mk


# Include directories
INCLUDES = -I$(AAFSDKINCLUDEDIR)


.PHONY : all
all : $(OBJDIR) $(AAFSDKBINDIR)/OpenExistingModify$(EXE)

$(AAFSDKBINDIR)/OpenExistingModify$(EXE) : $(CXXOBJS)
	$(LD) $(CXXOBJS) $(LINK_AAF_APP)


.PHONY : check
check : $(AAFSDKBINDIR)/OpenExistingModify$(EXE)
	@echo
	@echo "*** Testing modification of current toolkit generated file"
	$(APP_RUN_ENV) $(AAFSDKBINDIR)/OpenExistingModify$(EXE)
	@echo
	@echo "*** Testing modification of v1.0.2 toolkit generated file"
	cp AAFHeaderTest_v102.aaf Modify_v102.aaf && $(APP_RUN_ENV) $(AAFSDKBINDIR)/OpenExistingModify$(EXE) Modify_v102.aaf

.PHONY : valgrind-check
valgrind-check : $(AAFSDKBINDIR)/OpenExistingModify$(EXE)
	@echo
	@echo "*** Testing modification of current toolkit generated file"
	$(APP_RUN_ENV) $(VALGRIND_CMD) $(AAFSDKBINDIR)/OpenExistingModify$(EXE)
	@echo
	@echo "*** Testing modification of v1.0.2 toolkit generated file"
	cp AAFHeaderTest_v102.aaf Modify_v102.aaf && $(APP_RUN_ENV) $(VALGRIND_CMD) $(AAFSDKBINDIR)/OpenExistingModify$(EXE) Modify_v102.aaf


.PHONY : clean
clean :
	@$(RM) $(CXXOBJS)
	@$(RM) $(DEPS)
	@$(RM) $(AAFSDKBINDIR)/OpenExistingModify$(EXE)
	@$(RMDIR) $(OBJDIR)/ii_files


.PHONY : realclean
realclean : clean

include $(AAFBASE)/build/rules.mk
