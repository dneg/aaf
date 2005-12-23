#!/bin/bash
# This script generates a file used by TypedVisitor header, using classgen.txt

INPUTFILE=classgen.txt
OUTHEADERFILE=TypedVisitor.h.gen

#Call the generator script to produce a class list
. generator.sh
CLASSLIST=`cat $INPUTFILE`

rm -f TypedVisitor.h.gen

####GENERATE THE HEADER FILE METHODS
exec 4>$OUTHEADERFILE

count=0
for aafobj in $CLASSLIST
do
    count=`expr $count + 1`
    echo '    virtual bool PreOrderVisit( AAFTypedObjNode<'$aafobj'>& node) { return DefaultPreOrderBehaviour(); }' >&4
    echo '    virtual bool PostOrderVisit( AAFTypedObjNode<'$aafobj'>& node){ return DefaultPostOrderBehaviour(); }' >&4
done

echo >&4

#Remove the text file used by this script
rm classgen.txt
