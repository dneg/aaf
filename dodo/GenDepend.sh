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

echo "#" This file automatically generated make.
echo "#" Special case AAFModule since no object is to be built only headers...
echo AAFModule.fidl : macros/fidl.mac macros/base.mac
echo AAFModule.frefh : macros/frefh.mac macros/base.mac
echo AAFModule.comc : macros/comc.mac macros/base.mac
echo ""
echo "#" Special case AAFTypes since no object is to be built only headers...
echo AAFTypes.idl : macros/idl.mac macros/base.mac
echo AAFTypes.refh : macros/refh.mac macros/base.mac
echo ""
echo "#" Special case AAFRoot since this is a private implementation object...
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
	echo $base.fidl : macros/fidl.mac macros/base.mac ; \
	echo $base.frefh : macros/frefh.mac macros/base.mac ; \
	echo $base.comcx : macros/comcx.mac macros/base.mac ; \
	echo $base.exp : macros/exp.mac macros/base.mac ; \
done
echo ""
echo AAFPluginTypes.idl : macros/idl.mac macros/base.mac
echo AAFPluginTypes.refh : macros/refh.mac macros/base.mac
echo ""
for base in ${PLUGIN_OBJECTS} ; do \
	echo "" ; \
	echo $base.fidl : macros/fidl.mac macros/base.mac ; \
	echo $base.exp : macros/exp.mac macros/base.mac ; \
	echo $base.frefh : macros/frefh.mac macros/base.mac ; \
done
