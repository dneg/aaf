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
#include <DetailLevelTestResult.h>

#include <Test.h>

#include <Requirement.h>
#include <RequirementMismatchException.h>

#include <iostream>

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

DetailLevelTestResult::DetailLevelTestResult( const shared_ptr<const Test> associatedTest,
                                              const wstring& explain,
                                              const wstring& reqId,
                                              Result result,
					      shared_ptr<Node> spNode )
  : LowLevelTestResult( associatedTest, L"", L"", explain ),
    _reqId( reqId ),
    _spNode( spNode )
{
  shared_ptr<const Requirement> spReq = GetRequirement();
  SetName( spReq->GetName() );
  SetDescription( spReq->GetDescription() );
  SetResult( _reqId, result );
}

DetailLevelTestResult::~DetailLevelTestResult()
{}

void DetailLevelTestResult::SetResult( const wstring& reqId, Result result )
{
  // Some data structure maintenance:
  //
  // This result object should never store the result for more than
  // one requirement and reqId should always match that used to
  // construct the object and if a requirement is mapped then it
  // should be reqId.  Any of these invariants could be violated as a
  // result of a programming error, therefore, assert these
  // conditions.
  //
  {
    // Requirement should match.
    assert( reqId == _reqId );

    // There should only be one (or zero, if this is called from the
    // constructor) mapped requirement...
    size_t reqCount = this->GetRequirements(FAIL).size() +
                      this->GetRequirements(WARN).size() +
                      this->GetRequirements(PASS).size();
    assert( reqCount <= 1 );
  
    // .. and if there is one, it should be reqId
    if ( reqCount )
    {
      Result containedIn = UNDEFINED;

      assert( this->ContainsRequirement( reqId, containedIn ) );

      // .. and the map it is stored in should match the individual
      // result value for this object.
      assert( containedIn == this->GetResult() );
      (void)containedIn;
    }
  }

  // Everything is as it should be therefore place the reqId in the
  // appropriate result map and set the result value (on the base
  // class) for this result object instance.
  this->AddRequirement( result, this->GetRequirement() );
    
  // Call down to the base class to set the result value for this
  // object.
  this->ProtectedSetResult(result);

  // That's it. We now have the result value recorded against it's
  // requirement in the appropriate requirement map and the actual
  // result value is set (yes, it's redundant).  These will be
  // propogated up to the higher level result objects when the result
  // tree is consolidated.
}

const enum TestResult::ResultLevel DetailLevelTestResult::GetResultType() const
{
  return TestResult::DETAIL;
}

shared_ptr<const Requirement> DetailLevelTestResult::GetRequirement() const
{
  // Confirm that reqId is registered against the associated test.  We
  // don't want to report test results for requirments that are not
  // registered for the test because we depend on the registered
  // requirements to report accurate information regarding which
  // requirements are covered by which test.
  if ( !this->GetAssociatedTest()->IsRequirementRegistered( _reqId ) )
  {
    wstring msg;
    msg = L"Cannot set result for requirement \"" + _reqId +
          L"\" because it is not registered against test \"" + this->GetName() + L"\"";
    throw RequirementMismatchException ( msg.c_str() );
  }

  // Look up and return the requirement.
  return this->GetAssociatedTest()->GetRequirement( _reqId );
}

shared_ptr<Node> DetailLevelTestResult::GetAssociatedNode() const
{
  return _spNode;
}

const wstring& DetailLevelTestResult::GetId() const
{
  return _reqId;
}

} // end of namespace diskstream
