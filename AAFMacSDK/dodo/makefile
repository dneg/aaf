############################################
# File: makefile                           #
#                                          #
# Copyright (c) 1998 Avid Technology, Inc. #
#                                          #
############################################
dod = {aaf}dodo:
AAFMacSDK = {aaf}AAFMacSDK:
DODO = {AAFMacSDK}dodo:tool:dodo
convertmk = {AAFMacSDK}dodo:tool:ConvertMake


all Ä makedodo makesdk

makesdk Ä makedodo
	make -f makesdk.mak -d aaf="{aaf}" > makesdk.makeout
	makesdk.makeout

makedodo Ä macaafobjects.mk macdodo.make
	make -f macdodo.make -d aaf="{aaf}" > macdodo.makeout
	macdodo.makeout

macaafobjects.mk Ä "{dod}aafobjects.mk"
	echo 'Creating macaafobjects.mk...'
	"{convertmk}" < "{dod}aafobjects.mk" > macaafobjects.mk

macdodo.make Ä macaafobjects.mk makemacdodo.mak1 makemacdodo.mak2
	make -f makemacdodo.make -d aaf="{aaf}" > makemacdodo.makeout
	makemacdodo.makeout

cleansdk Ä
	make -f makesdk.mak clean > makesdk.makeout
	makesdk.makeout
	delete -i -ay makesdk.makeout
	

cleandodo Ä
	if "`exists -f macdodo.make`"
		make clean -f macdodo.make -d aaf="{aaf}" > macdodo.makeout
		macdodo.makeout
	end
	delete -i -ay macdodo.makeout

cleanmacdodo Ä
	make clean -f makemacdodo.make -d aaf="{aaf}" > makemacdodo.makeout
	makemacdodo.makeout
	delete -i -ay makemacdodo.makeout


clean Ä cleansdk cleandodo cleanmacdodo
	delete -i -ay macdodo.make
