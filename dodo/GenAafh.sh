#################################################
#                                               #
# Copyright (c) 1998-2001 Avid Technology, Inc. #
#                                               #
#################################################

echo "//=--------------------------------------------------------------------------="
cat CopyrightMessage.txt
echo "//=--------------------------------------------------------------------------="
echo "// AAF Interfaces."
echo "//=--------------------------------------------------------------------------="
echo "//"
echo \#ifndef __AAF_h__
echo \#define __AAF_h__
echo ""
echo \#ifndef __AAFCOMPlatform_h__
echo \#include \"AAFCOMPlatform.h\"
echo \#endif
echo ""
echo \#ifndef __AAFTypes_h__
echo \#include \"AAFTypes.h\"
echo \#endif
echo ""
echo \#ifdef __cplusplus
for class in ${DODO_TARGET_NAMES} ${AAFCOMINTERFACESONLY} ; do \
	echo interface I$class\;; \
done
echo \#else
for class in ${DODO_TARGET_NAMES} ${AAFCOMINTERFACESONLY} ; do \
	echo typedef interface I$class I$class\;;  \
done
echo \#endif
for class in ${DODO_TARGET_NAMES} ${AAFCOMINTERFACESONLY} ; do \
	echo ""; \
	echo "// I$class"; \
	echo ""; \
	cat $class.frefh; \
done
echo ""
echo ""
echo ""
cat AAFModule.frefh; \
echo ""
echo \#endif // __AAF_h__
