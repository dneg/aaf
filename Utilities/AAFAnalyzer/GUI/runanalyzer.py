import sys, getopt, analyzer

# Usage: on the comman line enter 
#python runanalyzer.py coverage_file_to_create AAFAnalyzer_executable file1.aaf file2.aaf file3.aaf ... AAFRequirements.xml 
#where,
#	 coverage_file_to_create – is the coverage file to generate
#	 AAFAnalyzer_executable – is the location of the AAFAnalyzer executable
#	file1.aaf file2.aaf ... - are the .aaf files to be processed
#	AAFRequirements.xml – is the location of the AAFRequirements.xml document

analyzer.analyze(sys.argv[1],sys.argv[2], sys.argv[3:-1], sys.argv[-1])
