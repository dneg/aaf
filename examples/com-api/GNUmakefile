
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
	cd ComCutsOnlyTest && $(MAKE) clean
	cd ComEssenceDataTest && $(MAKE) clean
	cd ComExtension && $(MAKE) clean
	cd ComPropDirectAccess && $(MAKE) clean
	cd ComPropDirectDump && $(MAKE) clean
	cd ExportAudioExample && $(MAKE) clean
	cd ExportSimpleComposition && $(MAKE) clean
	cd ImportAudioExample && $(MAKE) clean
	cd InfoDumper && $(MAKE) clean
	cd MetadataExample && $(MAKE) clean
	cd comClientTest && $(MAKE) clean

