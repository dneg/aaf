import sys, os
import subprocess

# Script to run the AAF Analyzer over a file set and consolidate the
# output in a file.  Also writes out TestResult error data to a
# similar python data structure in a python file.

def analyze(tmpResultFile, executable, files, requirements):		
	remove=[]
	try:	
		# open the file to write the coverage data
		coutfile = open(tmpResultFile, 'w+')

		try:
			for filename in files:
				
				execpath="/"+'/'.join(executable.split('/')[1:-1])
				
				try:
					if not os.path.exists(filename):
						remove.append(filename)
						raise IOError, filename + ' does not exist'
					
					args = [executable, "-reqs", requirements, "-filecoverage", filename]
					retcode=0
					print "Analyzing ", filename
					if sys.platform == 'win32' or sys.platform == 'cygwin':
						retcode = subprocess.call(args, env=os.environ, stdout=coutfile)
					else:
						retcode = subprocess.call(args, env={"LD_LIBRARY_PATH": execpath}, stdout=coutfile)

					coutfile.write('\n\n')
					
					# Return codes of 0, 1, and 2 are all considered okay because the indicate
					# the analyzer did not exit abnormally.
					if retcode not in [0,1,2]:
						print >>sys.stderr, "analyzer teminated abnormally with error:", retcode
						remove.append(filename)
						raise IOError, 'File "%s" caused the analyzer to exit abnormally and is not included in the report.' % filename;
								
				except IOError, e:
					print >>sys.stderr, "IOError: ", e
				except OSError, e:
					print >>sys.stderr, "OSError:", e

		finally:
			coutfile.flush()
			
			# Remove files that caused the analyzer to exit abnormally.
			for name in remove:
				files.remove(name)
			filescov(coutfile, files)
			if len(remove) > 0:
				print >>sys.stderr, "The following files caused the analyzer to exit abnormally:\n"
				for name in remove:
					print >>sys.stderr, "\t%s\n" % name;
			else:
				print "All files were processed to completion by the analyzer."
			coutfile.close()
			
			# Return updated list of files that successfully ran through the AAF Analyzer
			return files, remove
			
	except IOError, e:
		print >>sys.stderr, "IOError: ", e
		
# Helper method that calculates the File Set coverage data based on
# all the files and writes to the beginning of the file
#
# JPT REVIEW - This routine writes the summary back into the temporary
# result file under than name "File Set". i.e. The file set is report
# in the same way that indivual files are reported.  This is not very
# flexible. It woudl be better to return the file set data as a value
# (and perhaps do away completly with the temp file).
#

def filescov(openTmpResultFile, files):

	passed=[]
	warn=[]
	fail=[]

	# See to beginning. Copy the file into memory. Write the set
	# info, then rewrite the old contents. (Yes... somewhat
	# convoluted). This ensures the set is as the first result
	# column.

	openTmpResultFile.seek(0)
	curfile=openTmpResultFile.readlines()

	for line in curfile:
		if 'Passing Requirements:' in line and 'None' not in line:
			passed=passed+(line.replace('Passing Requirements: ', '').replace('\n', '')).split('; ')
		if 'Warning Requirements:' in line and 'None' not in line:
			warn=warn+(line.replace('Warning Requirements: ', '').replace('\n', '')).split('; ')
		if 'Failing Requirements:' in line and 'None' not in line:
			fail=fail+(line.replace('Failing Requirements: ', '').replace('\n', '')).split('; ')
	
	openTmpResultFile.seek(0)
	passed=list(set(passed).difference(set(warn)).difference(set(fail)))
	passed.sort()
	warn=list(set(warn).difference(set(fail)))
	warn.sort()
	fail=list(set(fail))
	fail.sort()
	openTmpResultFile.write('File Set:\n')
	
	if passed==[]:
		passed.append('None')
	if warn==[]:
		warn.append('None')
	if fail==[]:
		fail.append('None') 
	
	for f in files:
		openTmpResultFile.write('\t%s\n' % f)
	openTmpResultFile.write('Passing Requirements: ' + '; '.join(passed) +'\n')
	openTmpResultFile.write('Warning Requirements: ' + '; '.join(warn) +'\n')
	openTmpResultFile.write('Failing Requirements: ' + '; '.join(fail) +'\n\n')
	openTmpResultFile.writelines(curfile)

# END
