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


AAFBASE ?= ../../..


.PHONY : all
all : ComModTestAAF CreateSequence EssenceAccess MultiGenTest ComFileKindTest

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

.PHONY : clean
clean :
	cd ComModTestAAF && $(MAKE) GENDEPS=0 $@
	cd CreateSequence && $(MAKE) GENDEPS=0 $@
	cd EssenceAccess && $(MAKE) GENDEPS=0 $@
	cd MultiGenTest && $(MAKE) GENDEPS=0 $@
	cd ComFileKindTest && $(MAKE) GENDEPS=0 $@

.PHONY : realclean
realclean :
	cd ComModTestAAF && $(MAKE) GENDEPS=0 $@
	cd CreateSequence && $(MAKE) GENDEPS=0 $@
	cd EssenceAccess && $(MAKE) GENDEPS=0 $@
	cd MultiGenTest && $(MAKE) GENDEPS=0 $@
	cd ComFileKindTest && $(MAKE) GENDEPS=0 $@

