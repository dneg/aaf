###############################################################################
#
# The contents of this file are subject to the AAF SDK Public
# Source License Agreement (the "License"); You may not use this file
# except in compliance with the License.  The License is available in
# AAFSDKPSL.TXT, or you may obtain a copy of the License from the AAF
# Association or its successor.
# 
# Software distributed under the License is distributed on an "AS IS"
# basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
# the License for the specific language governing rights and limitations
# under the License.
# 
# The Original Code of this file is Copyright 1998-2001, Licensor of the
# AAF Association.
# 
# The Initial Developer of the Original Code of this file and the
# Licensor of the AAF Association is Avid Technology.
# All rights reserved.
#
###############################################################################

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
