#################################################
#                                               #
# Copyright (c) 1998-1999 Avid Technology, Inc. #
#                                               #
#################################################

for file in ${HUMAN_TYPED_IMPL} ; do \
	grep -v $file\.impl depend.tmp | grep -v $file\.comt > depend.tmp2 ; \
	rm -f depend.tmp ; \
	mv depend.tmp2 depend.tmp ; \
done
