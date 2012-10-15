//=---------------------------------------------------------------------=
//
// $Id$ $Name$
//
// The contents of this file are subject to the AAF SDK Public Source
// License Agreement Version 2.0 (the "License"); You may not use this
// file except in compliance with the License.  The License is available
// in AAFSDKPSL.TXT, or you may obtain a copy of the License from the
// Advanced Media Workflow Association, Inc., or its successor.
//
// Software distributed under the License is distributed on an "AS IS"
// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
// the License for the specific language governing rights and limitations
// under the License.  Refer to Section 3.3 of the License for proper use
// of this Exhibit.
//
// WARNING:  Please contact the Advanced Media Workflow Association,
// Inc., for more information about any additional licenses to
// intellectual property covering the AAF Standard that may be required
// to create and distribute AAF compliant products.
// (http://www.amwa.tv/policies).
//
// Copyright Notices:
// The Original Code of this file is Copyright 1998-2009, licensor of the
// Advanced Media Workflow Association.  All rights reserved.
//
//=---------------------------------------------------------------------=

//Test/Result files
#include "TestRegistry.h"
#include "TestRegistryException.h"
#include "Test.h"
#include "TestInfo.h"
#include "TopLevelTestResult.h"

//Requirement files
#include <RequirementRegistry.h>
#include <RequirementRegistryException.h>

namespace {

using namespace aafanalyzer;

} // end of namespace


//======================================================================
//======================================================================
//======================================================================

namespace aafanalyzer {

using namespace std;

//static variable
TestRegistry* TestRegistry::_pTestRegistry = NULL;

TestRegistry::TestRegistry()
{
}

TestRegistry::~TestRegistry()
{
    if(_pTestRegistry != NULL)
    {
        delete _pTestRegistry;
        _pTestRegistry = NULL;
    }
}

TestRegistry& TestRegistry::GetInstance()
{
    //using Singleton pattern, create object iff it does not exist
    if(_pTestRegistry == NULL)
    {
        _pTestRegistry = new TestRegistry();
    }

    return *_pTestRegistry;
}

void TestRegistry::Register( const TestInfo& info )
{
  Requirement::RequirementMapSP coveredByTest(new Requirement::RequirementMap());
  const wstring name = info.GetName();
  if ( _testSet.find( name ) == _testSet.end() )
  {
    RequirementRegistry& reqRegistry = RequirementRegistry::GetInstance();
    //Get all requirements matching the list of requirement ids.  An
    //exception is thrown if the requirement is not registered.
    const boost::shared_ptr<const vector<wstring> > spRequirements = info.GetRequirementIds();
    vector<wstring>::const_iterator iter;
    
    for ( iter = spRequirements->begin(); iter != spRequirements->end(); iter++)
    {
      wstring id = *iter;
      
      boost::shared_ptr<const Requirement> req(reqRegistry.GetRequirement(id));
      
      if ( coveredByTest->find(id) == coveredByTest->end() )
      {
	(*coveredByTest)[id] = req;
      }
      if ( _coveredRequirements.find(id) == _coveredRequirements.end() )
      {
	_coveredRequirements[id] = req;
      }
    }
    _testSet[name] = coveredByTest;
  }
  else
  {
    wstring msg;
    msg = L"Test " + name + L" is already registered.";
    throw TestRegistryException( msg.c_str() );
  }
}

const boost::shared_ptr<Requirement::RequirementMap> TestRegistry::GetRequirementsForTest( const wstring& name ) const
{
    boost::shared_ptr<const Requirement::RequirementMap> spConstReqs = this->GetConstRequirementsForTest( name );
    Requirement::RequirementMapSP spReqs( new Requirement::RequirementMap( *spConstReqs) );
    return spReqs;
}

const boost::shared_ptr<const Requirement::RequirementMap> TestRegistry::GetConstRequirementsForTest( const wstring& name ) const
{
    Map::const_iterator target = _testSet.find( name );
    if ( target == _testSet.end() )
    {
        wstring msg;
        msg = L"Test " + name + L" has not been registered.";
        throw TestRegistryException( msg.c_str() );
    }
    return target->second;
}

const Requirement::RequirementMap& TestRegistry::GetRequirementCoverage() const
{
    return _coveredRequirements;
}

bool TestRegistry::VerifyTestResultCoverage(const boost::shared_ptr<TopLevelTestResult> results) const
{
    //Given a top level result, make sure that all that are supposed to be
    //covered have been covered.
    Requirement::RequirementMap outstandingReqs(_coveredRequirements);
    this->VerifyTestResultCoverage(results, outstandingReqs);
    
    return (outstandingReqs.size() == 0);
}

void TestRegistry::VerifyTestResultCoverage(const boost::shared_ptr<const TestResult> result, Requirement::RequirementMap& outstandingReqs) const
{
    //Given a test result and a requirement map, remove all requirements covered
    //by the result and child results from the map.
    for (int curReqLevel = TestResult::PASS; curReqLevel <= TestResult::FAIL; curReqLevel++)
    {
        //Find all the requirements and loop through them.
        Requirement::RequirementMap requirements = result->GetRequirements((TestResult::Result)curReqLevel);
        Requirement::RequirementMap::iterator iter;
        for ( iter = requirements.begin(); iter != requirements.end(); iter++ )
        {
            outstandingReqs.erase( iter->first );
        }
    }
    
    //If the result contains any other subtest results, remove the requirements
    //in the subtest results from the map.
    if ( outstandingReqs.size() != 0 && result->ContainsSubtests() )
    {
        TestResult::SubtestResultVector subtestResults = result->GetSubtestResults();
        for (unsigned int i = 0; i < subtestResults.size(); i++) {
            this->VerifyTestResultCoverage(subtestResults.at(i), outstandingReqs);
            if ( outstandingReqs.size() == 0 ) 
            {
                break;
            }
        }
    }

}

} // end of namespace diskstream
