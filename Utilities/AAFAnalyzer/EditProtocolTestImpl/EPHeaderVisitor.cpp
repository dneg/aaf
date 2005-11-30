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
#include "EPHeaderVisitor.h"
#include "EPHeaderTest.h"

//Test/Result files
#include <DetailLevelTestResult.h>
#include <TestRegistry.h>

//Requirement files
#include <RequirementRegistry.h>

//Ax files
#include <AxHeader.h>

#include <AAFResult.h>
#include <AAFOPDefs.h>

namespace {
    
using namespace aafanalyzer;

} // end of namespace

//======================================================================

namespace aafanalyzer {

using namespace boost;
 
EPHeaderVisitor::EPHeaderVisitor( wostream& log )
    : _log(log),
      _spResult( new DetailLevelTestResult( L"Edit Protocol Header Visitor",
                                            L"Visit the AAF File Header and verify it meets the Edit Protocol requirements.",
                                            L"", // explain
                                            L"", // DOCREF REQUIRED
                                            TestResult::PASS,
                                            TestRegistry::GetInstance().GetRequirementsForTest( EPHeaderTest::GetTestInfo().GetName() )
               )                          )
{}
    
EPHeaderVisitor::~EPHeaderVisitor()
{}

bool EPHeaderVisitor::PreOrderVisit( AAFTypedObjNode<IAAFHeader>& node )
{
    
    AxHeader axHeader( node.GetAAFObjectOfType() );
    
    try
    {
        aafUID_t opPattern = axHeader.GetOperationalPattern();
        if ( opPattern != kAAFOPDef_EditProtocol )
        {
            _spResult->SetRequirementStatus( TestResult::FAIL, RequirementRegistry::GetInstance().GetRequirement( L"REQ_EP_255" ) );
            _spResult->SetExplanation( L"Header::OperationalPattern property is not equal to OpEditProtocol." );
            _spResult->SetResult( TestResult::FAIL );
        }
    }
    catch ( const AxExHResult& ex )
    {
        if ( ex.getHResult() == AAFRESULT_PROP_NOT_PRESENT )
        {
            _spResult->SetRequirementStatus( TestResult::FAIL, RequirementRegistry::GetInstance().GetRequirement( L"REQ_EP_255" ) );
            _spResult->SetExplanation( L"Header::OperationalPattern property is not present." );
            _spResult->SetResult( TestResult::FAIL );
        }
        else
        {
            throw ex;
        }
    }
    return false;
}

shared_ptr<DetailLevelTestResult> EPHeaderVisitor::GetResult()
{
    return _spResult;
}

} // end of namespace aafanalyzer
