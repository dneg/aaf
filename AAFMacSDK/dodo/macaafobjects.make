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


all Ä macaafobjects.mk


MAKEFILE = macaafobjects.make


macaafobjects.mk Ä {MAKEFILE} "{dod}aafobjects.mk" "{dod}CopyrightMessage.txt" "{dod}sync_copyright.pl"
	perl "{dod}sync_copyright.pl" --copyright "{dod}CopyrightMessage.txt" --shell_style > macaafobjects.t00
	ConvertMake < "{dod}aafobjects.mk" >> macaafobjects.t00
	(
		echo ""
		echo ""
		echo '#' Define common variables:
		echo 'dod = {aaf}dodo:'
		echo 'macros = {aaf}dodo:macros:'
		echo 'exp = {aaf}dodo:'
		echo 'frefh = {aaf}dodo:'
		echo 'refh = {aaf}ref-impl:include:ref-api:'
		echo 'comh = {aaf}ref-impl:src:com-api:'
		echo 'comc = {aaf}ref-impl:src:com-api:'
		echo 'comt = {aaf}test:com:ComModTestAAF:ModuleTests:'
		echo 'impl = {aaf}ref-impl:src:impl:'
		echo 'AAFMacSDK = {aaf}AAFMacSDK:'
		echo 'DODO = dodo'
		echo ""
		echo ""
		echo ""
		echo ""
	) >> macaafobjects.t00
	copy_if_diff -p macaafobjects.t00 macaafobjects.mk
	rm -f macaafobjects.t00


cleantmp Ä
	delete -i -p -ay macaafobjects.t00
	
clean Ä cleantmp

realclean Ä clean
	delete -i -p -ay macaafobjects.mk
