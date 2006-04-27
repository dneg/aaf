from subprocess import *
from Tkinter import *
import parse, os, tktable, sys, analyzer

#Usage:
#On the command line enter: 
#python analyzerhtml.py coverage_file_to_create file.html file1.aaf file2.aaf ...
#where, 
#	coverage_file_to_create - is the coverage file created by analyzer.py
#	file.html - is the html file to generate
#	file1.aaf file2.aaf ... - are the .aaf files to process


filename=sys.argv[1]
htmlfilename=sys.argv[2]
execpath = sys.argv[3]
longfiles=sys.argv[4:-1]
reqspath=sys.argv[-1]

files=['File Set']+analyzer.analyze(filename ,execpath, longfiles, reqspath)
command=execpath + ' -alldetailedreqs ' + reqspath
pipe = Popen(command, shell=True, stdout=PIPE, stderr=None)
command=execpath + ' -coverage ' + reqspath
pipe2 = Popen(command, shell=True, stdout=PIPE, stderr=None)
ret=parse.parse_requirements(pipe, pipe2)
requirements=ret[0]		
parse.parse_file_coverage(filename, requirements)

for i in range(0, len(files)):
	if sys.platform == 'win32':
		files[i]=files[i].split('\\')[-1]
	else:
		files[i]=files[i].split('/')[-1]

data=parse.get_data(files, requirements)

parse.write_html(htmlfilename, data)
