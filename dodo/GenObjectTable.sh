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

echo "// This file contains invocations of the macros described below."
echo "//"
echo "// To use this file -"
echo "//   1) #define the macros to suit your usage"
echo "//   2) #include this file"
echo "//   3) #undef the macros"
echo ""
echo "// Default empty definitions so that you only have to define"
echo "// those macros you actually want to use."
echo "//"
echo \#ifndef AAF_BEGIN_OBJECT_MAP
echo \#define AAF_BEGIN_OBJECT_MAP\(x\)
echo \#endif
echo ""
echo \#ifndef AAF_END_OBJECT_MAP
echo \#define AAF_END_OBJECT_MAP\(\)
echo \#endif
echo ""
echo \#ifndef AAF_OBJECT_ENTRY
echo \#define AAF_OBJECT_ENTRY\(name\)
echo \#endif
echo ""
echo \#ifndef AAF_PRIVATE_OBJECT_ENTRY
echo \#define AAF_PRIVATE_OBJECT_ENTRY\(name\)
echo \#endif
echo ""
echo ""
echo "//"
echo "// Include all objects in the following table:"
echo "//"
echo AAF_BEGIN_OBJECT_MAP\(AAFObjectMap\)
for AAF in ${PRIVATE_AAFOBJECTS} ; do \
	echo "	AAF_PRIVATE_OBJECT_ENTRY("$AAF")"
done
for AAF in ${AAFOBJECTS} ; do \
	echo "	AAF_OBJECT_ENTRY("$AAF")"
done
echo AAF_END_OBJECT_MAP\(\)
