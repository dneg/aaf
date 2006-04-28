import sys

# Method parses requirement data from Analyzer
def parse_requirements(pipe, pipe2):
	# pipe = All AAF Requirement details (output of ./AAFAnalyzer -alldetailedreqs AAFRequirements.xml)
	unparsedreqs=pipe.stdout.readlines()
	
	# pipe2 = All AAF Requirements covered by the AAFAnalyzer (output of ./AAFAnalyzer -coverage AAFRequirements.xml)
	testsuitecov=''.join(pipe2.stdout.readlines())

	reqnum=0
	filereqs=0
	reqs={}
	req=None
	for i in range (0,len(unparsedreqs)):

		if unparsedreqs[i].count('ID:')>0:
			reqid=unparsedreqs[i].strip("ID:").strip()
			req=Requirement(reqid)
			reqs[reqid]=req
			# If the requirement appears in the list of the AAFAnalyzer's covered requirements set as covered
			if reqid in testsuitecov:
				req.set_is_covered('Y')
		elif unparsedreqs[i].count('Name:')>0:
			req.set_name(unparsedreqs[i].strip("Name:").strip())
		elif unparsedreqs[i].count('Type:')>0:
			req.set_type(unparsedreqs[i].strip("Type:").strip())
			if req.get_type()=='file' or req.is_covered()=='Y':
				filereqs=filereqs+1
		elif unparsedreqs[i].count('Category:')>0:
			req.set_category(unparsedreqs[i].strip("Category:").strip())
		elif unparsedreqs[i].count('Description:')>0:
			req.set_details(unparsedreqs[i].strip("Description:").strip())
		elif unparsedreqs[i].count('Document:')>0:
			req.set_document(unparsedreqs[i].strip("Document:").strip())
		elif unparsedreqs[i].count('Version:')>0:
			req.set_version(unparsedreqs[i].strip("Version:").strip())
		elif unparsedreqs[i].count('Section:')>0:
			req.set_section(unparsedreqs[i].strip("Section:").strip())
	return (reqs, filereqs)
	
# Method parses requirement coverage data from file created by the analyzer.py script
def parse_file_coverage(filename, requirements):
	
	try:
		cfile=open(filename, 'r')
		line=cfile.readline()
		while line<>'':
			if 'File Set' in line or 'File Name:' in line:
				if 'File Set' in line:
					filen='File Set'
				else:
					if sys.platform == 'win32':
						filen=line.strip('File Name: ').strip().split('\\')[-1]
					else:
						filen=line.strip('File Name: ').strip().split('/')[-1]
			
			# Update the Pass/Fail/Warn status on each requirement for each file via a dictionary stored in the Requirement object	
			elif 'Passing Requirements:' in line and 'None' not in line:
				for req in line.strip().replace('Passing Requirements: ','').split('; ')
					requirements[req].set_file_coverage(filen,'Passed')
	
			elif 'Warning Requirements:' in line and 'None' not in line: 
				for req in line.replace('Warning Requirements:','').strip().split('; '):
					requirements[req].set_file_coverage(filen, 'Passed with Warning')
	
			elif 'Failing Requirements:' in line and 'None' not in line: 
				for req in line.replace('Failing Requirements:','').strip().split('; '):
					requirements[req].set_file_coverage(filen, 'Failed')

			line=cfile.readline()
	except IOError, e:
    		print >>sys.stderr, "Error reading file: " +filename+" ", e

	cfile.close()		

# dumps all the data (requirement, coverage) into a tabular form
def get_data(files, requirements):
	data=[]
	data.append(['All Requirements'])
	data.append([])
	data[0].append('Requirement Category')
	data[1].append([])
	data[1].append([])
	
	for k in range(0, len(files)):
		data[0].append(files[k])
		data[1].append([])
		
	reqs=requirements.values()
	reqs.sort()
	row=1
		
	# Summary row stats:
	categories={}
	fails=[0]*(len(files))
	warns=[0]*(len(files))	
	passes=[0]*(len(files))
				
	for i in range(0, len(reqs)):
		if reqs[i].is_covered()=='Y' or reqs[i].get_type()=='file':
			data.append([])
			row=row+1
			req = "%i,%i" % (row, 0)
    			cov = "%i,%i" % (row, 1)
    				
			data[row].append('+ ' + reqs[i].get_id()+': '+reqs[i].get_name())
			data[row].append(reqs[i].get_category())
				
			try:
				categories[reqs[i].get_category()]=categories[reqs[i].get_category()]+1
			except KeyError:
				categories[reqs[i].get_category()]=1
					
			for j in range(0, len(files)):
				
				try:
					cov=reqs[i].get_file_coverage(files[j])
					data[row].append(cov)
					if cov=='Failed':
						fails[j]=fails[j]+1
					elif cov=='Passed with Warning':
						warns[j]=warns[j]+1
					else:
						passes[j]=passes[j]+1
				except KeyError:
					data[row].append('-')
	
	# Write Summary row
	data[1][0]='SUMMARY'
	s=''
	keys=categories.keys()
	keys.sort()
	for cat in keys:
		s=s+str(categories[cat]) +' '+ cat + '\n'
	data[1][1]=s
	for j in range(0, len(files)):
		data[1][2+j]= str(passes[j])+' passed\n' + str(warns[j])+' warned\n' + str(fails[j])+' failed\n'
	
	return data

# Writes out the table data to an HTML file
def write_html(filename, array):
	
	try:
		cfile=open(filename, 'w+')
		cfile.write('<Table BORDER="0" CELLSPACING="3" CELLPADDING="1">')
		uncovered=[]
		htmlrow=0
		for row in range(0, len(array)):
		
			if array[row][2]=='-':
				uncovered.append(row)
				row=row+1
			else:
				if htmlrow%2==0:
					cfile.write('<TR bgcolor="lightgrey">\n')
				else:
					cfile.write('<TR bgcolor="#CCFFFF">\n')
							
				for col in range(0, len(array[row])):
				
					TD='<TD'
				
					if array[row][col]=='Failed':
						TD=TD+' bgcolor="#CC0000"'
					elif array[row][col]=='Passed with Warning':
						TD=TD+' bgcolor="#FFCC00"'
					if col>0:
						TD=TD+' NOWRAP=1 ALIGN="center"'
				
					TD=TD+'>'
					cfile.write(TD)
					if col==0:
						cfile.write(array[row][col].split('\n')[0].strip('+ '))
					elif row==1:
						lines=array[row][col].split('\n')
						for line in lines:
							cfile.write(line)
							cfile.write('<BR>')
					else:
						cfile.write(array[row][col])
				
					cfile.write('</TD>')
				cfile.write('</TR>\n')
			htmlrow=htmlrow+1
		
		cfile.write('<FONT COLOR="grey">')
		for row in uncovered:
		
			if htmlrow%2==0:
				cfile.write('<TR bgcolor="lightgrey" fgcolor="grey">\n')
			else:
				cfile.write('<TR bgcolor="#CCFFFF" fgcolor="grey">\n')
				
			for col in range(0, len(array[row])):
		
				TD='<TD'
				
				if col>0:
					TD=TD+' NOWRAP=1 ALIGN="center"'
				
				TD=TD+'>'
				cfile.write(TD)
				cfile.write('<FONT COLOR=GREY>')
				if col==0:
					cfile.write(array[row][col].split('\n')[0].strip('+ '))
				else:
					cfile.write(array[row][col])
				cfile.write('</FONT>')
				cfile.write('</TD>')
				
			cfile.write('</TR>\n')
			htmlrow=htmlrow+1
				
		cfile.write('</Table>')
		cfile.close()
	except IOError, e:
		print >>sys.stderr, "Error writing file: ", e

def parse_errors():
	try:
		import analyzer_errors
		return analyzer_errors.get_errors()
	except ImportError:
		print >>sys.stderr, "No error file found. Error browser will not function."
	
	return None
	
# Class to manage AAF Requirement data
class Requirement:
		
	cats=['general', 'import-export', 'compositional', 'metadata',
                          'mixdown', 'auxiliary-file', 'annotations', 'effect',
                          'optional properties', 'structured storage', 'protocol',
                          'adhoc']
	types=['file', 'app', 'definition']
                          
	def __init__(self, req):
		
		self.reqid=req
		self.covered='N'
		self.filecov={}
	
	def __cmp__(self, other):
		
		res=cmp(self.get_category(), other.get_category())
		if res==0:
			return cmp(self.reqid, other.reqid)
		else:
			return res
	
	def get_all(self):
	
		all=self.get_id()+': '+self.get_name()+ '\n\n'
		all=all+'Description: ' +self.get_details() + '\n\n' 
		all=all+'Type:         ' +self.get_type() + '\n'
		all=all+'Document:  ' +self.get_document() + '\n' 
		all=all+'Section:     ' +self.get_section() + '\n' 
		all=all+'Version:     ' +self.get_version() 
		
		return all
				
	def get_id(self):
		return self.reqid
	
	def get_name(self):
		return self.name
		
	def get_details(self):
		return self.details
	
	def get_category(self):
		return Requirement.cats[int(self.category)]
	
	def get_type(self):
		return Requirement.types[int(self.typ)]
	
	def get_document(self):
		return self.document
	
	def get_version(self):
		return self.version
		
	def get_section(self):
		return self.section
	
	def is_covered(self):
		return self.covered
	
	def get_file_coverage(self, index):
		return self.filecov[index]
	
	def set_id(self, req):
		self.reqid=req
	
	def set_name(self, n):
		self.name=n
		
	def set_details(self, det):
		self.details=det
	
	def set_type(self, t):
		self.typ=t
	
	def set_category(self, cat):
		self.category=cat
		
	def set_document(self, doc):
		self.document=doc
		
	def set_version(self, ver):
		self.version=ver
		
	def set_section(self, sec):
		self.section=sec
	
	def set_is_covered(self, cov):
		self.covered=cov
		
	def set_file_coverage(self,filen, cov):
		self.filecov[filen]=cov

# Class to manage specific TestResult (AAF Analyzer c++ class) data
class Error:
	
	def __init__(self, n):
		self.name=n
		self.details=[]
		self.fail=[]
		self.warn=[]
		self.suberrors=[]
	
	def get_all(self):
	
		all=		'Test Name:   	     '+self.get_name()
 		all=all+'\nDesc:   		     ' + self.get_description()
 		
 		for i in range(0, len(self.get_detail())):
 			all=all+'\nDetail: 	    	     ' + self.get_detail()[i]
 		all=all+'\nResult: 		     ' + self.get_result()
 		
 		if self.get_fail()==[]:
 			self.set_fail('None')
 		all=all+'\nFailing Requirements:   ' + self.get_fail()[0]
 		for i in range(1, len(self.get_fail())):
 			all=all+ '; ' + self.get_fail()[i]
 			
 		if self.get_warn()==[]:
 			self.set_warn('None')
 		all=all+'\nWarning Requirements: ' + self.get_warn()[0]
 		for i in range(1, len(self.get_warn())):
 			all=all+ '; ' + self.get_warn()[i]
 		
 		all=all+'\nReason:		     ' + self.get_reason()
 		
 		return all
	
	def get_fail(self):
		return self.fail
		
	def get_warn(self):
		return self.warn
	
	def get_name(self):
		return self.name
		
	def get_description(self):
		return self.description
	
	def get_result(self):
		return self.result
	
	def get_reason(self):
		return self.reason
	
	def get_detail(self):
		return self.details
	
	def get_suberrors(self):
		return self.suberrors
	
	def set_fail(self, f):
		self.fail.append(f)
	
	def set_warn(self, w):
		self.warn.append(w)
	
	def set_name(self, n):
		self.name=n
		
	def set_description(self, desc):
		self.description=desc
	
	def set_result(self, res):
		self.result=res
		
	def set_reason(self, reas):
		self.reason=reas
		
	def set_detail(self, det):
		self.details.append(det)
	
	def set_suberror(self, err):
		self.suberrors.append(err)
	
	def set_suberrors(self, errs):
		self.suberrors=errs
