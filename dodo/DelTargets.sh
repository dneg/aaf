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

if [ -z "${SRC_DIR}" ] || \
   [ -z "${PRIVATE_AAFOBJECTS}" ] || \
   [ -z "${AAFOBJECTS}" ] || \
   [ -z "${INCLUDE_DIR}" ] || \
   [ -z "${AUTO_GEN_IMPL}" ] || \
   [ -z "${TEST_DIR}" ];
then
	echo DelTargets.sh: One of the required variables is not set.
	exit 1
fi

for file in ${PRIVATE_AAFOBJECTS} ${AAFOBJECTS} ; do \
	echo rm -f ${SRC_DIR}/com-api/C$file.h ; \
	rm -f ${SRC_DIR}/com-api/C$file.h ; \
	echo rm -f ${SRC_DIR}/com-api/C$file.cpp ; \
	rm -f ${SRC_DIR}/com-api/C$file.cpp ; \
	echo rm -f ${INCLUDE_DIR}/ref-api/$file.h ; \
	rm -f ${INCLUDE_DIR}/ref-api/$file.h ; \
done

for file in ${AUTO_GEN_IMPL} ; do \
	echo rm -f ${SRC_DIR}/impl/Impl$file.cpp ; \
	rm -f ${SRC_DIR}/impl/Impl$file.cpp ; \
	echo rm -f ${SRC_DIR}/impl/Impl$file.h ; \
	rm -f ${SRC_DIR}/impl/Impl$file.h ; \
	echo rm -f ${TEST_DIR}/C${file}Test.cpp ; \
	rm -f ${TEST_DIR}/C${file}Test.cpp ; \
done
