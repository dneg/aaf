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

echo "//=--------------------------------------------------------------------------="
echo "// AAF Interfaces."
echo "//=--------------------------------------------------------------------------="
echo "//"
echo \#ifndef __AAF_h__
echo \#define __AAF_h__
echo ""
echo \#ifndef __AAFCOMPlatform_h__
echo \#include \"AAFCOMPlatform.h\"
echo \#endif
echo ""
echo \#ifndef __AAFTypes_h__
echo \#include \"AAFTypes.h\"
echo \#endif
echo ""
echo \#ifdef __cplusplus
for class in ${DODO_TARGET_NAMES} ${AAFCOMINTERFACESONLY} ; do \
	echo interface I$class\;; \
done
echo \#else
for class in ${DODO_TARGET_NAMES} ${AAFCOMINTERFACESONLY} ; do \
	echo typedef interface I$class I$class\;;  \
done
echo \#endif
for class in ${DODO_TARGET_NAMES} ${AAFCOMINTERFACESONLY} ; do \
	echo ""; \
	echo "// I$class"; \
	echo ""; \
	cat $class.frefh; \
done
echo ""
echo ""
echo ""
cat AAFModule.frefh; \
echo ""
echo \#endif // __AAF_h__
