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
#include "DecorateEPTest.h"

//Test/Result files
#include <TestLevelTestResult.h>
#include <DetailLevelTestResult.h>
#include <TestRegistry.h>

//Edit Protocol Analyzer Base files
#include <EPTypedObjNode.h>
#include <EPDerivationChainObject.h>
#include <EPTrack.h>
#include <EPEffect.h>

//AAF Analyzer Base files
#include <MobNodeMap.h>
#include <TypedVisitor.h>
#include <TestGraph.h>

//Analyzer Base files
#include <DepthFirstTraversal.h>

//Ax files
#include <AxMob.h>
#include <AxMetaDef.h>
#include <AxEssence.h>
#include <AxMobSlot.h>
#include <AxDefObject.h>

//AAF files
#include <AAFResult.h>
#include <AAFExtEnum.h>
#include <AAFClassDefUIDs.h>
#include <AAFOperationDefs.h>

//STL files
#include <set>

namespace {

using namespace aafanalyzer;
using namespace boost;

const wchar_t* TEST_NAME = L"Edit Protocol Decoration";
const wchar_t* TEST_DESC = L"Decorate the nodes of the graph with their edit protocol material types.";

//======================================================================

// EPDecoratorVisitor visits all mob types and decorates the nodes with the
// appropriate edit protocol material type.

class EPDecoratorVisitor : public TypedVisitor
{
public:
  EPDecoratorVisitor( wostream& log, shared_ptr<const TestGraph> spTestGraph )
    : TypedVisitor(),
      _log( log ),
      _spResult( new DetailLevelTestResult( L"EPDecoratorVisitor",
                                            L"Visit mobs and decorate them with their derivation chain material types.",
                                            L"", // explain
                                            L"", // DOCREF REQUIRED
                                            TestResult::PASS,
                                            TestRegistry::GetInstance().GetRequirementsForTest( DecorateEPTest::GetTestInfo().GetName() )
               )                          ),
      _spGraph( spTestGraph )
  {
    _knownDescriptors.insert( kAAFClassID_FileDescriptor );
    _knownDescriptors.insert( kAAFClassID_RecordingDescriptor );
    _knownDescriptors.insert( kAAFClassID_ImportDescriptor );
    _knownDescriptors.insert( kAAFClassID_TapeDescriptor );
    _knownDescriptors.insert( kAAFClassID_FilmDescriptor );
    _knownDescriptors.insert( kAAFClassID_AuxiliaryDescriptor );
    _knownDescriptors.insert( kAAFClassID_EssenceDescriptor );    //Used to indicate failure.
  }

  virtual ~EPDecoratorVisitor()
  {}
 
  virtual bool PreOrderVisit( AAFTypedObjNode<IAAFCompositionMob>& node )
  {
    
    AxCompositionMob axCompMob( node.GetAAFObjectOfType() );

    try
    {
      //Find the usage code to find the proper type of decoration for the node.
      aafUID_t usageCode = axCompMob.GetUsageCode();
      if ( usageCode == kAAFUsage_TopLevel )
      {
          this->DecorateNode<IAAFCompositionMob, EPTopLevelComposition>( node );
      }
      else if ( usageCode == kAAFUsage_LowerLevel )
      {
          this->DecorateNode<IAAFCompositionMob, EPLowerLevelComposition>( node );
      }
      else if ( usageCode == kAAFUsage_SubClip )
      {
          this->DecorateNode<IAAFCompositionMob, EPSubClipComposition>( node );
      }
      else if ( usageCode == kAAFUsage_AdjustedClip )
      {
          this->DecorateNode<IAAFCompositionMob, EPAdjustedClipComposition>( node );
      }
      //There are no other valid composition mob/usage code combinations.  That
      //means that the material type for the derivation chain is unknown,
      //however, the edit protocol does not state that all mobs must be in the
      //derivation chain.
    }
    catch ( const AxExHResult& ex )
    {
      //If the exception is that the property is not present, this means that
      //the meterial type for the derivation chain is unknown, however, the
      //edit protocol does not state that all mobs must be in the derivation
      //chain, so, continue without decorating the object.
      if ( ex.getHResult() != AAFRESULT_PROP_NOT_PRESENT )
      {
        throw ex;
      }
    }
    
    return true;

  }

  virtual bool PreOrderVisit( AAFTypedObjNode<IAAFMasterMob>& node )
  {
    
    AxMasterMob axMastMob( node.GetAAFObjectOfType() );

    try
    {
      //If this is a template clip, decorate it.
      aafUID_t usageCode = axMastMob.GetUsageCode();

      if ( usageCode == kAAFUsage_Template )
      {
        this->DecorateNode<IAAFMasterMob, EPTemplateClip>( node );
      }
      //There are no other valid composition mob/usage code combinations.  That
      //means that the material type for the derivation chain is unknown,
      //however, the edit protocol does not state that all mobs must be in the
      //derivation chain.
    }
    catch ( const AxExHResult& ex )
    {
      //If this is a clip, then decorate it.
      if ( ex.getHResult() == AAFRESULT_PROP_NOT_PRESENT )
      {
        this->DecorateNode<IAAFMasterMob, EPClip>( node );
      }
      else
      {
        throw ex;
      }
    }
    
    return true;

  }

  virtual bool PreOrderVisit( AAFTypedObjNode<IAAFSourceMob>& node )
  {
    
    AxSourceMob axSrcMob( node.GetAAFObjectOfType() );

    try
    {
        
      /*
       * 
       * Note: A visitor could be used to properly classify Source Mobs into
       *       their correct Edit Protocol material types.  This would require
       *       implementing a PreOrderVisit for every accepted descriptor and
       *       every known descriptor that is derived from an accepted
       *       descriptor.  Also, a mechanism would be needed to pass the
       *       Edit Protocol type from the visitor to this function.  Unless a
       *       good reason presents itself to use the above mentioned method,
       *       the current method will be used.
       * 
       */
        
      //Find the type of descriptor on the node to properly decorate it.
      AxEssenceDescriptor descriptor( axSrcMob.GetEssenceDescriptor() );
      AxClassDef clsDef( descriptor.GetDefinition() );
      aafUID_t descriptorAUID = GetAcceptedAUID( clsDef );

      if ( descriptorAUID == kAAFClassID_FileDescriptor )
      {
        this->DecorateNode<IAAFSourceMob, EPFileSource>( node );
      }
      else if ( descriptorAUID == kAAFClassID_RecordingDescriptor )
      {
        this->DecorateNode<IAAFSourceMob, EPRecordingSource>( node );
      }
      else if ( descriptorAUID == kAAFClassID_ImportDescriptor )
      {
        this->DecorateNode<IAAFSourceMob, EPImportSource>( node );
      }
      else if ( descriptorAUID == kAAFClassID_TapeDescriptor )
      {
        this->DecorateNode<IAAFSourceMob, EPTapeSource>( node );
      }
      else if ( descriptorAUID == kAAFClassID_FilmDescriptor )
      {
        this->DecorateNode<IAAFSourceMob, EPFilmSource>( node );
      }
      else if ( descriptorAUID == kAAFClassID_AuxiliaryDescriptor )
      {
        this->DecorateNode<IAAFSourceMob, EPAuxiliarySource>( node );
      }
      //There are no other valid source mob/descriptor code combinations.  That
      //means that the material type for the derivation chain is unknown,
      //however, the edit protocol does not state that all mobs must be in the
      //derivation chain.

    }
    catch ( const AxExHResult& ex )
    {
      //If the exception is that the essence descriptor is not present, this
      //means that the meterial type for the derivation chain is unknown, 
      //however, the edit protocol does not state that all mobs must be in the 
      //derivation chain, so, continue without decorating the object.
      if ( ex.getHResult() != AAFRESULT_PROP_NOT_PRESENT )
      {
        throw ex;
      }
    }
    
    return true;

  }

  virtual bool PreOrderVisit( AAFTypedObjNode<IAAFTimelineMobSlot>& node )
  {
    
    AxTimelineMobSlot axMobSlot( node.GetAAFObjectOfType() );
    DecorateMobSlot<IAAFTimelineMobSlot>( axMobSlot, node );
    return true;
    
  }
  
  virtual bool PreOrderVisit( AAFTypedObjNode<IAAFStaticMobSlot>& node )
  {
    
    AxStaticMobSlot axMobSlot( node.GetAAFObjectOfType() );
    DecorateMobSlot<IAAFStaticMobSlot>( axMobSlot, node );
    return true;
    
  }
  
  virtual bool PreOrderVisit( AAFTypedObjNode<IAAFEventMobSlot>& node )
  {
    
    AxEventMobSlot axMobSlot( node.GetAAFObjectOfType() );
    DecorateMobSlot<IAAFEventMobSlot>( axMobSlot, node );
    return true;
    
  }
  
  virtual bool PreOrderVisit( AAFTypedObjNode<IAAFMobSlot>& node )
  {
    
    AxMobSlot axMobSlot( node.GetAAFObjectOfType() );
    DecorateMobSlot<IAAFMobSlot>( axMobSlot, node );    
    return true;
    
  }
  
  virtual bool PreOrderVisit( AAFTypedObjNode<IAAFOperationGroup>& node )
  {
    AxOperationGroup axOpGroup( node.GetAAFObjectOfType() );
    AxOperationDef axOpDef( axOpGroup.GetOperationDef() );
    aafUID_t opDef = axOpDef.GetAUID();
    
    if ( opDef == kAAFOperationDef_VideoDissolve )
    {
        this->DecorateNode<IAAFOperationGroup, EPVideoDissolveEffect>( node );
    }
    else if ( opDef == kAAFOperationDef_SMPTEVideoWipe )
    {
        this->DecorateNode<IAAFOperationGroup, EPSMPTEVideoWipeEffect>( node );
    }
    else if ( opDef == kAAFOperationDef_VideoSpeedControl )
    {
        this->DecorateNode<IAAFOperationGroup, EPVideoSpeedControlEffect>( node );
    }
    else if ( opDef == kAAFOperationDef_VideoRepeat )
    {
        this->DecorateNode<IAAFOperationGroup, EPVideoRepeatEffect>( node );
    }
    else if ( opDef == kAAFOperationDef_Flip )
    {
        this->DecorateNode<IAAFOperationGroup, EPVideoFlipEffect>( node );
    }
    else if ( opDef == kAAFOperationDef_Flop )
    {
        this->DecorateNode<IAAFOperationGroup, EPVideoFlopEffect>( node );
    }
    else if ( opDef == kAAFOperationDef_FlipFlop )
    {
        this->DecorateNode<IAAFOperationGroup, EPVideoFlipFlopEffect>( node );
    }
    else if ( opDef == kAAFOperationDef_VideoPosition )
    {
        this->DecorateNode<IAAFOperationGroup, EPVideoPositionEffect>( node );
    }
    else if ( opDef == kAAFOperationDef_VideoCrop )
    {
        this->DecorateNode<IAAFOperationGroup, EPVideoCropEffect>( node );
    }
    else if ( opDef == kAAFOperationDef_VideoScale )
    {
        this->DecorateNode<IAAFOperationGroup, EPVideoScaleEffect>( node );
    }
    else if ( opDef == kAAFOperationDef_VideoRotate )
    {
        this->DecorateNode<IAAFOperationGroup, EPVideoRotateEffect>( node );
    }
    else if ( opDef == kAAFOperationDef_VideoCornerPinning )
    {
        this->DecorateNode<IAAFOperationGroup, EPVideoCornerPinningEffect>( node );
    }
    else if ( opDef == kAAFOperationDef_VideoAlphaWithinVideoKey )
    {
        this->DecorateNode<IAAFOperationGroup, EPAlphaWithVideoKeyEffect>( node );
    }
    else if ( opDef == kAAFOperationDef_VideoSeparateAlphaKey )
    {
        this->DecorateNode<IAAFOperationGroup, EPSeparateAlphaKeyEffect>( node );
    }
    else if ( opDef == kAAFOperationDef_VideoLuminanceKey )
    {
        this->DecorateNode<IAAFOperationGroup, EPLuminanceKeyEffect>( node );
    }
    else if ( opDef == kAAFOperationDef_VideoChromaKey )
    {
        this->DecorateNode<IAAFOperationGroup, EPChromaKeyEffect>( node );
    }
    else if ( opDef == kAAFOperationDef_MonoAudioGain )
    {
        this->DecorateNode<IAAFOperationGroup, EPMonoAudioGainEffect>( node );
    }
    else if ( opDef == kAAFOperationDef_MonoAudioPan )
    {
        this->DecorateNode<IAAFOperationGroup, EPMonoAudioPanEffect>( node );
    }
    else if ( opDef == kAAFOperationDef_MonoAudioDissolve )
    {
        this->DecorateNode<IAAFOperationGroup, EPSingleParameterAudioDissolveEffect>( node );
    }
    else if ( opDef == kAAFOperationDef_TwoParameterMonoAudioDissolve )
    {
        this->DecorateNode<IAAFOperationGroup, EPTwoParameterAudioDissolveEffect>( node );
    }
    else
    {
        this->DecorateNode<IAAFOperationGroup, EPEffect>( node );
    }
    
    return true;
  }
  
  shared_ptr<DetailLevelTestResult> GetResult()
  {
    return _spResult;
  }
  
private:

  // prohibited
  EPDecoratorVisitor();
  EPDecoratorVisitor( const EPDecoratorVisitor& );
  EPDecoratorVisitor& operator=( const EPDecoratorVisitor& );

  //Return a AUID that matches an acceptable descriptor type or the base
  //type (kAAFClassID_EssenceDescriptor).
  aafUID_t GetAcceptedAUID( AxClassDef& clsDef ) const
  {
    aafUID_t auid = clsDef.GetAUID();
       
    if ( _knownDescriptors.find(auid) != _knownDescriptors.end() )
    {
        return auid;
    }

    AxClassDef parentDef( clsDef.GetParent() );
    return GetAcceptedAUID( parentDef );
    
  }
  
  template <typename AAFObjectType>
  void DecorateMobSlot ( AxMobSlot& axMobSlot, AAFTypedObjNode<AAFObjectType>& node )
  {

    AxDataDef axDataDef( axMobSlot.GetDataDef() );
    
    //Check if this is an essence track.
    if ( axDataDef.IsPictureKind() || axDataDef.IsSoundKind() )
    {
        
      shared_ptr<EPAudioTrack> spAudio( EPAudioTrack::CreateAudioTrack( axMobSlot ) );
      shared_ptr<EPVideoTrack> spVideo( EPVideoTrack::CreateVideoTrack( axMobSlot ) );
                
      //Now check if it is an audio or video track.
      if ( spAudio )
      {
        this->DecorateNode<AAFObjectType, EPAudioTrack>( node, spAudio );
      }
      else if ( spVideo )
      {
        this->DecorateNode<AAFObjectType, EPVideoTrack>( node, spVideo );
      }
      else
      {
        this->DecorateNode<AAFObjectType, EPEssenceTrack>( node );
      }
    }
    else
    {
      if ( axDataDef.IsTimecodeKind() )
      {
        this->DecorateNode<AAFObjectType, EPTimecodeTrack>( node );
      }
      else if ( axDataDef.IsEdgecodeKind() )
      {
        this->DecorateNode<AAFObjectType, EPEdgecodeTrack>( node );
      }
      else
      {
        this->DecorateNode<AAFObjectType, EPNonEssenceTrack>( node );
      }
    }
    
  }
  
  template <typename AAFObjectType, typename EPObjectType>
  void DecorateNode( AAFTypedObjNode<AAFObjectType>& node )
  {
    //Get a shared pointer to the node.
    shared_ptr<AAFTypedObjNode<AAFObjectType> > spNode =
        dynamic_pointer_cast<AAFTypedObjNode<AAFObjectType> >( 
            node.GetSharedPointerToNode() );

    //Create the EPObject
    shared_ptr<EPObjectType> spEPObj( new EPObjectType() );

    //Create a decorated node.
    shared_ptr<EPTypedObjNode<AAFObjectType, EPObjectType> > 
        spDecorated( new EPTypedObjNode<AAFObjectType, EPObjectType>( spNode, spEPObj ) );
        
    //Decorate the node in the graph.
    _spGraph->Decorate( spDecorated );   
  }
  
  template <typename AAFObjectType, typename EPObjectType>
  void DecorateNode( AAFTypedObjNode<AAFObjectType>& node, shared_ptr<EPObjectType> spEPObj )
  {
    //Get a shared pointer to the node.
    shared_ptr<AAFTypedObjNode<AAFObjectType> > spNode =
        dynamic_pointer_cast<AAFTypedObjNode<AAFObjectType> >( 
            node.GetSharedPointerToNode() );

    //Create a decorated node.
    shared_ptr<EPTypedObjNode<AAFObjectType, EPObjectType> > 
        spDecorated( new EPTypedObjNode<AAFObjectType, EPObjectType>( spNode, spEPObj ) );
        
    //Decorate the node in the graph.
    _spGraph->Decorate( spDecorated );   
  }

  wostream& _log;
  shared_ptr<DetailLevelTestResult> _spResult;
  shared_ptr<const TestGraph> _spGraph;
  set<aafUID_t> _knownDescriptors;
  
};

//======================================================================

} // end of namespace

//======================================================================

namespace aafanalyzer {

using namespace std;

DecorateEPTest::DecorateEPTest( wostream& log,
				    shared_ptr<const TestGraph> spGraph )
  : Test( log, GetTestInfo() )
{
    SetTestGraph( spGraph );
}

DecorateEPTest::~DecorateEPTest()
{}

shared_ptr<TestLevelTestResult> DecorateEPTest::Execute()
{
  shared_ptr<EPDecoratorVisitor> spVisitor(
       new EPDecoratorVisitor( GetOutStream(), GetTestGraph() ) );

  DepthFirstTraversal dfs(GetTestGraph()->GetEdgeMap(), GetTestGraph()->GetRootNode());

  //set result properties
  const shared_ptr<const Test> me = this->shared_from_this();
  Requirement::RequirementMapSP spMyReqs( new Requirement::RequirementMap( this->GetCoveredRequirements() ) );
  shared_ptr<TestLevelTestResult> spResult( new TestLevelTestResult( me, spMyReqs ) );
  spResult->SetName( GetName() );
  spResult->SetDescription( GetDescription() );

  dfs.TraverseDown( spVisitor, GetTestGraph()->GetRootNode() );  
  spResult->AppendSubtestResult( spVisitor->GetResult() );
  spResult->SetResult( spResult->GetAggregateResult() );

  return spResult;
}

AxString DecorateEPTest::GetName() const
{
  return TEST_NAME;
}

AxString DecorateEPTest::GetDescription() const
{
  return TEST_DESC;
}

const TestInfo DecorateEPTest::GetTestInfo()
{
    shared_ptr<vector<AxString> > spReqIds(new vector<AxString>);
    //TODO: Push any requirements that exist (AK - I don't think there are any, this can't fail).
    return TestInfo(L"DecorateEPTest", spReqIds);
}

} // end of namespace aafanalyzer
