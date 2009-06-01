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


AAFBASE ?= ../../..


.PHONY : all
all : ComModTestAAF CreateSequence EssenceAccess MultiGenTest ComFileKindTest UTF8FileNameTest OpenExistingModify ScaleTest ResultToTextTest MemoryLeakTest XMLStoredFormat

.PHONY : ComModTestAAF
ComModTestAAF :
	cd ComModTestAAF && $(MAKE)

.PHONY : CreateSequence
CreateSequence :
	cd CreateSequence && $(MAKE)

.PHONY : EssenceAccess
EssenceAccess :
	cd EssenceAccess && $(MAKE)

.PHONY : MultiGenTest
MultiGenTest :
	cd MultiGenTest && $(MAKE)

.PHONY : ComFileKindTest
ComFileKindTest :
	cd ComFileKindTest && $(MAKE)

.PHONY : UTF8FileNameTest
UTF8FileNameTest :
	cd UTF8FileNameTest && $(MAKE)

.PHONY : OpenExistingModify
OpenExistingModify :
	cd OpenExistingModify && $(MAKE)

.PHONY : ScaleTest
ScaleTest :
	cd ScaleTest && $(MAKE)

.PHONY : ResultToTextTest
ResultToTextTest :
	cd ResultToTextTest && $(MAKE)

.PHONY : MemoryLeakTest
MemoryLeakTest :
	cd MemoryLeakTest && $(MAKE)

.PHONY : XMLStoredFormat
XMLStoredFormat :
	cd XMLStoredFormat && $(MAKE)

.PHONY : check
check :
	cd ComModTestAAF && $(MAKE) $@
	cd CreateSequence && $(MAKE) $@
	cd EssenceAccess && $(MAKE) $@
	cd MultiGenTest && $(MAKE) $@
	cd ComFileKindTest && $(MAKE) $@
	cd UTF8FileNameTest && $(MAKE) $@
	cd OpenExistingModify && $(MAKE) $@
	cd ScaleTest && $(MAKE) $@
	cd ResultToTextTest && $(MAKE) $@
	cd MemoryLeakTest && $(MAKE) $@
	cd XMLStoredFormat && $(MAKE) $@

.PHONY : valgrind-check
valgrind-check :
	cd ComModTestAAF && $(MAKE) $@
	cd CreateSequence && $(MAKE) $@
	cd EssenceAccess && $(MAKE) $@
	cd MultiGenTest && $(MAKE) $@
	cd ComFileKindTest && $(MAKE) $@
	cd UTF8FileNameTest && $(MAKE) $@
	cd OpenExistingModify && $(MAKE) $@
	cd ScaleTest && $(MAKE) $@
	cd ResultToTextTest && $(MAKE) $@
	cd MemoryLeakTest && $(MAKE) $@
	cd XMLStoredFormat && $(MAKE) $@

.PHONY : clean
clean :
	cd ComModTestAAF && $(MAKE) GENDEPS=0 $@
	cd CreateSequence && $(MAKE) GENDEPS=0 $@
	cd EssenceAccess && $(MAKE) GENDEPS=0 $@
	cd MultiGenTest && $(MAKE) GENDEPS=0 $@
	cd ComFileKindTest && $(MAKE) GENDEPS=0 $@
	cd UTF8FileNameTest && $(MAKE) GENDEPS=0 $@
	cd OpenExistingModify && $(MAKE) GENDEPS=0 $@
	cd ScaleTest && $(MAKE) GENDEPS=0 $@
	cd ResultToTextTest && $(MAKE) GENDEPS=0 $@
	cd MemoryLeakTest && $(MAKE) GENDEPS=0 $@
	cd XMLStoredFormat && $(MAKE) GENDEPS=0 $@

.PHONY : realclean
realclean :
	cd ComModTestAAF && $(MAKE) GENDEPS=0 $@
	cd CreateSequence && $(MAKE) GENDEPS=0 $@
	cd EssenceAccess && $(MAKE) GENDEPS=0 $@
	cd MultiGenTest && $(MAKE) GENDEPS=0 $@
	cd ComFileKindTest && $(MAKE) GENDEPS=0 $@
	cd UTF8FileNameTest && $(MAKE) GENDEPS=0 $@
	cd OpenExistingModify && $(MAKE) GENDEPS=0 $@
	cd ScaleTest && $(MAKE) GENDEPS=0 $@
	cd ResultToTextTest && $(MAKE) GENDEPS=0 $@
	cd MemoryLeakTest && $(MAKE) GENDEPS=0 $@
	cd XMLStoredFormat && $(MAKE) GENDEPS=0 $@
	
