############################################
# File: makemacdodo.make                   #
#                                          #
# Copyright (c) 1998 Avid Technology, Inc. #
#                                          #
############################################
dod = {aaf}dodo:
AAFMacSDK = {aaf}AAFMacSDK:
DODO = {AAFMacSDK}dodo:tool:dodo
convertmk = {AAFMacSDK}dodo:tool:ConvertMake


all Ä macdodo.make


MAKEFILE     = makemacdodo.make


macaafobjects.mk Ä "{dod}aafobjects.mk"
	echo 'Creating macaafobjects.mk...'
	"{convertmk}" < "{dod}aafobjects.mk" > macaafobjects.t00
	Rename -y macaafobjects.t00 macaafobjects.mk
	

makemacdodo.mk Ä macaafobjects.mk makemacdodo.mak1
	echo 'Creating makemacdodo.mk...'
	catenate macaafobjects.mk > makemacdodo.t01
	echo "" >> makemacdodo.t01
	echo "" >> makemacdodo.t01
	catenate makemacdodo.mak1 >> makemacdodo.t01
	Rename -y makemacdodo.t01 makemacdodo.mk
	

macdodo.mak1  Ä makemacdodo.mk
	make -f makemacdodo.mk -d aaf="{aaf}" > makemacdodo.output
	makemacdodo.output


macdodo.make  Ä macdodo.mak1 makemacdodo.mak2
	echo 'making macdodo.make...'
	catenate macdodo.mak1 > macdodo.t02
	echo "" >> macdodo.t02
	echo "" >> macdodo.t02
	echo "" >> macdodo.t02
	catenate makemacdodo.mak2 >> macdodo.t02
	Rename -y macdodo.t02 macdodo.make


clean Ä
	if "`exists -f makemacdodo.mk`"
		make clean -f makemacdodo.mk -d aaf="{aaf}" > makemacdodo.output
		Delete -i -ay makemacdodo.mk
		Delete -i -ay makemacdodo.output
	end
	if "`exists -f macaafobjects.mk`"
		Delete -i -ay macaafobjects.mk
	end
	if "`exists -f macdodo.mak1`"
		SetFile -a l macdodo.mak1
		Delete -i -ay macdodo.mak1
	end
	if "`exists -f macdodo.make`"
		SetFile -a l macdodo.make
		Delete -i -ay macdodo.make
	end
