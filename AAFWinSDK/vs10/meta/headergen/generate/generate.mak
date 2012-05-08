#
# The path to the AAF-toolkit
#
# assume execute from (for example) D:\Code\AAF-1.1\AAF\AAFWinSDK\vs9\meta\headergen\generate
#
!if "$(AAFTOOLKIT)"==""
AAFTOOLKIT = ..\..\..\..\..
!endif

#
# meta binaries directory
#
META_BIN = ..\..\..\Debug\meta\headergen
EXE = .exe

#
# meta output directory 
#
GEN_H_DIR = $(AAFTOOLKIT)\meta\headergen\include


PROGS=$(META_BIN)\classDefUid$(EXE) \
      $(META_BIN)\extEnumUid$(EXE) \
      $(META_BIN)\propertyDefUid$(EXE) \
      $(META_BIN)\propertyId$(EXE) \
      $(META_BIN)\storedObjectUid$(EXE) \
      $(META_BIN)\typeDefUid$(EXE) \
      $(META_BIN)\genDefInstances$(EXE)

# main build target
all :   $(GEN_H_DIR) \
        $(GEN_H_DIR)\AAFClassDefUIDs.h \
        $(GEN_H_DIR)\AAFExtEnum.h \
        $(GEN_H_DIR)\AAFPropertyDefs.h \
        $(GEN_H_DIR)\AAFPropertyIDs.h \
        $(GEN_H_DIR)\AAFStoredObjectIDs.h \
        $(GEN_H_DIR)\AAFTypeDefUIDs.h  \
        $(GEN_H_DIR)\AAFOperationDefs.h \
        $(GEN_H_DIR)\AAFParameterDefs.h \
        $(GEN_H_DIR)\AAFDataDefs.h \
        $(GEN_H_DIR)\AAFContainerDefs.h \
        $(GEN_H_DIR)\AAFInterpolatorDefs.h \
        $(GEN_H_DIR)\AAFOPDefs.h \
		$(GEN_H_DIR)\AAFKLVDataDefs.h \
		$(GEN_H_DIR)\AAFTaggedValueDefs.h \
		$(GEN_H_DIR)\AAFPluginDefs.h \
		$(GEN_H_DIR)\AAFCodecDefs.h \
		$(GEN_H_DIR)\AAFCompressionDefs.h

$(GEN_H_DIR) :
	IF exist $(GEN_H_DIR) (echo headergen include directory $(GEN_H_DIR) exists) ELSE (mkdir $(GEN_H_DIR) && echo headergen include directory $(GEN_H_DIR) created)

#
# targets for generated source files
#

$(GEN_H_DIR)\AAFClassDefUIDs.h : $(META_BIN)\classDefUids$(EXE)
	$(META_BIN)\classDefUids$(EXE) AAFClassDefUIDs > $(GEN_H_DIR)\AAFClassDefUIDs.h

$(GEN_H_DIR)\AAFExtEnum.h : $(META_BIN)\extEnumUid$(EXE)
	$(META_BIN)\extEnumUid AAFExtEnum > $(GEN_H_DIR)\AAFExtEnum.h

$(GEN_H_DIR)\AAFPropertyDefs.h : $(META_BIN)\propertyDefUid$(EXE)
	$(META_BIN)\propertyDefUid AAFPropertyDefs > $(GEN_H_DIR)\AAFPropertyDefs.h

$(GEN_H_DIR)\AAFPropertyIDs.h : $(META_BIN)\propertyId$(EXE)
	$(META_BIN)\propertyId AAFPropertyIDs > $(GEN_H_DIR)\AAFPropertyIDs.h

$(GEN_H_DIR)\AAFStoredObjectIDs.h : $(META_BIN)\storedObjectUid$(EXE)
	$(META_BIN)\storedObjectUid AAFStoredObjectIDs > $(GEN_H_DIR)\AAFStoredObjectIDs.h

$(GEN_H_DIR)\AAFTypeDefUIDs.h : $(META_BIN)\typeDefUid$(EXE)
	$(META_BIN)\typeDefUid AAFTypeDefUIDs > $(GEN_H_DIR)\AAFTypeDefUIDs.h



#
# targets for generated Definition Object instance header files
#

###NOTE specification of optional prefix ("kAAF" or similar)

$(GEN_H_DIR)\AAFOperationDefs.h : $(META_BIN)\genDefInstances$(EXE)
	$(META_BIN)\genDefInstances$(EXE) OperationDefinition kAAF > $(GEN_H_DIR)\AAFOperationDefs.h

$(GEN_H_DIR)\AAFParameterDefs.h : $(META_BIN)\genDefInstances$(EXE)
	$(META_BIN)\genDefInstances$(EXE) ParameterDefinition kAAF > $(GEN_H_DIR)\AAFParameterDefs.h

$(GEN_H_DIR)\AAFDataDefs.h : $(META_BIN)\genDefInstances$(EXE)
	$(META_BIN)\genDefInstances$(EXE) DataDefinition kAAF > $(GEN_H_DIR)\AAFDataDefs.h

$(GEN_H_DIR)\AAFContainerDefs.h : $(META_BIN)\genDefInstances$(EXE)
	$(META_BIN)\genDefInstances$(EXE) ContainerDefinition kAAF > $(GEN_H_DIR)\AAFContainerDefs.h

$(GEN_H_DIR)\AAFInterpolatorDefs.h : $(META_BIN)\genDefInstances$(EXE)
	$(META_BIN)\genDefInstances$(EXE) InterpolationDefinition kAAF > $(GEN_H_DIR)\AAFInterpolatorDefs.h

$(GEN_H_DIR)\AAFOPDefs.h : $(META_BIN)\genDefInstances$(EXE)
	$(META_BIN)\genDefInstances$(EXE) OPDefinition kAAF > $(GEN_H_DIR)\AAFOPDefs.h

$(GEN_H_DIR)\AAFKLVDataDefs.h : $(META_BIN)\genDefInstances$(EXE)
	$(META_BIN)\genDefInstances$(EXE) KLVDataDefinition kAAF > $(GEN_H_DIR)\AAFKLVDataDefs.h

$(GEN_H_DIR)\AAFTaggedValueDefs.h : $(META_BIN)\genDefInstances$(EXE)
	$(META_BIN)\genDefInstances$(EXE) TaggedValueDefinition kAAF > $(GEN_H_DIR)\AAFTaggedValueDefs.h

$(GEN_H_DIR)\AAFPluginDefs.h : $(META_BIN)\genDefInstances$(EXE)
	$(META_BIN)\genDefInstances$(EXE) PluginDefinition kAAF > $(GEN_H_DIR)\AAFPluginDefs.h

$(GEN_H_DIR)\AAFCodecDefs.h : $(META_BIN)\genDefInstances$(EXE)
	$(META_BIN)\genDefInstances$(EXE) CodecDefinition kAAF > $(GEN_H_DIR)\AAFCodecDefs.h

$(GEN_H_DIR)\AAFCompressionDefs.h : $(META_BIN)\genDefInstances$(EXE)
	$(META_BIN)\genDefInstances$(EXE) CompressionDefinition kAAF > $(GEN_H_DIR)\AAFCompressionDefs.h	
	
# Clean target doesn't do anything yet
clean:
