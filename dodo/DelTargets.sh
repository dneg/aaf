#################################################
#                                               #
# Copyright (c) 1998-1999 Avid Technology, Inc. #
#                                               #
#################################################

for file in ${AAFOBJECTS} ; do \
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
