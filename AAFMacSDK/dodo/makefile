############################################
# File: makefile                           #
#                                          #
# Copyright (c) 1998 Avid Technology, Inc. #
#                                          #
############################################

all Ä macdodo.make
	make -f macdodo.make -d aaf={aaf} > macdodo.makeout
	macdodo.makeout


macdodo.make Ä 
	make -f makemacdodo.make -d aaf={aaf} > makemacdodo.makeout
	makemacdodo.makeout


cleandodo Ä
	if "`exists -f macdodo.make`"
		make clean -f macdodo.make -d aaf={aaf} > macdodo.makeout
		macdodo.makeout
	end
	delete -i -ay macdodo.makeout

cleanmacdodo Ä
	make clean -f makemacdodo.make -d aaf={aaf} > makemacdodo.makeout
	makemacdodo.makeout
	delete -i -ay makemacdodo.makeout


clean Ä cleandodo cleanmacdodo
