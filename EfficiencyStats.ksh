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

if [ $# = 0 ]
then
	print "please rerun EfficiencyStats specifying Debug or Release\n"
	exit -1
elif [ $1 = "Release" ]
then
	OUTPUT_DIR="ReleaseTestResults"
	SPREADSHEET="EfficiencyStatsRel.xls"
elif [ $1 = "Debug" ]
then
	OUTPUT_DIR="DebugTestResults"
	SPREADSHEET="EfficiencyStatsDeb.xls"
else
	print "Cant recognize $1 "
	exit -1
fi



print "Summary of tests" > $SPREADSHEET
PrintIntFromLine ()
{
	let "INT=$# - 2"
	shift $INT

	if [ $2 != "%" ]
	then
		print -n "\t $2" >> $SPREADSHEET
	else
		print -n "\t $1" >> $SPREADSHEET
	fi
}

PrintTimingIntFromLine ()
{
	print -n "\t $6" >> $SPREADSHEET
}


##########################
# Print Output file header
##########################
print "FileName \t File Size \t Total Prop Bytes \t AAF MetaData \t Objects \t Props \t Avg Num Props Per Obj \t Avg Num Bytes Per Obj \t Avg Num Bytes per Prop \t SS OH \t AAF OH \t Total OH \t Object Access Time (ms) \t Total Traversal Time for AAFDump (ms) \t File Open Time (ms) \t File Close Time (ms)" > $SPREADSHEET

#print "FileName \t File Size \t Bytes in all Streams \t Num Stgs \t Num Strms \t Num Elems \t Avg Bytes per Strm \t SS OH" > $SPREADSHEET

###################################################
# Get list of all .txt files produced from dump.exe
###################################################
cd $OUTPUT_DIR
FILELIST="`ls *.dmp.log`"
cd ..

#######################
# Initialize previous
#######################
previous="empty"

#####################################################
# Loop through all of the .txt files and extract info
#####################################################
for filename in $FILELIST
do
	## puts space between the filename sets
	if [ ${filename%%.*} != $previous ]; then
		print "" | tee -a $SPREADSHEET
	fi

	#####################
	# Open file for input
	#####################
	if [ -a $OUTPUT_DIR/$filename ]; then

		###########################
		# Print Name of File Dumped
		###########################
		print -n "${filename%%.dmp*}" >> $SPREADSHEET
		print "${filename%%.dmp*}"		
	
		########################################################
		# Gather Efficiency Stats from last 14 lines of log file
		########################################################
		Tail -14 $OUTPUT_DIR/$filename | while read LINE
		do
			for Word in $LINE
			do
				if	[ $Word = "Total" ] || \
					[ $Word = "Average" ] || \
					[ $Word = "overhead" ] || \
					[ $Word = "Overhead" ]
				then
					PrintIntFromLine $LINE
				fi
			done			
		done


		#####################################################
		# Open corresponding .scrpt.log file for timing stats
		#####################################################
		if [ -a $OUTPUT_DIR/${filename%%.dmp*}.scrpt.log ]; then
	
			######################
			# Gather Timing Stats
			######################
			Tail -7 $OUTPUT_DIR/${filename%%.dmp*}.scrpt.log | Head -4 | while read SLINE
			do
				PrintTimingIntFromLine $SLINE
			done
		fi

		#####################################################
		# Print new line in spreadsheet to end filename stats
		#####################################################
		print "" >> $SPREADSHEET

		###############################################################################
		# Verify and check for existence of all three aaf file generations - .scrpt.aaf
		# Do this by looking for the associated .dmp.log files
		###############################################################################

		until [ $filename = "${filename%%.aaf*}.aaf.scrpt.aaf.scrpt.aaf.dmp.log" ]
		do
			if [ ! -a $OUTPUT_DIR/${filename%%.dmp*}.scrpt.aaf.dmp.log ]; then
				print "${filename%%.dmp*}.scrpt.aaf ---- failed to be created!!!" | tee -a $SPREADSHEET
				filename="${filename%%.dmp*}.scrpt.aaf.dmp.log"
			else
				break
			fi
		done

	else
		print "\n$OUTPUT_DIR/$filename NOT FOUND" >> $SPREADSHEET
	fi

	previous=${filename%%.*}

done	# end of "for filename in $FILELIST"
