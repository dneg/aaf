#################################################
#                                               #
# Copyright (c) 1998-1999 Avid Technology, Inc. #
#                                               #
#################################################

echo "cpp_quote(\"//=--------------------------------------------------------------------------=\")"
echo "cpp_quote(\"// (C) Copyright 1998-2000 Avid Technology.\")"
echo "cpp_quote(\"//\")"
echo "cpp_quote(\"// This file was GENERATED for the AAF SDK on \")"
echo "cpp_quote(\"//  `date`\")"
echo "cpp_quote(\"//\")"
echo "cpp_quote(\"// Permission to use, copy and modify this software and accompanying \")"
echo "cpp_quote(\"// documentation, and to distribute and sublicense application software \")"
echo "cpp_quote(\"// incorporating this software for any purpose is hereby granted, \")"
echo "cpp_quote(\"// provided that (i) the above copyright notice and this permission \")"
echo "cpp_quote(\"// notice appear in all copies of the software and related documentation, \")"
echo "cpp_quote(\"// and (ii) the name Avid Technology, Inc. may not be used in any \")"
echo "cpp_quote(\"// advertising or publicity relating to the software without the specific, \")"
echo "cpp_quote(\"// prior written permission of Avid Technology, Inc. \")"
echo "cpp_quote(\"//\")"
echo "cpp_quote(\"// THE SOFTWARE IS PROVIDED AS-IS AND WITHOUT WARRANTY OF ANY KIND, \")"
echo "cpp_quote(\"// EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY \")"
echo "cpp_quote(\"// WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE. \")"
echo "cpp_quote(\"// IN NO EVENT SHALL AVID TECHNOLOGY, INC. BE LIABLE FOR ANY DIRECT, \")"
echo "cpp_quote(\"// SPECIAL, INCIDENTAL, PUNITIVE, INDIRECT, ECONOMIC, CONSEQUENTIAL OR \")"
echo "cpp_quote(\"// OTHER DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER ARISING OUT OF \")"
echo "cpp_quote(\"// OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE AND \")"
echo "cpp_quote(\"// ACCOMPANYING DOCUMENTATION, INCLUDING, WITHOUT LIMITATION, DAMAGES \")"
echo "cpp_quote(\"// RESULTING FROM LOSS OF USE, DATA OR PROFITS, AND WHETHER OR NOT \")"
echo "cpp_quote(\"// ADVISED OF THE POSSIBILITY OF DAMAGE, REGARDLESS OF THE THEORY OF \")"
echo "cpp_quote(\"// LIABILITY. \")"
echo "cpp_quote(\"// \")"
echo "cpp_quote(\"//=--------------------------------------------------------------------------=\")"
echo "cpp_quote(\"// Private AAF Interfaces.\")"
echo "cpp_quote(\"//=--------------------------------------------------------------------------=\")"
echo "cpp_quote(\"//\")"
echo ""
echo ""
echo import \"AAF.idl\"\;
echo ""
for class in AAFRoot ${PRIVATE_AAFOBJECTS} ; do \
	echo interface I$class\;; \
done
for class in AAFRoot ${PRIVATE_AAFOBJECTS} ; do \
	echo ""; \
	echo ""; \
	cat $class.fidl; \
done
