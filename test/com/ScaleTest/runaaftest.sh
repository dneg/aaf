#!/bin/sh
###############################################################################
#
# $Id$ $Name$
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
# The Original Code of this file is Copyright 1998-2004, Licensor of the
# AAF Association.
#
###############################################################################
#
#	Shell script for testing scalability.
#
#	Copyright (C) 2004 Anuradha Suraparaju <asuraparaju@users.sourceforge.net>
#
###############################################################################

runtest()
{
	exename=$1	
	numEss=$2
	if [ $3 ]
	then
		arg=$3
	else
		arg="none"
	fi

	if [ $4 ]
	then
		arg2=$4
	else
		arg2="none"
	fi

	ext="_nl"

	case $exename in
	eli2aaf)
		if [ $arg == "-netloc" ]
		then
			cmd="$exename $arg $i4kflag $AAF_DATA_DIR/$numEss.eli $numEss$ext.aaf"
		else
			cmd="$exename $i4kflag $AAF_DATA_DIR/$numEss.eli $numEss.aaf"
		fi
		;;
	InfoDumper)
		if [ $arg == "none" ]
		then
			cmd="$exename $AAF_TEST_DIR/$numEss.aaf"
		else
			filen=$numEss.aaf
			if [ $arg == "-netloc" ]
			then
				filen=$numEss$ext.aaf
			fi
			if [ $arg2 == "-netloc" ]
			then
				filen=$numEss$ext.aaf
			fi
			if [ $arg == "-eagerLoad" ]
			then
				cmd="$exename $arg $AAF_TEST_DIR/$filen"
			else
				cmd="$exename $AAF_TEST_DIR/$filen"
			fi
		fi
			

		;;
	*)
		echo "unknown command $exename"
		exit
	esac

	out=`(time -p $cmd) 3>&2 2>&1 1>&3 1>/dev/null`
	hr=$?
	if [ $hr -eq 0 ]
	then
		t=`echo $out | grep real | sed 's/.*real //' | awk '{ printf ("%s", $1)}'`
	fi
}


runLoop()
{
	exename=$1
	arg=$2
	arg2=$3

	minimumLogN="0.0"
	countDecades="5"
	stepsPerDecade="5"

	m=0
	
	while [ $m -le $countDecades ]
	do
		k=0;
		while [ $k -lt $stepsPerDecade ]
		do
			logN=`echo "$k $stepsPerDecade" | awk '{ printf ("%f", $1/$2) }'`
			N=`echo $m $logN | awk '{ printf ("%d", int(10**$1 * int((10*(10**$2) + 0.5))/10 +0.5))}'`
			if [ $N -gt $maxcuts ]
			then
				break
			fi
			t=0
			hr=0;
			runtest $exename $N $arg $arg2
			k=`expr $k + 1`
			errstr="OK"
			if [ $hr -ne 0 ]
			then
				errstr="***ERROR***"
			fi
			echo "$N $m $logN $t $hr $errstr" | awk '{ printf ( "%7u,  %4.2f,  %f,  %8.2e, 0x%08X, %s\n", $1, $2+$3, $4, log($4)/log(10), $5, $6)}'
			if [ $m -eq $countDecades ]
			then
				break
			fi
		done
		m=`expr $m + 1`
		if [ $N -gt $maxcuts ]
		then
			break
		fi
	done
}

# MAIN start
# where the executables and shared modules reside
# copy eli2aaf, InfoDumper, libcom-api.so, libaafpgapi.so to this directory.
BIG_FILE="stadium_house"
AAF_TEST_DIR=`pwd`
export PATH=$AAF_TEST_DIR:$PATH
export LD_LIBRARY_PATH=$AAF_TEST_DIR:$LD_LIBRARY_PATH

#Uncomment appropriate line depending on OS
BASEDIR=$HOME
#BASEDIR=c:

AAF_DATA_DIR=$BASEDIR/aaftestdata  #where the data resides

if [ -z "$*" ] ; then
	echo "Usage: $0 operation_list maxcuts"
	echo "    operation_list  is a space separated list of read|write|bigfile"
	echo "    4k              set for 4k sectors"
	echo "    maxcuts         is the maximum number of essence elements"
	exit 1
fi

# parse arguments
maxcuts=1000000
i4kflag=""
for arg in $*
do
	if [ $arg == "read" ] ; then
		readFile=1
	elif [ $arg == "write" ] ; then
		writeFile=1
	elif [ $arg == "bigfile" ] ; then
		bigFile=1
	elif [ $arg == "4k" ] ; then
		i4kflag="-largeSectors"
	else
		maxcuts=$arg
	fi
done



echo "      N, log10N,          T,     log10T,    HRESULT, ERROR"

if [ -n "$writeFile" ]
then
	echo "eli2aaf"
	runLoop "eli2aaf"
	echo ""
	echo "eli2aaf -netloc"
	runLoop "eli2aaf"  "-netloc"
	echo ""
fi
if [ -n "$readFile" ]
then
	echo "InfoDumper"
	runLoop "InfoDumper" 
	echo ""
	echo "InfoDumper on files with netloc"
	runLoop "InfoDumper" "-netloc"
	echo ""
	echo "InfoDumper -eagerLoad"
	runLoop "InfoDumper" "-eagerLoad"
	echo ""
	echo "InfoDumper -eagerLoad on files with netloc"
	runLoop "InfoDumper" "-eagerLoad" "-netloc"
	echo ""
fi

if [ -n "$bigFile" ]
then
	hr=0
	echo "eli2aaf $BIG_FILE"
	runtest "eli2aaf" $BIG_FILE
	errstr="OK"
	if [ $hr -ne 0 ]
	then
		errstr="***ERROR***"
	fi
	echo "8 0 0 $t $hr $errstr" | awk '{ printf ( "%7u,  %4.2f,  %f,  %8.2e, 0x%08X, %s\n", $1, $2+$3, $4, log($4)/log(10), $5, $6)}'
	echo ""
	
	hr=0
	echo "eli2aaf $BIG_FILE -netloc"
	runtest "eli2aaf" $BIG_FILE "-netloc"
	errstr="OK"
	if [ $hr -ne 0 ]
	then
		errstr="***ERROR***"
	fi
	echo "8 0 0 $t $hr $errstr" | awk '{ printf ( "%7u,  %4.2f,  %f,  %8.2e, 0x%08X, %s\n", $1, $2+$3, $4, log($4)/log(10), $5, $6)}'
fi
