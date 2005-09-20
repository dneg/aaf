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

#include "TestLevelTestResult.h"
#include <TestRegistry.h>
#include <RequirementMismatchException.h>

#include <sstream>

namespace {

using namespace aafanalyzer;

} // end of namespace


//======================================================================
//======================================================================
//======================================================================

namespace aafanalyzer 
{

using namespace std;

TestLevelTestResult::TestLevelTestResult( const boost::shared_ptr<const Test>& associatedTest,
                                          const Requirement::RequirementMapSP& requirements )
  : LowLevelTestResult( requirements ),
    _spAssociatedTest( associatedTest )
{
    if ( TestRegistry::GetInstance().IsUnsafeRequirements() )
    {
        //Ensure that the requirements covered by the result match the requirements
        //covered by the associated test.  If one vector is larger, search for the
        //extra element in the large vector.
        Requirement::RequirementMap coveredRequirements = *requirements;
        Requirement::RequirementMap associatedRequirements = (associatedTest->GetCoveredRequirements());
        if ( associatedRequirements.size() > coveredRequirements.size() )
        {
            this->FindRequirementMismatch(associatedRequirements,
                                          coveredRequirements,
                                          L"associated test " + associatedTest->GetName(),
                                          L"unnamed test result");
        } else
        {
            this->FindRequirementMismatch(coveredRequirements,
                                          associatedRequirements,
                                          L"unnamed test result",
                                          L"associated test " + associatedTest->GetName());
        }
    }
}

TestLevelTestResult::TestLevelTestResult( const AxString& name, 
                                          const AxString& desc,
                                          const AxString& explain,
                                          const AxString& docRef,
                                          Result defaultResult, 
                                          const boost::shared_ptr<const Test>& associatedTest,
                                          const Requirement::RequirementMapSP& requirements  )
  : LowLevelTestResult( name, desc, explain, docRef, defaultResult, requirements ),
    _spAssociatedTest( associatedTest )
{
    if ( TestRegistry::GetInstance().IsUnsafeRequirements() )
    {
        //Ensure that the requirements covered by the result match the requirements
        //covered by the associated test.  If one vector is larger, search for the
        //extra element in the large vector.
        Requirement::RequirementMap coveredRequirements = *requirements;
        Requirement::RequirementMap associatedRequirements = (associatedTest->GetCoveredRequirements());
        if ( associatedRequirements.size() > coveredRequirements.size() )
        {
            this->FindRequirementMismatch(associatedRequirements,
                                          coveredRequirements,
                                          L"associated test " + associatedTest->GetName(),
                                          L"test result " + name);
        } else
        {
            this->FindRequirementMismatch(coveredRequirements,
                                          associatedRequirements,
                                          L"test result " + name,
                                          L"associated test " + associatedTest->GetName());
        }
    }
}

TestLevelTestResult::~TestLevelTestResult()
{}

void TestLevelTestResult::AppendSubtestResult( const boost::shared_ptr<const DetailLevelTestResult>& subtestResult )
{
    //Throw an exception if the Detail Level Test Result contains a requirement
    //that is not covered by this Test Result.
    this->CheckDescendantRequirements( subtestResult, subtestResult->GetName() );
    
    this->AddSubtestResult( subtestResult );

    //If the result of the appended test is worse than any other subtest then
    //update the aggregate result.
    if ( subtestResult->GetResult() > this->GetAggregateResult() ) {
        this->SetEnumResult(subtestResult->GetResult());
    }
}

const enum TestResult::ResultLevel TestLevelTestResult::GetResultType() const
{
  return TestResult::TEST;
}

void TestLevelTestResult::FindRequirementMismatch( 
                                 const Requirement::RequirementMap& source,
                                 const Requirement::RequirementMap& matchWith,
                                 const AxString& sourceName,
                                 const AxString& matchName ) const
{
    //Loop through the source vector
    Requirement::RequirementMap::const_iterator iter;
    for ( iter = source.begin(); iter != source.end(); iter++ )
    {
        //If there is an element in the source vector that is not in the
        //mathWith vector, then throw an exception.
        if ( matchWith.find( iter->first ) == matchWith.end() )
        {
            std::wostringstream msg;
            msg << iter->first << L" in " << sourceName << L" but not in " << matchName << ".";
            throw RequirementMismatchException ( msg.str().c_str() );
        }
    }
}

void TestLevelTestResult::CheckDescendantRequirements( 
            const boost::shared_ptr<const TestResult>& subtestResult,
            const AxString& name )
{
    //Check that all requirements in the subtest result are in this test result.
    for (int reqLevel = PASS; reqLevel <= FAIL; reqLevel++)
    {
        Requirement::RequirementMap subRequirements = subtestResult->GetRequirements((Result)reqLevel);
        Requirement::RequirementMap myRequirements = this->_spAssociatedTest->GetCoveredRequirements();
        this->FindRequirementMismatch(subRequirements, myRequirements, name, L"test result " + this->GetName());
    }
    
    //If the subtest result contains any other subtest results, check that the
    //requirements in those subtest results are in this test result.
    if ( subtestResult->ContainsSubtests() )
    {
        TestResult::SubtestResultVector grandchildResults = subtestResult->GetSubtestResults();
        for (unsigned int i = 0; i < grandchildResults.size(); i++) {
            boost::shared_ptr<const TestResult> spNext = grandchildResults.at(i);
            this->CheckDescendantRequirements(spNext, name + L" - " + spNext->GetName());
        }
    }
    
}

} // end of namespace diskstream
