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
echo "// Private AAF Interfaces."
echo "//=--------------------------------------------------------------------------="
echo "//"
echo \#ifndef __AAFPrivate_h__
echo \#define __AAFPrivate_h__
echo ""
echo \#ifndef __AAF_h__
echo \#include \"AAF.h\"
echo \#endif
echo ""
echo \#ifdef __cplusplus
for class in AAFRoot ${PRIVATE_AAFOBJECTS} ; do \
	echo interface I$class\;; \
done
echo \#else
for class in AAFRoot ${PRIVATE_AAFOBJECTS} ; do \
	echo typedef interface I$class I$class\;;  \
done
echo \#endif
for class in AAFRoot ${PRIVATE_AAFOBJECTS} ; do \
	echo ""; \
	echo "// I$class"; \
	echo ""; \
	cat $class.frefh; \
done
echo ""
echo \#endif // __AAFPrivate_h__
