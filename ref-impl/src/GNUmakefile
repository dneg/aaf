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

.PHONY : all
all : OM impl com-api


.PHONY : OM
OM :
	@echo "\nBuilding OM\n"
	cd OM && $(MAKE)


.PHONY : impl
impl : OM
	@echo "\nBuilding impl\n"
	cd impl && $(MAKE)


.PHONY : com-api
com-api : OM impl
	@echo "\nBuilding com-api\n"
	cd com-api && $(MAKE)


.PHONY : clean
clean : 
	cd OM && $(MAKE) GENDEPS=0 $@
	cd impl && $(MAKE) GENDEPS=0 $@
	cd com-api && $(MAKE) GENDEPS=0 $@


.PHONY : realclean
realclean :
	cd OM && $(MAKE) GENDEPS=0 $@
	cd impl && $(MAKE) GENDEPS=0 $@
	cd com-api && $(MAKE) GENDEPS=0 $@


