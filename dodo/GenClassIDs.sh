#################################################
#                                               #
# Copyright (c) 1998-2001 Avid Technology, Inc. #
#                                               #
#################################################

echo "//=--------------------------------------------------------------------------="
cat CopyrightMessage.txt
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
