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

#include <EPMobDepPhase.h>

#include <CompMobDependency.h>
#include <EPDerivationTest.h>

#include <TestResult.h>

#include <sstream>

namespace {

using namespace aafanalyzer;

const wchar_t* PHASE_NAME=L"Mob Dependency Analysis Phase";
const wchar_t* PHASE_DESC=L"Analyze mob references to ensure structure complies with the Edit Protocol.";


} // end of namespace

//======================================================================

namespace aafanalyzer {

EPMobDepPhase::EPMobDepPhase( wostream& log, shared_ptr<TestGraph> spGraph )
  : TestPhase( log ),
    _log( log ),
    _spGraph( spGraph )
{}

EPMobDepPhase::~EPMobDepPhase()
{}

AxString EPMobDepPhase::GetDescription() const
{
  return PHASE_DESC;
}

AxString EPMobDepPhase::GetName() const
{
  return PHASE_NAME;
}

boost::shared_ptr<TestResult> EPMobDepPhase::Execute()
{
  shared_ptr<TestResult> spPhaseResult( new TestResult( PHASE_NAME,        // name
						       PHASE_DESC,        // desc
						       L"",               // explain
						       L"",               // DOCREF REQUIRED
						       TestResult::PASS ) );

  // First, compute the composition mob dependencies.

  CompMobDependency depTest( _log, _spGraph );
  shared_ptr<TestResult> depTestResult = depTest.Execute();
  spPhaseResult->AppendSubtestResult( depTest.Execute() );

  spPhaseResult->SetResult( spPhaseResult->GetAggregateResult() );

  CompMobDependency::CompMobNodeVectorSP spRootNodes = depTest.GetRootCompMobNodes();
  wstringstream ss;
  ss << spRootNodes->size() << L" unreferenced composition ";
  if ( spRootNodes->size() == 1 )
  {
    ss << L"mob ";
  }
  else
  {
    ss << L"mobs ";
  }
  ss << "found.";
  spPhaseResult->AddDetail( ss.str() );

  // Second, run the dependency test to verify the chains starting
  // with the identified root compositions.
  EPDerivationTest derivationTest( _log, _spGraph, spRootNodes );
  spPhaseResult->AppendSubtestResult( derivationTest.Execute() );

  spPhaseResult->SetResult( spPhaseResult->GetAggregateResult() );

  return spPhaseResult;
}

} // end of namespace aafanalyzer
