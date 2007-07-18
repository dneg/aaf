#! /usr/bin/python
# -*- coding: utf-8 -*-

import sys, getopt, analyzer

# Usage:
#   python runanalyzer.py coverage_file_to_create AAFAnalyzer_executable \
#            file1.aaf file2.aaf file3.aaf ... AAFRequirements.xml 
#
# where,
#  coverage_file_to_create is the coverage file to generate
#  AAFAnalyzer_executable is the location of the AAFAnalyzer executable
#  AAFRequirements.xml is the location of the AAFRequirements.xml document
#  file{1,2,3,...}.aaf are the .aaf files to be processed

analyzer.analyze( sys.argv[1], sys.argv[2], sys.argv[4:-1], sys.argv[3] )
