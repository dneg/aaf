#! C:/mksnt/sh.exe

###############################################################################
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
###############################################################################

ROOT_DIR="`PWD`"
LOGFILE=${ROOT_DIR}/SystemTest.log

AAF_FILES_DIR="${ROOT_DIR}/TestAAFFiles"
SCRIPT_FILES_DIR="${ROOT_DIR}/TestScriptFiles"
OMF_FILES_DIR="${ROOT_DIR}/TestOMFFiles"


OLD_PATH=""

CLEAN=0
RELEASE=0
DEBUG=0
MODTESTS_AND_EXAMPLES=0


Status=0
AAFDumpStatus=0
GenStatus=0
ValStatus=0
dumpStatus=0


#####################
# Parse Command Line
#####################
until [ $# = 0 ]
do
	case $1 in
		-clean ) CLEAN=1 ;;
		-r ) RELEASE=1 ;;
		-d ) DEBUG=1 ;;
		-e ) MODTESTS_AND_EXAMPLES=1 ;;
		-h ) echo "\n-clean = clean output dirs"
			 echo "-r = system tests the Release sdk"
			 echo "-d = system tests the Debug sdk"
			 echo "-e = will call RunModTestsAndExamples.ksh before running system tests"
			 exit 1 ;;
	esac
	shift
done

if [ RELEASE -ne 1 ] && [ DEBUG -ne 1 ] && [ CLEAN -ne 1 ]; then
	print "Use -h for help"
	exit 1
fi


DirectoryTest ()
{
	Directory=$1

	MAKE_DIR_IF_NOT_FOUND=1
	VERBOSE=1
	Result=0
	EMPTY=1
	NOT_FOUND=2
	
	until [ $# = 0 ]
	do
		case $1 in
			-m ) MAKE_DIR_IF_NOT_FOUND=0 ;;
			-v ) VERBOSE=0;;
		esac
		shift
	done
	
	Result=0
	EMPTY=1
	NOT_FOUND=2
	
	vprint ()
	{
		string=$1

		if [ $VERBOSE -eq 0 ]; then print "$string"; fi
	}

	if [ -d $Directory ]; then
		
		LSF="`ls $Directory`"

		if [ -z "$LSF" ]; then
			Result=$EMPTY
			vprint "\n\tDirectory is Empty:  $Directory\n"
		fi
	else
		vprint "\n\tDirectory Not Found:  $Directory\n"
		Result=$NOT_FOUND
		if [ $MAKE_DIR_IF_NOT_FOUND -eq 0 ]; then
			if mkdir $Directory; then
				print "Created $Directory\n"
				Result=0
			else
				print "\n\tAttempt to Create Directory Failed!  $Directory\n"
			fi
		fi		
	fi

	return $Result
}


Clean ()
{
	CleanTarget=$1

	EMPTY=1
	NOT_FOUND=2

	print -n "Checking for directory:   ${CleanTarget}TestResults \t"

	DirectoryTest ${ROOT_DIR}/${CleanTarget}TestResults
	TestResult=$?

	if [ $TestResult -eq 0 ]; then
		print "Deleted Contents"
		rm ${ROOT_DIR}/${CleanTarget}TestResults/*
	elif [ $TestResult -eq $EMPTY ]; then
		print "Directory is Empty"
	elif [ $TestResult -eq $NOT_FOUND ]; then
		print "Directory Not Found"
	fi
}



SetPath ()
{
	Target=$1

	print "\nSetting PATH $Target \n"
	OLD_PATH=$PATH
	PATH="`PWD`/AAFWinSDK/${Target}/RefImpl;$PATH"
}


ResetPath ()
{
	print "\nResetting PATH\n"
	PATH=$OLD_PATH
}

 

RunThruTimsDump ()
{
	AAFFileName=$1
	FileDir=$2

	STATUS=-1
	
	if ${DUMP_DIR}/dump.exe -a -s ${FileDir}/$AAFFileName > ${AAFFileName}.dmp.log 
	then
		print "      RunThruTimsDump...	ok" | tee -a  $LOGFILE
		STATUS=0
	else
		print "      RunThruTimsDump...	FAILED" | tee -a  $LOGFILE
	fi

	return $STATUS
}

RunThruDumper ()
{
	AAFFileName=$1
	FileDir=$2

	STATUS=-1

	if ${UTILS_DIR}/AAFDump.exe -vp -i ${FileDir}/${AAFFileName} -o ${AAFFileName}.scrpt > ${AAFFileName}.scrpt.log
	then
		print "      RunThruDumper...		ok" | tee -a  $LOGFILE
		STATUS=0
	else
		print "      RunThruDumper...		FAILED" | tee -a  $LOGFILE
	fi

	return $STATUS
}

RunThruValidator ()
{
	AAFFileName=$1
	FileDir=$2

	STATUS=-1
	cd $OUTPUT_DIR

	if ${UTILS_DIR}/AAFValidate.exe -v -e -E -l -M -p -r -s -S -i ${FileDir}/$AAFFileName > ${AAFFileName}.val.log
	then
		print "      RunThruValidator...	ok" | tee -a  $LOGFILE
		STATUS=0
	else
		print "      RunThruValidator...	FAILED" | tee -a  $LOGFILE
	fi

	return $STATUS
}

RunThruGenerator ()
{
	ScriptFileName=$1
	FileDir=$2

	STATUS=-1
	cd $OUTPUT_DIR

	if ${UTILS_DIR}/MakeAAFFile.exe -p -i ${FileDir}/$ScriptFileName -o ${ScriptFileName}.aaf > ${ScriptFileName}.aaf.fg.log
	then
		print "      RunThruGenerator...	ok" | tee -a  $LOGFILE
		STATUS=0
	else
		print "      RunThruGenerator...	FAILED" | tee -a  $LOGFILE
	fi

	return $STATUS
}

RunOMFThruAafOmf ()
{
	OMFFileName=$1
	FileDir=$2

	STATUS=-1
	cd $OUTPUT_DIR

	if ${UTILS_DIR}/AafOmf.exe  ${FileDir}/${OMFFileName} ${OMFFileName}.aaf > ${OMFFileName}.aaf.AafOmf.log
	then
		print "      RunOMFThruAafOmf...	ok" | tee -a  $LOGFILE
		STATUS=0
	else
		print "      RunOMFThruAafOmf...	FAILED" | tee -a  $LOGFILE
	fi

	return $STATUS
}

DiffDumperLogFiles ()
{
	DmpFileName=$1

	cd $OUTPUT_DIR
	
	DiffFiles ()
	{
		DmpFileA=$1
		DmpFileB=$2
		

		if [ -a $DmpFileA ] && [ -a $DmpFileB ]; then
			print "      diff'ing:  $DmpFileA" >> ${DmpFileName}.diff.log
			print "                 $DmpFileB" >> ${DmpFileName}.diff.log
			print "*************************************************************************************\n" >> ${DmpFileName}.diff.log
		
			if diff -C 1 $DmpFileA $DmpFileB  >> ${DmpFileName}.diff.log; then
				print "      DumpDiff...               ok" | tee -a  $LOGFILE
			else
				print "      DumpDiff...               FAILED" | tee -a  $LOGFILE
			fi
		
			print "\n\n" >> ${DmpFileName}.diff.log
		fi
	}

	DiffFiles ${DmpFileName}.dmp.log ${DmpFileName}.scrpt.aaf.dmp.log
	DiffFiles ${DmpFileName}.scrpt.aaf.dmp.log ${DmpFileName}.scrpt.aaf.scrpt.aaf.dmp.log

}


RunCreateSequence ()
{
	Target=$1
	RCSLOG="${OUTPUT_DIR}/CreateSequenceOpenClose.log"

	date > $RCSLOG

	let Max=8
	let Multiplier=2

	print "\n---------------------------------------------------------------------" | tee -a  $LOGFILE
	print "Creating AAF Files with CreateSequence.ksh\n" | tee -a  $LOGFILE

	let Count=1
	while [ Count -le $Max ];
	do
		print "CreateSequence${Count}.aaf" | tee -a $RCSLOG
		${ROOT_DIR}/AAFWinSDK/${Target}/Test/CreateSequence $Count ${OUTPUT_DIR}/CreateSequence${Count}.aaf >> $RCSLOG

		let Count=Count*$Multiplier
	done
}


RunThroughTestSuite ()
{
	TITLE=$1
	FILE_TYPE=$2
	FILE_LIST_LOCATION=$3
	SECONDARY_LOCATION=$4

	if [ $# -eq 3 ]; then
		SECONDARY_LOCATION=$FILE_LIST_LOCATION
	fi

	print "\n\n\n---------------------------------------------------------------------" | tee -a  $LOGFILE
	print "Running $TITLE through System Test Suite \n" | tee -a  $LOGFILE
		
	DirectoryTest $FILE_LIST_LOCATION -v
	if [ $? -eq 0 ]; then 
		cd $FILE_LIST_LOCATION
		FILE_LIST="`ls ${FILE_TYPE}`"
		DirectoryTest $SECONDARY_LOCATION -v -m
	else
		FILE_LIST=""
	fi


	for File in $FILE_LIST; do
		print "\n   $File\n" | tee -a $LOGFILE

		## If script file, first run thru generator
		if echo "$File" | grep -q "\.scrpt$" ; then
			if RunThruGenerator $File ${FILE_LIST_LOCATION}; then
				File="${File}.aaf"
				FILE_LIST_LOCATION=$SECONDARY_LOCATION
			else
				return -1
			fi					
		## If omf file, first run thru converter
		elif echo "$File" | grep -q "\.omf$" ; then
			if RunOMFThruAafOmf $File ${FILE_LIST_LOCATION}; then
				File="${File}.aaf"
				FILE_LIST_LOCATION=$SECONDARY_LOCATION
			else
				return -1
			fi
		fi

		cd ${OUTPUT_DIR}

		## This test suite assumes $File is an AAF file		
		RunThruTimsDump $File ${FILE_LIST_LOCATION}
		RunThruValidator $File ${SECONDARY_LOCATION}
		if RunThruDumper $File ${SECONDARY_LOCATION}; then
			if RunThruGenerator ${File}.scrpt  ${OUTPUT_DIR}; then
				RunThruTimsDump ${File}.scrpt.aaf  ${OUTPUT_DIR}
				RunThruValidator ${File}.scrpt.aaf  ${OUTPUT_DIR}
				if RunThruDumper ${File}.scrpt.aaf  ${OUTPUT_DIR}; then
					if RunThruGenerator ${File}.scrpt.aaf.scrpt  ${OUTPUT_DIR}; then
						RunThruTimsDump ${File}.scrpt.aaf.scrpt.aaf  ${OUTPUT_DIR}
						RunThruValidator ${File}.scrpt.aaf.scrpt.aaf  ${OUTPUT_DIR}
						RunThruDumper ${File}.scrpt.aaf.scrpt.aaf  ${OUTPUT_DIR}
					fi
				fi
			fi
		fi

		DiffDumperLogFiles $AAFfile
	done
}


RunSystemTest ()
{
	TARGET=$1

	AAF_MODTEST_DIR=${ROOT_DIR}/AAFWinSDK/${TARGET}/Test
	AAF_EXAMPLE_DIR=${ROOT_DIR}/AAFWinSDK/${TARGET}/Examples/Com

	UTILS_DIR=${ROOT_DIR}/AAFWinSDK/${TARGET}/Utilities
	DUMP_DIR=${ROOT_DIR}/AAFWinSDK/${TARGET}/DevUtils
	OUTPUT_DIR="${ROOT_DIR}/${TARGET}TestResults"
	
	SetPath $TARGET


	#############################################
	# Get test file lists
	#############################################

	DirectoryTest ${OUTPUT_DIR} -m

	RunCreateSequence $TARGET
	
	RunThroughTestSuite "CreateSequence AAF Files" "CreateSequence*.aaf" $OUTPUT_DIR
	
	RunThroughTestSuite "Module Test AAF Files" "*.aaf" $AAF_MODTEST_DIR
	
	RunThroughTestSuite "Examples AAF Files" "*.aaf" $AAF_EXAMPLE_DIR

	RunThroughTestSuite "TestAAFFiles" "*.aaf" $AAF_FILES_DIR

	RunThroughTestSuite "TestScriptFiles" "*.scrpt" $SCRIPT_FILES_DIR $OUTPUT_DIR

	RunThroughTestSuite "TestOMFFiles" "*.omf" $OMF_FILES_DIR $OUTPUT_DIR

	print "\n\n\n"

	ResetPath

	cd $ROOT_DIR
	print "\nGenerating Efficiency and File Size spreadsheet\n"
	EfficiencyStats $TARGET

}


############################################
# Check for CLEAN
############################################
if [ CLEAN -eq 1 ]; then
	if [ RELEASE -eq 1 ]; then
		Clean Release
	fi

	if [ DEBUG -eq 1 ]; then
		Clean Debug
	fi

	if [ RELEASE -eq 0 ] && [ DEBUG -eq 0 ]; then
		Clean Release
		Clean Debug
	fi
	
	exit 1
fi


############################################
# Run the System Tests
############################################
print "AAF System Test" >> $LOGFILE
date >> $LOGFILE

if [ RELEASE -eq 1 ]; then
	if [ MODTESTS_AND_EXAMPLES -eq 1 ]; then
		print "\n\nRunning Script:  RunModTestAndExamples.ksh -r\n\n"
		RunModTestAndExamples -r
	fi

	RunSystemTest Release
fi

if [ DEBUG -eq 1 ]; then
	if [ MODTESTS_AND_EXAMPLES -eq 1 ]; then
		print "\n\nRunning Script:  RunModTestAndExamples.ksh -d\n\n"
		RunModTestAndExamples -d
	fi

	RunSystemTest Debug
fi


cd $ROOT_DIR

exit $Status
