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
# The Original Code of this file is Copyright 1998-2006, Licensor of the
# AAF Association.
#
# The Initial Developer of the Original Code of this file and the
# Licensor of the AAF Association is Avid Technology.
# All rights reserved.
#
###############################################################################

INSTALL_H_DIR=../../ref-impl/include
GEN_H_DIR=./include
INSTALL_BUILTINS_DIR=../../ref-impl/src/impl
EXE=
OBJ=.o
COMPILE=c++
COMPILEFLAGS=-Wall -Werror -I.. -I../../ref-impl/include/ref-api -I../../ref-impl/include
LINK=c++
RM=rm -f
CP= cp -p

PROGS=classDefUid$(EXE) \
	extEnumUid$(EXE) \
	propertyDefUid$(EXE) \
	propertyId$(EXE) \
	storedObjectUid$(EXE) \
	typeDefUid$(EXE) \
	genDefInstances$(EXE) \
	genDefInstBuiltins$(EXE) \
	result$(EXE)

HEADERS=AAFClassDefUIDs.h \
	AAFExtEnum.h \
	AAFPropertyDefs.h \
	AAFPropertyIDs.h \
	AAFStoredObjectIDs.h \
	AAFTypeDefUIDs.h  \
	AAFOperationDefs.h \
	AAFParameterDefs.h \
	AAFDataDefs.h \
	AAFContainerDefs.h \
	AAFInterpolatorDefs.h \
	AAFOPDefs.h \
	AAFKLVDataDefs.h \
	AAFTaggedValueDefs.h \
	AAFPluginDefs.h \
	AAFCodecDefs.h \
	AAFCompressionDefs.h \
	AAFResult.h

BUILTINS=ImplAAFDictionary.cpp_ContainerDefs \
	ImplAAFDictionary.cpp_DataDefs

# main build target
all : $(PROGS) \
	$(GEN_H_DIR) \
	$(addprefix $(GEN_H_DIR)/,$(HEADERS)) \
	$(addprefix $(GEN_H_DIR)/,$(BUILTINS))

# utilities
headerGenUtils$(OBJ) : headerGenUtils.cpp headerGenUtils.h
	$(COMPILE) $(COMPILEFLAGS) -c $< -o $@

# targets for each program
classDefUid$(EXE) : headerGenUtils$(OBJ) classDefUid$(OBJ)
	$(LINK) $^ -o $@

classDefUid$(OBJ) : classDefUid.cpp headerGenUtils.h ../AAFMetaDictionary.h 
	$(COMPILE) $(COMPILEFLAGS) -c $< -o $@

extEnumUid$(EXE) : headerGenUtils$(OBJ) extEnumUid$(OBJ)
	$(LINK) $^ -o $@

extEnumUid$(OBJ) : extEnumUid.cpp headerGenUtils.h ../AAFMetaDictionary.h
	$(COMPILE) $(COMPILEFLAGS) -c $< -o $@

propertyDefUid$(EXE) : headerGenUtils$(OBJ) propertyDefUid$(OBJ)
	$(LINK) $^ -o $@

propertyDefUid$(OBJ) : propertyDefUid.cpp headerGenUtils.h ../AAFMetaDictionary.h
	$(COMPILE) $(COMPILEFLAGS) -c $< -o $@

propertyId$(EXE) : headerGenUtils$(OBJ) propertyId$(OBJ)
	$(LINK) $^ -o $@

propertyId$(OBJ) : propertyId.cpp headerGenUtils.h ../AAFMetaDictionary.h
	$(COMPILE) $(COMPILEFLAGS) -c $< -o $@

storedObjectUid$(EXE) : headerGenUtils$(OBJ) storedObjectUid$(OBJ)
	$(LINK) $^ -o $@

storedObjectUid$(OBJ) : storedObjectUid.cpp headerGenUtils.h ../AAFMetaDictionary.h
	$(COMPILE) $(COMPILEFLAGS) -c $< -o $@

typeDefUid$(EXE) : headerGenUtils$(OBJ) typeDefUid$(OBJ)
	$(LINK) $^ -o $@

typeDefUid$(OBJ) : typeDefUid.cpp headerGenUtils.h ../AAFMetaDictionary.h
	$(COMPILE) $(COMPILEFLAGS) -c $< -o $@

genDefInstances$(EXE) : headerGenUtils$(OBJ) genDefInstances$(OBJ)
	$(LINK) $^ -o $@

genDefInstances$(OBJ) : genDefInstances.cpp headerGenUtils.h ../AAFMetaDictionary.h
	$(COMPILE) $(COMPILEFLAGS) -c $< -o $@

genDefInstBuiltins$(EXE) : headerGenUtils$(OBJ) genDefInstBuiltins$(OBJ)
	$(LINK) $^ -o $@

genDefInstBuiltins$(OBJ) : genDefInstBuiltins.cpp headerGenUtils.h ../AAFMetaDictionary.h
	$(COMPILE) $(COMPILEFLAGS) -c $< -o $@

result$(EXE) : headerGenUtils$(OBJ) result$(OBJ)
	$(LINK) $^ -o $@

result$(OBJ) : result.cpp headerGenUtils.h ../../ref-impl/include/AAFMetaResult.h
	$(COMPILE) $(COMPILEFLAGS) -c $< -o $@


# targets for generated source files
$(GEN_H_DIR) :
	mkdir $(GEN_H_DIR)

$(GEN_H_DIR)/AAFClassDefUIDs.h : classDefUid$(EXE)
	./classDefUid AAFClassDefUIDs > $(GEN_H_DIR)/AAFClassDefUIDs.h

$(GEN_H_DIR)/AAFExtEnum.h : extEnumUid$(EXE)
	./extEnumUid AAFExtEnum > $(GEN_H_DIR)/AAFExtEnum.h

$(GEN_H_DIR)/AAFPropertyDefs.h : propertyDefUid$(EXE)
	./propertyDefUid AAFPropertyDefs > $(GEN_H_DIR)/AAFPropertyDefs.h

$(GEN_H_DIR)/AAFPropertyIDs.h : propertyId$(EXE)
	./propertyId AAFPropertyIDs > $(GEN_H_DIR)/AAFPropertyIDs.h

$(GEN_H_DIR)/AAFStoredObjectIDs.h : storedObjectUid$(EXE)
	./storedObjectUid AAFStoredObjectIDs > $(GEN_H_DIR)/AAFStoredObjectIDs.h

$(GEN_H_DIR)/AAFTypeDefUIDs.h : typeDefUid$(EXE)
	./typeDefUid AAFTypeDefUIDs > $(GEN_H_DIR)/AAFTypeDefUIDs.h

# targets for generated Definition Object instance header files

###NOTE specification of optional prefix ("kAAF" or similar)

$(GEN_H_DIR)/AAFOperationDefs.h : genDefInstances$(EXE)
	./genDefInstances OperationDefinition kAAF > $(GEN_H_DIR)/AAFOperationDefs.h

$(GEN_H_DIR)/AAFParameterDefs.h : genDefInstances$(EXE)
	./genDefInstances ParameterDefinition kAAF > $(GEN_H_DIR)/AAFParameterDefs.h

$(GEN_H_DIR)/AAFDataDefs.h : genDefInstances$(EXE)
	./genDefInstances DataDefinition kAAF > $(GEN_H_DIR)/AAFDataDefs.h

$(GEN_H_DIR)/AAFContainerDefs.h : genDefInstances$(EXE)
	./genDefInstances ContainerDefinition kAAF > $(GEN_H_DIR)/AAFContainerDefs.h

$(GEN_H_DIR)/AAFInterpolatorDefs.h : genDefInstances$(EXE)
	./genDefInstances InterpolationDefinition kAAF > $(GEN_H_DIR)/AAFInterpolatorDefs.h

$(GEN_H_DIR)/AAFOPDefs.h : genDefInstances$(EXE)
	./genDefInstances OPDefinition kAAF > $(GEN_H_DIR)/AAFOPDefs.h

$(GEN_H_DIR)/AAFKLVDataDefs.h : genDefInstances$(EXE)
	./genDefInstances KLVDataDefinition kAAF > $(GEN_H_DIR)/AAFKLVDataDefs.h

$(GEN_H_DIR)/AAFTaggedValueDefs.h : genDefInstances$(EXE)
	./genDefInstances TaggedValueDefinition kAAF > $(GEN_H_DIR)/AAFTaggedValueDefs.h

$(GEN_H_DIR)/AAFPluginDefs.h : genDefInstances$(EXE)
	./genDefInstances PluginDefinition kAAF > $(GEN_H_DIR)/AAFPluginDefs.h

$(GEN_H_DIR)/AAFCodecDefs.h : genDefInstances$(EXE)
	./genDefInstances CodecDefinition kAAF > $(GEN_H_DIR)/AAFCodecDefs.h

$(GEN_H_DIR)/AAFCompressionDefs.h : genDefInstances$(EXE)
	./genDefInstances CompressionDefinition kAAF > $(GEN_H_DIR)/AAFCompressionDefs.h

# targets for generated Definition Object instance builtins files

$(GEN_H_DIR)/ImplAAFDictionary.cpp_DataDefs : genDefInstBuiltins$(EXE)
	./genDefInstBuiltins DataDefinition kAAF > $(GEN_H_DIR)/ImplAAFDictionary.cpp_DataDefs

$(GEN_H_DIR)/ImplAAFDictionary.cpp_ContainerDefs : genDefInstBuiltins$(EXE)
	./genDefInstBuiltins ContainerDefinition kAAF > $(GEN_H_DIR)/ImplAAFDictionary.cpp_ContainerDefs

$(GEN_H_DIR)/AAFResult.h : result$(EXE)
	./result AAFResult > $(GEN_H_DIR)/AAFResult.h

# install targets

$(INSTALL_H_DIR)/AAFClassDefUIDs.h : $(GEN_H_DIR)/AAFClassDefUIDs.h
	$(CP) $(GEN_H_DIR)/AAFClassDefUIDs.h $(INSTALL_H_DIR)/AAFClassDefUIDs.h

$(INSTALL_H_DIR)/AAFExtEnum.h : $(GEN_H_DIR)/AAFExtEnum.h
	$(CP) $(GEN_H_DIR)/AAFExtEnum.h $(INSTALL_H_DIR)/AAFExtEnum.h

$(INSTALL_H_DIR)/AAFPropertyDefs.h : $(GEN_H_DIR)/AAFPropertyDefs.h
	$(CP) $(GEN_H_DIR)/AAFPropertyDefs.h $(INSTALL_H_DIR)/AAFPropertyDefs.h

$(INSTALL_H_DIR)/AAFPropertyIDs.h : $(GEN_H_DIR)/AAFPropertyIDs.h
	$(CP) $(GEN_H_DIR)/AAFPropertyIDs.h $(INSTALL_H_DIR)/AAFPropertyIDs.h

$(INSTALL_H_DIR)/AAFStoredObjectIDs.h : $(GEN_H_DIR)/AAFStoredObjectIDs.h
	$(CP) $(GEN_H_DIR)/AAFStoredObjectIDs.h $(INSTALL_H_DIR)/AAFStoredObjectIDs.h

$(INSTALL_H_DIR)/AAFTypeDefUIDs.h : $(GEN_H_DIR)/AAFTypeDefUIDs.h
	$(CP) $(GEN_H_DIR)/AAFTypeDefUIDs.h $(INSTALL_H_DIR)/AAFTypeDefUIDs.h

# install targets for generated Definition Object instance header files

$(INSTALL_H_DIR)/AAFOperationDefs.h : $(GEN_H_DIR)/AAFOperationDefs.h
	$(CP) $(GEN_H_DIR)/AAFOperationDefs.h $(INSTALL_H_DIR)/AAFOperationDefs.h

$(INSTALL_H_DIR)/AAFParameterDefs.h : $(GEN_H_DIR)/AAFParameterDefs.h
	$(CP) $(GEN_H_DIR)/AAFParameterDefs.h $(INSTALL_H_DIR)/AAFParameterDefs.h

$(INSTALL_H_DIR)/AAFDataDefs.h : $(GEN_H_DIR)/AAFDataDefs.h
	$(CP) $(GEN_H_DIR)/AAFDataDefs.h $(INSTALL_H_DIR)/AAFDataDefs.h

$(INSTALL_H_DIR)/AAFContainerDefs.h : $(GEN_H_DIR)/AAFContainerDefs.h
	$(CP) $(GEN_H_DIR)/AAFContainerDefs.h $(INSTALL_H_DIR)/AAFContainerDefs.h

$(INSTALL_H_DIR)/AAFInterpolatorDefs.h : $(GEN_H_DIR)/AAFInterpolatorDefs.h
	$(CP) $(GEN_H_DIR)/AAFInterpolatorDefs.h $(INSTALL_H_DIR)/AAFInterpolatorDefs.h

$(INSTALL_H_DIR)/AAFOPDefs.h : $(GEN_H_DIR)/AAFOPDefs.h
	$(CP) $(GEN_H_DIR)/AAFOPDefs.h $(INSTALL_H_DIR)/AAFOPDefs.h

$(INSTALL_H_DIR)/AAFKLVDataDefs.h : $(GEN_H_DIR)/AAFKLVDataDefs.h
	$(CP) $(GEN_H_DIR)/AAFKLVDataDefs.h $(INSTALL_H_DIR)/AAFKLVDataDefs.h

$(INSTALL_H_DIR)/AAFTaggedValueDefs.h : $(GEN_H_DIR)/AAFTaggedValueDefs.h
	$(CP) $(GEN_H_DIR)/AAFTaggedValueDefs.h $(INSTALL_H_DIR)/AAFTaggedValueDefs.h

$(INSTALL_H_DIR)/AAFPluginDefs.h : $(GEN_H_DIR)/AAFPluginDefs.h
	$(CP) $(GEN_H_DIR)/AAFPluginDefs.h $(INSTALL_H_DIR)/AAFPluginDefs.h

$(INSTALL_H_DIR)/AAFCodecDefs.h : $(GEN_H_DIR)/AAFCodecDefs.h
	$(CP) $(GEN_H_DIR)/AAFCodecDefs.h $(INSTALL_H_DIR)/AAFCodecDefs.h

$(INSTALL_H_DIR)/AAFCompressionDefs.h : $(GEN_H_DIR)/AAFCompressionDefs.h
	$(CP) $(GEN_H_DIR)/AAFCompressionDefs.h $(INSTALL_H_DIR)/AAFCompressionDefs.h

$(INSTALL_H_DIR)/AAFResult.h : $(GEN_H_DIR)/AAFResult.h
	$(CP) $(GEN_H_DIR)/AAFResult.h $(INSTALL_H_DIR)/AAFResult.h

# install targets for generated Definition Object instance builtins files

$(INSTALL_BUILTINS_DIR)/ImplAAFDictionary.cpp_DataDefs : $(GEN_H_DIR)/ImplAAFDictionary.cpp_DataDefs
	$(CP) $(GEN_H_DIR)/ImplAAFDictionary.cpp_DataDefs $(INSTALL_BUILTINS_DIR)/ImplAAFDictionary.cpp_DataDefs

$(INSTALL_BUILTINS_DIR)/ImplAAFDictionary.cpp_ContainerDefs : $(GEN_H_DIR)/ImplAAFDictionary.cpp_ContainerDefs
	$(CP) $(GEN_H_DIR)/ImplAAFDictionary.cpp_ContainerDefs $(INSTALL_BUILTINS_DIR)/ImplAAFDictionary.cpp_ContainerDefs

# other targets

clean:
	$(RM) $(PROGS)
	$(RM) *$(OBJ) *~
	$(RM) $(addprefix $(GEN_H_DIR)/,$(HEADERS))
	$(RM) $(addprefix $(GEN_H_DIR)/,$(BUILTINS))

install: $(addprefix $(INSTALL_H_DIR)/,$(HEADERS)) \
	$(addprefix $(INSTALL_BUILTINS_DIR)/,$(BUILTINS))

