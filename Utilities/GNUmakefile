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


.PHONY : all
all : aafomf AAFAnalyzer aafextract eli2aaf aafviewer aafdump aaffmtconv aafembed

# AafOmf depends upon OMF development libraries which are no longer distributed
# with the SDK.  They are available from http://aafassociation.org/download/
.PHONY : aafomf
aafomf:
	@if [ -r ../OMF/Include/omDefs.h ] ; then cd AafOmf && $(MAKE); else echo; echo "OMF libraries not present - AafOmf will not be built"; echo; fi

# AAFAnalyzer needs the Boost C++ libraries, so check they are present first
# by looking either in /usr/include or the ADD_CFLAGS variable
.PHONY : AAFAnalyzer
AAFAnalyzer:
	@if [ -r /usr/include/boost/shared_ptr.hpp ] || echo $(ADD_CFLAGS) | grep -i boost ; then cd AAFAnalyzer && $(MAKE) ; else echo "Boost includes not found - AAFAnalyzer will not be built"; echo ; fi

.PHONY : aafextract
aafextract:
	cd aafextract && $(MAKE)

.PHONY : eli2aaf
eli2aaf:
	cd eli2aaf && $(MAKE)

.PHONY : aafviewer
aafviewer:
	cd aafviewer/src/aaf2dot && $(MAKE)
	cd aafviewer/src/aafmeta2dot && $(MAKE)

.PHONY : aafdump
aafdump:
	cd aafdump && $(MAKE)

.PHONY : aaffmtconv
aaffmtconv:
	cd aaffmtconv && $(MAKE)

.PHONY : aafembed
aafembed:
	cd aafembed && $(MAKE)

# Check target builds utiltities that are required by other check
# tests possibly outside of the utilitiy directory (originally this
# was added for the ComFileExtDevUtilVerityTest.sh script).
.PHONY : check
check : aafviewer aaffmtconv aafembed aafdump
	echo Utilities check dependencies build complete.

.PHONY : clean
clean :
	cd AafOmf && $(MAKE) GENDEPS=0 $@
	cd AAFAnalyzer && $(MAKE) GENDEPS=0 $@
	cd aafextract && $(MAKE) GENDEPS=0 $@
	cd eli2aaf && $(MAKE) GENDEPS=0 $@
	cd aafviewer/src/aaf2dot && $(MAKE) GENDEPS=0 $@
	cd aafviewer/src/aafmeta2dot && $(MAKE) GENDEPS=0 $@
	cd aafdump && $(MAKE) GENDEPS=0 $@
	cd aaffmtconv && $(MAKE) GENDEPS=0 $@
	cd aafembed && $(MAKE) GENDEPS=0 $@

.PHONY : realclean
realclean :
	cd AafOmf && $(MAKE) GENDEPS=0 $@
	cd AAFAnalyzer && $(MAKE) GENDEPS=0 $@
	cd aafextract && $(MAKE) GENDEPS=0 $@
	cd eli2aaf && $(MAKE) GENDEPS=0 $@
	cd aafviewer/src/aaf2dot && $(MAKE) GENDEPS=0 $@
	cd aafviewer/src/aafmeta2dot && $(MAKE) GENDEPS=0 $@
	cd aafdump && $(MAKE) GENDEPS=0 $@
	cd aaffmtconv && $(MAKE) GENDEPS=0 $@
	cd aafembed && $(MAKE) GENDEPS=0 $@
