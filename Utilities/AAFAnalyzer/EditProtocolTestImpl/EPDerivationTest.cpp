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

#include <EPDerivationTest.h>

#include <TypedVisitor.h>
#include <DepthFirstTraversal.h>

#include <AxMob.h>

namespace {

using namespace aafanalyzer;

const wchar_t* TEST_NAME = L"Mob Derivation Test";
const wchar_t* TEST_DESC = L"Verify the correctness of mob derivation chains.";

//======================================================================

// MobChainVisitor visitors all mob types. It asserts the ordering
// conventions specified by the Edit Protocol.  Those conventions are
// implemented by TBD.

class MobChainVisitor : public TypedVisitor
{
public:
  MobChainVisitor( wostream& log )
    : TypedVisitor(),
      _log( log )
  {}

  virtual ~MobChainVisitor()
  {}
 
  virtual bool PreOrderVisit( AAFTypedObjNode<IAAFCompositionMob> spNode )
  {
    _log << L"Visit composition mob." << endl;
    return true;
  }

  virtual bool PreOrderVisit( AAFTypedObjNode<IAAFMasterMob> spNode )
  {
    _log << L"Visit master mob." << endl;
    return true;
  }

  virtual bool PreOrderVisit( AAFTypedObjNode<IAAFSourceMob> spNode )
  {
    _log << L"Visit source mob." << endl;
    return true;
  }

private:
  wostream& _log;
};

//======================================================================

shared_ptr<TestResult> AnalyzeMobChain( wostream& log,
					shared_ptr<TestGraph> spGraph,
					CompMobDependency::CompMobNodeSP spRootComposition )
{
  shared_ptr<TestResult> spResult( new TestResult( L"Verify Mob Chain",
						   L"Verify the structure of one mob chain.",
						   L"", // explain
						   L"",  // DOCREF REQUIRED
						   TestResult::PASS ) );
  
  
  AxCompositionMob axCompMob( spRootComposition->GetAAFObjectOfType() );
  AxString mobName = axCompMob.GetName();
  spResult->AddDetail( L"Analyzing root composition mob \"" + mobName + L"\"" );

  DepthFirstTraversal dft( spGraph->GetEdgeMap(),
			   spRootComposition );

  shared_ptr<MobChainVisitor>  spVisitor( new MobChainVisitor(log) );
  dft.TraverseDown( spVisitor );

  
  return spResult;
}

//======================================================================

class Analyzer
{
public:
  Analyzer( wostream& log,
		   shared_ptr<TestGraph> spGraph,
		   shared_ptr<TestResult> spPhaseResult )
    : _log( log ),
      _spGraph( spGraph ),
      _spPhaseResult( spPhaseResult )
  {}

  void operator () ( const CompMobDependency::CompMobNodeSP& spRootComposition )
  {
    _spPhaseResult->AppendSubtestResult( AnalyzeMobChain( _log, _spGraph, spRootComposition ) );
  }

private:
  wostream& _log;
  shared_ptr<TestGraph> _spGraph;
  shared_ptr<TestResult> _spPhaseResult;
};

//======================================================================

} // end of namespace

//======================================================================

namespace aafanalyzer {

EPDerivationTest::EPDerivationTest( wostream& log,
				    shared_ptr<TestGraph> spGraph,
				    CompMobDependency::CompMobNodeVectorSP spTopLevelCompMobs )
  : Test( log ),
    _spGraph( spGraph ),
    _spTopLevelCompMobs( spTopLevelCompMobs )
{}

EPDerivationTest::~EPDerivationTest()
{}

shared_ptr<TestResult> EPDerivationTest::Execute()
{
  shared_ptr<TestResult> spPhaseResult( new TestResult(TEST_NAME,
						  TEST_DESC,
						  L"", // explain
						  L"",  // DOCREF REQUIRED
						  TestResult::PASS ) );
  


  Analyzer analyzer( GetOutStream(), _spGraph, spPhaseResult );
  for_each( _spTopLevelCompMobs->begin(), _spTopLevelCompMobs->end(), analyzer );

  spPhaseResult->SetResult( spPhaseResult->GetAggregateResult() );

  return spPhaseResult;
}

AxString EPDerivationTest::GetName() const
{
  return TEST_NAME;
}

AxString EPDerivationTest::GetDescription() const
{
  return TEST_DESC;
}

} // end of namespace aafanalyzer
