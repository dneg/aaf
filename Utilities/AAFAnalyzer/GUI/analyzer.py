import sys, os
from subprocess import *

# Script to run the AAF Analyzer over a file set and consolidate the output in a file.  Also writes out TestResult error data
# to a similar python data structure in a python file.
def analyze(savefile, executable, files, requirements):   
	remove=[]
	try:	
		# open the file to write the coverage data
		cfile=open(savefile, 'w+')
		# open the python file to write the Error data structure(s)
		efile=open('analyzer_errors.py', 'w+')
		
		efile.write('import parse\n')
		efile.write('def get_errors():\n')
		efile.write('\terrors=[]\n')
		efile.flush()
		try:
			for filename in files:
				
				execpath="/"+'/'.join(executable.split('/')[1:-1])
				
				try:
					if (os.path.exists(filename)==False):
						remove.append(filename)
						raise IOError, filename+' does not exist'
	
					command=' '.join([executable , "-filecoverage", "-python", filename , requirements])
					
					retcode=0
					
					if sys.platform == 'win32':
    						retcode = call(command, shell=True, stdout=cfile, stderr=efile)
    					else:	
    						retcode = call(command, shell=True, env={"LD_LIBRARY_PATH": execpath}, stdout=cfile, stderr=efile)
    						
    					cfile.write('\n\n')
    					if retcode < 0:
        					print >>sys.stderr, "Analyzer was terminated by signal", -retcode
        					files.remove(filename)
        					raise IOError, filename+' produced an error in the Analyzer and will not be reported on.'
        				
				except IOError, e:
					print >>sys.stderr, e
				except OSError, e:
    					print >>sys.stderr, "Execution failed:", e
		finally:
			cfile.flush()
			
			# Remove files that produced an error in the AAF Analyzer
			for name in remove:
				files.remove(name)
			filescov(cfile, files)
			efile.write('\treturn errors')
			cfile.close()
			efile.close()
			
			# Return updated list of files that successfully ran through the AAF Analyzer
			return files
			
	except IOError, e:
		print >>sys.stderr, "Error opening file: ", e
		
# Helper method that calculates the File Set coverage data based on all the files and writes to the beginning of the file
def filescov(filename, files):
	passed=[]
	warn=[]
	fail=[]
	filename.seek(0)
	curfile=filename.readlines()
	for line in curfile:
		
		if 'Passing Requirements:' in line and 'None' not in line:
			passed=passed+(line.replace('Passing Requirements: ', '').replace('\n', '')).split('; ')
		if 'Warning Requirements:' in line and 'None' not in line:
			warn=warn+(line.replace('Warning Requirements: ', '').replace('\n', '')).split('; ')
		if 'Failing Requirements:' in line and 'None' not in line:
			fail=fail+(line.replace('Failing Requirements: ', '').replace('\n', '')).split('; ')
	
	filename.seek(0)
	passed=list(set(passed).difference(set(warn)).difference(set(fail)))
	passed.sort()
	warn=list(set(warn).difference(set(fail)))
	warn.sort()
	fail=list(set(fail))
	fail.sort()
	filename.write('File Set: ')
	
	if passed==[]:
		passed.append('None')
	if warn==[]:
		warn.append('None')
	if fail==[]:
		fail.append('None')	
	
	for f in files:
		filename.write(f+'\n          ')
	filename.write('\n')
	filename.write('Passing Requirements: ' + '; '.join(passed) +'\n')
	filename.write('Warning Requirements: ' + '; '.join(warn) +'\n')
	filename.write('Failing Requirements: ' + '; '.join(fail) +'\n\n')
	filename.writelines(curfile)

		
