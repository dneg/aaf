#################################################
#                                               #
# Copyright (c) 1998-2001 Avid Technology, Inc. #
#                                               #
#################################################

echo "/* this file contains the actual definitions of */"
echo "/* the IIDs and CLSIDs */"
echo ""
echo "/* link this file in with the server and any clients */"
cat CopyrightMessage.txt
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
for class in AAFRoot ${PRIVATE_AAFOBJECTS} ; do \
	awk -f dod2iid.awk C=$class $class.dod
done
echo \#ifdef __cplusplus
echo "}"
echo \#endif
echo ""
