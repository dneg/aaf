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
