#################################################
# File: macaafobjects.make                      #
#                                               #
# Copyright (c) 1998-2001 Avid Technology, Inc. #
#                                               #
#################################################
dod = {aaf}dodo:


all Ä macaafobjects.mk


MAKEFILE = macaafobjects.make


macaafobjects.mk Ä {MAKEFILE} "{dod}aafobjects.mk"
	ConvertMake < "{dod}aafobjects.mk" > macaafobjects.t00
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
	delete -i -p -ay macaafobjects.mk

realclean Ä clean