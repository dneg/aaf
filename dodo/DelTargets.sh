#################################################
#                                               #
# Copyright (c) 1998-1999 Avid Technology, Inc. #
#                                               #
#################################################

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
