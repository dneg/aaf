INSTALL_H_DIR=../../ref-impl/include
EXE=
OBJ=.o
COMPILE=c++
COMPILEFLAGS=-I../../AAFi686LinuxSDK/g++/include
LINK=c++

PROGS=classDefUid$(EXE) \
      extEnumUid$(EXE) \
      propertyDefUid$(EXE) \
      propertyId$(EXE) \
      storedObjectUid$(EXE) \
      typeDefUid$(EXE)

# main build target
all : $(PROGS) \
       AAFClassDefUIDs.h \
        AAFExtEnum.h \
        AAFPropertyDefs.h \
        AAFPropertyIDs.h \
        AAFStoredObjectIDs.h \
        AAFTypeDefUIDs.h

# utilities
headerGenUtils$(OBJ) : headerGenUtils.cpp headerGenUtils.h
	$(COMPILE) $(COMPILEFLAGS) -c $< -o $@

AAFMetaDictionary.h : ../AAFMetaDictionary.h
	cp ../AAFMetaDictionary.h AAFMetaDictionary.h


# targets for each program
classDefUid$(EXE) : headerGenUtils$(OBJ) classDefUid$(OBJ)
	$(LINK) $^ -o $@

classDefUid$(OBJ) : classDefUid.cpp headerGenUtils.h \
        AAFMetaDictionary.h literalAuidDef.h 
	$(COMPILE) $(COMPILEFLAGS) -c $< -o $@

extEnumUid$(EXE) : headerGenUtils$(OBJ) extEnumUid$(OBJ)
	$(LINK) $^ -o $@

extEnumUid$(OBJ) : extEnumUid.cpp headerGenUtils.h \
        AAFMetaDictionary.h literalAuidDef.h
	$(COMPILE) $(COMPILEFLAGS) -c $< -o $@

propertyDefUid$(EXE) : headerGenUtils$(OBJ) propertyDefUid$(OBJ)
	$(LINK) $^ -o $@

propertyDefUid$(OBJ) : propertyDefUid.cpp headerGenUtils.h \
        AAFMetaDictionary.h literalAuidDef.h
	$(COMPILE) $(COMPILEFLAGS) -c $< -o $@

propertyId$(EXE) : headerGenUtils$(OBJ) propertyId$(OBJ)
	$(LINK) $^ -o $@

propertyId$(OBJ) : propertyId.cpp headerGenUtils.h \
        AAFMetaDictionary.h literalAuidDef.h
	$(COMPILE) $(COMPILEFLAGS) -c $< -o $@

storedObjectUid$(EXE) : headerGenUtils$(OBJ) storedObjectUid$(OBJ)
	$(LINK) $^ -o $@

storedObjectUid$(OBJ) : storedObjectUid.cpp headerGenUtils.h \
        AAFMetaDictionary.h literalAuidDef.h
	$(COMPILE) $(COMPILEFLAGS) -c $< -o $@

typeDefUid$(EXE) : headerGenUtils$(OBJ) typeDefUid$(OBJ)
	$(LINK) $^ -o $@

typeDefUid$(OBJ) : typeDefUid.cpp headerGenUtils.h \
        AAFMetaDictionary.h literalAuidDef.h
	$(COMPILE) $(COMPILEFLAGS) -c $< -o $@

# targets for generated source files
AAFClassDefUIDs.h : classDefUid$(EXE)
	./classDefUid AAFClassDefUIDs > AAFClassDefUIDs.h

AAFExtEnum.h : extEnumUid$(EXE)
	./extEnumUid AAFExtEnum > AAFExtEnum.h

AAFPropertyDefs.h : propertyDefUid$(EXE)
	./propertyDefUid AAFPropertyDefs > AAFPropertyDefs.h

AAFPropertyIDs.h : propertyId$(EXE)
	./propertyId AAFPropertyIDs > AAFPropertyIDs.h

AAFStoredObjectIDs.h : storedObjectUid$(EXE)
	./storedObjectUid AAFStoredObjectIDs > AAFStoredObjectIDs.h

AAFTypeDefUIDs.h : typeDefUid$(EXE)
	./typeDefUid AAFTypeDefUIDs > AAFTypeDefUIDs.h


#install targets
$(INSTALL_H_DIR)/AAFClassDefUIDs.h : AAFClassDefUIDs.h
	touch $(INSTALL_H_DIR)/AAFClassDefUIDs.h
	chmod a+w $(INSTALL_H_DIR)/AAFClassDefUIDs.h
	cp AAFClassDefUIDs.h $(INSTALL_H_DIR)/AAFClassDefUIDs.h

$(INSTALL_H_DIR)/AAFExtEnum.h : AAFExtEnum.h
	touch $(INSTALL_H_DIR)/AAFExtEnum.h
	chmod a+w $(INSTALL_H_DIR)/AAFExtEnum.h
	cp AAFExtEnum.h $(INSTALL_H_DIR)/AAFExtEnum.h

$(INSTALL_H_DIR)/AAFPropertyDefs.h : AAFPropertyDefs.h
	touch $(INSTALL_H_DIR)/AAFPropertyDefs.h
	chmod a+w $(INSTALL_H_DIR)/AAFPropertyDefs.h
	cp AAFPropertyDefs.h $(INSTALL_H_DIR)/AAFPropertyDefs.h

$(INSTALL_H_DIR)/AAFPropertyIDs.h : AAFPropertyIDs.h
	touch $(INSTALL_H_DIR)/AAFPropertyIDs.h
	chmod a+w $(INSTALL_H_DIR)/AAFPropertyIDs.h
	cp AAFPropertyIDs.h $(INSTALL_H_DIR)/AAFPropertyIDs.h

$(INSTALL_H_DIR)/AAFStoredObjectIDs.h : AAFStoredObjectIDs.h
	touch $(INSTALL_H_DIR)/AAFStoredObjectIDs.h
	chmod a+w $(INSTALL_H_DIR)/AAFStoredObjectIDs.h
	cp AAFStoredObjectIDs.h $(INSTALL_H_DIR)/AAFStoredObjectIDs.h

$(INSTALL_H_DIR)/AAFTypeDefUIDs.h : AAFTypeDefUIDs.h
	touch $(INSTALL_H_DIR)/AAFTypeDefUIDs.h
	chmod a+w $(INSTALL_H_DIR)/AAFTypeDefUIDs.h
	cp AAFTypeDefUIDs.h $(INSTALL_H_DIR)/AAFTypeDefUIDs.h



# other targets
clean:
	rm -f $(PROGS)
	rm -rf *$(OBJ) *~
	rm -rf AAFClassDefUIDs.h
	rm -rf AAFExtEnum.h
	rm -rf AAFPropertyDefs.h
	rm -rf AAFPropertyIDs.h
	rm -rf AAFStoredObjectIDs.h
	rm -rf AAFTypeDefUIDs.h
	rm -rf AAFMetaDictionary.h

install: $(INSTALL_H_DIR)/AAFClassDefUIDs.h \
        $(INSTALL_H_DIR)/AAFExtEnum.h \
        $(INSTALL_H_DIR)/AAFPropertyDefs.h \
        $(INSTALL_H_DIR)/AAFPropertyIDs.h \
        $(INSTALL_H_DIR)/AAFStoredObjectIDs.h \
        $(INSTALL_H_DIR)/AAFTypeDefUIDs.h
