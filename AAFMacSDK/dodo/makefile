########################################################################
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
########################################################################

dod = {aaf}dodo:


all Ä start base_mac makedodo makesdk finish

start Ä $OutOfDate
	if "" ­ "`exists -f dodocompleted.txt`"
		Rename -y dodocompleted.txt dodocompleted.old
	end
	echo "Started  : `date`" > dodocompleted.tmp


makesdk Ä makedodo
	make -f makesdk.mak -d aaf="{aaf}" > makesdk.makeout
	makesdk.makeout

makedodo Ä base_mac macaafobjects.mk macdodo.make
	make -f macdodo.make -d aaf="{aaf}" > macdodo.makeout
	macdodo.makeout

macaafobjects.mk Ä "{dod}aafobjects.mk" "{dod}CopyrightMessage.txt" "{dod}sync_copyright.pl"
	make -f macaafobjects.make > macaafobjects.make.out
	macaafobjects.make.out

base_mac Ä
	make -f base_mac.make > make.base_mac.out
	make.base_mac.out

macdodo.make Ä macaafobjects.mk makemacdodo.mak1 makemacdodo.mak2
	make -f makemacdodo.make -d aaf="{aaf}" > makemacdodo.makeout
	makemacdodo.makeout

	
cleansdk Ä
	make clean -f makesdk.mak > makesdk.makeout
	makesdk.makeout
	rm -f makesdk.makeout
	
realclean_sdk Ä
	make realclean -f makesdk.mak > makesdk.makeout
	makesdk.makeout
	rm -f makesdk.makeout
	

cleandodo Ä
	if "" ­ "`exists -f macdodo.make`"
		make clean -f macdodo.make -d aaf="{aaf}" > macdodo.makeout
		macdodo.makeout
	end
	rm -f macdodo.makeout
	

realclean_dodo Ä
	if "" ­ "`exists -f macdodo.make`"
		make realclean -f macdodo.make -d aaf="{aaf}" > macdodo.makeout
		macdodo.makeout
	end
	rm -f macdodo.makeout


cleanmacdodo Ä
	make clean -f makemacdodo.make -d aaf="{aaf}" > makemacdodo.makeout
	makemacdodo.makeout
	rm -f makemacdodo.makeout
	make clean -f base_mac.make > make.base_mac.out
	make.base_mac.out
	rm make.base_mac.out

realclean_macdodo Ä
	make realclean -f makemacdodo.make -d aaf="{aaf}" > makemacdodo.makeout
	makemacdodo.makeout
	rm -f makemacdodo.makeout
	make realclean -f base_mac.make > make.base_mac.out
	make.base_mac.out
	rm make.base_mac.out


cleantmp Ä
	rm -f dodocompleted.txt
	rm -f dodocompleted.old
	rm -f dodocompleted.tmp


clean Ä cleansdk cleandodo cleanmacdodo cleantmp

realclean Ä realclean_sdk realclean_dodo realclean_macdodo cleantmp


finish Ä
	echo "Completed: `date`" >> dodocompleted.tmp
	Rename -y dodocompleted.tmp dodocompleted.txt