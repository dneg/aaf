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


all: ComCutsOnlyTest ComEssenceDataTest ComExtension ComPropDirectAccess \
	ComPropDirectDump ExportAudioExample ExportSimpleComposition \
	ImportAudioExample InfoDumper MetadataExample comClientTest

.PHONY : ComCutsOnlyTest
ComCutsOnlyTest:
	cd ComCutsOnlyTest && $(MAKE)

.PHONY : ComEssenceDataTest
ComEssenceDataTest:
	cd ComEssenceDataTest && $(MAKE)

.PHONY : ComExtension
ComExtension:
	cd ComExtension && $(MAKE)

.PHONY : ComPropDirectAccess
ComPropDirectAccess:
	cd ComPropDirectAccess && $(MAKE)

.PHONY: ComPropDirectDump
ComPropDirectDump:
	cd ComPropDirectDump && $(MAKE)

.PHONY: ExportAudioExample
ExportAudioExample:
	cd ExportAudioExample && $(MAKE)

.PHONY : ExportSimpleComposition
ExportSimpleComposition:
	cd ExportSimpleComposition && $(MAKE)

.PHONY : ImportAudioExample
ImportAudioExample:
	cd ImportAudioExample && $(MAKE)

.PHONY : InfoDumper
InfoDumper:
	cd InfoDumper && $(MAKE)

.PHONY : MetadataExample
MetadataExample :
	cd MetadataExample && $(MAKE)

.PHONY : comClientTest
comClientTest :
	cd comClientTest && $(MAKE)

.PHONY : clean
clean :
	cd ComCutsOnlyTest && $(MAKE) GENDEPS=0 $@
	cd ComEssenceDataTest && $(MAKE) GENDEPS=0 $@
	cd ComExtension && $(MAKE) GENDEPS=0 $@
	cd ComPropDirectAccess && $(MAKE) GENDEPS=0 $@
	cd ComPropDirectDump && $(MAKE) GENDEPS=0 $@
	cd ExportAudioExample && $(MAKE) GENDEPS=0 $@
	cd ExportSimpleComposition && $(MAKE) GENDEPS=0 $@
	cd ImportAudioExample && $(MAKE) GENDEPS=0 $@
	cd InfoDumper && $(MAKE) GENDEPS=0 $@
	cd MetadataExample && $(MAKE) GENDEPS=0 $@
	cd comClientTest && $(MAKE) GENDEPS=0 $@

.PHONY : realclean
realclean :
	cd ComCutsOnlyTest && $(MAKE) GENDEPS=0 $@
	cd ComEssenceDataTest && $(MAKE) GENDEPS=0 $@
	cd ComExtension && $(MAKE) GENDEPS=0 $@
	cd ComPropDirectAccess && $(MAKE) GENDEPS=0 $@
	cd ComPropDirectDump && $(MAKE) GENDEPS=0 $@
	cd ExportAudioExample && $(MAKE) GENDEPS=0 $@
	cd ExportSimpleComposition && $(MAKE) GENDEPS=0 $@
	cd ImportAudioExample && $(MAKE) GENDEPS=0 $@
	cd InfoDumper && $(MAKE) GENDEPS=0 $@
	cd MetadataExample && $(MAKE) GENDEPS=0 $@
	cd comClientTest && $(MAKE) GENDEPS=0 $@

