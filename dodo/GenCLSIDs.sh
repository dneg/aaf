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
echo "// Declarations for all of the private AAF code class ids"
echo "// This file is private to the AAF Reference Implementation."
echo "//=--------------------------------------------------------------------------="
echo "//"
echo \#ifndef __AAFCLSIDs_h__
echo \#define __AAFCLSIDs_h__
echo ""
echo \#ifndef __AAFCOMPlatformTypes_h__
echo \#include \"AAFCOMPlatformTypes.h\"
echo \#endif
echo ""
for class in ${PRIVATE_AAFOBJECTS} ${AAFOBJECTS} ; do \
	echo "EXTERN_C const CLSID CLSID_$class;"; \
done
echo ""
echo \#endif // __AAFCLSIDs_h__
