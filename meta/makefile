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

EXCEL="c:/program files/Microsoft Office/Office/EXCEL.EXE"

all : AAFMetaDictionary.h

AAFMetaDict.csv : AAFMetaDict.xls
	@ echo "1) Save the new .xls file as a .csv file"
	@ echo "   Choose File/Save As ..."
	@ echo "   File name: AAFMetaDict.csv"
	@ echo "   Save as type: CSV(OS/2 or MSDOS)"
	@ echo "   Choose Yes in the annoying dialog"
	@ echo "2) get out of Excel"
	@ echo "   Choose File/Exit"
	@ echo "   Choose No in the annoying dialog"
	$(EXCEL) /e /r AAFMetaDict.xls /p .

AAFMetaDictionary.h : AAFMetaDict.csv
	awk -f mkdict.awk AAFMetaDict.csv > AAFMetaDictionary.h

clean:
	rm -f AAFMetaDict.csv
	rm -f AAFMetaDictionary.h
