#################################################
#                                               #
# Copyright (c) 1998-2001 Avid Technology, Inc. #
#                                               #
#################################################

echo "//=--------------------------------------------------------------------------="
cat CopyrightMessage.txt
echo "//=--------------------------------------------------------------------------="
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
