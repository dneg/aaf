#################################################
#                                               #
# Copyright (c) 1998-2001 Avid Technology, Inc. #
#                                               #
#################################################

echo "//=--------------------------------------------------------------------------="
cat CopyrightMessage.txt
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
