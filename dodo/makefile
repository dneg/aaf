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

UPDATE = ../update.ksh

all : dodo-app macros/base.mac targets.mk depend.mk targets

BLD_CFG_DIR = ../ref-impl/bld_cfg

include $(BLD_CFG_DIR)/common.mk

dodo-app :
	@$(ECHO) "\n======== [ dodo/tool ] \n"
	cd tool && $(MAKE) -f makefile

test_only :
	cd tool && $(MAKE) -f makefile test_only
	$(MAKE) -f dodotargets.mak
	$(MAKE) -f dododepend.mak
	$(MAKE) -f maketargets.mak

run :
	$(MAKE) -f dodotargets.mak
	$(MAKE) -f dododepend.mak
	$(MAKE) -f maketargets.mak
	cd tool && $(MAKE) -f makefile run

macros/base.mac : CopyrightMessage.txt sync_copyright.pl
	@perl sync_copyright.pl -macro macros/base.mac -copyright CopyrightMessage.txt > base.mac.tmp
	$(UPDATE) base.mac.tmp macros/base.mac
	@rm -f base.mac.tmp

targets.mk : aafobjects.mk GenTargets.sh
	@$(ECHO) "\n======== [ dodo ] \"Building targets.mk\"\n"
	$(MAKE) -f dodotargets.mak

depend.mk : aafobjects.mk GenDepend.sh GenDepend2.sh
	@$(ECHO) "\n======== [ dodo ] \"Building depend.mk\"\n"
	$(MAKE) -f dododepend.mak

targets : targets.mk depend.mk
	@$(ECHO) "\n======== [ dodo ] \"Building targets\"\n"
	$(MAKE) -f maketargets.mak CCASE_MAKE_COMPAT=gnu



clean :
	cd tool && $(MAKE) -f makefile GENDEPS=0 clean
	$(MAKE) -f dodotargets.mak clean
	$(MAKE) -f dododepend.mak clean
	$(TOUCH) -t 198101011000 targets.mk depend.mk
	$(MAKE) -f maketargets.mak clean

realclean : clean
	$(MAKE) -f maketargets.mak $@
	$(RM) -f targets.mk depend.mk

