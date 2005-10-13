#!/bin/bash
# This script generates the templates in TypedNodeTemplate.cpp.gen used by AAFTypedObjNode.cpp

INPUTFILE=classgen.txt
OUTPUTFILE=TypedNodeImplTemplate.cpp.gen
CLASSLIST=''

#Call the generator script to produce a class list
. generator.sh
CLASSLIST=`cat $INPUTFILE`


rm -f TypedNodeImplTemplate.cpp.gen
count=0
for aafobj in $CLASSLIST
do
    count=`expr $count + 1`
    echo "template class AAFTypedObjNodeImpl<${aafobj}>;" >> $OUTPUTFILE
done

rm classgen.txt
