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
# The Original Code of this file is Copyright 1998-2005, Licensor of the
# AAF Association.
#
# The Initial Developer of the Original Code of this file and the
# Licensor of the AAF Association is Avid Technology.
# All rights reserved.
#
###############################################################################

AWK=awk
CP=cp -p
ECHO=echo
RM=rm -f

AWKFILE=mkdict.awk
EXCEL="c:/program files/Microsoft Office/Office/EXCEL.EXE"
INSTALL_DIR=../ref-impl/include

all : AAFMetaDictionary.h

AAFMetaDict.csv : AAFMetaDict.xls
	@$(ECHO) "1) Save the new .xls file as a .csv file"
	@$(ECHO) "   Choose File/Save As ..."
	@$(ECHO) "   File name: AAFMetaDict.csv"
	@$(ECHO) "   Save as type: CSV(OS/2 or MSDOS)"
	@$(ECHO) "   Choose Yes in the annoying dialog"
	@$(ECHO) "2) Get out of Excel"
	@$(ECHO) "   Choose File/Exit"
	@$(ECHO) "   Choose No in the annoying dialog"
	$(EXCEL) /e /r AAFMetaDict.xls /p .

AAFMetaDictionary.h : AAFMetaDict.csv $(AWKFILE)
	$(AWK) -f $(AWKFILE) -v SYM=r_name AAFMetaDict.csv > AAFMetaDictionary.h

$(INSTALL_DIR)/AAFMetaDictionary.h: AAFMetaDictionary.h
	@$(ECHO) "Installing new Meta Dictionary"
	$(RM) $(INSTALL_DIR)/AAFMetaDictionary.h
	$(CP) AAFMetaDictionary.h $(INSTALL_DIR)/AAFMetaDictionary.h 

install: $(INSTALL_DIR)/AAFMetaDictionary.h

clean:
	$(RM) AAFMetaDict.csv
	$(RM) AAFMetaDictionary.h

