############################################
# File: makesdk.mak                        #
#                                          #
# Copyright (c) 2000 Avid Technology, Inc. #
#                                          #
############################################
AAFMacSDK = {aaf}AAFMacSDK:
incl = {AAFMacSDK}include:
refi = {aaf}ref-impl:include:
refh = {refi}ref-api:


targetIncludes = ¶
	"{incl}AAFClassDefUIDs.h" ¶
	"{incl}AAFCodecDefs.h" ¶
	"{incl}AAFContainerDefs.h" ¶
	"{incl}AAFDataDefs.h" ¶
	"{incl}AAFDefUIDs.h" ¶
	"{incl}AAFEssenceFormats.h" ¶
	"{incl}AAFExtEnum.h" ¶
	"{incl}AAFFileKinds.h" ¶
	"{incl}AAFFileMode.h" ¶
	"{incl}AAFFileSignatures.h" ¶
	"{incl}AAFInterpolatorDefs.h" ¶
	"{incl}AAFMetaDictionary.h" ¶
	"{incl}AAFOperationCategories.h" ¶
	"{incl}AAFOperationDefs.h" ¶
	"{incl}AAFParameterDefs.h" ¶
	"{incl}AAFPluginDefs.h" ¶
	"{incl}AAFPropertyDefs.h" ¶
	"{incl}AAFPropertyIDs.h" ¶
	"{incl}AAFResult.h" ¶
	"{incl}AAFSmartPointerBase.h" ¶
	"{incl}AAFStoredObjectIDs.h" ¶
	"{incl}AAFTypeDefUIDs.h" ¶
	"{incl}AAF.h" ¶
	"{incl}AAF_i.c" ¶
	"{incl}AAFCOMPlatform.h" ¶
	"{incl}AAFCOMPlatformTypes.h" ¶
	"{incl}AAFPlugin.h" ¶
	"{incl}AAFPlugin_i.c" ¶
	"{incl}AAFPluginTypes.h" ¶
	"{incl}AAFSmartPointer.h" ¶
	"{incl}AAFTypes.h"


all Ä  checkDirectories "{incl}AAFSmartPointer.h"

	

"{incl}" Ä "{refi}"
"{incl}" Ä "{refh}" 


checkDirectories Ä 
	if "" == "`exists -d {incl}`"
		NewFolder "{incl}"
	end
	Set Exit 0						# don't exit early
	backup -from {aaf}ref-impl:include: -to {aaf}AAFMacSDK:include: -check from -a > makesdk.tmp
	backup -from {aaf}ref-impl:include:ref-api -to {aaf}AAFMacSDK:include: -check from -a | StreamEdit -e '/Prvate/ Delete' >>  makesdk.tmp
	Set Exit 1
	if `count -c makesdk.tmp` ­ 0
		execute makesdk.tmp
		if "`Search -e 'Duplicate' makesdk.tmp`"		
			for item in `files -f -o -s {aaf}AAFMacSDK:include:`
				if "`Search "{item}" makesdk.tmp`"	
					SetFile -c CWIE -a l "{item}"
					OrphanFiles "{item}"
				end
			end
		end
	end
	# Create build directories so that CodeWarrior will not
	# produce any warnings about missing access paths when a 
	# project is opened.
	if "" == "`exists -d {aaf}AAFMacSDK:bin:aafext:`"
		NewFolder "{aaf}AAFMacSDK:bin:aafext:"
	end
	if "" == "`exists -d {aaf}AAFMacSDK:bin:debug:`"
		NewFolder "{aaf}AAFMacSDK:bin:debug:"
	end
	if "" == "`exists -d {aaf}AAFMacSDK:bin:debug:aafext:`"
		NewFolder "{aaf}AAFMacSDK:bin:debug:aafext:"
	end
	if "" == "`exists -d {aaf}AAFMacSDK:lib:`"
		NewFolder "{aaf}AAFMacSDK:lib:"
	end
	if "" == "`exists -d {aaf}AAFMacSDK:lib:debug`"
		NewFolder "{aaf}AAFMacSDK:lib:debug"
	end

"{incl}AAFSmartPointer.h" Ä "{refi}com-api:AAFSmartPointer.h"
	Duplicate -y "{refi}com-api:AAFSmartPointer.h" "{incl}AAFSmartPointer.h"
	SetFile -c CWIE -a l "{incl}AAFSmartPointer.h"
	OrphanFiles "{incl}AAFSmartPointer.h"
	
clean Ä 
	if "`exists -d {incl}`"
		delete -an -y -i "{incl}" 
	end


