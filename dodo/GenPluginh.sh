#################################################
#                                               #
# Copyright (c) 1998-2001 Avid Technology, Inc. #
#                                               #
#################################################

echo "//=--------------------------------------------------------------------------="
cat CopyrightMessage.txt
echo "//=--------------------------------------------------------------------------="
echo "// AAF Plugin Interfaces."
echo "//=--------------------------------------------------------------------------="
echo "//"
echo \#ifndef __AAFPlugin_h__
echo \#define __AAFPlugin_h__
echo ""
echo \#ifndef __AAF_h__
echo \#include \"AAF.h\"
echo \#endif
echo ""
echo \#ifndef __AAFPluginTypes_h__
echo \#include \"AAFPluginTypes.h\"
echo \#endif
echo ""
echo \#ifdef __cplusplus
for class in ${PLUGIN_OBJECTS} ; do \
	echo interface I$class\;; \
done
echo \#else
for class in ${PLUGIN_OBJECTS} ; do \
	echo typedef interface I$class I$class\;;  \
done
echo \#endif
for class in ${PLUGIN_OBJECTS} ; do \
	echo ""; \
	echo "// I$class"; \
	echo ""; \
	cat $class.frefh; \
done
echo ""
echo \#endif // __AAFPlugin_h__
