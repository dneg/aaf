#!/bin/bash
# This script generates a file used by EPTypedVisitor header, using EPTypedObj.cpp

TEMPLATEPARAMS=`grep 'template class EPTypedObjNode.*' EPTypedObjNode.cpp | sed 's/template class EPTypedObjNode//' | sed 's/\;//' | sed 's/, */,/'`
OUTHEADERFILE=EPTypedVisitor.h.gen

rm -f EPTypedVisitor.h.gen

####GENERATE THE HEADER FILE METHODS
exec 4>$OUTHEADERFILE

count=0
for params in $TEMPLATEPARAMS
do
    count=`expr $count + 1`
    echo '    virtual bool PreOrderVisit( EPTypedObjNode'$params'& node) { return this->ForwardPreOrderVisit'$params'( node ); }' >&4
    echo '    virtual bool PostOrderVisit( EPTypedObjNode'$params'& node) { return this->ForwardPostOrderVisit'$params'( node ); }' >&4
done

echo >&4
