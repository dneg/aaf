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
# The Original Code of this file is Copyright 1998-2004, Licensor of the
# AAF Association.
#
# The Initial Developer of the Original Code of this file and the
# Licensor of the AAF Association is Avid Technology.
# All rights reserved.
#
###############################################################################

INSTALL_H_DIR=../../ref-impl/include

# main build target
all : AAFClassDefUIDs.h \
        AAFExtEnum.h \
        AAFPropertyDefs.h \
        AAFPropertyIDs.h \
        AAFStoredObjectIDs.h \
        AAFTypeDefUIDs.h

# utilities
headerGenUtils.obj : headerGenUtils.cpp headerGenUtils.h

AAFMetaDictionary.h : ../AAFMetaDictionary.h
	cp ../AAFMetaDictionary.h AAFMetaDictionary.h


# targets for each program
classDefUid.exe : headerGenUtils.obj classDefUid.obj

classDefUid.obj : classDefUid.cpp headerGenUtils.h \
        AAFMetaDictionary.h literalAuidDef.h 

extEnumUid.exe : headerGenUtils.obj extEnumUid.obj

extEnumUid.obj : extEnumUid.cpp headerGenUtils.h \
        AAFMetaDictionary.h literalAuidDef.h

propertyDefUid.exe : headerGenUtils.obj propertyDefUid.obj

propertyDefUid.obj : propertyDefUid.cpp headerGenUtils.h \
        AAFMetaDictionary.h literalAuidDef.h

propertyId.exe : headerGenUtils.obj propertyId.obj

propertyId.obj : propertyId.cpp headerGenUtils.h \
        AAFMetaDictionary.h literalAuidDef.h

storedObjectUid.exe : headerGenUtils.obj storedObjectUid.obj

storedObjectUid.obj : storedObjectUid.cpp headerGenUtils.h \
        AAFMetaDictionary.h literalAuidDef.h

typeDefUid.exe : headerGenUtils.obj typeDefUid.obj

typeDefUid.obj : typeDefUid.cpp headerGenUtils.h \
        AAFMetaDictionary.h literalAuidDef.h


# targets for generated source files
AAFClassDefUIDs.h : classDefUid.exe
	./classDefUid AAFClassDefUIDs > AAFClassDefUIDs.h

AAFExtEnum.h : extEnumUid.exe
	./extEnumUid AAFExtEnum > AAFExtEnum.h

AAFPropertyDefs.h : propertyDefUid.exe
	./propertyDefUid AAFPropertyDefs > AAFPropertyDefs.h

AAFPropertyIDs.h : propertyId.exe
	./propertyId AAFPropertyIDs > AAFPropertyIDs.h

AAFStoredObjectIDs.h : storedObjectUid.exe
	./storedObjectUid AAFStoredObjectIDs > AAFStoredObjectIDs.h

AAFTypeDefUIDs.h : typeDefUid.exe
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
	rm -rf *.obj *.exe *~
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
