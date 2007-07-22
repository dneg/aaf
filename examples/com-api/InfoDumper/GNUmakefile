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


EXAMPLE = InfoDumper

include ../comexamplerules.mk

TEST_CMD = $(APP_RUN_ENV) $(AAFSDKBINDIR)/InfoDumper$(EXE)
.PHONY : check
check : $(AAFSDKBINDIR)/InfoDumper$(EXE)
	for f in ../*/*.aaf ../../../test/com/*/*.aaf; do if echo $$f | grep UTF8FileNameTest > /dev/null; then echo "Skipping $$f until wctou8() et al are removed from the SDK"; else echo InfoDumper $$f; (cd `dirname $$f`; $(TEST_CMD) `basename $$f` > /dev/null) || exit $$?; fi; done
