#################################################
#                                               #
# Copyright (c) 1998-1999 Avid Technology, Inc. #
#                                               #
#################################################

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
outputln "#" Special case AAFTypes since no object is to be built only headers...
outputln "#" special case the utility classes since they will not be exposed by com 
outputln DODO_TARGETS = '\\'
for base in AAFTypes AAFRoot ${PRIVATE_AAFOBJECTS} ${AAFOBJECTS} ${AAFCOMINTERFACESONLY} ; do
	outputln '	'$base.all' \\'
done
outputln '	'AAFModule.all
outputln ''
output   'PRIVATE_FIDL_TARGETS = '
for base in ${PRIVATE_AAFOBJECTS} ;  do
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
outputln 'PLUGIN_TARGETS = \\'
outputln '	AAFTypes.all \\' 
output   '	AAFPluginTypes.all '
for base in ${PLUGIN_OBJECTS} ;  do
	outputln '\\'
	output   '	'$base.all' '
done
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
