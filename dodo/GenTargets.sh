#################################################
#                                               #
# Copyright (c) 1998-1999 Avid Technology, Inc. #
#                                               #
#################################################

echo "#" This file automatically generated make.
echo "#" Special case AAFTypes since no object is to be built only headers...
echo "#" special case the utility classes since they will not be exposed by com 
echo DODO_TARGETS = '\' 
echo '	'AAFTypes.all \\c 
for base in AAFRoot ${PRIVATE_AAFOBJECTS} ${AAFOBJECTS} ${AAFCOMINTERFACESONLY} ;  do \
	echo '\' ; \
	echo '	'$base.all \\c ; \
done
echo '\'
echo '	'AAFModule.all \\c ;
echo ''
echo ''
echo PRIVATE_FIDL_TARGETS = \\c 
for base in ${PRIVATE_AAFOBJECTS} ;  do \
	echo '\' ; \
	echo '	'$base.fidl \\c ; \
done
echo ''
echo ''
echo FIDL_TARGETS = \\c 
for base in ${AAFOBJECTS} ${AAFCOMINTERFACESONLY} ;  do \
	echo '\' ; \
	echo '	'$base.fidl \\c ; \
done
echo '\'
echo '	'AAFModule.fidl \\c ;
echo ''
echo ''
echo PRIVATE_FREFH_TARGETS = \\c 
for base in AAFRoot ${PRIVATE_AAFOBJECTS} ;  do \
	echo '\' ; \
	echo '	'$base.frefh \\c ; \
done
echo ''
echo ''
echo FREFH_TARGETS = \\c 
for base in ${AAFOBJECTS} ${AAFCOMINTERFACESONLY} ;  do \
	echo '\' ; \
	echo '	'$base.frefh \\c ; \
done
echo '\'
echo '	'AAFModule.frefh \\c ;
echo ''
echo ''
echo PLUGIN_TARGETS = '\' 
echo '	'AAFTypes.all' \' 
echo '	'AAFPluginTypes.all \\c 
for base in ${PLUGIN_OBJECTS} ;  do \
	echo '\' ; \
	echo '	'$base.all \\c ; \
done
echo ''
echo ''
echo PLUGIN_FIDL_TARGETS = \\c 
for base in ${PLUGIN_OBJECTS} ;  do \
	echo '\' ; \
	echo '	'$base.fidl \\c ; \
done
echo ''
echo ''
echo PLUGIN_FREFH_TARGETS = \\c 
for base in ${PLUGIN_OBJECTS} ;  do \
	echo '\' ; \
	echo '	'$base.frefh \\c ; \
done
echo ''
