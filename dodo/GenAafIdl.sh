#################################################
#                                               #
# Copyright (c) 1998-2001 Avid Technology, Inc. #
#                                               #
#################################################

echo "cpp_quote(\"//=--------------------------------------------------------------------------=\")"
perl -n -e 'chomp;' -e 'printf("cpp_quote(\"%s\")\n" , $_);' CopyrightMessage.txt
echo "cpp_quote(\"//=--------------------------------------------------------------------------=\")"
echo "cpp_quote(\"// AAF Interfaces.\")"
echo "cpp_quote(\"//=--------------------------------------------------------------------------=\")"
echo "cpp_quote(\"//\")"
echo ""
echo \#ifndef DO_NO_IMPORTS
echo import \"unknwn.idl\"\;
echo \#endif
echo ""
echo \#ifndef DO_NO_IMPORTS
echo import \"objidl.idl\"\;
echo \#endif
echo ""
echo \#ifndef DO_NO_IMPORTS
echo import \"AAFTypes.idl\"\;
echo \#endif
echo ""
for class in ${DODO_TARGET_NAMES} ${AAFCOMINTERFACESONLY} ; do \
	echo interface I$class\;; \
done
for class in ${DODO_TARGET_NAMES} ${AAFCOMINTERFACESONLY} ; do \
	echo ""; \
	echo ""; \
	cat $class.fidl; \
done
echo ""
echo ""
echo ""
cat AAFModule.fidl; \
echo ""
