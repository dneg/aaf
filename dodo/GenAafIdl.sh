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

echo "cpp_quote(\"//=--------------------------------------------------------------------------=\")"
echo "cpp_quote(\"// AAF Interfaces.\")"
echo "cpp_quote(\"//=--------------------------------------------------------------------------=\")"
echo "cpp_quote(\"//\")"
echo ""
echo \#ifndef DO_NO_IMPORTS
echo import \"unknwn.idl\"\;
echo \#endif
echo ""
echo \#ifndef DO_NO_IMPORTS
echo import \"objidl.idl\"\;
echo \#endif
echo ""
echo \#ifndef DO_NO_IMPORTS
echo import \"AAFTypes.idl\"\;
echo \#endif
echo ""
for class in ${DODO_TARGET_NAMES} ${AAFCOMINTERFACESONLY} ; do \
	echo interface I$class\;; \
done
for class in ${DODO_TARGET_NAMES} ${AAFCOMINTERFACESONLY} ; do \
	echo ""; \
	echo ""; \
	cat $class.fidl; \
done
echo ""
echo ""
echo ""
cat AAFModule.fidl; \
echo ""
