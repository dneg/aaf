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
#include "HighLevelTestResult.h"

namespace {

using namespace aafanalyzer;

} // end of namespace


//======================================================================
//======================================================================
//======================================================================

namespace aafanalyzer 
{

using namespace std;

HighLevelTestResult::HighLevelTestResult()
  : TestResult()
{}

HighLevelTestResult:: HighLevelTestResult( const wstring& name, 
                                           const wstring& desc,
                                           const wstring& explain,
                                           const wstring& docRef,
                                           Result defaultResult )
  : TestResult( name, desc, explain, docRef, defaultResult )
{}

HighLevelTestResult::~HighLevelTestResult()
{}

void HighLevelTestResult::UpdateRequirementStatus()
{
  this->ClearRequirements();
  //Only update if there are subtests.
  if (this->ContainsSubtests()) {

    TestResult::SubtestResultVector subResults = this->GetSubtestResults();
    //Repear for each type of result (PASS, WARN, FAIL) in every subtest.
    for (unsigned int i = 0; i < subResults.size(); i++) 
    {
      for (int curReqLevel = PASS; curReqLevel <= FAIL; curReqLevel++)
      {
        //Find all the requirements and loop through them.
        Requirement::RequirementMap spRequirements = subResults.at(i)->GetRequirements((Result)curReqLevel);           
        Requirement::RequirementMap::iterator iter;
        for ( iter = spRequirements.begin(); iter != spRequirements.end(); iter++ )
        {
            //If the requirement was already in a map store it in the map with
            //the worst possible status.  Otherwise, add the requirement to the
            //map that it is in, in the child subtest.
            Result oldReqLevel;
            if ( this->ContainsRequirment( iter->first, oldReqLevel ) ) {
                if (oldReqLevel < curReqLevel)
                {
                    this->RemoveRequirement(iter->first);
                    this->AddRequirement((Result)curReqLevel, iter->second);
                }
            } else {
                this->AddRequirement((Result)curReqLevel, iter->second);
            }
        }
      }
    }
  }
}

} // end of namespace diskstream
