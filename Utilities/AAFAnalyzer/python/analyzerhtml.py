#! /usr/bin/python

import subprocess
import parse
import os
import sys
import analyzer

# Usage:
# On the command line enter: 
# python analyzerhtml.py execpath coverage_file_to_create file.html file1.aaf file2.aaf ...
# where, 
#       execpath - analyzer executable
#	temp test result file - is the coverage file created by analyzer.py
#	output.html - is the html file to generate
#       requirements.xml - the requirements file path
#	file1.aaf file2.aaf - are the .aaf files to process


#tmpfilename   = sys.argv[1]
#htmlfilename = sys.argv[2]
#execpath     = sys.argv[3]
#reqspath     = sys.argv[4]
#aaffiles     = sys.argv[5:-1]

def analyzerhtml(tmpfilename, htmlfilename, execpath, reqspath, aaffiles ):

	if not len(aaffiles) > 0:
		print "empty AAF file list, no action taken"
		return

	(files,removedFiles) =  analyzer.analyze(tmpfilename ,execpath, aaffiles, reqspath)
	files = ['File Set'] + files

	# Run the analyzer to generate a detailed requirement listing
	reqsDetailCmdArgs = [execpath, "-report", "-detail", "-type", "all", "-reqs", reqspath]
	reqsDetailPipe = subprocess.Popen(reqsDetailCmdArgs, stdout=subprocess.PIPE, stderr=None)

	# Run the analyzer to generate a list of requirements that are covered
	# by test implementations.
	reqsCovCmdArgs = [execpath, "-report", "-testcoverage", "-reqs", reqspath]
	reqsCovPipe = subprocess.Popen( reqsCovCmdArgs, stdout=subprocess.PIPE, stderr=None )
	
	# Parse the detailed requirement listing and the coverage listing.
	requirements = parse.parse_requirements(reqsDetailPipe.stdout, reqsCovPipe.stdout)
	
	parse.parse_file_coverage( tmpfilename, requirements )

	for i in range(0, len(files)):
		if sys.platform == 'win32' or sys.platform == 'cygwin':
			files[i]=files[i].split('\\')[-1]
		else:
			files[i]=files[i].split('/')[-1]

	# Parse the test result data to generate the result table and output
	# the table in html
	testResults = parse.get_result_data(files, requirements)
			
	parse.write_html(htmlfilename, testResults, requirements, removedFiles)

	print "succesfully wrote html file:", htmlfilename

def main():
	analyzerhtml(sys.argv[1], sys.argv[2], sys.argv[3], sys.argv[4], sys.argv[5:-1])

# If this is being run standalong then execute main
if sys.argv[0] == "analyzerhtml.py":
	main()
