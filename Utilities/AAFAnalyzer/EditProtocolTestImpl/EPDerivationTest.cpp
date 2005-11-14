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
#include "EPDerivationTest.h"
#include "DerivationChainStateMachine.h"
#include "StateMachineException.h"

//Test/Result files
#include <TestLevelTestResult.h>
#include <DetailLevelTestResult.h>
#include <TestRegistry.h>

//Requirement files
#include <Requirement.h>
#include <RequirementRegistry.h>

//Edit Protocol Analyzer Base files
#include <EPTypedVisitor.h>
#include <EPTypedObjNode.h>

//AAF Analyzer Base files
#include <MobNodeMap.h>

//Analyzer Base files
#include <DepthFirstTraversal.h>
#include <TestGraph.h>

//Ax files
#include <AxMob.h>
#include <AxMetaDef.h>
#include <AxEssence.h>
#include <AxComponent.h>

//AAF files
#include <AAFResult.h>
#include <AAFClassDefUIDs.h>

//STL files
#include <vector>

namespace {

using namespace aafanalyzer;
using namespace boost;

const wchar_t* TEST_NAME = L"Mob Derivation Test";
const wchar_t* TEST_DESC = L"Verify the correctness of mob derivation chains.";

//======================================================================

// MobChainVisitor visitors all mob types. It asserts the ordering
// conventions specified by the Edit Protocol.
//
// These conventions are implemented using the statemachine implemented
// by DerivationChainStateMachine.

class MobChainVisitor : public EPTypedVisitor
{
public:
  MobChainVisitor( wostream& log )
    : EPTypedVisitor(),
      _log( log ),
      _spResult( new DetailLevelTestResult( L"MobChainVisitor",
                                            L"Visit mobs and verify derivation order.",
                                            L"", // explain
                                            L"", // DOCREF REQUIRED
                                            TestResult::PASS,
                                            TestRegistry::GetInstance().GetRequirementsForTest( EPDerivationTest::GetTestInfo().GetName() )
               )                          ),
      _stateMachine( log )
  {}

  virtual ~MobChainVisitor()
  {}

  virtual bool PreOrderVisit( EPTypedObjNode<IAAFCompositionMob, EPTopLevelComposition>& node )
  {
    AxCompositionMob axCompMob( node.GetAAFObjectOfType() );
    return this->VisitMobWithUsageCode( axCompMob, node.GetLID(), EPTopLevelComposition::GetName() );
  }
  
  virtual bool PreOrderVisit( EPTypedObjNode<IAAFCompositionMob, EPLowerLevelComposition>& node )
  {
    AxCompositionMob axCompMob( node.GetAAFObjectOfType() );
    return this->VisitMobWithUsageCode( axCompMob, node.GetLID(), EPLowerLevelComposition::GetName() );
  }
  
  virtual bool PreOrderVisit( EPTypedObjNode<IAAFCompositionMob, EPSubClipComposition>& node )
  {
    AxCompositionMob axCompMob( node.GetAAFObjectOfType() );
    return this->VisitMobWithUsageCode( axCompMob, node.GetLID(), EPSubClipComposition::GetName() );
  }
  
  virtual bool PreOrderVisit( EPTypedObjNode<IAAFCompositionMob, EPAdjustedClipComposition>& node )
  {
    AxCompositionMob axCompMob( node.GetAAFObjectOfType() );
    return this->VisitMobWithUsageCode( axCompMob, node.GetLID(), EPAdjustedClipComposition::GetName() );
  }
  
  virtual bool PreOrderVisit( EPTypedObjNode<IAAFMasterMob, EPTemplateClip>& node )
  {
    AxMasterMob axMastMob( node.GetAAFObjectOfType() );
    return this->VisitMobWithUsageCode( axMastMob, node.GetLID(), EPTemplateClip::GetName() );
  }
  
  virtual bool PreOrderVisit( EPTypedObjNode<IAAFMasterMob, EPClip>& node )
  {
       
    AxMasterMob axMastMob( node.GetAAFObjectOfType() );
    
    AxString nodeName = this->GetMobName( axMastMob, EPClip::GetName() );
    
    AxString detailMsg;
    bool successfulTransition = false;
    vector<shared_ptr<const Requirement> > failingReqIds;

    successfulTransition = _stateMachine.Transition( kAAFClassID_MasterMob, nodeName, detailMsg, failingReqIds );
    _spResult->AddDetail( detailMsg );
    if ( !successfulTransition )
    {
      for ( unsigned int i = 0; i < failingReqIds.size(); i++ )
      {
        _spResult->SetRequirementStatus( TestResult::FAIL, failingReqIds.at( i ) );
      }
      _spResult->SetExplanation( nodeName + L" is out of place in the derrivation chain." );
      _spResult->SetResult( TestResult::FAIL );
      return false;
    }

    //If this node has already been visited, and this visit resulted in a
    //successful transition, all nodes that follow on the derivation chain must
    //be in the correct sequence, therefore, the traversal of this part of the
    //derivation chain can be terminated.
    
    Node::LID lid = node.GetLID();
    
    if ( this->HaveVisited( lid ) )
    {
        return false;
    }
    this->RecordVisit( lid );
    
    return true;

  }
  
  virtual bool PreOrderVisit( EPTypedObjNode<IAAFSourceMob, EPFileSource>& node )
  {
    return this->VisitMobWithDescriptor( node, EPFileSource::GetName() );
  }
  
  virtual bool PreOrderVisit( EPTypedObjNode<IAAFSourceMob, EPRecordingSource>& node )
  {
    return this->VisitMobWithDescriptor( node, EPRecordingSource::GetName() );
  }
  
  virtual bool PreOrderVisit( EPTypedObjNode<IAAFSourceMob, EPImportSource>& node )
  {
    return this->VisitMobWithDescriptor( node, EPImportSource::GetName() );
  }
  
  virtual bool PreOrderVisit( EPTypedObjNode<IAAFSourceMob, EPTapeSource>& node )
  {
    return this->VisitMobWithDescriptor( node, EPTapeSource::GetName() );
  }
  
  virtual bool PreOrderVisit( EPTypedObjNode<IAAFSourceMob, EPFilmSource>& node )
  {
    return this->VisitMobWithDescriptor( node, EPFilmSource::GetName() );
  }
  
  virtual bool PreOrderVisit( AAFTypedObjNode<IAAFSourceClip>& node )
  {
    AxSourceClip axSrcClip( node.GetAAFObjectOfType() );
    AxString detailMsg;
    bool successfulTransition = false;
    vector<shared_ptr<const Requirement> > failingReqIds;
 
    try
    {

      aafMobID_t srcID = axSrcClip.GetSourceReference().sourceID;
      shared_ptr<Node> spNode;
      spNode = MobNodeMap::GetInstance().GetMobNode(srcID);
    
      if ( srcID == AxConstants::NULL_MOBID )
      {
        successfulTransition = _stateMachine.Transition( kAAFClassID_SourceClip, L"Source Clip", detailMsg, failingReqIds );
      }
      else if ( !spNode )
      {
        successfulTransition = _stateMachine.Transition( kAAFClassID_SourceMob, L"Source Clip", detailMsg, failingReqIds );
      }
      else
      {
        //This is a source clip that is not null and referencing another mob in
        //this file so just continue on.
        return true;
      }
    }
    catch ( const AxExHResult& ex )
    {
      successfulTransition = false;
      detailMsg = L"Source Clip does not have a Source Reference property.";
    }
    
    _spResult->AddDetail( detailMsg );
    if ( !successfulTransition )
    {
      for ( unsigned int i = 0; i < failingReqIds.size(); i++ )
      {
        //If the Requirement is REQ_EP_019, then, it should present a warning,
        //not a failure.
        if ( failingReqIds.at( i ) == RequirementRegistry::GetInstance().GetRequirement( L"REQ_EP_019" ) )
        {
            _spResult->SetRequirementStatus( TestResult::WARN, failingReqIds.at( i ) );
            if ( _spResult->GetResult() == TestResult::PASS )
            {
                _spResult->SetResult( TestResult::WARN );
            }
        }
        else
        {
            _spResult->SetRequirementStatus( TestResult::FAIL, failingReqIds.at( i ) );
            _spResult->SetResult( TestResult::FAIL );
        }
      }
      _spResult->SetExplanation( L"Source Clip is out of place in the derrivation chain." );
      return false;
    }
    
    //If this node has already been visited, and this visit resulted in a
    //successful transition, all nodes that follow on the derivation chain must
    //be in the correct sequence, therefore, the traversal of this part of the
    //derivation chain can be terminated.
    
    Node::LID lid = node.GetLID();
    
    if ( this->HaveVisited( lid ) )
    {
        return false;
    }
    
    return true;

  }

  virtual bool PreOrderVisit( AAFTypedObjNode<IAAFCompositionMob>& node )
  {
    AxCompositionMob axCompMob( node.GetAAFObjectOfType() );
    AxString nodeName = this->GetMobName( axCompMob, L"Composition Mob" ); 
    _spResult->AddDetail( nodeName + L" does not have an Edit Protocol material type." );
    _spResult->SetExplanation( nodeName + L" is out of place in the derrivation chain." );
    _spResult->SetResult( TestResult::FAIL );

    return false;
  }
  
  virtual bool PreOrderVisit( AAFTypedObjNode<IAAFMasterMob>& node )
  {
    AxMasterMob axMastMob( node.GetAAFObjectOfType() );
    AxString nodeName = this->GetMobName( axMastMob, L"Master Mob" );
    _spResult->AddDetail( nodeName + L" does not have an Edit Protocol material type." );
    _spResult->SetExplanation( nodeName + L" is out of place in the derrivation chain." );
    _spResult->SetResult( TestResult::FAIL );

    return false;
  }

  virtual bool PreOrderVisit( AAFTypedObjNode<IAAFSourceMob>& node )
  {
    AxSourceMob axSrcMob( node.GetAAFObjectOfType() );
    AxString nodeName = this->GetMobName( axSrcMob, L"Source Mob" );
    _spResult->AddDetail( nodeName + L" does not have an Edit Protocol material type." );
    _spResult->SetExplanation( nodeName + L" is out of place in the derrivation chain." );
    _spResult->SetResult( TestResult::FAIL );

    return false;
  }
  
  virtual bool PreOrderVisit( AAFTypedObjNode<IAAFMob>& node )
  {
    AxMob axMob( node.GetAAFObjectOfType() );
    AxString nodeName = this->GetMobName( axMob, L"Mob" );
    _spResult->AddDetail( nodeName + L" does not have an Edit Protocol material type." );
    _spResult->SetExplanation( nodeName + L" is out of place in the derrivation chain." );
    _spResult->SetResult( TestResult::FAIL );

    return false;
  }

  virtual bool PostOrderVisit( AAFTypedObjNode<IAAFCompositionMob>& node )
  {
    AxCompositionMob axCompMob( node.GetAAFObjectOfType() );
    return this->PostVisitMob( axCompMob, L"Composition Mob");
  }
  
  virtual bool PostOrderVisit( AAFTypedObjNode<IAAFMasterMob>& node )
  {
    AxMasterMob axMastMob( node.GetAAFObjectOfType() );
    return this->PostVisitMob( axMastMob, L"Master Mob");
  }
  
  virtual bool PostOrderVisit( AAFTypedObjNode<IAAFSourceMob>& node )
  {
    AxSourceMob axSrcMob( node.GetAAFObjectOfType() );
    return this->PostVisitMob( axSrcMob, L"Source Mob");
  }

  virtual bool PostOrderVisit( AAFTypedObjNode<IAAFSourceClip>& node )
  {
    AxSourceClip axSrcClip( node.GetAAFObjectOfType() );

    aafMobID_t srcID = axSrcClip.GetSourceReference().sourceID;
    shared_ptr<Node> spNode;
    spNode = MobNodeMap::GetInstance().GetMobNode(srcID);
    
    //Only pop the current state of the stack if this is an End of Chain or
    //Out of File source reference.
    if ( srcID == AxConstants::NULL_MOBID || !spNode )
    {
      _stateMachine.TransitionBack();
    }
    
    return true;
  }
 
  virtual bool EdgeVisit(AAFMobReference& edge)
  {
    return true;
  }

  shared_ptr<DetailLevelTestResult> GetResult()
  {
    return _spResult;
  }

private:

  // prohibited
  MobChainVisitor();
  MobChainVisitor( const MobChainVisitor& );
  MobChainVisitor& operator=( const MobChainVisitor& );

  //Return a AUID that is in the state machines Event map.
  aafUID_t GetAcceptedAUID( AxClassDef& clsDef ) const
  {
    aafUID_t auid = clsDef.GetAUID();
    
    if ( _stateMachine.IsKnownEvent( auid ) )
    {
        return auid;
    }

    AxClassDef parentDef( clsDef.GetParent() );
    return GetAcceptedAUID( parentDef );
    
  }
  
  bool VisitMobWithUsageCode( AxMob& axMob, Node::LID lid, const AxString& type )
  {
    AxString nodeName = this->GetMobName( axMob, type );

    AxString detailMsg;
    bool successfulTransition = false;
    vector<shared_ptr<const Requirement> > failingReqIds;

    successfulTransition = _stateMachine.Transition( axMob.GetUsageCode(), nodeName, detailMsg, failingReqIds );   
    _spResult->AddDetail( detailMsg );
    if ( !successfulTransition )
    {
      for ( unsigned int i = 0; i < failingReqIds.size(); i++ )
      {
        _spResult->SetRequirementStatus( TestResult::FAIL, failingReqIds.at( i ) );
      }
      _spResult->SetExplanation( nodeName + L" is out of place in the derrivation chain." );
      _spResult->SetResult( TestResult::FAIL );
      return false;
    }
    
    //If this node has already been visited, and this visit resulted in a
    //successful transition, all nodes that follow on the derivation chain must
    //be in the correct sequence, therefore, the traversal of this part of the
    //derivation chain can be terminated.
       
    if ( this->HaveVisited( lid ) )
    {
        return false;
    }
    
    return true;
    
  }
  
  bool VisitMobWithDescriptor( AAFTypedObjNode<IAAFSourceMob>& node, const AxString& type )
  {
    AxSourceMob axSrcMob( node.GetAAFObjectOfType() );
    AxString nodeName = this->GetMobName( axSrcMob, type );

    AxString detailMsg;
    bool successfulTransition = false;
    vector<shared_ptr<const Requirement> > failingReqIds;

    AxEssenceDescriptor descriptor( axSrcMob.GetEssenceDescriptor() );
    AxClassDef clsDef( descriptor.GetDefinition() );
    aafUID_t descriptorAUID = GetAcceptedAUID( clsDef );
    successfulTransition = _stateMachine.Transition( descriptorAUID, nodeName, detailMsg, failingReqIds );   
    _spResult->AddDetail( detailMsg );
    if ( !successfulTransition )
    {
      for ( unsigned int i = 0; i < failingReqIds.size(); i++ )
      {
        _spResult->SetRequirementStatus( TestResult::FAIL, failingReqIds.at( i ) );
      }
      _spResult->SetExplanation( nodeName + L" is out of place in the derrivation chain." );
      _spResult->SetResult( TestResult::FAIL );
      return false;
    }

    //If this node has already been visited, and this visit resulted in a
    //successful transition, all nodes that follow on the derivation chain must
    //be in the correct sequence, therefore, the traversal of this part of the
    //derivation chain can be terminated.

    Node::LID lid = node.GetLID();
       
    if ( this->HaveVisited( lid ) )
    {
        return false;
    }
    
    return true;
    
  }

  bool PostVisitMob( AxMob& axMob, const AxString& type )
  {
    bool successfulTransition = _stateMachine.TransitionBack();

    AxString nodeName = this->GetMobName( axMob, type );

    if ( !successfulTransition )
    {        
        _spResult->SetRequirementStatus( TestResult::FAIL, RequirementRegistry::GetInstance().GetRequirement( L"REQ_EP_017" ) );
        _spResult->SetExplanation( L"End of derivation chain encountered without a zero-valued source clip or out of file reference at " + nodeName );
        _spResult->SetResult( TestResult::FAIL );
    }
    
    return successfulTransition;
  }

  wostream& _log;
  shared_ptr<DetailLevelTestResult> _spResult;
  DerivationChainStateMachine _stateMachine;
  
};

//======================================================================

shared_ptr<DetailLevelTestResult> AnalyzeMobChain( wostream& log,
					shared_ptr<const TestGraph> spGraph,
					CompMobDependency::CompMobNodeSP spRootComposition )
{

  shared_ptr<DetailLevelTestResult> spResult( new DetailLevelTestResult( L"Verify Mob Chain",
						   L"Verify the structure of one mob chain.",
						   L"", // explain
						   L"",  // DOCREF REQUIRED
						   TestResult::PASS,
                           TestRegistry::GetInstance().GetRequirementsForTest( EPDerivationTest::GetTestInfo().GetName() ) ) );
  
  AxCompositionMob axCompMob( spRootComposition->GetAAFObjectOfType() );
  AxString mobName;
  try
  {
    mobName = axCompMob.GetName();
  }
  catch ( const AxExHResult& ex )
  {
    if ( ex.getHResult() == AAFRESULT_PROP_NOT_PRESENT )
    {
      mobName = L"Unnamed Composition Mob";
    }
    else
    {
      throw ex;
    }
  }
  spResult->AddDetail( L"Analyzing root composition mob \"" + mobName + L"\"" );

  DepthFirstTraversal dft( spGraph->GetEdgeMap(),
			   spRootComposition );

  shared_ptr<MobChainVisitor>  spVisitor( new MobChainVisitor(log) );
  dft.TraverseDown( spVisitor );

  for (int reqLevel = TestResult::PASS; reqLevel <= TestResult::FAIL; reqLevel++)
  {
    Requirement::RequirementMap childReqs = spVisitor->GetResult()->GetRequirements( (TestResult::Result)reqLevel );
    Requirement::RequirementMap::const_iterator iter;
    for( iter = childReqs.begin(); iter != childReqs.end(); ++iter )
    {
      spResult->SetRequirementStatus( (TestResult::Result)reqLevel, iter->second );
    }
  }
  spResult->AppendSubtestResult( spVisitor->GetResult() );
  spResult->SetResult( spResult->GetAggregateResult() );
  
  if ( spResult->GetResult() == TestResult::FAIL )
  {
    spResult->SetExplanation(L"Visitor Failed - See \"" + spVisitor->GetResult()->GetName() + L"\" Visitor for details.");
  }
  
  return spResult;
}

//======================================================================

class Analyzer
{
public:
  Analyzer( wostream& log,
		   shared_ptr<const TestGraph> spGraph,
		   shared_ptr<TestLevelTestResult> spPhaseResult )
    : _log( log ),
      _spGraph( spGraph ),
      _spPhaseResult( spPhaseResult )
  {}

  void operator () ( const CompMobDependency::CompMobNodeSP& spRootComposition )
  {
    shared_ptr<DetailLevelTestResult> detailResult = AnalyzeMobChain( _log, _spGraph, spRootComposition );
    _spPhaseResult->AppendSubtestResult( detailResult );
    for (int reqLevel = TestResult::PASS; reqLevel <= TestResult::FAIL; reqLevel++)
    {
      Requirement::RequirementMap childReqs = detailResult->GetRequirements( (TestResult::Result)reqLevel );
      Requirement::RequirementMap::const_iterator iter;
      for( iter = childReqs.begin(); iter != childReqs.end(); ++iter )
      {
        _spPhaseResult->SetRequirementStatus( (TestResult::Result)reqLevel, iter->second );
      }
    }
  }

private:
  wostream& _log;
  shared_ptr<const TestGraph> _spGraph;
  shared_ptr<TestLevelTestResult> _spPhaseResult;
  
  // prohibited
  Analyzer();
  Analyzer& operator=( const Analyzer& );
  
};

//======================================================================

} // end of namespace

//======================================================================

namespace aafanalyzer {

using namespace std;

EPDerivationTest::EPDerivationTest( wostream& log,
				    shared_ptr<const TestGraph> spGraph,
				    CompMobDependency::CompMobNodeVectorSP spTopLevelCompMobs )
  : Test( log, GetTestInfo() ),
    _spTopLevelCompMobs( spTopLevelCompMobs )
{
    SetTestGraph(spGraph);
}

EPDerivationTest::~EPDerivationTest()
{}

shared_ptr<TestLevelTestResult> EPDerivationTest::Execute()
{
  const shared_ptr<const Test> me = this->shared_from_this();
  Requirement::RequirementMapSP spMyReqs(new Requirement::RequirementMap(this->GetCoveredRequirements()));
  shared_ptr<TestLevelTestResult> spPhaseResult( new TestLevelTestResult(TEST_NAME,
						       TEST_DESC,
						       L"", // explain
						       L"",  // DOCREF REQUIRED
						       TestResult::PASS,
                               me, 
                               spMyReqs ) );

  Analyzer analyzer( GetOutStream(), GetTestGraph(), spPhaseResult );
  for_each( _spTopLevelCompMobs->begin(), _spTopLevelCompMobs->end(), analyzer );

  spPhaseResult->SetResult( spPhaseResult->GetAggregateResult() );
  if ( spPhaseResult->GetResult() == TestResult::FAIL )
  {
    spPhaseResult->SetExplanation(L"Test Failed - See \"Verify Mob Chain\" Visitor for details");
  }

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

const TestInfo EPDerivationTest::GetTestInfo()
{
    shared_ptr<vector<AxString> > spReqIds(new vector<AxString>);
    spReqIds->push_back(L"REQ_EP_017");     //Derivation Chain ends with zero-value source clip.
    spReqIds->push_back(L"REQ_EP_018");     //Reference to Master or Composition mob cannot be out of file.
    spReqIds->push_back(L"REQ_EP_019");     //Reference to Source mob should not be out of file.
    spReqIds->push_back(L"REQ_EP_025");     //Only valid transition to Top-Level is from Initial.
    spReqIds->push_back(L"REQ_EP_026");     //Valid transitions from Top-Level.
    spReqIds->push_back(L"REQ_EP_030");     //Only Top-Level and Lower-Level may reference Lower-Level.
    spReqIds->push_back(L"REQ_EP_031");     //Valid transitions from Lower-Level ~ The specification does not allow Lower-Level to Lower-Level but the test does.
    spReqIds->push_back(L"REQ_EP_036");     //Valid transitions from Sub-Clip.
    spReqIds->push_back(L"REQ_EP_045");     //Valid transitions from Adjusted Clip.
    spReqIds->push_back(L"REQ_EP_050");     //Template Clip must end chain.
    spReqIds->push_back(L"REQ_EP_055");     //Valid transitions from Clip.
    spReqIds->push_back(L"REQ_EP_063");     //Valid transitions from File Source ~ transitions are not actually specified, also, a transition to another File Source is assumed valid.
    spReqIds->push_back(L"REQ_EP_064");     //Transition from File Source to: Recording Source is valid, End of Chain is invalid ~ with 063 implies if a File Source reference another File Source a Recording Source is required, this is not implemented.
    spReqIds->push_back(L"REQ_EP_072");     //Recording source must end chain.
    spReqIds->push_back(L"REQ_EP_076");     //Valid Transitions from Import Source ~ not actually specified.
    spReqIds->push_back(L"REQ_EP_083");     //Valid Transitions from Tape Source ~ not actually specified.
    //TODO: Push an item for valid transitions from Film Source.
    return TestInfo(L"EPDerivationTest", spReqIds);
}

} // end of namespace aafanalyzer
