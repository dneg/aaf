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
#include <EPObjects.h>

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

//AAF files
#include <AAFResult.h>
#include <AAFExtEnum.h>
#include <AAFClassDefUIDs.h>

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
//
// About the EdgeVisit methods: To visit all mobs in the derivation
// chain we must follow containment edges, and mob reference edges.
// The base class follows these by default so we do nothing here. We do
// not, however, want to follow slot references or component
// references. Doing so would be useless because we'd miss the
// referenced mob object, further, it would be redundant if we are
// also folloing the mob reference.


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
    _knownDescriptors.insert( kAAFClassID_EssenceDescriptor );    //Used to indicate failure.
  }

  virtual ~EPDecoratorVisitor()
  {}
 
  virtual bool PreOrderVisit( AAFTypedObjNode<IAAFCompositionMob>& node )
  {
    
    //If the node has already been decorated by this test, don't bother
    //decorating it again.
    if ( this->IsDecorated( node.GetLID() ) )
    {
      return false;
    }

    AxCompositionMob axCompMob( node.GetAAFObjectOfType() );

    try
    {
      //Find the usage code to find the proper type of decoration for the node.
      aafUID_t usageCode = axCompMob.GetUsageCode();
      shared_ptr<AAFTypedObjNode<IAAFCompositionMob> > spNode =
        dynamic_pointer_cast<AAFTypedObjNode<IAAFCompositionMob> >( 
          node.GetSharedPointerToNode() );
      if ( usageCode == kAAFUsage_TopLevel )
      {
        shared_ptr<EPTypedObjNode<IAAFCompositionMob, EPTopLevelComposition> > 
            spDecorated( new EPTypedObjNode<IAAFCompositionMob, EPTopLevelComposition>( spNode ) );
        _spGraph->Decorate( spDecorated );
      }
      else if ( usageCode == kAAFUsage_LowerLevel )
      {
        shared_ptr<EPTypedObjNode<IAAFCompositionMob, EPLowerLevelComposition> > 
            spDecorated( new EPTypedObjNode<IAAFCompositionMob, EPLowerLevelComposition>( spNode ) );
        _spGraph->Decorate( spDecorated );
      }
      else if ( usageCode == kAAFUsage_SubClip )
      {
        shared_ptr<EPTypedObjNode<IAAFCompositionMob, EPSubClipComposition> > 
            spDecorated( new EPTypedObjNode<IAAFCompositionMob, EPSubClipComposition>( spNode ) );
        _spGraph->Decorate( spDecorated );
      }
      else if ( usageCode == kAAFUsage_AdjustedClip )
      {
        shared_ptr<EPTypedObjNode<IAAFCompositionMob, EPAdjustedClipComposition> > 
            spDecorated( new EPTypedObjNode<IAAFCompositionMob, EPAdjustedClipComposition>( spNode ) );
        _spGraph->Decorate( spDecorated );
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
    
    _decoratedNodes.insert( node.GetLID() );
    
    return true;

  }

  virtual bool PreOrderVisit( AAFTypedObjNode<IAAFMasterMob>& node )
  {
    
    //If the node has already been decorated by this test, don't bother
    //decorating it again.
    if ( this->IsDecorated( node.GetLID() ) )
    {
      return false;
    }
    AxMasterMob axMastMob( node.GetAAFObjectOfType() );

    try
    {
      //If this is a template clip, decorate it.
      aafUID_t usageCode = axMastMob.GetUsageCode();

      if ( usageCode == kAAFUsage_Template )
      {
        shared_ptr<AAFTypedObjNode<IAAFMasterMob> > spNode =
          dynamic_pointer_cast<AAFTypedObjNode<IAAFMasterMob> >( 
            node.GetSharedPointerToNode() );
        shared_ptr<EPTypedObjNode<IAAFMasterMob, EPTemplateClip> > 
            spDecorated( new EPTypedObjNode<IAAFMasterMob, EPTemplateClip>( spNode ) );
        _spGraph->Decorate( spDecorated );
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
        shared_ptr<AAFTypedObjNode<IAAFMasterMob> > spNode =
          dynamic_pointer_cast<AAFTypedObjNode<IAAFMasterMob> >( 
            node.GetSharedPointerToNode() );
        shared_ptr<EPTypedObjNode<IAAFMasterMob, EPClip> > 
            spDecorated( new EPTypedObjNode<IAAFMasterMob, EPClip>( spNode ) );
        _spGraph->Decorate( spDecorated );
      }
      else
      {
        throw ex;
      }
    }
    
    _decoratedNodes.insert( node.GetLID() );
    
    return true;

  }

  virtual bool PreOrderVisit( AAFTypedObjNode<IAAFSourceMob>& node )
  {
    
    //If the node has already been decorated by this test, don't bother
    //decorating it again.
    if ( this->IsDecorated( node.GetLID() ) )
    {
      return false;
    }
    AxSourceMob axSrcMob( node.GetAAFObjectOfType() );

    try
    {
      //Find the type of descriptor on the node to properly decorate it.
      AxEssenceDescriptor descriptor( axSrcMob.GetEssenceDescriptor() );
      AxClassDef clsDef( descriptor.GetDefinition() );
      aafUID_t descriptorAUID = GetAcceptedAUID( clsDef );
      shared_ptr<AAFTypedObjNode<IAAFSourceMob> > spNode =
        dynamic_pointer_cast<AAFTypedObjNode<IAAFSourceMob> >( 
            node.GetSharedPointerToNode() );

      if ( descriptorAUID == kAAFClassID_FileDescriptor )
      {
        shared_ptr<EPTypedObjNode<IAAFSourceMob, EPFileSource> > 
            spDecorated( new EPTypedObjNode<IAAFSourceMob, EPFileSource>( spNode ) );
        _spGraph->Decorate( spDecorated );
      }
      else if ( descriptorAUID == kAAFClassID_RecordingDescriptor )
      {
        shared_ptr<EPTypedObjNode<IAAFSourceMob, EPRecordingSource> > 
            spDecorated( new EPTypedObjNode<IAAFSourceMob, EPRecordingSource>( spNode ) );
        _spGraph->Decorate( spDecorated );
      }
      else if ( descriptorAUID == kAAFClassID_ImportDescriptor )
      {
        shared_ptr<EPTypedObjNode<IAAFSourceMob, EPImportSource> > 
            spDecorated( new EPTypedObjNode<IAAFSourceMob, EPImportSource>( spNode ) );
        _spGraph->Decorate( spDecorated );
      }
      else if ( descriptorAUID == kAAFClassID_TapeDescriptor )
      {
        shared_ptr<EPTypedObjNode<IAAFSourceMob, EPTapeSource> > 
            spDecorated( new EPTypedObjNode<IAAFSourceMob, EPTapeSource>( spNode ) );
        _spGraph->Decorate( spDecorated );
      }
      else if ( descriptorAUID == kAAFClassID_FilmDescriptor )
      {
        shared_ptr<EPTypedObjNode<IAAFSourceMob, EPFilmSource> > 
            spDecorated( new EPTypedObjNode<IAAFSourceMob, EPFilmSource>( spNode ) );
        _spGraph->Decorate( spDecorated );
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
    
    _decoratedNodes.insert( node.GetLID() );
    
    return true;

  }
 
  virtual bool EdgeVisit(AAFComponentReference& edge)
  {
    return false;
  }

  virtual bool EdgeVisit(AAFSlotReference& edge)
  {
    return false;
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
  
  bool IsDecorated( Node::LID id )
  {
    return ( _decoratedNodes.find( id ) != _decoratedNodes.end() );
  }

  wostream& _log;
  shared_ptr<DetailLevelTestResult> _spResult;
  shared_ptr<const TestGraph> _spGraph;
  set<aafUID_t> _knownDescriptors;
  set<Node::LID> _decoratedNodes;
  
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
