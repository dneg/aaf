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


.PHONY : all
all : aafomf eli2aaf aafviewer

# AafOmf depends upon OMF development libraries which are no longer distributed
# with the SDK.  They are available from http://aafassociation.org/download/
.PHONY : aafomf
aafomf:
	@if [ -r ../OMF/Include/omDefs.h ] ; then cd AafOmf && $(MAKE); else echo; echo "OMF libraries not present - AafOmf will not be built"; echo; fi

.PHONY : eli2aaf
eli2aaf:
	cd eli2aaf && $(MAKE)

.PHONY : aafviewer
aafviewer:
	cd aafviewer/src/aaf2dot && $(MAKE)
	cd aafviewer/src/aafmeta2dot && $(MAKE)

.PHONY : clean
clean :
	cd AafOmf && $(MAKE) GENDEPS=0 $@
	cd eli2aaf && $(MAKE) GENDEPS=0 $@
	cd aafviewer/src/aaf2dot && $(MAKE) GENDEPS=0 $@
	cd aafviewer/src/aafmeta2dot && $(MAKE) GENDEPS=0 $@

.PHONY : realclean
realclean :
	cd AafOmf && $(MAKE) GENDEPS=0 $@
	cd eli2aaf && $(MAKE) GENDEPS=0 $@
	cd aafviewer/src/aaf2dot && $(MAKE) GENDEPS=0 $@
	cd aafviewer/src/aafmeta2dot && $(MAKE) GENDEPS=0 $@
