#! C:/mksnt/sh.exe
#

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

Status=0


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
	print "\n\n"
	print "****************************************************************************"
	print "****************************************************************************\n\n"
}

RunExamples ()
{
	Target=$1

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

		PrintSeparator
		cd ..
	fi

	if [ TIMDUMP -eq 1 ] || [ ALL -eq 1 ]; then
		cd DevUtils
		cp ../Test/AAFSequenceTest.aaf .
		dump AAFSequenceTest.aaf

		PrintSeparator
		cd ..
	fi


	cd Examples/com
	if [ CLIENTTEST -eq 1 ] || [ ALL -eq 1 ]; then
		COMClientAAF
		PrintSeparator
	fi

	if [ CUTSTEST -eq 1 ] || [ ALL -eq 1 ]; then
		ComCutsTestAAF
		PrintSeparator
	fi

	if [ ESSENCETEST -eq 1 ] || [ ALL -eq 1 ]; then
		cp ../../../examples/com-api/ComEssenceDataTest/Laser.wav .
		ComEssenceDataTest
		PrintSeparator
	fi
	
	if [ PROPDIRECTDUMP -eq 1 ] || [ ALL -eq 1 ]; then
		ComPropDirectDump EssenceTest.aaf
		PrintSeparator
	fi

	if [ AAFINFO -eq 1 ] || [ ALL -eq 1 ]; then
		ComAAFInfo EssenceTest.aaf
		PrintSeparator
	fi
	cd ../..


	if [ AAFOMFTEST -eq 1 ] || [ ALL -eq 1 ]; then
		cd Utilities
		cp ../Test/AAFSequenceTest.aaf .
		AafOmf -omf AAFSequenceTest.aaf

		PrintSeparator

		cp D:/views/Complx2x.omf .
		AafOmf Complx2x.omf
		cd ..

		PrintSeparator
	fi

	cd $START_DIR
}



if [ CHECK_DEBUG -eq 1 ]; then
	SetPath "Debug"

	RunExamples "Debug"

	ResetPath
fi


if [ CHECK_RELEASE -eq 1 ]; then
	SetPath "Release"

	RunExamples "Release"

	ResetPath
fi
