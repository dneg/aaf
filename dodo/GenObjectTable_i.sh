#################################################
#                                               #
# Copyright (c) 1998-2001 Avid Technology, Inc. #
#                                               #
#################################################

echo "//=--------------------------------------------------------------------------="
cat CopyrightMessage.txt
echo "//=--------------------------------------------------------------------------="
echo ""
echo "//"
echo "// Include the CLSID declarations..."
echo "//"
echo \#include \"AAFCLSIDs.h\"
echo ""
echo "//"
echo "// Include the appropriate header files:"
echo "//"
for AAF in ${PRIVATE_AAFOBJECTS} ; do \
    echo \#include \"C$AAF.h\"
done
for AAF in ${AAFOBJECTS} ; do \
    echo \#include \"C$AAF.h\"
done
echo ""
echo "//"
echo "// Include Private AAF Objects in Table:"
echo "//"
echo \#define AAF_PRIVATE_OBJECT_ENTRY\(name\) AAF_OBJECT_ENTRY\(name\)
echo ""
echo "//"
echo "// Include AAF Object Table:"
echo "//"
echo \#include \"AAFObjectTable.h\"
