#!/bin/bash
# This script generates a header & cpp file for TypedVisitor, using classgen.txt

INPUTFILE=classgen.txt
OUTHEADERFILE=TypedVisitor.h
OUTCPPFILE=TypedVisitor.cpp

#Call the generator script to produce a class list
. generator.sh
CLASSLIST=`cat $INPUTFILE`

#Change to the AAFAnalyzerBase dir to place the files in.
cd AAFAnalyzerBase/

####GENERATE THE HEADER FILE
exec 4>$OUTHEADERFILE
echo '//Auto generated file for Typed Visitor methods' >&4
echo >&4
echo '#ifndef __TYPEDVISITOR_h__' >&4
echo '#define __TYPEDVISITOR_h__' >&4
echo >&4
echo '//project files' >&4
echo '#include "Visitor.h"' >&4
echo '#include "AAFTypedObjNode.h"' >&4
echo >&4
echo '//AAF files' >&4
echo '#include <AAF.h>' >&4
echo >&4
echo '//boost files' >&4
echo '#include <boost/shared_ptr.hpp>' >&4
echo >&4
echo 'namespace aafanalyzer {' >&4
echo >&4
echo 'class AAFContainment;' >&4
echo 'class AAFComponentReference;' >&4
echo 'class AAFMobReference;' >&4
echo 'class AAFSlotReference;' >&4
echo >&4
echo >&4

echo 'class TypedVisitor : public Visitor' >&4
echo '{' >&4
echo '  public:'>&4
echo '    TypedVisitor();' >&4
echo '    ~TypedVisitor();' >&4
echo >&4

count=0
for aafobj in $CLASSLIST
do
    count=`expr $count + 1`
    echo '    bool PreOrderVisit(boost::shared_ptr<AAFTypedObjNode<'$aafobj'> > spNode);' >&4
    echo '    bool PostOrderVisit(boost::shared_ptr<AAFTypedObjNode<'$aafobj'> > spNode);' >&4
done

echo >&4
echo '    bool EdgeVisit(boost::shared_ptr<AAFContainment> spEdge);' >&4
echo '    bool EdgeVisit(boost::shared_ptr<AAFComponentReference> spEdge);' >&4
echo '    bool EdgeVisit(boost::shared_ptr<AAFMobReference> spEdge);' >&4
echo '    bool EdgeVisit(boost::shared_ptr<AAFSlotReference> spEdge); ' >&4
echo '};' >&4
echo >&4
echo '} // end of namespace diskstream' >&4
echo >&4
echo '#endif/*__TEMPLATE_h__*/' >&4


####GENERATE THE CPP FILE
exec 5>$OUTCPPFILE
echo '//Auto generated file for Typed Visitor methods' >&5
echo >&5
echo '#include "TypedVisitor.h"' >&5
echo >&5
echo '//project files' >&5
echo '#include "AAFContainment.h"' >&5
echo '#include "AAFComponentReference.h"' >&5
echo '#include "AAFMobReference.h"' >&5
echo '#include "AAFSlotReference.h"' >&5
echo >&5
echo 'namespace {' >&5
echo 'using namespace aafanalyzer;' >&5
echo '} // end of namespace' >&5
echo >&5
echo >&5
echo >&5
echo 'namespace aafanalyzer' >&5
echo '{' >&5
echo 'TypedVisitor::TypedVisitor()' >&5
echo '{ }' >&5
echo >&5
echo 'TypedVisitor::~TypedVisitor()' >&5
echo '{ }' >&5
echo >&5

count=0
for aafobj in $CLASSLIST
do
    count=`expr $count + 1`
    echo 'bool TypedVisitor::PreOrderVisit(boost::shared_ptr<AAFTypedObjNode<'$aafobj'> > spNode)' >&5
    echo '{ return true; }' >&5
    echo >&5
    echo 'bool TypedVisitor::PostOrderVisit(boost::shared_ptr<AAFTypedObjNode<'$aafobj'> > spNode)' >&5
    echo '{ return true; }' >&5
    echo >&5
done

echo 'bool TypedVisitor::EdgeVisit(boost::shared_ptr<AAFContainment> spEdge)' >&5
echo '{ return true; }' >&5
echo >&5
echo 'bool TypedVisitor::EdgeVisit(boost::shared_ptr<AAFComponentReference> spEdge)' >&5
echo '{ return true; }' >&5
echo >&5
echo 'bool TypedVisitor::EdgeVisit(boost::shared_ptr<AAFMobReference> spEdge)' >&5
echo '{ return true; }' >&5
echo >&5
echo 'bool TypedVisitor::EdgeVisit(boost::shared_ptr<AAFSlotReference> spEdge)' >&5
echo '{ return true; }' >&5
echo >&5
echo '} // end of namespace diskstream' >&5
echo >&5

#Remove the text file used by this script
cd ..
rm classgen.txt
