#################################################
# File: makesdk.mak                             #
#                                               #
# Copyright (c) 2000-2001 Avid Technology, Inc. #
#                                               #
#################################################
AAFMacSDK = "{aaf}AAFMacSDK:"
incl = "{AAFMacSDK}include:"
refi = "{aaf}ref-impl:include:"
refh = "{refi}ref-api:"
comh = "{refi}com-api:"
bin  = "{AAFMacSDK}bin:"
bind = "{AAFMacSDK}bin:debug:"
lib  = "{AAFMacSDK}lib:"
libd = "{AAFMacSDK}lib:debug:"


all Ä  makesdk




make_makesdk Ä $OutOfDate makesdk.mak
	(
		echo '#################################################'
		echo '# File: makesdk.make                            #'
		echo '#                                               #'
		echo '# This file was GENERATED for the AAF SDK       #'
		echo '#                                               #'
		echo '# (C) Copyright 2001 Avid Technology.           #'
		echo '#                                               #'
		echo '#################################################'
		echo ""
		echo ""
		echo 'AAFMacSDK = {aaf}AAFMacSDK:'
		echo 'incl = {AAFMacSDK}include:'
		echo 'refi = {aaf}ref-impl:include:'
		echo 'refh = {refi}ref-api:'
		echo 'comh = {refi}com-api:'
		echo 'bin  = {AAFMacSDK}bin:'
		echo 'bind = {AAFMacSDK}bin:debug:'
		echo 'lib  = {AAFMacSDK}lib:'
		echo 'libd = {AAFMacSDK}lib:debug:'
		echo ""
		echo ""
		echo 'targetIncludes = ¶'
		set refimpl "`files -s "{refi}" | StreamEdit -e '/Private/ Delete'`"
		for item in {refimpl}
			echo '	"{incl}'{item}'" ¶'
		end
		set refapi "`files -s "{refh}" | StreamEdit -e '/Private/ Delete'`"
		for item in {refapi}
			echo '	"{incl}'{item}'" ¶'
		end
		echo '	"{incl}AAFSmartPointer.h"'
		echo ""
		echo ""
		echo 'all Ä checkDirectories {targetIncludes}'
		echo ""
		echo ""
		echo "# Directory dependencies"
		echo '"{incl}" Ä "{refi}"'
		echo '"{incl}" Ä "{refh}"'
		echo '"{incl}" Ä "{comh}"'
		echo ""
		echo ""
		echo 'checkDirectories Ä'
		echo '	if "" == "`exists -d "{bin}"`"'
		echo '		NewFolder "{bin}"'
		echo '	end'
		echo '	if "" == "`exists -d "{bind}"`"'
		echo '		NewFolder "{bind}"'
		echo '	end'
		echo '	if "" == "`exists -d "{incl}"`"'
		echo '		NewFolder "{incl}"'
		echo '	end'
		echo '	if "" == "`exists -d "{lib}"`"'
		echo '		NewFolder "{lib}"'
		echo '	end'
		echo '	if "" == "`exists -d "{libd}"`"'
		echo '		NewFolder "{libd}"'
		echo '	end'
		echo ""
		echo ""
		echo ""
		echo ""
		for item in {refimpl}
			echo '"{incl}'{item}'" Ä "{refi}'{item}'"'
			echo '	copy_if_diff -p "{refi}'{item}'" ''"{incl}'{item}'"'
			echo ""
			echo ""
		end
		for item in {refapi}
			echo '"{incl}'{item}'" Ä "{refh}'{item}'"'
			echo '	copy_if_diff -p "{refh}'{item}'" ''"{incl}'{item}'"'
			echo ""
			echo ""
		end
		echo '"{incl}AAFSmartPointer.h" Ä "{comh}AAFSmartPointer.h"'
		echo '	copy_if_diff -p "{comh}AAFSmartPointer.h" "{incl}AAFSmartPointer.h"'
		echo ""
		echo ""
		echo "clean Ä"
		echo '	echo "No files removed" > dev:null'
		echo ""
		echo "realclean Ä"
		echo '	for item in {targetIncludes}'
		echo '		rm -v -f "{item}"'
		echo '	end'
		echo ""
		echo ""
	) > makesdk.tmp
	copy_if_diff makesdk.tmp makesdk.make
	rm -f makesdk.tmp


makesdk.make Ä make_makesdk


makesdk Ä makesdk.make
	make -f makesdk.make -d aaf="{aaf}" > makesdk.make.all.out
	makesdk.make.all.out
	

cleantmp Ä	
	rm -f makesdk.tmp
	rm -f makesdk.make.all.out
	rm -f makesdk.make.clean.out
	rm -f makesdk.make.realclean.out


clean_makesdk Ä
	if "" ­ "`exists makesdk.make`"
		make clean -f makesdk.make > makesdk.make.clean.out
		makesdk.make.clean.out
	end


realclean_makesdk Ä
	if "" ­ "`exists makesdk.make`"
		make realclean -f makesdk.make > makesdk.make.realclean.out
		makesdk.make.realclean.out
		rm -f -v makesdk.make
	end
	
	
clean Ä clean_makesdk cleantmp

	
realclean Ä realclean_makesdk cleantmp
