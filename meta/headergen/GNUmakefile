INSTALL_H_DIR=../../ref-impl/include
GEN_H_DIR=./include
EXE=
OBJ=.o
COMPILE=c++
COMPILEFLAGS=-I.. -I../../AAFi686LinuxSDK/g++/include
LINK=c++

PROGS=classDefUid$(EXE) \
      extEnumUid$(EXE) \
      propertyDefUid$(EXE) \
      propertyId$(EXE) \
      storedObjectUid$(EXE) \
      typeDefUid$(EXE)

# main build target
all : $(PROGS) \
        $(GEN_H_DIR)/AAFClassDefUIDs.h \
        $(GEN_H_DIR)/AAFExtEnum.h \
        $(GEN_H_DIR)/AAFPropertyDefs.h \
        $(GEN_H_DIR)/AAFPropertyIDs.h \
        $(GEN_H_DIR)/AAFStoredObjectIDs.h \
        $(GEN_H_DIR)/AAFTypeDefUIDs.h

# utilities
headerGenUtils$(OBJ) : headerGenUtils.cpp headerGenUtils.h
	$(COMPILE) $(COMPILEFLAGS) -c $< -o $@

# targets for each program
classDefUid$(EXE) : headerGenUtils$(OBJ) classDefUid$(OBJ)
	$(LINK) $^ -o $@

classDefUid$(OBJ) : classDefUid.cpp headerGenUtils.h \
        ../AAFMetaDictionary.h 
	$(COMPILE) $(COMPILEFLAGS) -c $< -o $@

extEnumUid$(EXE) : headerGenUtils$(OBJ) extEnumUid$(OBJ)
	$(LINK) $^ -o $@

extEnumUid$(OBJ) : extEnumUid.cpp headerGenUtils.h \
        ../AAFMetaDictionary.h
	$(COMPILE) $(COMPILEFLAGS) -c $< -o $@

propertyDefUid$(EXE) : headerGenUtils$(OBJ) propertyDefUid$(OBJ)
	$(LINK) $^ -o $@

propertyDefUid$(OBJ) : propertyDefUid.cpp headerGenUtils.h \
        ../AAFMetaDictionary.h
	$(COMPILE) $(COMPILEFLAGS) -c $< -o $@

propertyId$(EXE) : headerGenUtils$(OBJ) propertyId$(OBJ)
	$(LINK) $^ -o $@

propertyId$(OBJ) : propertyId.cpp headerGenUtils.h \
        ../AAFMetaDictionary.h
	$(COMPILE) $(COMPILEFLAGS) -c $< -o $@

storedObjectUid$(EXE) : headerGenUtils$(OBJ) storedObjectUid$(OBJ)
	$(LINK) $^ -o $@

storedObjectUid$(OBJ) : storedObjectUid.cpp headerGenUtils.h \
        ../AAFMetaDictionary.h
	$(COMPILE) $(COMPILEFLAGS) -c $< -o $@

typeDefUid$(EXE) : headerGenUtils$(OBJ) typeDefUid$(OBJ)
	$(LINK) $^ -o $@

typeDefUid$(OBJ) : typeDefUid.cpp headerGenUtils.h \
        ../AAFMetaDictionary.h
	$(COMPILE) $(COMPILEFLAGS) -c $< -o $@

# targets for generated source files
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


#install targets
$(INSTALL_H_DIR)/AAFClassDefUIDs.h : $(GEN_H_DIR)/AAFClassDefUIDs.h
	cp $(GEN_H_DIR)/AAFClassDefUIDs.h $(INSTALL_H_DIR)/AAFClassDefUIDs.h

$(INSTALL_H_DIR)/AAFExtEnum.h : $(GEN_H_DIR)/AAFExtEnum.h
	cp $(GEN_H_DIR)/AAFExtEnum.h $(INSTALL_H_DIR)/AAFExtEnum.h

$(INSTALL_H_DIR)/AAFPropertyDefs.h : $(GEN_H_DIR)/AAFPropertyDefs.h
	cp $(GEN_H_DIR)/AAFPropertyDefs.h $(INSTALL_H_DIR)/AAFPropertyDefs.h

$(INSTALL_H_DIR)/AAFPropertyIDs.h : $(GEN_H_DIR)/AAFPropertyIDs.h
	cp $(GEN_H_DIR)/AAFPropertyIDs.h $(INSTALL_H_DIR)/AAFPropertyIDs.h

$(INSTALL_H_DIR)/AAFStoredObjectIDs.h : $(GEN_H_DIR)/AAFStoredObjectIDs.h
	cp $(GEN_H_DIR)/AAFStoredObjectIDs.h $(INSTALL_H_DIR)/AAFStoredObjectIDs.h

$(INSTALL_H_DIR)/AAFTypeDefUIDs.h : $(GEN_H_DIR)/AAFTypeDefUIDs.h
	cp $(GEN_H_DIR)/AAFTypeDefUIDs.h $(INSTALL_H_DIR)/AAFTypeDefUIDs.h



# other targets
clean:
	rm -f $(PROGS)
	rm -rf *$(OBJ) *~
	rm -rf $(GEN_H_DIR)/AAFClassDefUIDs.h
	rm -rf $(GEN_H_DIR)/AAFExtEnum.h
	rm -rf $(GEN_H_DIR)/AAFPropertyDefs.h
	rm -rf $(GEN_H_DIR)/AAFPropertyIDs.h
	rm -rf $(GEN_H_DIR)/AAFStoredObjectIDs.h
	rm -rf $(GEN_H_DIR)/AAFTypeDefUIDs.h

install: $(INSTALL_H_DIR)/AAFClassDefUIDs.h \
        $(INSTALL_H_DIR)/AAFExtEnum.h \
        $(INSTALL_H_DIR)/AAFPropertyDefs.h \
        $(INSTALL_H_DIR)/AAFPropertyIDs.h \
        $(INSTALL_H_DIR)/AAFStoredObjectIDs.h \
        $(INSTALL_H_DIR)/AAFTypeDefUIDs.h
