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
AAFMacSDK = {aaf}AAFMacSDK:
DODO = {AAFMacSDK}dodo:tool:dodo
convertmk = {AAFMacSDK}dodo:tool:ConvertMake


all Ä macdodo.make


MAKEFILE     = makemacdodo.make


macaafobjects.mk Ä "{dod}aafobjects.mk" "{dod}CopyrightMessage.txt" "{dod}sync_copyright.pl"
	make -f macaafobjects.make > macaafobjects.make.out
	macaafobjects.make.out
	

makemacdodo.mk Ä macaafobjects.mk makemacdodo.mak1
	catenate macaafobjects.mk > makemacdodo.t01
	echo "" >> makemacdodo.t01
	echo "" >> makemacdodo.t01
	catenate makemacdodo.mak1 >> makemacdodo.t01
	copy_if_diff makemacdodo.t01 makemacdodo.mk
	rm -f makemacdodo.t01
	

macdodo.mak1  Ä makemacdodo.mk
	make -f makemacdodo.mk -d aaf="{aaf}" > makemacdodo.output
	makemacdodo.output


macdodo.make  Ä macdodo.mak1 makemacdodo.mak2
	catenate macdodo.mak1 > macdodo.t02
	echo "" >> macdodo.t02
	echo "" >> macdodo.t02
	echo "" >> macdodo.t02
	catenate makemacdodo.mak2 >> macdodo.t02
	copy_if_diff macdodo.t02 macdodo.make
	rm -f macdodo.t02


clean_makemacdodo Ä
	if "" ­ "`exists -f makemacdodo.mk`"
		make clean -f makemacdodo.mk -d aaf="{aaf}" > makemacdodo.output
		makemacdodo.output
	end

realclean_makemacdodo Ä
	if "" ­ "`exists -f makemacdodo.mk`"
		make realclean -f makemacdodo.mk -d aaf="{aaf}" > makemacdodo.output
		makemacdodo.output
	end


cleantmp Ä
	rm -f makemacdodo.mk
	rm -f macdodo.mak1


clean Ä clean_makemacdodo cleantmp

realclean Ä realclean_makemacdodo cleantmp
	rm -f macdodo.make
	rm -f macaafobjects.mk
