#################################################
#                                               #
# Copyright (c) 1998-1999 Avid Technology, Inc. #
#                                               #
#################################################

echo "/* this file contains the actual definitions of */"
echo "/* the IIDs and CLSIDs */"
echo ""
echo "/* link this file in with the server and any clients */"
echo "//=--------------------------------------------------------------------------="
echo "// (C) Copyright 1998-1999 Avid Technology."
echo "//"
echo "// This file was GENERATED for the AAF SDK on "
echo "//  "`date`
echo "//"
echo "// Permission to use, copy and modify this software and accompanying "
echo "// documentation, and to distribute and sublicense application software "
echo "// incorporating this software for any purpose is hereby granted, "
echo "// provided that (i) the above copyright notice and this permission "
echo "// notice appear in all copies of the software and related documentation, "
echo "// and (ii) the name Avid Technology, Inc. may not be used in any "
echo "// advertising or publicity relating to the software without the specific, "
echo "// prior written permission of Avid Technology, Inc. "
echo "//"
echo "// THE SOFTWARE IS PROVIDED AS-IS AND WITHOUT WARRANTY OF ANY KIND, "
echo "// EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY "
echo "// WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE. "
echo "// IN NO EVENT SHALL AVID TECHNOLOGY, INC. BE LIABLE FOR ANY DIRECT, "
echo "// SPECIAL, INCIDENTAL, PUNITIVE, INDIRECT, ECONOMIC, CONSEQUENTIAL OR "
echo "// OTHER DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER ARISING OUT OF "
echo "// OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE AND "
echo "// ACCOMPANYING DOCUMENTATION, INCLUDING, WITHOUT LIMITATION, DAMAGES "
echo "// RESULTING FROM LOSS OF USE, DATA OR PROFITS, AND WHETHER OR NOT "
echo "// ADVISED OF THE POSSIBILITY OF DAMAGE, REGARDLESS OF THE THEORY OF "
echo "// LIABILITY. "
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
