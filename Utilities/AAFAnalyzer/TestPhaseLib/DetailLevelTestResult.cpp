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

//Test/Result files
#include "DetailLevelTestResult.h"
#include "RequirementMismatchException.h"

namespace {

using namespace aafanalyzer;

} // end of namespace


//======================================================================
//======================================================================
//======================================================================

namespace aafanalyzer 
{

using namespace std;
using namespace boost;

DetailLevelTestResult::DetailLevelTestResult( const Requirement::RequirementMapSP& requirements )
  : LowLevelTestResult( requirements )
{}

DetailLevelTestResult::DetailLevelTestResult( const wstring& name, 
                                              const wstring& desc,
                                              const wstring& explain,
                                              const wstring& docRef,
                                              Result defaultResult, 
                                              const Requirement::RequirementMapSP& requirements )
  : LowLevelTestResult( name, desc, explain, docRef, defaultResult, requirements )
{}

DetailLevelTestResult::~DetailLevelTestResult()
{}

void DetailLevelTestResult::AppendSubtestResult( const shared_ptr<const DetailLevelTestResult>& subtestResult )
{
    //Don't allow a test result to append itself
    if (subtestResult.get() == this)
    {
        return;
    }
    
    this->AddSubtestResult( subtestResult );

    //If the result of the appended test is worse than any other subtest then
    //update the aggregate result.
    if ( subtestResult->GetResult() > this->GetAggregateResult() ) {
        this->SetEnumResult(subtestResult->GetResult());
    }
}

const enum TestResult::ResultLevel DetailLevelTestResult::GetResultType() const
{
  return TestResult::DETAIL;
}

void DetailLevelTestResult::AddInformationResult( const wstring& reqId, const wstring& explain, Result result )
{
    Result reqStatus;
    if ( !this->ContainsRequirment( reqId, reqStatus ) )
    {
        wstring msg;
        msg = L"Cannot add information result for requirement: " + reqId + L" because it is not in the DetailLevelTestResult " + this->GetName();
        throw RequirementMismatchException ( msg.c_str() );
    }
    
    Requirement::RequirementMapSP reqMapSP(new Requirement::RequirementMap);
    shared_ptr<const Requirement> requirement = (*(this->GetMyRequirements( reqStatus )))[reqId];
    (*reqMapSP)[reqId] = requirement;
    
    shared_ptr<DetailLevelTestResult> spResult( new DetailLevelTestResult( this->GetName(),
                                L"-", // desc
                                explain,
                                L"-", // docref
                                result,
                                reqMapSP ) );
    spResult->SetRequirementStatus( result, requirement );
    
    this->AppendSubtestResult( spResult );
    this->SetResult( this->GetAggregateResult() );
    this->SetRequirementStatus( result, requirement );
}

} // end of namespace diskstream
