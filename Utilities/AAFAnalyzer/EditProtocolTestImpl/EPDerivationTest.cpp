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
#include <AxUtil.h>

//AAF files
#include <AAFResult.h>
#include <AAFClassDefUIDs.h>

//STL files
#include <vector>

#include <iostream>

// set to 1 to activate debug output
#define DEBUG_OUT 0

namespace {

using namespace aafanalyzer;
using namespace boost;

// JPT REVIEW - This should be reusable.
AxString Indent( int l )
{
  AxString indent;
  for (; l > 0 ; l-- ) 
  {
    indent += L"  ";
  }
  return indent;
}


const wchar_t* TEST_NAME = L"Mob Derivation Test";
const wchar_t* TEST_DESC = L"Verify the correctness of mob derivation chains.";

//======================================================================

// MobChainVisitor visits all mob types. It asserts the ordering
// conventions specified by the Edit Protocol.
//
// These conventions are implemented using the (state machine based)
// parser implemented by DerivationChainStateMachine.

class MobChainVisitor : public EPTypedVisitor
{
public:
  MobChainVisitor( wostream& log,
                   shared_ptr<TestLevelTestResult> spTestResult )
    : EPTypedVisitor( Visitor::FOLLOW_REFERENCES ),
      _log( log ),
      _spTestResult( spTestResult ),
      _stateMachine( log ),
      _level( 0 )
  {}

  virtual ~MobChainVisitor()
  {}

  virtual bool PreOrderVisit( EPTypedObjNode<IAAFCompositionMob, EPTopLevelComposition>& node )
  {
    PreTrace(node, EPTopLevelComposition::GetName());
    AxCompositionMob axCompMob( node.GetAAFObjectOfType() );
    return this->VisitMobWithUsageCode( axCompMob, node.GetLID(), EPTopLevelComposition::GetName() );
  }
  
  virtual bool PreOrderVisit( EPTypedObjNode<IAAFCompositionMob, EPLowerLevelComposition>& node )
  {
    PreTrace(node, EPLowerLevelComposition::GetName());
    AxCompositionMob axCompMob( node.GetAAFObjectOfType() );
    return this->VisitMobWithUsageCode( axCompMob, node.GetLID(), EPLowerLevelComposition::GetName() );
  }
  
  virtual bool PreOrderVisit( EPTypedObjNode<IAAFCompositionMob, EPSubClipComposition>& node )
  {
    PreTrace(node,EPSubClipComposition::GetName());
    AxCompositionMob axCompMob( node.GetAAFObjectOfType() );
    return this->VisitMobWithUsageCode( axCompMob, node.GetLID(), EPSubClipComposition::GetName() );
  }
  
  virtual bool PreOrderVisit( EPTypedObjNode<IAAFCompositionMob, EPAdjustedClipComposition>& node )
  {
    PreTrace(node,  EPAdjustedClipComposition::GetName());
    AxCompositionMob axCompMob( node.GetAAFObjectOfType() );
    return this->VisitMobWithUsageCode( axCompMob, node.GetLID(), EPAdjustedClipComposition::GetName() );
  }
  
  virtual bool PreOrderVisit( EPTypedObjNode<IAAFMasterMob, EPTemplateClip>& node )
  {
    PreTrace(node, EPTemplateClip::GetName());
    AxMasterMob axMastMob( node.GetAAFObjectOfType() );
    return this->VisitMobWithUsageCode( axMastMob, node.GetLID(), EPTemplateClip::GetName() );
  }
  
  virtual bool PreOrderVisit( EPTypedObjNode<IAAFMasterMob, EPClip>& node )
  {
    PreTrace(node, EPClip::GetName() );
    AxMasterMob axMastMob( node.GetAAFObjectOfType() );
    
    AxString nodeName = this->GetMobName( axMastMob, EPClip::GetName() );
    
    AxString detailMsg;
    bool successfulTransition = false;
    vector<shared_ptr<const Requirement> > failingReqIds;

    successfulTransition = _stateMachine.Transition( kAAFClassID_MasterMob, nodeName, detailMsg, failingReqIds );
    _spTestResult->AddDetail( detailMsg );

    if ( !successfulTransition )
    {
      for ( unsigned int i = 0; i < failingReqIds.size(); i++ )
      {
        _spTestResult->AddSingleResult( failingReqIds[i]->GetId(),
                                        nodeName + L" is out of place in the derivation chain.",
                                        TestResult::FAIL );
      }

      return false;
    }
    
    return true;
  }

  virtual bool PreOrderVisit( EPTypedObjNode<IAAFSourceMob, EPRGBAImageFileSource>& node )
  {
    shared_ptr<EPTypedObjNode<IAAFSourceMob, EPFileSource> > spGeneric( node.DownCast<IAAFSourceMob, EPFileSource>() );
    return this->PreOrderVisit( *spGeneric );
  }
  
  virtual bool PreOrderVisit( EPTypedObjNode<IAAFSourceMob, EPCDCIImageFileSource>& node )
  {
    shared_ptr<EPTypedObjNode<IAAFSourceMob, EPFileSource> > spGeneric( node.DownCast<IAAFSourceMob, EPFileSource>() );
    return this->PreOrderVisit( *spGeneric );
  }
  
  virtual bool PreOrderVisit( EPTypedObjNode<IAAFSourceMob, EPImageFileSource>& node )
  {
    shared_ptr<EPTypedObjNode<IAAFSourceMob, EPFileSource> > spGeneric( node.DownCast<IAAFSourceMob, EPFileSource>() );
    return this->PreOrderVisit( *spGeneric );
  }
  
  virtual bool PreOrderVisit( EPTypedObjNode<IAAFSourceMob, EPMonoAudioFileSource>& node )
  {
    shared_ptr<EPTypedObjNode<IAAFSourceMob, EPFileSource> > spGeneric( node.DownCast<IAAFSourceMob, EPFileSource>() );
    return this->PreOrderVisit( *spGeneric );
  }
  
  virtual bool PreOrderVisit( EPTypedObjNode<IAAFSourceMob, EPMultiChannelAudioFileSource>& node )
  {
    shared_ptr<EPTypedObjNode<IAAFSourceMob, EPFileSource> > spGeneric( node.DownCast<IAAFSourceMob, EPFileSource>() );
    return this->PreOrderVisit( *spGeneric );
  }

  virtual bool PreOrderVisit( EPTypedObjNode<IAAFSourceMob, EPFileSource>& node )
  {
    PreTrace(node,EPFileSource::GetName());
    return this->VisitMobWithDescriptor( node, EPFileSource::GetName() );
  }
  
  virtual bool PreOrderVisit( EPTypedObjNode<IAAFSourceMob, EPRecordingSource>& node )
  {
    PreTrace(node, EPRecordingSource::GetName());
    return this->VisitMobWithDescriptor( node, EPRecordingSource::GetName() );
  }
  
  virtual bool PreOrderVisit( EPTypedObjNode<IAAFSourceMob, EPImportSource>& node )
  {
    PreTrace(node,EPImportSource::GetName());
    return this->VisitMobWithDescriptor( node, EPImportSource::GetName() );
  }
  
  virtual bool PreOrderVisit( EPTypedObjNode<IAAFSourceMob, EPTapeSource>& node )
  {
    PreTrace(node, EPTapeSource::GetName());
    return this->VisitMobWithDescriptor( node, EPTapeSource::GetName() );
  }
  
  virtual bool PreOrderVisit( EPTypedObjNode<IAAFSourceMob, EPFilmSource>& node )
  {
    PreTrace(node, EPFilmSource::GetName() );
    return this->VisitMobWithDescriptor( node, EPFilmSource::GetName() );
  }

  virtual bool PreOrderVisit( AAFTypedObjNode<IAAFSourceClip>& node )
  {
    PreTrace(node, L"source clip" );
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
    catch ( const AxExHResult& )
    {
      successfulTransition = false;
      detailMsg = L"Source Clip does not have a Source Reference property.";
    }
    
    _spTestResult->AddDetail( detailMsg );
    if ( !successfulTransition )
    {
      for ( unsigned int i = 0; i < failingReqIds.size(); i++ )
      {
        //If the Requirement is REQ_EP_019, then, it should present a warning,
        //not a failure.
        if ( failingReqIds.at( i ) == RequirementRegistry::GetInstance().GetRequirement( L"REQ_EP_019" ) )
        {
          // JPT REVIEW - More info required. What mob? Name? MobID?
          _spTestResult->AddSingleResult( L"REQ_EP_019",
                                          L"Out of file mob reference.",
                                          TestResult::WARN );
        }
        else
        {
          // JPT REVIEW - No explanation provided.
          _spTestResult->AddSingleResult( failingReqIds[i]->GetId(),
                                          L"",
                                          TestResult::FAIL );
        }
      }
    
#if 0
      // JPT REVIEW - Must review this. It doesn't fit well with the
      // "single result" approach.

      //TODO: This should not be necessary.  There needs to be a requirement
      //      that fails on an illegal End Of Chain.  Until something fails,
      //      this code must be here to ensure a failure is reported.
      if ( _spResult->GetResult() == TestResult::PASS )
      {
          _spResult->SetResult( TestResult::FAIL );
      }
      
      _spResult->SetExplanation( L"Source Clip is out of place in the derrivation chain." );
#endif

      return false;
    }

    return true;
  }

  void REQ_EP_258_Failure( const wstring& mobTypeName,
                          AxMob& mob )
  {
    wstring explain = mobTypeName + L" named \"" + mob.GetName(L"<unnamed>")
                    + L"\" is out of place in the derrivation chain.";    

    shared_ptr<DetailLevelTestResult>
      spFailure = _spTestResult->AddSingleResult( L"REQ_EP_258",
                                                  explain,
                                                  TestResult::FAIL );
    spFailure->AddDetail( L"Does not have an Edit Protocol material type." );
  }

  virtual bool PreOrderVisit( AAFTypedObjNode<IAAFCompositionMob>& node )
  {
    PreTrace(node, L"composition");
    AxCompositionMob axCompMob( node.GetAAFObjectOfType() );
    AxString nodeName = this->GetMobName( axCompMob, L"Composition Mob" ); 
    REQ_EP_258_Failure( L"Composition Mob", axCompMob );
    return false;
  }
  
  virtual bool PreOrderVisit( AAFTypedObjNode<IAAFMasterMob>& node )
  {
    PreTrace(node, L"master" );
    AxMasterMob axMastMob( node.GetAAFObjectOfType() );
    REQ_EP_258_Failure( L"Master Mob", axMastMob );
    return false;
  }

  virtual bool PreOrderVisit( AAFTypedObjNode<IAAFSourceMob>& node )
  {
    PreTrace(node, L"source" );
    AxSourceMob axSrcMob( node.GetAAFObjectOfType() );
    REQ_EP_258_Failure( L"Source Mob", axSrcMob );
    return false;
  }
  
  virtual bool PostOrderVisit( AAFTypedObjNode<IAAFCompositionMob>& node )
  {
    PostTrace(node);
    AxCompositionMob axCompMob( node.GetAAFObjectOfType() );
    return this->PostVisitMob( axCompMob, L"Composition Mob");
  }
  
  virtual bool PostOrderVisit( AAFTypedObjNode<IAAFMasterMob>& node )
  {
    PostTrace(node);
    AxMasterMob axMastMob( node.GetAAFObjectOfType() );
    return this->PostVisitMob( axMastMob, L"Master Mob");
  }
  
  virtual bool PostOrderVisit( AAFTypedObjNode<IAAFSourceMob>& node )
  {
    PostTrace(node);
    AxSourceMob axSrcMob( node.GetAAFObjectOfType() );
    return this->PostVisitMob( axSrcMob, L"Source Mob");
  }

  virtual bool PostOrderVisit( AAFTypedObjNode<IAAFSourceClip>& node )
  {
    PostTrace(node);
    AxSourceClip axSrcClip( node.GetAAFObjectOfType() );

    aafMobID_t srcID = axSrcClip.GetSourceReference().sourceID;
    shared_ptr<Node> spNode;
    spNode = MobNodeMap::GetInstance().GetMobNode(srcID);
    
    // Only pop the current state of the stack if this is an End of
    // Chain or Out of File source reference.
    if ( srcID == AxConstants::NULL_MOBID || !spNode )
    {
      AxString detailMsg;
      bool successfulTransition = _stateMachine.TransitionBack(detailMsg);

      // JPT REVIEW - Is there any case where this would fail and that
      // is an error?  e.g. what if a zero, or out of file, reference
      // is found inside a composition?
      assert( successfulTransition );

      if ( !detailMsg.empty() )
      {
        _spTestResult->AddDetail( detailMsg );
      }
    }
    
    return true;
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
    _spTestResult->AddDetail( detailMsg );
    if ( !successfulTransition )
    {
      for ( unsigned int i = 0; i < failingReqIds.size(); i++ )
      {
        _spTestResult->AddSingleResult( failingReqIds[i]->GetId(),
                                        nodeName + L" is out of place in the derrivation chain.",
                                        TestResult::FAIL );
      }
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
    _spTestResult->AddDetail( detailMsg );
    if ( !successfulTransition )
    {
      for ( unsigned int i = 0; i < failingReqIds.size(); i++ )
      {
        _spTestResult->AddSingleResult( failingReqIds[i]->GetId(),
                                        nodeName + L" is out of place in the derrivation chain.",
                                        TestResult::FAIL );
      }
      return false;
    }

    return true;
  }

  bool PostVisitMob( AxMob& axMob, const AxString& type )
  {
    AxString detailMsg;
    bool successfulTransition = _stateMachine.TransitionBack( detailMsg );
    if ( !detailMsg.empty() )
    {
      _spTestResult->AddDetail( detailMsg );
    }

    AxString nodeName = this->GetMobName( axMob, type );

    if ( !successfulTransition )
    {
      _spTestResult->AddSingleResult( L"REQ_EP_017",
                                      L"End of derivation chain encountered without a zero-valued "
                                      L"source clip or out of file reference at " + nodeName,
                                      TestResult::FAIL );
    }
    return successfulTransition;
  }

  void PreTrace( const AAFObjNode& node, const wstring& what )
  {
    _level++;

#if DEBUG_OUT
    wcout << L"Pre:  " << Indent(_level) << node.GetLID() << L" " << node.GetName();

    IAAFMobSP spMob;
    if ( AxIsA<IAAFMob>( AxObject(node.GetAAFObject()), spMob ) )
    {
      AxMob axMob(spMob);
      //wcout << L", Name = " << axMob.GetName() << L", " << AxStringUtil::mobid2Str(axMob.GetMobID());
      wcout << L", Name = " << axMob.GetName();
    }
    wcout << L", " << what << L" (" << _level << L")" << endl;
#endif
  }

  void PostTrace( const AAFObjNode& node )
  {
#if DEBUG_OUT
    wcout << L"Post: " << Indent(_level) << node.GetLID() << L" " << node.GetName()
          << L" (" << _level << L")" << endl;
#endif
    _level--;
  }

  wostream& _log;
  shared_ptr<TestLevelTestResult> _spTestResult;
  DerivationChainStateMachine _stateMachine;

  unsigned int _level;
};

//======================================================================

void AnalyzeMobChain( wostream& log,
                      shared_ptr<const TestGraph> spGraph,
                      CompMobDependency::CompMobNodeSP spRootComposition,
                      shared_ptr<TestLevelTestResult> spTestResult )
{
  AxCompositionMob axCompMob( spRootComposition->GetAAFObjectOfType() );
  AxString mobName = axCompMob.GetName( L"<unnamed>" );

  spTestResult->AddDetail( L"Analyzing root composition mob \"" + mobName + L"\"" );

  DepthFirstTraversal dft( spGraph->GetEdgeMap(),
                           spRootComposition );

  shared_ptr<MobChainVisitor>  spVisitor( new MobChainVisitor(log, spTestResult) );
  dft.TraverseDown( spVisitor );
}

//======================================================================

class Analyzer
{
public:
  Analyzer( wostream& log,
            shared_ptr<const TestGraph> spGraph,
            shared_ptr<TestLevelTestResult> spTestResult )
    : _log( log ),
      _spGraph( spGraph ),
      _spTestResult( spTestResult )
  {}

  void operator () ( const CompMobDependency::CompMobNodeSP& spRootComposition )
  {
    AnalyzeMobChain( _log, _spGraph, spRootComposition, _spTestResult );
  }

private:
  wostream& _log;
  shared_ptr<const TestGraph> _spGraph;
  shared_ptr<TestLevelTestResult> _spTestResult;
  
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
  shared_ptr<TestLevelTestResult> spTestResult = CreateTestResult();

  Analyzer analyzer( GetOutStream(), GetTestGraph(), spTestResult );

  for_each( _spTopLevelCompMobs->begin(), _spTopLevelCompMobs->end(), analyzer );

  return spTestResult;
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

    spReqIds->push_back(L"REQ_EP_017");     // Derivation Chain ends with zero-value source clip.
    spReqIds->push_back(L"REQ_EP_018");     // Reference to Master or Composition mob cannot be out of file.
    spReqIds->push_back(L"REQ_EP_019");     // Reference to Source mob should not be out of file.
    spReqIds->push_back(L"REQ_EP_025");     // Only valid transition to Top-Level is from Initial.
    spReqIds->push_back(L"REQ_EP_026");     // Valid transitions from Top-Level.
    spReqIds->push_back(L"REQ_EP_030");     // Only Top-Level and Lower-Level may reference Lower-Level.
    spReqIds->push_back(L"REQ_EP_031");     // Valid transitions from Lower-Level ~ The specification does not
                                            // allow Lower-Level to Lower-Level but the test does.
    spReqIds->push_back(L"REQ_EP_036");     // Valid transitions from Sub-Clip.
    spReqIds->push_back(L"REQ_EP_045");     // Valid transitions from Adjusted Clip.
    spReqIds->push_back(L"REQ_EP_050");     // Template Clip must end chain.
    spReqIds->push_back(L"REQ_EP_055");     // Valid transitions from Clip.
    spReqIds->push_back(L"REQ_EP_063");     // Valid transitions from File Source ~ transitions are not actually
                                            // specified, also, a transition to another File Source is assumed valid.
    spReqIds->push_back(L"REQ_EP_064");     // Transition from File Source to: Recording Source is valid, End of Chain
                                            // is invalid ~ with 063 implies if a File Source reference another
                                            // File Source a Recording Source is required, this is not implemented.
    spReqIds->push_back(L"REQ_EP_072");     // Recording source must end chain.
    spReqIds->push_back(L"REQ_EP_076");     // Valid Transitions from Import Source ~ not actually specified.
    spReqIds->push_back(L"REQ_EP_083");     // Valid Transitions from Tape Source ~ not actually specified.

    // JPT REVIEW - I added these two requirements to cover two cases
    // where failures results were being set on a result without a
    // clear requirement. They should be reviewed.
    spReqIds->push_back(L"REQ_EP_258");      // All Mob objects shall have a have a known Edit Protocol material type.

    //TODO: Push an item for valid transitions from Film Source.
    return TestInfo(L"EPDerivationTest", spReqIds);
}

} // end of namespace aafanalyzer
