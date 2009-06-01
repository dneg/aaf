###############################################################################
#
# $Id$ $Name$
#
# The contents of this file are subject to the AAF SDK Public Source
# License Agreement Version 2.0 (the "License"); You may not use this
# file except in compliance with the License.  The License is available
# in AAFSDKPSL.TXT, or you may obtain a copy of the License from the
# Advanced Media Workflow Association, Inc., or its successor.
#
# Software distributed under the License is distributed on an "AS IS"
# basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
# the License for the specific language governing rights and limitations
# under the License.  Refer to Section 3.3 of the License for proper use
# of this Exhibit.
#
# WARNING:  Please contact the Advanced Media Workflow Association,
# Inc., for more information about any additional licenses to
# intellectual property covering the AAF Standard that may be required
# to create and distribute AAF compliant products.
# (http://www.amwa.tv/policies).
#
# Copyright Notices:
# The Original Code of this file is Copyright 1998-2009, licensor of the
# Advanced Media Workflow Association.  All rights reserved.
#
# The Initial Developer of the Original Code of this file and the
# licensor of the Advanced Media Workflow Association is
# Avid Technology.
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
	$(AWK) -f $(AWKFILE) -v APP=AAF -v ALIAS=YES -v SYM=s_sym AAFMetaDict.csv > AAFMetaDictionary.h

$(INSTALL_DIR)/AAFMetaDictionary.h: AAFMetaDictionary.h
	@$(ECHO) "Installing new Meta Dictionary"
	$(RM) $(INSTALL_DIR)/AAFMetaDictionary.h
	$(CP) AAFMetaDictionary.h $(INSTALL_DIR)/AAFMetaDictionary.h 

install: $(INSTALL_DIR)/AAFMetaDictionary.h

clean:
	$(RM) AAFMetaDict.csv
	$(RM) AAFMetaDictionary.h

