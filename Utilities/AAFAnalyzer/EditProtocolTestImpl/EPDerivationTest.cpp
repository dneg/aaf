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

#include <AAFExtEnum.h>
#include <AAFResult.h>

namespace {

using namespace aafanalyzer;

const wchar_t* TEST_NAME = L"Mob Derivation Test";
const wchar_t* TEST_DESC = L"Verify the correctness of mob derivation chains.";

//======================================================================

// MobChainVisitor visitors all mob types. It asserts the ordering
// conventions specified by the Edit Protocol.
//
// These conventions are implemented using the statemachine implemented
// by <class to be implemented>.
//
// Pending the implementation of the state machine, we simple verify
// that the root compositions are in top-level.
//
// About the EdgeVisit methods: To visit all mobs in the derivation
// chain we must follow containment edges, and mob reference edges.
// The base class follows these by default so we do nothing here. We
// not, however, want to follow slot references or component
// references. Doing so would be useless because we'd miss the
// referenced mob object, further, it would be redundant if we are
// also folloing the mob reference.


class MobChainVisitor : public TypedVisitor
{
public:
  MobChainVisitor( wostream& log )
    : TypedVisitor(),
      _log( log ),
      _spResult( new TestResult( L"MobChainVisitor",
				 L"Visit mobs and verify derivation order.",
				 L"", // explain
				 L"", // DOCREF REQUIRED
				 TestResult::PASS ) )
  {}

  virtual ~MobChainVisitor()
  {}
 
  virtual bool PreOrderVisit( AAFTypedObjNode<IAAFCompositionMob>& node )
  {
    // TEMPORARY - Simply confirm that root node is top level!
    AxCompositionMob axCompMob( node.GetAAFObjectOfType() );
    bool isTopLevel = true;
    try
    {
      aafUID_t usageCode = axCompMob.GetUsageCode();
      if ( kAAFUsage_TopLevel != usageCode )
      {
	isTopLevel = false;
      }
    }
    catch ( const AxExHResult& ex )
    {
      if ( ex.getHResult() == AAFRESULT_PROP_NOT_PRESENT )
      {
	isTopLevel = false;
      }
      else
      {
	throw ex;
      }
    }

    if ( isTopLevel )
    {
      _spResult->AddDetail( L"Root composition mob has correct usage code, kAAFUsage_TopLevel" );
    }
    else
    {
      _spResult->SetResult( TestResult::FAIL );
      _spResult->AddDetail( L"Mob usage code is not kAAFUsage_TopLevel" );
    }

    // TEMPORARY - We're only checking the composition roots, so don't
    // continue the traversal.
    return false;
  }

  virtual bool PreOrderVisit( AAFTypedObjNode<IAAFMasterMob>& node )
  {
    // Stop traversal pending full implementation.
    return false;
  }

  virtual bool PreOrderVisit( AAFTypedObjNode<IAAFSourceMob>& node )
  {
    // Stop traversal pending full implementation.
    return false;
  }

  virtual bool EdgeVisit(AAFComponentReference& edge)
  {
    return true;
  }

  virtual bool EdgeVisit(AAFSlotReference& edge)
  {
    return true;
  }
 
  shared_ptr<TestResult> GetResult()
  {
    return _spResult;
  }

private:

  wostream& _log;
  shared_ptr<TestResult> _spResult;
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

  spResult->AppendSubtestResult( spVisitor->GetResult() );
  spResult->SetResult( spResult->GetAggregateResult() );
  
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
