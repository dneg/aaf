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

#include "DetailLevelTestResult.h"

namespace {

using namespace aafanalyzer;

} // end of namespace


//======================================================================
//======================================================================
//======================================================================

namespace aafanalyzer 
{

using namespace std;

DetailLevelTestResult::DetailLevelTestResult( const Requirement::RequirementMapSP& requirements )
  : LowLevelTestResult( requirements )
{}

DetailLevelTestResult::DetailLevelTestResult( const AxString& name, 
                                              const AxString& desc,
                                              const AxString& explain,
                                              const AxString& docRef,
                                              Result defaultResult, 
                                              const Requirement::RequirementMapSP& requirements )
  : LowLevelTestResult( name, desc, explain, docRef, defaultResult, requirements )
{}

DetailLevelTestResult::~DetailLevelTestResult()
{}

void DetailLevelTestResult::AppendSubtestResult( const boost::shared_ptr<const DetailLevelTestResult>& subtestResult )
{
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

} // end of namespace diskstream
