#################################################
#                                               #
# Copyright (c) 1998-2001 Avid Technology, Inc. #
#                                               #
#################################################

echo "cpp_quote(\"//=--------------------------------------------------------------------------=\")"
perl -n -e 'chomp;' -e 'printf("cpp_quote(\"%s\")\n" , $_);' CopyrightMessage.txt
echo "cpp_quote(\"//=--------------------------------------------------------------------------=\")"
echo "cpp_quote(\"// Private AAF Interfaces.\")"
echo "cpp_quote(\"//=--------------------------------------------------------------------------=\")"
echo "cpp_quote(\"//\")"
echo ""
echo ""
echo import \"AAF.idl\"\;
echo ""
for class in AAFRoot ${PRIVATE_AAFOBJECTS} ; do \
	echo interface I$class\;; \
done
for class in AAFRoot ${PRIVATE_AAFOBJECTS} ; do \
	echo ""; \
	echo ""; \
	cat $class.fidl; \
done
