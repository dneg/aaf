#################################################
#                                               #
# Copyright (c) 1998-1999 Avid Technology, Inc. #
#                                               #
#################################################

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
for class in ${AAFOBJECTS} ; do \
	echo "extern \"C\" const aafClassID_t CLSID_$class;"; \
done
echo ""
echo \#endif // __AAFClassIDs_h__
