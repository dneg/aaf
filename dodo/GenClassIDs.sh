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
echo \#ifndef __AAFClassIDs_h__
echo \#define __AAFClassIDs_h__
echo ""
echo \#ifndef __AAFTypes_h__
echo \#include \"AAFTypes.h\"
echo \#endif
echo ""
echo ""
for class in ${PRIVATE_AAFOBJECTS} ${AAFOBJECTS} ; do \
	echo "extern \"C\" const aafClassID_t CLSID_$class;"; \
done
echo ""
echo \#endif // __AAFClassIDs_h__
