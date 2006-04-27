from Tkinter import *
from subprocess import *
import tktable, os, tkFileDialog, analyzer, Tree, tkMessageBox, parse, Pmw


#Application class, owns GUI components
class App:

	def __init__(self, master):
		master.title('AAF Analyzer')
		mainframe = Frame(master)
        	mainframe.pack(fill=BOTH, expand=1)
		
		# Set up frame to hold the add files list box and buttons (top) and
		# frame to hold coverage table and error browser
		self.topframe = Frame(mainframe)
        	self.topframe.pack(side=TOP, fill=X, expand=0)
        	self.bottomframe = Pmw.PanedWidget(mainframe, orient='horizontal')
        	self.bottomleft = self.bottomframe.add('bl', size=.75)
        	self.bottomright = self.bottomframe.add('br')
        	self.browserpane= Pmw.PanedWidget(	self.bottomright, orient='vertical')
        	self.browserpane.pack(side=TOP, fill=BOTH, expand=1)
        	
        	self.toppane = self.browserpane.add('top', size=.40)
 		self.bottompane = self.browserpane.add('bottom')

        	
        	self.bottomleft.columnconfigure(0, weight=1)
        	self.bottomleft.rowconfigure(0, weight=1)
        	        	
        	
        	self.topframe.columnconfigure(1, weight=1)
        	self.topframe.rowconfigure(0, weight=1)
        	self.topframe.rowconfigure(1, weight=1)	
        	self.topframe.rowconfigure(2, weight=1)
        	self.topframe.rowconfigure(3, weight=1)

        	# Set up button widgets
		self.addfiles = Button(self.topframe, text="Add Files", command=self.add_files)
		self.addfiles.grid(row=0, column=0, sticky='news')	
		self.removefiles = Button(self.topframe, text="Remove Files", command=self.remove_files)
		self.removefiles.grid(row=1, column=0, sticky='news')	
        	
        	self.execute = Button(self.topframe, text="Run Analyzer", command=self.new_file)
        	self.execute.grid(row=2, column=0, sticky='nsew')
        	self.openfile = Button(self.topframe, text="Open Existing File", command=self.open_file)
        	self.openfile.grid(row=3, column=0, sticky='nsew')
        	self.writehtml = Button(self.topframe, text="Write Out to HTML", command=self.call_html)
        	self.writehtml.grid(row=4, column=0, sticky='nsew')
        	
		self.lb = Listbox(self.topframe, background='white', selectmode=EXTENDED, width=40, height=5)
		self.lb.grid(row=0, column=1, rowspan=5, sticky='news')	
		
		sb=Scrollbar(self.topframe)
		sb.grid(row=0, column=2, sticky='ns', rowspan=5)
		self.lb.configure(yscrollcommand=sb.set)
		sb.configure(command=self.lb.yview)
		
		# class variables
		self.requirements={}
		self.files=[]
		self.numfiles=0
		self.errors=[]
		self.data=[]
		self.var=None
		self.results=None
		self.error_details=None
		self.current_node=None
		
		#TAKE OUT AFTER TESTING:
		self.lb.insert(END, '/home/jlow/workspace/AAF/Utilities/AAFAnalyzer/RegressionTest/TestFiles/EPAcyclicTest.aaf')
		self.lb.insert(END, '/home/jlow/workspace/AAF/Utilities/AAFAnalyzer/RegressionTest/TestFiles/EPEditRateTest5.aaf')
		self.lb.insert(END, '/blah')
		self.lb.insert(END, '/home/jlow/workspace/AAF/Utilities/AAFAnalyzer/RegressionTest/TestFiles/EPNameTest.aaf')
		self.lb.insert(END, '/home/jlow/workspace/AAF/Utilities/AAFAnalyzer/RegressionTest/TestFiles/EPTrackContentsTest.aaf')
		
		
		
 	# tree widget event method (called on tree node expand/collapse)
 	def get_contents(self, node):
		
 		if node.id=='root':
 			for k in range(1, self.numfiles):
				node.widget.add_node(name=self.files[k], id='error', flag=1)
		elif node.id=='error':
			node.set_error(node.parent_node.error.get_suberrors()[node.parent_node.child_nodes.index(node)])
			for k in range(0, len(node.error.get_suberrors())):
				nodename=node.error.get_suberrors()[k].get_name().replace('Test Name: ','')
 				node.widget.add_node(name=nodename, id='error', flag=1)#, expanded_icon=node.widget.regular_icon)
 			
 	# tree widget event method (called on tree click)
 	def callback(self, event):
 		if event:
 			node=event.widget.cursor_node(None)
 			if node.id=='error' and node.expanded(): 				
 				details=node.error.get_all()+'\n\n\nAffected Requirements'		
 				if node.error.get_fail()[0]<>'None': 	
					details=details+'\n\nFailed:\n\n'  
					for i in range(0, len( node.error.get_fail())):
						details = details + self.requirements[node.error.get_fail()[i]].get_all() +'\n\n\n'
				if node.error.get_warn()[0]<>'None': 	
					details=details+'\n\nPassed, with warning:\n\n'
					for i in range(0, len( node.error.get_warn())):
							details = details + self.requirements[node.error.get_warn()[i]].get_all() +'\n\n\n'
 				self.error_details.delete(1.0, END)
 				self.error_details.insert(END, details)
 			else:
 				self.error_details.delete(1.0, END)
 	

 	# Creates tree widget for error browsing
 	def make_browser(self):
 		
 		self.toppane.rowconfigure(0, weight=1)
    		self.toppane.rowconfigure(1, weight=0)
    		self.toppane.columnconfigure(0, weight=1)
    		self.toppane.columnconfigure(1, weight=0)
    		self.bottompane.columnconfigure(0, weight=1)
    		self.bottompane.columnconfigure(1, weight=0)
    		self.bottompane.rowconfigure(0, weight=0)
    		self.bottompane.rowconfigure(1, weight=1)
    		self.bottompane.rowconfigure(2, weight=0)

		tree=MyTree(master=self.toppane, 
			root_id='root', 
			root_label='Files', 
			get_contents_callback=self.get_contents, 
			height=(self.numfiles-1)*40, 
			width=300,
			node_class=MyNode)
		
		tree.root.set_error(self.errors)
		tree.set_app(self)
    		tree.grid(row=0, column=0, sticky='news')
 		tree.bind("<Button-1>", self.callback)
 		

		sby1 = Scrollbar( self.toppane, command=tree.yview, orient="vertical" )
		sbx1 = Scrollbar( self.toppane, command=tree.xview, orient="horizontal" )
		tree.config( xscrollcommand=sbx1.set, yscrollcommand=sby1.set )
		sby1.grid(row=0, column=1, sticky='ns')
		sbx1.grid(row=1, column=0, sticky='ew')
		
		
    		title=Label(self.bottompane, text="Error Details")
    		title.grid(row=0, column=0, sticky='new')
        		

    		self.error_details=Text(self.bottompane, font=("Helvetica"), width=20, wrap='none')
    		self.error_details.grid(row=1, column=0, sticky='news')
    		
    		sby = Scrollbar( self.bottompane, command=self.error_details.yview, orient="vertical" )
		sbx = Scrollbar( self.bottompane, command=self.error_details.xview, orient="horizontal" )
		self.error_details.config( xscrollcommand=sbx.set, yscrollcommand=sby.set )
		sby.grid(row=1, column=1, sticky='ns')
		sbx.grid(row=2, column=0, sticky='ew')
    		
    		return tree
	
	
    	# Creates	table widget to display requirement coverage
	def make_table(self, numrows, numcols):
		
		self.var = tktable.ArrayVar(self.bottomleft)
		self.results=tktable.Table(self.bottomleft,
			rows=numrows+1, 
			cols=2+numcols, 
			colwidth=20,
			variable=self.var, 
			state='disabled',
			colstretchmode='all',
			rowstretchmode='all',
			browsecmd=self.show_details,
			titlerows=1,
			resizeborders='none')
		
		self.results.width( 0, 34 )
		self.results.width(1, len('Requirement Category'))
		self.results.tag_row('summary')
		self.results.tag_cell('app')
		self.results.tag_cell('failed')
		self.results.tag_cell('warn')
		self.results.tag_col('reqs', 0)
		self.results.tag_row('uncov')
		
		self.results.tag_configure('summary', multiline=1, font='Default 12 bold', anchor='center')
		self.results.tag_configure('app', font='Default 12 bold')
		self.results.tag_configure('failed', background='#CC0000', foreground='white')
		self.results.tag_configure('warn', background='#FFCC00')
		self.results.tag_configure('reqs', anchor='w', wrap=1)
		self.results.tag_configure('uncov',foreground='darkgrey' )
		
		sy = Scrollbar( self.bottomleft, command=self.results.yview, orient="vertical" )
		sx = Scrollbar( self.bottomleft, command=self.results.xview, orient="horizontal" )
		self.results.config( xscrollcommand=sx.set, yscrollcommand=sy.set )
		sy.grid(row=0, column=1, sticky='ns')
		sx.grid(row=1, column=0, sticky='ew')
		self.results.grid(row=0, column=0, sticky='news')
	
	def populate_table(self, data):
		
		reqs=self.requirements.values()
		reqs.sort()
		
		for row in range(0, len(data)):
			
			height=len(data[row][0])/(self.results.width(0)+4)+1
			self.results.height( row, height)
				
			for col in range(0, len(data[row])):
				
				index = "%i,%i" % (row, col)
				self.var.set(index, data[row][col])
				
				if data[row][col]=='Failed':
					self.results.tag_cell('failed', index)
				elif data[row][col]=='Passed with Warning':
					self.results.tag_cell('warn', index)
					
			try:
				if self.requirements[data[row][0][2:12]].get_type()=='app' and self.requirements[data[row][0][2:12]].is_covered()=='Y':
    					self.results.tag_cell('app', "%i,%i" % (row, 0))
    			
    				if self.requirements[data[row][0][2:12]].is_covered()=='N':
					self.results.tag_row('uncov', row)
				
    			except KeyError:
    				donothing='true'	
				
		self.results.tag_row('summary', 1)
		self.results.height( 1, max(data[1][1].count('\n'), 3))	
		for k in range(0, len(self.files)):	
			self.results.width( k+2, max(len(self.files[k]), len('Passed with Warning')-2))		
				
	# Table widget event method (called on click in 'All Requirements' column)
	def show_details(self, event):

		if self.results.index('active')[1]==0 and self.results.index('active')[0]>1:
			index="%i,%i" % self.results.index('active')
			if self.var.get(index)[0].strip()=='+': 
				reqid=self.var.get(index)[2:12]				 
				self.var.set(index, '- ' +self.requirements[reqid].get_all() )
				height=2+self.var.get(index).count('\n')+len(self.requirements[reqid].get_details())/(self.results.width(0))
				self.results.height( self.results.index('active')[0], height )
			else:
				self.var.set(index, '+' + self.var.get(index)[1:].split('\n')[0])			
				height=len(self.var.get('active'))/(self.results.width(0)+4)+1
				self.results.height( self.results.index('active')[0], height)
				
		self.results.activate("0,0")
	
	# 'Add Files' Button widget event method
	def add_files(self):
		filename=tkFileDialog.askopenfilename(filetypes=[('AAF Files', '*.aaf')],title='Add File', initialdir=os.getcwd())
		if filename==():
			return
		self.lb.insert(END, filename)
	
	# 'Remove Files' Button widget event method
	def remove_files(self):
		rem = list(self.lb.curselection())
		rem.sort(reverse=True)
		for index in rem:
			self.lb.delete(int(index))
	
	
	def call_html(self):
		
		if self.results==None:
			tkMessageBox.showwarning("No Data to Write", "No data to write to HTML. Run Analyzer first.")
			return
			
		filename=tkFileDialog.asksaveasfilename(title='Write coverage data to HTML', filetypes=[('HTML files', '*.html')],initialdir=os.getcwd())
		if filename==():
			return
		parse.write_html(filename,self.data)
	
	# GUI main run method
	def run(self, filename, openfile):
		execpath=''
		reqspath=''
		if sys.platform == 'win32':
			execpath=os.getcwd()+'\..\..\..\AAFWinSDK\Debug\AAFAnalyzer.exe'
			reqspath=os.getcwd()+'\..\Requirements\AAFRequirements.xml'
		else:
			execpath=os.getcwd()+'/../../../AAFi686LinuxSDK/g++/bin/debug/AAFAnalyzer'
			reqspath=os.getcwd()+'/../Requirements/AAFRequirements.xml'
			
		command=execpath + ' -alldetailedreqs ' + reqspath
		pipe = Popen(command, shell=True, stdout=PIPE, stderr=None)
		command=execpath + ' -coverage ' + reqspath
		pipe2 = Popen(command, shell=True, stdout=PIPE, stderr=None)
		ret=parse.parse_requirements(pipe, pipe2)
		self.requirements=ret[0]		
		parse.parse_file_coverage(filename, self.requirements)
		
		self.make_table(ret[1]+1, self.numfiles)
		
		errors=parse.parse_errors()
		self.errors=parse.Error('Files')
		self.errors.set_suberrors(errors)
		tree = self.make_browser()
		
		if openfile:
			tree.config(state='disabled')
			self.error_details.insert(END, 'No error file generated.  Error Browser disabled.')
			self.error_details.config(state='disabled')
		
		self.data=parse.get_data(self.files, self.requirements)
		self.populate_table(self.data)
		self.bottomframe.pack(side=BOTTOM, fill=BOTH, expand=1)
	
	# 'Run Analyzer' Button widget event method
	def new_file(self):
		execpath=''
		reqspath=''
		if sys.platform == 'win32':
			execpath=os.getcwd()+'\..\..\..\AAFWinSDK\Debug\AAFAnalyzer.exe'
			reqspath=os.getcwd()+'\..\Requirements\AAFRequirements.xml'
		else:
			execpath=os.getcwd()+'/../../../AAFi686LinuxSDK/g++/bin/debug/AAFAnalyzer'
			reqspath=os.getcwd()+'/../Requirements/AAFRequirements.xml'
			
		self.files=[]
		if self.lb.size()==0:
			tkMessageBox.showwarning("No Files Added", "Must add at least one file to run Analyer.")
			return
		# PUT LINE BACK IN AFTER TESTING:
		#filename=tkFileDialog.asksaveasfilename(title='Save coverage data as', initialdir=os.getcwd())
		
		self.files=self.files+analyzer.analyze('test.txt',execpath, list(self.lb.get(0, END)), reqspath)
		
		for i in range(0, len(self.files)):
			if sys.platform == 'win32':
				self.files[i]=self.files[i].split('\\')[-1]
			else:
				self.files[i]=self.files[i].split('/')[-1]	
		self.files=['File Set'] +self.files
		self.numfiles=len(self.files)
		self.run('test.txt', 0)
	
	# 'Open File' Button widget event method
	def open_file(self):
		
		filename=tkFileDialog.askopenfilename(title='Open File', initialdir=os.getcwd())
		if filename==():
			return
		try:
			cfile=open(filename, 'r')
			line=cfile.readline()
			self.files=['File Set']
			if 'File Set' not in line:
				tkMessageBox.showwarning("Invalid File", "The file does not appear to be valid.")
				cfile.close()
				return	
			while line.strip()<>'':
				if sys.platform == 'win32':
					self.files.append(line.strip().split('\\')[-1])
				else:
					self.files.append(line.strip().split('/')[-1])
				line=cfile.readline()
			cfile.close()
	
		except IOError, e:
			print >>sys.stderr, "Error opening file: ", e
			return
			
		self.numfiles=len(self.files)
		self.run(filename, 1)
	
			
class MyTree (Tree.Tree):

	def set_app(self, a):
		self.app=a
	
	def next(self, event=None):
		Tree.Tree.next(self, event)
		app.callback(event)
            
	def prev(self, event=None):
		
		Tree.Tree.prev(self, event)
		app.callback(event)
       
	def ascend(self, event=None):
		
		Tree.Tree.ascend(self, event)
		app.callback(event)

	def descend(self, event=None):
		
		Tree.Tree.descend(self, event)
		app.callback(event)	
	
	

class MyNode (Tree.Node):
	
	def set_error(self, error):
		self.error=error
		
root = Tk()
app = App(root)


root.mainloop()
