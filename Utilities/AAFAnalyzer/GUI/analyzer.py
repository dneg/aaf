import sys, os
from subprocess import *

def analyze(savefile, executable, files, requirements):   
	remove=[]
	try:	
		cfile=open(savefile, 'w+')
		efile=open('analyzer_errors.py', 'w+')
		efile.write('import parse\n')
		efile.write('def get_errors():\n')
		efile.write('\terrors=[]\n')
		efile.flush()
		try:
			for filename in files:
				execpath=''
				if sys.platform == 'win32':
					execpath="\\"+'\\'.join(executable.split('\\')[1:-1])	
				else:
					execpath="/"+'/'.join(executable.split('/')[1:-1])
					
				try:
					if (os.path.exists(filename)==False):
						remove.append(filename)
						raise IOError, filename+' does not exist'
	
					command=' '.join([executable , "-filecoverage", "-python", filename , requirements])
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
			for name in remove:
				files.remove(name)
			filescov(cfile, files)
			efile.write('\treturn errors')
			cfile.close()
			efile.close()
			return files
			
	except IOError, e:
		print >>sys.stderr, "Error opening file: ", e
		
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

		
