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

#
# Function: output
#
# output() writes its arguments separated by blanks on the standard output.
# The arguments will not be terminated by a new-line.
#
function output
{
	printf "$*"
}

#
# Function: outputln
#
# outputln() writes its arguments separated by blanks and
# terminated by a new-line on the standard output.
#
function outputln
{
	printf "$*\n"
}

outputln "#" This file automatically generated make.
outputln DODO_TARGETS = '\\'
outputln '	'../ref-impl/src/impl/ImplAAFRoot.h' \\'
outputln '	'../ref-impl/src/impl/ImplAAFRoot.cpp' \\'

for base in AAFRoot ${PRIVATE_AAFOBJECTS} ${AAFOBJECTS} ; do
	outputln '	'../ref-impl/src/com-api/C$base.cpp' \\'
	outputln '	'../ref-impl/src/com-api/C$base.h' \\'
done
outputln '	'../ref-impl/src/com-api/CAAFModule.cpp
outputln ''

output   'PRIVATE_FIDL_TARGETS = '
for base in AAFRoot ${PRIVATE_AAFOBJECTS} ;  do
	outputln '\\'
	output   '	'$base.fidl' '
done
outputln ''
outputln ''
output   'FIDL_TARGETS = '
for base in ${AAFOBJECTS} ${AAFCOMINTERFACESONLY} ;  do
	outputln '\\'
	output   '	'$base.fidl' '
done
outputln '\\'
outputln '	AAFModule.fidl '
outputln ''
output   'PRIVATE_FREFH_TARGETS = '
for base in AAFRoot ${PRIVATE_AAFOBJECTS} ;  do
	outputln '\\'
	output   '	'$base.frefh' '
done
outputln ''
outputln ''
output   'FREFH_TARGETS = '
for base in ${AAFOBJECTS} ${AAFCOMINTERFACESONLY} ;  do
	outputln '\\'
	output   '	'$base.frefh' '
done
outputln '\\'
outputln '	AAFModule.frefh '
outputln ''
outputln ''
output   'PLUGIN_FIDL_TARGETS = '
for base in ${PLUGIN_OBJECTS} ;  do
	outputln '\\'
	output   '	'$base.fidl' '
done
outputln ''
outputln ''
output   'PLUGIN_FREFH_TARGETS = '
for base in ${PLUGIN_OBJECTS} ;  do
	outputln '\\'
	output   '	'$base.frefh' '
done
outputln ''

