#! C:/mksnt/sh.exe


ROOT_DIR="`PWD`"

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


Clean ()
{
	CleanTarget=$1

	print "Checking for ${CleanTarget}TestResults directory"
	if [ -d ${ROOT_DIR}/${CleanTarget}TestResults ]; then
		cd ${ROOT_DIR}/${CleanTarget}TestResults
		print "   Removing all contents of ${CleanTarget}TestResults directory"
		rm *
	else
		print "   ${CleanTarget}TestResults directory does not exist"
	fi
}


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


LOGFILE=${ROOT_DIR}/SystemTest.log
print "AAF System Test" >> $LOGFILE
date >> $LOGFILE



RegisterTargetDLLs ()
{	
	Target=$1
	Options=$2

	print "Registering $Target $Options AAFPGAPI.dll"
	if regsvr32 ${Options} ${ROOT_DIR}/AAFWinSDK/${Target}/RefImpl/AAFPGAPI.dll
	then
		print "    Succeeded"
	else
		print "    Failed"
		exit -1
	fi

	print "Registering $Target $Options AAFCOAPI.dll"
	if regsvr32 ${Options} ${ROOT_DIR}/AAFWinSDK/${Target}/RefImpl/AAFCOAPI.dll
	then
		print "    Succeeded"
	else
		print "    Failed"
		exit -1
	fi

	print "Registering $Target $Options AAFINTP.dll "
	if regsvr32 ${Options} ${ROOT_DIR}/AAFWinSDK/${Target}/RefImpl/AAFINTP.dll
	then
		print "    Succeeded"
	else
		print "    Failed"
		exit -1
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

	##${DUMP_DIR}/dump.exe -a -s ${FileDir}/$AAFFileName > ${AAFFileName}.dmp.log 
	##if [ $? -eq 0 ]  ## $? is an int in which is stored the exit status returned by last command
	
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


RunSystemTest ()
{
	TARGET=$1

	AAF_MODTEST_DIR=${ROOT_DIR}/AAFWinSDK/${TARGET}/Test
	AAF_EXAMPLE_DIR=${ROOT_DIR}/AAFWinSDK/${TARGET}/Examples/Com

	UTILS_DIR=${ROOT_DIR}/AAFWinSDK/${TARGET}/Utilities
	DUMP_DIR=${ROOT_DIR}/AAFWinSDK/${TARGET}/DevUtils
	OUTPUT_DIR="${ROOT_DIR}/${TARGET}TestResults"
	
	if [ ! -d $OUTPUT_DIR ]; then
		if mkdir $OUTPUT_DIR; then
			print "\n   Created $OUTPUT_DIR\n"
		else
			print "\n   Failed to create $OUTPUT_DIR\n"
			exit -1
		fi
	fi

	##Register DLLs and set PATH
	RegisterTargetDLLs $TARGET "/s"
	SetPath $TARGET


	#############################################
	# Get test file lists
	#############################################
	cd $AAF_MODTEST_DIR
	AAFModTestFilesList="`ls *.aaf`"

	cd $AAF_EXAMPLES_DIR
	AAFExamplesFilesList="`ls *.aaf`"

	cd $AAF_FILES_DIR
	TestAAFFilesList="`ls *.aaf`"

	cd $SCRIPT_FILES_DIR
	TestScriptFilesList="`ls *.scrpt`"

	cd $OMF_FILES_DIR
	TestOMFFilesList="`ls *.omf`"


	cd $OUTPUT_DIR

	print "\n--------------------" | tee -a  $LOGFILE
	print "Running Module Test AAF Files \n" | tee -a  $LOGFILE
	for AAFfile in $AAFModTestFilesList; do
		print "\n   $AAFfile\n" | tee -a $LOGFILE
	
		RunThruTimsDump $AAFfile ${AAF_MODTEST_DIR}
		RunThruValidator $AAFfile ${AAF_MODTEST_DIR}
		if RunThruDumper $AAFfile ${AAF_MODTEST_DIR}; then
			if RunThruGenerator ${AAFfile}.scrpt  ${OUTPUT_DIR}; then
				RunThruTimsDump ${AAFfile}.scrpt.aaf  ${OUTPUT_DIR}
				RunThruValidator ${AAFfile}.scrpt.aaf  ${OUTPUT_DIR}
				if RunThruDumper ${AAFfile}.scrpt.aaf  ${OUTPUT_DIR}; then
					if RunThruGenerator ${AAFfile}.scrpt.aaf.scrpt  ${OUTPUT_DIR}; then
						RunThruTimsDump ${AAFfile}.scrpt.aaf.scrpt.aaf  ${OUTPUT_DIR}
						RunThruValidator ${AAFfile}.scrpt.aaf.scrpt.aaf  ${OUTPUT_DIR}
						RunThruDumper ${AAFfile}.scrpt.aaf.scrpt.aaf  ${OUTPUT_DIR}
					fi
				fi
			fi
		fi

		DiffDumperLogFiles $AAFfile
	done


print "\n--------------------" | tee -a  $LOGFILE
	print "Running Examples AAF Files \n" | tee -a  $LOGFILE
	for AAFfile in $AAFExamplesFilesList; do
		print "\n   $AAFfile\n" | tee -a $LOGFILE
	
		RunThruTimsDump $AAFfile ${AAF_EXAMPLES_DIR}
		RunThruValidator $AAFfile ${AAF_EXAMPLES_DIR}
		if RunThruDumper $AAFfile ${AAF_EXAMPLES_DIR}; then
			if RunThruGenerator ${AAFfile}.scrpt  ${OUTPUT_DIR}; then
				RunThruTimsDump ${AAFfile}.scrpt.aaf  ${OUTPUT_DIR}
				RunThruValidator ${AAFfile}.scrpt.aaf  ${OUTPUT_DIR}
				if RunThruDumper ${AAFfile}.scrpt.aaf  ${OUTPUT_DIR}; then
					if RunThruGenerator ${AAFfile}.scrpt.aaf.scrpt  ${OUTPUT_DIR}; then
						RunThruTimsDump ${AAFfile}.scrpt.aaf.scrpt.aaf  ${OUTPUT_DIR}
						RunThruValidator ${AAFfile}.scrpt.aaf.scrpt.aaf  ${OUTPUT_DIR}
						RunThruDumper ${AAFfile}.scrpt.aaf.scrpt.aaf  ${OUTPUT_DIR}
					fi
				fi
			fi
		fi

		DiffDumperLogFiles $AAFfile
	done


print "\n--------------------" | tee -a  $LOGFILE
	print "Running TestAAFFiles \n" | tee -a  $LOGFILE
	for AAFfile in $TestAAFFilesList; do
		print "\n   $AAFfile\n" | tee -a $LOGFILE
	
		RunThruTimsDump $AAFfile ${AAF_FILES_DIR}
		RunThruValidator $AAFfile ${AAF_FILES_DIR}
		if RunThruDumper $AAFfile ${AAF_FILES_DIR}; then
			if RunThruGenerator ${AAFfile}.scrpt  ${OUTPUT_DIR}; then
				RunThruTimsDump ${AAFfile}.scrpt.aaf  ${OUTPUT_DIR}
				RunThruValidator ${AAFfile}.scrpt.aaf  ${OUTPUT_DIR}
				if RunThruDumper ${AAFfile}.scrpt.aaf  ${OUTPUT_DIR}; then
					if RunThruGenerator ${AAFfile}.scrpt.aaf.scrpt  ${OUTPUT_DIR}; then
						RunThruTimsDump ${AAFfile}.scrpt.aaf.scrpt.aaf  ${OUTPUT_DIR}
						RunThruValidator ${AAFfile}.scrpt.aaf.scrpt.aaf  ${OUTPUT_DIR}
						RunThruDumper ${AAFfile}.scrpt.aaf.scrpt.aaf  ${OUTPUT_DIR}
					fi
				fi
			fi
		fi

		DiffDumperLogFiles $AAFfile
	done


	print "\n----------------------" | tee -a  $LOGFILE
	print "Running TestScriptFiles \n" | tee -a  $LOGFILE
	for Scriptfile in $TestScriptFilesList; do
		print "\n   $Scriptfile\n" | tee -a $LOGFILE

		if RunThruGenerator $Scriptfile ${SCRIPT_FILES_DIR}; then
			RunThruTimsDump ${Scriptfile}.aaf ${OUTPUT_DIR}
			RunThruValidator ${Scriptfile}.aaf ${OUTPUT_DIR}
			if RunThruDumper "${Scriptfile}.aaf" ${OUTPUT_DIR}; then
				if RunThruGenerator $Scriptfile.aaf.scrpt ${OUTPUT_DIR}; then
					RunThruTimsDump ${Scriptfile}.aaf.scrpt.aaf ${OUTPUT_DIR}
					RunThruValidator ${Scriptfile}.aaf.scrpt.aaf ${OUTPUT_DIR}
					if RunThruDumper "${Scriptfile}.aaf.scrpt.aaf" ${OUTPUT_DIR}; then
						if RunThruGenerator ${Scriptfile}.aaf.scrpt.aaf.scrpt ${OUTPUT_DIR}; then
							RunThruTimsDump ${Scriptfile}.aaf.scrpt.aaf.scrpt.aaf ${OUTPUT_DIR}
							RunThruValidator ${Scriptfile}.aaf.scrpt.aaf.scrpt.aaf ${OUTPUT_DIR}
							RunThruDumper "${Scriptfile}.aaf.scrpt.aaf.scrpt.aaf" ${OUTPUT_DIR}
						fi
					fi
				fi
			fi
		fi
		
		DiffDumperLogFiles ${Scriptfile}.aaf
	done


	print "\n--------------------" | tee -a  $LOGFILE
	print "Running TestOMFFiles \n" | tee -a  $LOGFILE
	for OMFfile in $TestOMFFilesList; do
		print "\n   $OMFfile\n" | tee -a $LOGFILE

		if RunOMFThruAafOmf $OMFfile ${OMF_FILES_DIR}; then
			RunThruTimsDump ${OMFfile}.aaf ${OUTPUT_DIR}
			RunThruValidator ${OMFfile}.aaf ${OUTPUT_DIR}
			if RunThruDumper ${OMFfile}.aaf ${OUTPUT_DIR}; then
				if RunThruGenerator ${OMFfile}.aaf.scrpt ${OUTPUT_DIR}; then
					RunThruTimsDump ${OMFfile}.aaf.scrpt.aaf ${OUTPUT_DIR}
					RunThruValidator ${OMFfile}.aaf.scrpt.aaf ${OUTPUT_DIR}
					if RunThruDumper ${OMFfile}.aaf.scrpt.aaf ${OUTPUT_DIR}; then
						if RunThruGenerator ${OMFfile}.aaf.scrpt.aaf.scrpt ${OUTPUT_DIR}; then
							RunThruTimsDump ${OMFfile}.aaf.scrpt.aaf.scrpt.aaf ${OUTPUT_DIR}
							RunThruValidator ${OMFfile}.aaf.scrpt.aaf.scrpt.aaf ${OUTPUT_DIR}
							RunThruDumper ${OMFfile}.aaf.scrpt.aaf.scrpt.aaf ${OUTPUT_DIR}
						fi
					fi
				fi
			fi
		fi

		DiffDumperLogFiles ${OMFfile}.aaf
	done

	print "\n\n\n"

	## unregister and reset PATH
	RegisterTargetDLLs $TARGET "/s /u" 
	ResetPath
}



############################################
# Run the System Tests
############################################
if [ RELEASE -eq 1 ]; then
	if [ MODTESTS_AND_EXAMPLES -eq 1 ]; then
		RunModTestAndExamples -r
	fi

	RunSystemTest Release
fi

if [ DEBUG -eq 1 ]; then
	if [ MODTESTS_AND_EXAMPLES -eq 1 ]; then
		RunModTestAndExamples -d
	fi

	RunSystemTest Debug
fi


####################################################################
# Generate reports 
####################################################################
cd $ROOT_DIR

print "\nGenerating Efficiency and File Size spreadsheet\n"
EfficiencyStats $TARGET


cd $ROOT_DIR

exit $Status
