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

echo "/* this file contains the actual definitions of */"
echo "/* the IIDs and CLSIDs */"
echo ""
echo "/* link this file in with the server and any clients */"
echo "//=--------------------------------------------------------------------------="
echo "// Definitions for all public IID's needed by an AAF SDK Plugin author"
echo "//=--------------------------------------------------------------------------="
echo "//"
echo ""
echo \#ifdef __cplusplus
echo "extern \"C\"{"
echo \#endif
echo ""
echo ""
echo \#ifndef __IID_DEFINED__
echo \#define __IID_DEFINED__
echo ""
echo "typedef struct _IID"
echo "{"
echo "    unsigned long x;"
echo "    unsigned short s1;"
echo "    unsigned short s2;"
echo "    unsigned char  c[8];"
echo "} IID;"
echo ""
echo \#endif "// __IID_DEFINED__"
echo ""
echo \#ifndef CLSID_DEFINED
echo \#define CLSID_DEFINED
echo "typedef IID CLSID;"
echo \#endif "// CLSID_DEFINED"
echo ""
for class in ${PLUGIN_OBJECTS} ; do \
	awk -f dod2iid.awk C=$class $class.dod
done
echo \#ifdef __cplusplus
echo "}"
echo \#endif
echo ""
