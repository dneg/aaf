#################################################
#                                               #
# Copyright (c) 1998-1999 Avid Technology, Inc. #
#                                               #
#################################################

echo "#" This file automatically generated make.
echo "#" Special case AAFModule since no object is to be built only headers...
echo AAFModule.all : AAFModule.fidl
echo AAFModule.all : AAFModule.frefh
echo AAFModule.all : AAFModule.comc
echo AAFModule.fidl : macros/fidl.mac macros/base.mac
echo AAFModule.frefh : macros/frefh.mac macros/base.mac
echo AAFModule.comc : macros/comc.mac macros/base.mac
echo ""
echo "#" Special case AAFTypes since no object is to be built only headers...
echo AAFTypes.all : AAFTypes.idl
echo AAFTypes.all : AAFTypes.refh
echo AAFTypes.idl : macros/idl.mac macros/base.mac
echo AAFTypes.refh : macros/refh.mac macros/base.mac
echo ""
echo "#" Special case AAFRoot since this is a private implementation object...
echo AAFRoot.all : AAFRoot.comc AAFRoot.comh
echo AAFRoot.all : AAFRoot.implc AAFRoot.implh
echo AAFRoot.all : AAFRoot.fidl
echo AAFRoot.all : AAFRoot.frefh
echo AAFRoot.comc : macros/comc.mac macros/base.mac
echo AAFRoot.comh : macros/comh.mac macros/base.mac
echo AAFRoot.implc : macros/implc.mac macros/base.mac
echo AAFRoot.implh : macros/implh.mac macros/base.mac
echo AAFRoot.fidl : macros/fidl.mac macros/base.mac ; \
echo AAFRoot.frefh : macros/frefh.mac macros/base.mac ; \
echo AAFRoot.exp : macros/exp.mac macros/base.mac
echo ""
echo "#" Special case private objects since they should not have a public module test...
for base in ${PRIVATE_AAFOBJECTS} ; do \
	echo "" ; \
	echo $base.all : $base.comc $base.comh ; \
	echo $base.all : $base.comt ; \
	echo $base.all : $base.implc $base.implh ; \
	echo $base.all : $base.fidl ; \
	echo $base.all : $base.frefh ; \
	echo $base.comc : macros/comc.mac macros/base.mac ; \
	echo $base.comh : macros/comh.mac macros/base.mac ; \
	echo $base.implc : macros/implc.mac macros/base.mac ; \
	echo $base.implh : macros/implh.mac macros/base.mac ; \
	echo $base.fidl : macros/fidl.mac macros/base.mac ; \
	echo $base.frefh : macros/frefh.mac macros/base.mac ; \
	echo $base.exp : macros/exp.mac macros/base.mac ; \
	for import in `grep '^\#import' $base.dod | sed -e 's,\#import,,' | sed -e 's,.*/,,'` ; do \
		echo $base.comc : $import ; \
		echo $base.comh : $import ; \
		echo $base.implc : $import ; \
		echo $base.implh : $import ; \
		echo $base.fidl : $import ; \
		echo $base.frefh : $import ; \
		echo $base.exp : $import ; \
	done ; \
done
for base in ${DODO_TARGET_NAMES} ; do \
	echo "" ; \
	echo $base.all : $base.comc $base.comh ; \
	echo $base.all : $base.comt ; \
	echo $base.all : $base.implc $base.implh ; \
	echo $base.all : $base.fidl ; \
	echo $base.all : $base.frefh ; \
	echo $base.comc : macros/comc.mac macros/base.mac ; \
	echo $base.comh : macros/comh.mac macros/base.mac ; \
	echo $base.comt : macros/comt.mac macros/base.mac ; \
	echo $base.implc : macros/implc.mac macros/base.mac ; \
	echo $base.implh : macros/implh.mac macros/base.mac ; \
	echo $base.fidl : macros/fidl.mac macros/base.mac ; \
	echo $base.frefh : macros/frefh.mac macros/base.mac ; \
	echo $base.exp : macros/exp.mac macros/base.mac ; \
	for import in `grep '^\#import' $base.dod | sed -e 's,\#import,,' | sed -e 's,.*/,,'` ; do \
		echo $base.comc : $import ; \
		echo $base.comh : $import ; \
		echo $base.comt : $import ; \
		echo $base.implc : $import ; \
		echo $base.implh : $import ; \
		echo $base.fidl : $import ; \
		echo $base.frefh : $import ; \
		echo $base.exp : $import ; \
	done ; \
done
for base in ${AAFCOMINTERFACESONLY} ; do \
	echo "" ; \
	echo $base.all : $base.fidl $base.comcx $base.exp ; \
	echo $base.fidl : macros/fidl.mac macros/base.mac ; \
	echo $base.frefh : macros/frefh.mac macros/base.mac ; \
	echo $base.comcx : macros/comcx.mac macros/base.mac ; \
	echo $base.exp : macros/exp.mac macros/base.mac ; \
done
echo ""
echo AAFPluginTypes.all : AAFPluginTypes.idl
echo AAFPluginTypes.all : AAFPluginTypes.refh
echo AAFPluginTypes.idl : macros/idl.mac macros/base.mac
echo AAFPluginTypes.refh : macros/refh.mac macros/base.mac
echo ""
for base in ${PLUGIN_OBJECTS} ; do \
	echo "" ; \
	echo $base.all : $base.fidl $base.frefh ; \
	echo $base.fidl : macros/fidl.mac macros/base.mac ; \
	echo $base.exp : macros/exp.mac macros/base.mac ; \
	echo $base.frefh : macros/frefh.mac macros/base.mac ; \
done
