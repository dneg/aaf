#! C:/mksnt/sh.exe
# Terry Skotz

CHECK_DEBUG=0
CHECK_RELEASE=0
ESSENCETEST=0
MODULETEST=0
TIMDUMP=0
CLIENTTEST=0
CUTSTEST=0
PROPDIRECTDUMP=0
AAFINFO=0
AAFOMFTEST=0
ALL=0
PRINTPATH=0


PrintHelp ()
{
	echo "\nTo run module test and all examples for specified target, use either:"
	echo "-r  = Release"
	echo "-d  = Debug"
	echo "\nTo specify which tests to run, add any of the following:"
	echo "-a  = AafOmf"
	echo "-i  = ComAAFInfo"
	echo "-cl = ComClientTest"
	echo "-cu = ComCutsTest"
	echo "-e  = ComEssenceDataTest"
	echo "-m  = ComModAAF"
	echo "-p  = ComPropDirectDump"
	echo "-t  = dump\n\n"
	echo "-pp = Print PATH variable\n\n"
}


#####################
# Parse Command Line
#####################
if [ $# -eq 0 ]; then
	PrintHelp
	exit 1
elif [ $# -eq 1 ]; then
	if [ $1 = "-d" ] || [ $1 = "-r" ]; then
		ALL=1;
	fi
fi

until [ $# = 0 ]
do
	case $1 in
		-r ) CHECK_RELEASE=1 ;;
		-d ) CHECK_DEBUG=1 ;;
		-e ) ESSENCETEST=1;;
		-m ) MODULETEST=1;;
		-t ) TIMDUMP=1;;
		-cl ) CLIENTTEST=1;;
		-cu ) CUTSTEST=1;;
		-p ) PROPDIRECTDUMP=1;;
		-i ) AAFINFO=1;;
		-a ) AAFOMFTEST=1;;
		-pp ) PRINTPATH=1;;
		-h ) PrintHelp
			 exit 1 ;;
	esac
	shift
done


DEBUG="AAFWinSDK/Debug"
RELEASE="AAFWinSDK/Release"

OLD_PATH=""

STATUS=0

RESULTS=""

CheckExitCode ()
{
	ExitCode=$1
	PROGRAM=$2

	#Generate list of all the results
	RESULTS="${RESULTS}$ExitCode  $PROGRAM\n"

	if [ $ExitCode -ne 0 ]; then
		STATUS=1
	fi
}


SetPath ()
{
	Target=$1

	print "\nSetting PATH $Target \n"
	OLD_PATH=$PATH
	PATH="`PWD`/AAFWINSDK/${Target}/RefImpl;$PATH"
}


ResetPath ()
{
	print "\nResetting PATH\n"
	PATH=$OLD_PATH
}


PrintSeparator ()
{
	TEXT=$1
	print "\n\n"
	print "****************************************************************************"
	print "$TEXT"
	print "****************************************************************************\n\n"
}

RunMainScript ()
{
	Target=$1
	print "\n\nTarget:  $Target"
	SetPath "$Target"


	if [ PRINTPATH -eq 1 ]; then 
		print "PATH = $PATH" 
		print "\n\n"
	fi

	START_DIR="`PWD`"

	cd AAFWinSDK/$Target

	if [ MODULETEST -eq 1 ] || [ ALL -eq 1 ]; then
		cd Test
		cp ../../Test/Com/ComModTestAAF/Laser.wav .
		ComModAAF

		CheckExitCode $? "ComModAAF"

		cd ..
	fi

	if [ AAFOMFTEST -eq 1 ] || [ ALL -eq 1 ]; then
		PrintSeparator "AafOmf Convertor Test 1 -  AAF -> OMF"
		cd Utilities
		cp ../Test/AAFSequenceTest.aaf .
		AafOmf -omf AAFSequenceTest.aaf

		CheckExitCode $? "AafOmf Convertor Test 1 -  AAF -> OMF"

		PrintSeparator "AafOmf Convertor Test 2 -  OMF -> AAF"
		cp D:/views/Complx2x.omf .
		AafOmf Complx2x.omf

		CheckExitCode $? "AafOmf Convertor Test 2 -  OMF -> AAF"

		cd ..
	fi

	cd Examples/com
	if [ CLIENTTEST -eq 1 ] || [ ALL -eq 1 ]; then
		PrintSeparator "Running COMClientAAF"
		COMClientAAF

		CheckExitCode $? "COMClientAAF"
	fi

	if [ CUTSTEST -eq 1 ] || [ ALL -eq 1 ]; then
		PrintSeparator "Running ComCutsTestAAF"
		ComCutsTestAAF

		CheckExitCode $? "ComCutsTestAAF"
	fi

	if [ ESSENCETEST -eq 1 ] || [ ALL -eq 1 ]; then
		cp ../../../examples/com-api/ComEssenceDataTest/Laser.wav .
		PrintSeparator "Running ComEssenceDataTest"
		ComEssenceDataTest

		CheckExitCode $? "ComEssenceDataTest"
	fi

	if [ AAFINFO -eq 1 ] || [ ALL -eq 1 ]; then
		PrintSeparator "Running ComAAFInfo on EssenceTest.aaf"
		ComAAFInfo EssenceTest.aaf

		CheckExitCode $? "ComAAFInfo"
	fi
	
	if [ PROPDIRECTDUMP -eq 1 ] || [ ALL -eq 1 ]; then
		PrintSeparator "Running ComPropDirectDump on EssenceTest.aaf"
		ComPropDirectDump EssenceTest.aaf

		CheckExitCode $? "ComPropDirectDump" 
	fi
	cd ../..

	if [ TIMDUMP -eq 1 ] || [ ALL -eq 1 ]; then
		PrintSeparator "Running dump on AAFSequenceTest.aaf"
		cd DevUtils
		cp ../Test/AAFSequenceTest.aaf .
		dump AAFSequenceTest.aaf

		CheckExitCode $? "dump"

		cd ..
	fi

	cd $START_DIR

	ResetPath
}



if [ CHECK_DEBUG -eq 1 ]; then
	RunMainScript "Debug"
fi


if [ CHECK_RELEASE -eq 1 ]; then
	RunMainScript "Release"
fi


print "\nPrinting Test Exit Codes:\n$RESULTS" 


exit $STATUS
