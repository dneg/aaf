#################################################
# File: makefile                                #
#                                               #
# Copyright (c) 1998-2001 Avid Technology, Inc. #
#                                               #
#################################################
dod = {aaf}dodo:


all Ä start makedodo makesdk finish

start Ä $OutOfDate
	if "" ­ "`exists -f dodocompleted.txt`"
		Rename -y dodocompleted.txt dodocompleted.old
	end
	echo "Started  : `date`" > dodocompleted.tmp


makesdk Ä makedodo
	make -f makesdk.mak -d aaf="{aaf}" > makesdk.makeout
	makesdk.makeout

makedodo Ä macaafobjects.mk macdodo.make
	make -f macdodo.make -d aaf="{aaf}" > macdodo.makeout
	macdodo.makeout

macaafobjects.mk Ä "{dod}aafobjects.mk"
	make -f macaafobjects.make > macaafobjects.make.out
	macaafobjects.make.out

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

realclean_macdodo Ä
	make realclean -f makemacdodo.make -d aaf="{aaf}" > makemacdodo.makeout
	makemacdodo.makeout
	rm -f makemacdodo.makeout


cleantmp Ä
	rm -f dodocompleted.txt
	rm -f dodocompleted.old
	rm -f dodocompleted.tmp


clean Ä cleansdk cleandodo cleanmacdodo cleantmp

realclean Ä realclean_sdk realclean_dodo realclean_macdodo cleantmp
	rm -f macdodo.make


finish Ä
	echo "Completed: `date`" >> dodocompleted.tmp
	Rename -y dodocompleted.tmp dodocompleted.txt