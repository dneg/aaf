//=---------------------------------------------------------------------=
//
// $Id$
//
// The contents of this file are subject to the AAF SDK Public
// Source License Agreement (the "License"); You may not use this file
// except in compliance with the License.  The License is available in
// AAFSDKPSL.TXT, or you may obtain a copy of the License from the AAF
// Association or its successor.
//
// Software distributed under the License is distributed on an "AS IS"
// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
// the License for the specific language governing rights and limitations
// under the License.
//
// The Original Code of this file is Copyright 1998-2004, Licensor of the
// AAF Association.
//
//=---------------------------------------------------------------------=

//Edit Protocol Test files
#include "EPParameterVisitor.h"
#include "EPParameterTest.h"

//Test/Result files
#include <DetailLevelTestResult.h>
#include <TestRegistry.h>

//Analyzer Base files
#include <DepthFirstTraversal.h>

//Ax files
#include <AxParameter.h>
#include <AxDefObject.h>
//#include <AxEx.h>

//AAF files
#include <AAFClassDefUIDs.h>
#include <AAFInterpolatorDefs.h>
//#include <AAFResult.h>

namespace {

using namespace aafanalyzer;

} // end of namespace

//======================================================================

namespace aafanalyzer {

using namespace boost;
 
EPParameterVisitor::EPParameterVisitor( wostream& log, shared_ptr<EdgeMap> spEdgeMap )
    : _log(log),
      _spEdgeMap( spEdgeMap ),
      _spResult( new DetailLevelTestResult( L"Edit Protocol Parameter Visitor",
                                            L"Visit parameters and make sure they are valid.",
                                            L"", // explain
                                            L"", // DOCREF REQUIRED
                                            TestResult::PASS,
                                            TestRegistry::GetInstance().GetRequirementsForTest( EPParameterTest::GetTestInfo().GetName() )
               )                          )
{
    _validInterpolationDefs.insert( kAAFInterpolationDef_Constant );
    _validInterpolationDefs.insert( kAAFInterpolationDef_Linear );
    _validInterpolationDefs.insert( kAAFInterpolationDef_Log );
    _validInterpolationDefs.insert( kAAFInterpolationDef_Power );
    _validInterpolationDefs.insert( kAAFInterpolationDef_BSpline );
}
    
EPParameterVisitor::~EPParameterVisitor()
{}

bool EPParameterVisitor::PreOrderVisit( AAFTypedObjNode<IAAFParameter>& node )
{
    //Parameter isn't constant or varying so fail 174.
    AxString name = this->GetMobSlotName( _spEdgeMap, node );
    _spResult->AddInformationResult( L"REQ_EP_174", L"Parameter within " + name + L" is not a ConstantValue or VaryingValue.", TestResult::FAIL );
    return false;
}

bool EPParameterVisitor::PreOrderVisit( AAFTypedObjNode<IAAFVaryingValue>& node )
{
    AxVaryingValue axVaryingVal( node.GetAAFObjectOfType() );
    AxInterpolationDef axIntDef( axVaryingVal.GetInterpolationDefinition() );
    if ( _validInterpolationDefs.find( axIntDef.GetAUID() ) == _validInterpolationDefs.end() )
    {
        AxString name = this->GetMobSlotName( _spEdgeMap, node );
        _spResult->AddInformationResult(
            L"REQ_EP_175",
            L"VaryingValue object in " + name + L" has an illegal interpolation definition (AUID = " +
                AxStringUtil::uid2Str( axIntDef.GetAUID() ) + L").",
            TestResult::FAIL
        );
    }

    //Regardless of pass/fail, there is no need to traverse further.
    return false;
}

shared_ptr<DetailLevelTestResult> EPParameterVisitor::GetResult()
{
    return _spResult;
}

} // end of namespace aafanalyzer
