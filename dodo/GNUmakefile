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
# The Original Code of this file is Copyright 1998-2001, Licensor of the
# AAF Association.
# 
# The Initial Developer of the Original Code of this file and the
# Licensor of the AAF Association is Avid Technology.
# All rights reserved.
#
###############################################################################

UPDATE = ../update.ksh

.PHONY : all
all : dodo-app macros/base.mac targets.mk depend.mk targets

AAFBASE ?= ..

include $(AAFBASE)/build/common.mk

.PHONY : dodo-app
dodo-app :
	@$(ECHO) "\n======== [ dodo/tool ] \n"
	cd tool && $(MAKE) -f GNUmakefile

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
	$(MAKE) -f maketargets_gnu.mak

.PHONY : clean
clean :
	cd tool && $(MAKE) -f GNUmakefile GENDEPS=0 clean
	$(MAKE) -f dodotargets.mak clean
	$(MAKE) -f dododepend.mak clean
	$(TOUCH) -t 198101011000 targets.mk depend.mk
	$(MAKE) -f maketargets_gnu.mak clean

.PHONY : realclean
realclean : clean
	$(MAKE) -f maketargets_gnu.mak $@
	$(RM) -f targets.mk depend.mk

