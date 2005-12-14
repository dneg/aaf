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

//Base Test files
#include "ResolveRefVisitor.h"
#include "RefResolver.h"

//Test/Result files
#include <DetailLevelTestResult.h>
#include <TestRegistry.h>

//Requirement files
#include <Requirement.h>

//AAF Analyzer Base files
#include <AAFMobReference.h>
#include <AAFSlotReference.h>
#include <AAFComponentReference.h>
#include <MobNodeMap.h>

//Analyzer Base files
#include <EdgeMap.h>
#include <DepthFirstTraversal.h>

//Ax files
#include <AxMobSlot.h>
#include <AxComponent.h>
#include <AxUtil.h>

//AAF files
#include <AAFResult.h>
#include <AAFClassDefUIDs.h>

namespace {

using namespace aafanalyzer;
using namespace boost;
using namespace std;

class ResolveSlotRefVisitor : public TypedVisitor
{
 public:

  ResolveSlotRefVisitor(wostream& os, aafSlotID_t slotId)
    : _os(os),
      _slotId( slotId )
  {}

  virtual ~ResolveSlotRefVisitor()
  {}

  //Note: We cannot do a node cast in this situation since we need the node
  //      of the correct type to create an edge.
  virtual bool PreOrderVisit(AAFTypedObjNode<IAAFTimelineMobSlot>& node)
  {
    AxTimelineMobSlot axMobSlot( node.GetAAFObjectOfType() );
    aafSlotID_t slotId = axMobSlot.GetSlotID();

    if ( slotId == _slotId )
    {
        _spMobSlot = node.GetSharedPointerToNode();
        //The node was found, there is no need to traverse further.
        return false;
    }

    return true;
  }

  virtual bool PreOrderVisit(AAFTypedObjNode<IAAFStaticMobSlot>& node)
  {
    AxStaticMobSlot axMobSlot( node.GetAAFObjectOfType() );
    aafSlotID_t slotId = axMobSlot.GetSlotID();

    if ( slotId == _slotId )
    {
        _spMobSlot = node.GetSharedPointerToNode();
        //The node was found, there is no need to traverse further.
        return false;
    }

    return true;
  }

  virtual bool PreOrderVisit(AAFTypedObjNode<IAAFEventMobSlot>& node)
  {
    AxEventMobSlot axMobSlot( node.GetAAFObjectOfType() );
    aafSlotID_t slotId = axMobSlot.GetSlotID();

    if ( slotId == _slotId )
    {
        _spMobSlot = node.GetSharedPointerToNode();
        //The node was found, there is no need to traverse further.
        return false;
    }

    return true;
  }

  virtual bool PreOrderVisit(AAFTypedObjNode<IAAFMobSlot>& node)
  {
    AxMobSlot axMobSlot( node.GetAAFObjectOfType() );
    aafSlotID_t slotId = axMobSlot.GetSlotID();

    if ( slotId == _slotId )
    {
        _spMobSlot = node.GetSharedPointerToNode();
        //The node was found, there is no need to traverse further.
        return false;
    }

    return true;
  }

  virtual bool EdgeVisit(AAFContainment& edge)
  {
    //Don't continue the traversal if the mob slot has been found.
    return !_spMobSlot;
  }

  shared_ptr<Node> GetMobSlot() const
  {
    return _spMobSlot;
  }

 private:
  wostream& _os;
  aafSlotID_t _slotId;
  shared_ptr<Node> _spMobSlot;

  // prohibited
  ResolveSlotRefVisitor();
  ResolveSlotRefVisitor( const ResolveSlotRefVisitor& );
  ResolveSlotRefVisitor& operator=( const ResolveSlotRefVisitor& );
};

class ResolveComponentRefVisitor : public TypedVisitor
{
 public:

  typedef set<shared_ptr<Node> > NodeSet;
  typedef shared_ptr<NodeSet> NodeSetSP;

  ResolveComponentRefVisitor(wostream& os, shared_ptr<set<IAAFSmartPointer2<IAAFComponent> > > componentsToReference)
    : _os(os),
      _spNodes( new NodeSet ),
      _componentsToReference( componentsToReference )
  {}

  virtual ~ResolveComponentRefVisitor()
  {}

  virtual bool PreOrderVisit(AAFTypedObjNode<IAAFSequence>& node)
  {
    //We want to expand all sequences, so, continue the traversal.
    return true;
  }

  virtual bool PreOrderVisit(AAFTypedObjNode<IAAFComponent>& node)
  {
    //All components that are not sequences should end up executing this code.
    //Add the component to the set of nodes if it is in the correct time range:
    AxComponent axComponent( node.GetAAFObjectOfType() );
    IAAFSmartPointer2<IAAFComponent> spComponent;
    spComponent = axComponent;
    if ( _componentsToReference->find( spComponent ) != _componentsToReference->end() )
    {
      _spNodes->insert( node.GetSharedPointerToNode() );
    }

    //We have reached a non-sequence component, there is no need to traverse any
    //further.
    return false;
  }

  virtual bool PreOrderVisit(AAFTypedObjNode<IAAFTransition>& node)
  {
    shared_ptr<AAFTypedObjNode<IAAFComponent> > spGeneric( node.DownCastToAAF<IAAFComponent>() );
    return this->PreOrderVisit( *spGeneric );
  }

  virtual bool PreOrderVisit(AAFTypedObjNode<IAAFSegment>& node)
  {
    shared_ptr<AAFTypedObjNode<IAAFComponent> > spGeneric( node.DownCastToAAF<IAAFComponent>() );
    return this->PreOrderVisit( *spGeneric );
  }

  virtual bool PreOrderVisit(AAFTypedObjNode<IAAFFiller>& node)
  {
    shared_ptr<AAFTypedObjNode<IAAFComponent> > spGeneric( node.DownCastToAAF<IAAFComponent>() );
    return this->PreOrderVisit( *spGeneric );
  }

  virtual bool PreOrderVisit(AAFTypedObjNode<IAAFSourceReference>& node)
  {
    shared_ptr<AAFTypedObjNode<IAAFComponent> > spGeneric( node.DownCastToAAF<IAAFComponent>() );
    return this->PreOrderVisit( *spGeneric );
  }

  virtual bool PreOrderVisit(AAFTypedObjNode<IAAFSourceClip>& node)
  {
    shared_ptr<AAFTypedObjNode<IAAFComponent> > spGeneric( node.DownCastToAAF<IAAFComponent>() );
    return this->PreOrderVisit( *spGeneric );
  }

  virtual bool PreOrderVisit(AAFTypedObjNode<IAAFEvent>& node)
  {
    shared_ptr<AAFTypedObjNode<IAAFComponent> > spGeneric( node.DownCastToAAF<IAAFComponent>() );
    return this->PreOrderVisit( *spGeneric );
  }

  virtual bool PreOrderVisit(AAFTypedObjNode<IAAFGPITrigger>& node)
  {
    shared_ptr<AAFTypedObjNode<IAAFComponent> > spGeneric( node.DownCastToAAF<IAAFComponent>() );
    return this->PreOrderVisit( *spGeneric );
  }

  virtual bool PreOrderVisit(AAFTypedObjNode<IAAFCommentMarker>& node)
  {
    shared_ptr<AAFTypedObjNode<IAAFComponent> > spGeneric( node.DownCastToAAF<IAAFComponent>() );
    return this->PreOrderVisit( *spGeneric );
  }

  virtual bool PreOrderVisit(AAFTypedObjNode<IAAFDescriptiveMarker>& node)
  {
    shared_ptr<AAFTypedObjNode<IAAFComponent> > spGeneric( node.DownCastToAAF<IAAFComponent>() );
    return this->PreOrderVisit( *spGeneric );
  }

  virtual bool PreOrderVisit(AAFTypedObjNode<IAAFEdgecode>& node)
  {
    shared_ptr<AAFTypedObjNode<IAAFComponent> > spGeneric( node.DownCastToAAF<IAAFComponent>() );
    return this->PreOrderVisit( *spGeneric );
  }

  virtual bool PreOrderVisit(AAFTypedObjNode<IAAFTimecode>& node)
  {
    shared_ptr<AAFTypedObjNode<IAAFComponent> > spGeneric( node.DownCastToAAF<IAAFComponent>() );
    return this->PreOrderVisit( *spGeneric );
  }

  virtual bool PreOrderVisit(AAFTypedObjNode<IAAFTimecodeStream>& node)
  {
    shared_ptr<AAFTypedObjNode<IAAFComponent> > spGeneric( node.DownCastToAAF<IAAFComponent>() );
    return this->PreOrderVisit( *spGeneric );
  }

  virtual bool PreOrderVisit(AAFTypedObjNode<IAAFTimecodeStream12M>& node)
  {
    shared_ptr<AAFTypedObjNode<IAAFComponent> > spGeneric( node.DownCastToAAF<IAAFComponent>() );
    return this->PreOrderVisit( *spGeneric );
  }

  virtual bool PreOrderVisit(AAFTypedObjNode<IAAFPulldown>& node)
  {
    shared_ptr<AAFTypedObjNode<IAAFComponent> > spGeneric( node.DownCastToAAF<IAAFComponent>() );
    return this->PreOrderVisit( *spGeneric );
  }

  virtual bool PreOrderVisit(AAFTypedObjNode<IAAFOperationGroup>& node)
  {
    shared_ptr<AAFTypedObjNode<IAAFComponent> > spGeneric( node.DownCastToAAF<IAAFComponent>() );
    return this->PreOrderVisit( *spGeneric );
  }

  virtual bool PreOrderVisit(AAFTypedObjNode<IAAFNestedScope>& node)
  {
    shared_ptr<AAFTypedObjNode<IAAFComponent> > spGeneric( node.DownCastToAAF<IAAFComponent>() );
    return this->PreOrderVisit( *spGeneric );
  }

  virtual bool PreOrderVisit(AAFTypedObjNode<IAAFScopeReference>& node)
  {
    shared_ptr<AAFTypedObjNode<IAAFComponent> > spGeneric( node.DownCastToAAF<IAAFComponent>() );
    return this->PreOrderVisit( *spGeneric );
  }

  virtual bool PreOrderVisit(AAFTypedObjNode<IAAFEssenceGroup>& node)
  {
    shared_ptr<AAFTypedObjNode<IAAFComponent> > spGeneric( node.DownCastToAAF<IAAFComponent>() );
    return this->PreOrderVisit( *spGeneric );
  }

  virtual bool PreOrderVisit(AAFTypedObjNode<IAAFSelector>& node)
  {
    shared_ptr<AAFTypedObjNode<IAAFComponent> > spGeneric( node.DownCastToAAF<IAAFComponent>() );
    return this->PreOrderVisit( *spGeneric );
  }

  NodeSetSP GetReferencedNodes() const
  {
    return _spNodes;
  }

 private:
  wostream& _os;
  NodeSetSP _spNodes;
  shared_ptr<set<IAAFSmartPointer2<IAAFComponent> > > _componentsToReference;

  // prohibited
  ResolveComponentRefVisitor();
  ResolveComponentRefVisitor( const ResolveComponentRefVisitor& );
  ResolveComponentRefVisitor& operator=( const ResolveComponentRefVisitor& );
};

} // end of namespace


//======================================================================
//======================================================================
//======================================================================
namespace aafanalyzer {

using namespace std;
using namespace boost;

ResolveRefVisitor::ResolveRefVisitor(wostream& os, shared_ptr<EdgeMap> spEdgeMap )
: _os(os),
  _spEdgeMap(spEdgeMap),
  _spResult( new DetailLevelTestResult(
                           L"ResolveRefVisitor",
                           L"Visit source clip objects and resolve references..",
                           L"-",
                           L"-",
                           TestResult::PASS,
                           TestRegistry::GetInstance().GetRequirementsForTest( RefResolver::GetTestInfo().GetName() )
           )                          )
{}

ResolveRefVisitor::~ResolveRefVisitor()
{
}

bool ResolveRefVisitor::PreOrderVisit(AAFTypedObjNode<IAAFTimelineMobSlot>& node)
{
  AxTimelineMobSlot axMobSlot( node.GetAAFObjectOfType() );
  _parentSlots.push( make_pair( kAAFClassID_TimelineMobSlot, axMobSlot.GetEditRate() ) );
  return true;
}

bool ResolveRefVisitor::PreOrderVisit(AAFTypedObjNode<IAAFEventMobSlot>& node)
{
  AxEventMobSlot axMobSlot( node.GetAAFObjectOfType() );
  _parentSlots.push( make_pair( kAAFClassID_EventMobSlot, axMobSlot.GetEditRate() ) );
  return true;
}

bool ResolveRefVisitor::PreOrderVisit(AAFTypedObjNode<IAAFStaticMobSlot>& node)
{
  aafRational_t editRate;
  editRate.numerator = 0;
  editRate.denominator = 0;
  _parentSlots.push( make_pair( kAAFClassID_StaticMobSlot, editRate ) );
  return true;
}

bool ResolveRefVisitor::PreOrderVisit(AAFTypedObjNode<IAAFMobSlot>& node)
{
  aafRational_t editRate;
  editRate.numerator = 0;
  editRate.denominator = 0;
  _parentSlots.push( make_pair( kAAFClassID_MobSlot, editRate ) );
  return true;
}

bool ResolveRefVisitor::PostOrderVisit(AAFTypedObjNode<IAAFTimelineMobSlot>& node)
{
  _parentSlots.pop();
  return true;
}

bool ResolveRefVisitor::PostOrderVisit(AAFTypedObjNode<IAAFEventMobSlot>& node)
{
  _parentSlots.pop();
  return true;
}

bool ResolveRefVisitor::PostOrderVisit(AAFTypedObjNode<IAAFStaticMobSlot>& node)
{
  _parentSlots.pop();
  return true;
}

bool ResolveRefVisitor::PostOrderVisit(AAFTypedObjNode<IAAFMobSlot>& node)
{
  _parentSlots.pop();
  return true;
}

bool ResolveRefVisitor::PostOrderVisit(AAFTypedObjNode<IAAFSourceClip>& node)
{
  AxSourceClip axSrcClp(node.GetAAFObjectOfType());

  aafSourceRef_t srcRef = axSrcClp.GetSourceReference();
  aafMobID_t mobid = srcRef.sourceID;

  if ( AxConstants::NULL_MOBID == mobid )
  {
    return true;
  }

  shared_ptr<Node> spNode;
  spNode = MobNodeMap::GetInstance().GetMobNode(mobid);

  //Assert it really is a mob. (i.e. to verify the MobNodeMap is delivering good data.)
  //Then resolve the reference.
  if(spNode)
  {
    //verify mob
    IAAFMobSP spVerify;
    shared_ptr<AAFObjNode> spObjNode;
    spObjNode = dynamic_pointer_cast<AAFObjNode>(spNode);
    AxObject axObj(spObjNode->GetAAFObject());
    assert(AxIsA(axObj, spVerify));

    //mob has been verified, proceed to the sourceclip
    shared_ptr<AAFTypedObjNode<IAAFSourceClip> > spSrcClp;
    shared_ptr<Node> temp = node.GetSharedPointerToNode();
    spSrcClp = dynamic_pointer_cast<AAFTypedObjNode<IAAFSourceClip> >( temp );

    //ensure we have the two proper nodes (spNode, spSrcClp), create a Mob Edge and add to Edgemap
    if(spSrcClp)
    {
      shared_ptr<AAFMobReference> spMobRefEdge(new AAFMobReference(spSrcClp, spNode));
      _spEdgeMap->AddEdge(spMobRefEdge);

      //now create a Slot Edge from the source clip to the mobslot and add to Edgemap
      shared_ptr<ResolveSlotRefVisitor> spSlotVisitor( new ResolveSlotRefVisitor( _os, srcRef.sourceSlotID ) );
      DepthFirstTraversal slotDFT( _spEdgeMap, spNode );
      slotDFT.TraverseDown( spSlotVisitor );
      shared_ptr<Node> spMobSlotNode = spSlotVisitor->GetMobSlot();

      if ( spMobSlotNode )
      {
        shared_ptr<AAFSlotReference> spSlotEdge( new AAFSlotReference(spSrcClp, spMobSlotNode) );
        _spEdgeMap->AddEdge( spSlotEdge );

        //Now make a reference from the source clip to all non-sequence
        //components referenced from this mob slot.

        //Find all components below the MobSlot that need referencing.
        shared_ptr<set<IAAFSmartPointer2<IAAFComponent> > > componentsToReference =
            GetComponentsInRange( spMobSlotNode, axSrcClp );

        shared_ptr<ResolveComponentRefVisitor> spCompVisitor( new ResolveComponentRefVisitor( _os, componentsToReference ) );
        DepthFirstTraversal dfs( _spEdgeMap, spMobSlotNode );
        dfs.TraverseDown( spCompVisitor );
        ResolveComponentRefVisitor::NodeSetSP spNodes = spCompVisitor->GetReferencedNodes();

        ResolveComponentRefVisitor::NodeSet::const_iterator iter;

        for ( iter = spNodes->begin(); iter != spNodes->end(); iter++ )
        {
          shared_ptr<AAFComponentReference> spCompEdge( new AAFComponentReference(spSrcClp, *iter) );
          _spEdgeMap->AddEdge( spCompEdge );
        }

      }
    }
  }
  //keep track of unresolved source clips
  else
  {
    AxString explain( L"Out-file mob referenced. ID = " );
    explain += AxStringUtil::mobid2Str( mobid );
    shared_ptr<const DetailLevelTestResult> spWarning( new DetailLevelTestResult( _spResult->GetName(),
                                 L"-", // desc
                                 explain,
                                 L"-", // docref
                                 TestResult::WARN,
                                 *(new Requirement::RequirementMapSP(new Requirement::RequirementMap())) ) );
    //TODO: Pass a real RequirementVectorSP
    _spResult->AppendSubtestResult( spWarning );
    _spResult->SetResult( _spResult->GetAggregateResult() );
  }

  return true;
}

/*
 *
 * Note: EventMobSlots can only contain Events or Sequences of Events.
 *       Therefore, an EventMobSlot cannot contain a SourceClip, so, there will
 *       never be any references to resolve.
 *
 */
shared_ptr<set<IAAFSmartPointer2<IAAFComponent> > > ResolveRefVisitor::GetComponentsInRange( shared_ptr<Node> spMobSlotNode, AxSourceClip& axSrcClp )
{

  /*
   *
   * Note: Currently, references from TimelineMobSlots to TimelineMobSlots and
   *       EventMobSlots are being properly resolved.  References from
   *       TimelineMobSlots to StaticMobSlots are being resolved by referencing
   *       all non-Sequence Components in the StaticMobSlot.  However, this is
   *       not necessarily correct.
   *
   *        Consider: In a StaticMobSlot, Components shall not have a length
   *                  property.  However, any non-Event component that is
   *                  added to a Sequence must have a length property (giving
   *                  the Sequence a Length property), which violates the
   *                  condition that Coomponents in StaticMobSlots do not have
   *                  length properties.  This requirement is not enforced in
   *                  the code so references are resolved.
   *
   *       Realistically, it seems that a SourceClip referencing a StaticMobSlot
   *       should be required to reference a Segment that does not have a length
   *       property, which would be a non-Sequence component or a Sequence of
   *       Events with no length property.
   *
   *       As it stands, regression tests exist to test that references are
   *       resolved as the implementation has intended.  Therefore, if the
   *       semantics of what can be stored in a StaticMobSlot are changed, and
   *       the resolution code is updated, the regression test will fail.
   *
   *       Currently, references from EventMobSlots are not being resolved since
   *       SourceReferences are illegal within EventMobSlots.
   *
   *       Currently, references from StaticMobSlots to TimelineMobSlots,
   *       EventMobSlots and StaticMobSlots are being resolved by referencing
   *       all non-Sequence components within the referenced MobSlot.  For
   *       TimelineMobSlots and EventMobSlots, this seems incorrect.
   *       Realistically, it seems that the reference should be to the
   *       components that are in the referenced MobSlots timeline at the
   *       StartTime specified by the SourceClip (think a SourceClip with
   *       length = 0).  However, this presents a problem since the StartTime
   *       property is specified in edit units of the SourceClip's object
   *       context, and the StaticMobSlot containing the SourceClip does not
   *       have an EditRate property.  Also, the references from StaticMobSlots
   *       to StaticMobSlots is most likely incorrect.  The argument made above
   *       for referencing StaticMobSlots from TimelineMobSlots also applies to
   *       this situation.
   *
   *       Currently regression tests are implemented to ensure that references
   *       are resolved as the implementation has intended.  Therefore, if the
   *       mentioned issues are corrected, and the reference resolution is
   *       changed, then the regression test will fail.
   *
   */

  //We need to figure out which nodes fall in the referenced part of the
  //timeline.  This invloves considering the type of MobSlot that contains
  //the clip, and the type of mob slot that is being linked.
  shared_ptr<AAFObjNode> spAAFMobSlotNode = dynamic_pointer_cast<AAFObjNode>( spMobSlotNode );
  AxObject axObjSlot( spAAFMobSlotNode->GetAAFObject() );

  IAAFTimelineMobSlotSP   spTimelineMobSlot;
  IAAFEventMobSlotSP      spEventMobSlot;
  IAAFStaticMobSlotSP     spStaticMobSlot;
  IAAFMobSlotSP           spMobSlot;

  aafUID_t slotType = _parentSlots.top().first;

  //Consider the type of the mob slot we are referencing.
  if ( AxIsA( axObjSlot, spTimelineMobSlot ) )
  {
    AxTimelineMobSlot axMobSlot( spTimelineMobSlot );
    if ( slotType == kAAFClassID_TimelineMobSlot )
    {
      //We should reference everything in the range specified in the
      //source clip.  References from EventMobSlots to TimelineMobSlots
      //will be resolved regardless of the correctness of the conversion.
      AxSegment axSegment( axMobSlot.GetSegment() );
      return GetComponentsToTimelineSlot( axMobSlot, axSrcClp, axSegment, 0.0 );
    }
    else if ( slotType == kAAFClassID_StaticMobSlot ||
              slotType == kAAFClassID_MobSlot )
    {
      //Components in StaticMobSlots do not have length properties.
      //SourceClips in StaticMobSlots do not have StartTime properties.
      //Therefore, resolve all references, from StaticMobSlots to
      //TimelineMobSlots regardless of the correctness of the conversion.
      AxSegment axSegment( axMobSlot.GetSegment() );
      return GetComponentsToFromGenericSlot( axSegment );
    }
  }

  else if ( AxIsA( axObjSlot, spEventMobSlot ) )
  {
    AxEventMobSlot axMobSlot( spEventMobSlot );
    aafUID_t slotType = _parentSlots.top().first;
    if ( slotType == kAAFClassID_TimelineMobSlot )
    {
      //We should reference everything in the range specified in the
      //source clip.  References from TimelineMobSlots to EventMobSlots
      //will be resolved regardless of the correctness of the conversion.
      return GetComponentsToEventSlot( axMobSlot, axSrcClp );
    }
    else if ( slotType == kAAFClassID_StaticMobSlot ||
              slotType == kAAFClassID_MobSlot )
    {
      //Components in StaticMobSlots do not have length properties.
      //SourceClips in StaticMobSlots do not have StartTime properties.
      //Therefore, resolve all references, from StaticMobSlots to
      //EventMobSlots regardless of the correctness of the conversion.
      AxSegment axSegment( axMobSlot.GetSegment() );
      return GetComponentsToFromGenericSlot( axSegment );
    }
  }

  else if ( AxIsA( axObjSlot, spStaticMobSlot ) )
  {
    //StaticMobSlots do not have any time information, therefore resolve
    //references to all components in the StaticMobSlot.  References are
    //resolved from TimelineMobSlots and EventMobSlots regardless of
    //whether of not the conversion is correct.
    AxStaticMobSlot axMobSlot( spStaticMobSlot );
    AxSegment axSegment( axMobSlot.GetSegment() );
    return GetComponentsToFromGenericSlot( axSegment );
  }

  else if ( AxIsA( axObjSlot, spMobSlot ) )
  {
    //There is no aditional information about this type of MobSlot, so treat it
    //like a StaticMobSlot.

    AxMobSlot axMobSlot( spMobSlot );
    AxSegment axSegment( axMobSlot.GetSegment() );
    return GetComponentsToFromGenericSlot( axSegment );
  }

  //If the node was not a MobSlot, return an empty set.
  shared_ptr<set<IAAFSmartPointer2<IAAFComponent> > > referencedComponents( new set<IAAFSmartPointer2<IAAFComponent> > );
  return referencedComponents;

}

shared_ptr<set<IAAFSmartPointer2<IAAFComponent> > > ResolveRefVisitor::GetComponentsToTimelineSlot( AxTimelineMobSlot& axMobSlot, AxSourceClip& axSrcClp, AxSegment& axSegment, double startTime )
{

  shared_ptr<set<IAAFSmartPointer2<IAAFComponent> > > referencedComponents( new set<IAAFSmartPointer2<IAAFComponent> > );

  //Find the Source Clip's start time and length
  aafRational_t clipRate = _parentSlots.top().second;
  double clipStart = (double)axSrcClp.GetSourceReference().startTime * (double)clipRate.numerator / (double)clipRate.denominator;
  double clipLen = (double)axSrcClp.GetLength() * (double)clipRate.numerator / (double)clipRate.denominator;

  //Find the referenced materials offset and adjust the start
  //time accordingly
  aafRational_t segmentRate = axMobSlot.GetEditRate();
  double offset = (double)axMobSlot.GetOrigin() * (double)segmentRate.numerator / (double)segmentRate.denominator;
  clipStart -= offset;

  //If the component in the referenced slot ends before or begins
  //after the referenced time, we do not want to add a reference
  //to the graph.
  double segmentLen = (double)axSegment.GetLength() * (double)segmentRate.numerator / (double)segmentRate.denominator;

  if ( !( startTime + segmentLen <= clipStart || clipStart + clipLen <= startTime ) )
  {
    //We need to references this segment.  If it is not a sequence,
    //this is trivial.
    IAAFSequenceSP spSequence;
    if ( AxIsA( axSegment, spSequence ) )
    {
      //Now, we have to consider every component in the sequence.
      AxSequence axSequence( spSequence );
      aafUInt32 numComponents = axSequence.CountComponents();
      double time = startTime;
      for ( unsigned int i = 0; i < numComponents; i++ )
      {

        IAAFTransitionSP spTransition;

        AxComponent axComp( axSequence.GetComponentAt( i ) );
        double compLen = (double)axComp.GetLength() * (double)segmentRate.numerator / (double)segmentRate.denominator;

        //If this is a sequence, we need to consider all of the components
        //within it.
        if ( AxIsA( axComp, spSequence ) )
        {
            AxSequence childSequence( spSequence );
            shared_ptr<set<IAAFSmartPointer2<IAAFComponent> > > childComponents =
                GetComponentsToTimelineSlot( axMobSlot, axSrcClp, childSequence, time );
            set<IAAFSmartPointer2<IAAFComponent> >::const_iterator iter;
            for ( iter = childComponents->begin(); iter != childComponents->end(); iter++ )
            {
                referencedComponents->insert( *iter );
            }
        }
        else
        {
          //If this is a transition, we need to consider it from the begining of
          //the transition (not the end which was given by the end of the
          //previous component).
          if ( AxIsA( axComp, spTransition ) )
          {
            time -= compLen;
          }

          if ( !( clipStart + clipLen <= time || time + compLen <= clipStart ) )
          {
            IAAFSmartPointer2<IAAFComponent> spComp;
            spComp = axComp;
            referencedComponents->insert( spComp );
          }
        }

        //If this is not a transition, then the time should be adjusted.  If it
        //is a transition, the begining of the next segment is the same as the
        //begining of the transition.
        if ( !AxIsA( axComp, spTransition ) )
        {
          time += compLen;
        }

      }
    }
    else
    {
      IAAFSmartPointer2<IAAFComponent> spComp;
      spComp = AxQueryInterface<IAAFSegment, IAAFComponent>( axSegment );
      referencedComponents->insert( spComp );
    }
  }

  return referencedComponents;

}

shared_ptr<set<IAAFSmartPointer2<IAAFComponent> > > ResolveRefVisitor::GetComponentsToEventSlot( AxEventMobSlot& axMobSlot, AxSourceClip& axSrcClp )
{

  shared_ptr<set<IAAFSmartPointer2<IAAFComponent> > > referencedComponents( new set<IAAFSmartPointer2<IAAFComponent> > );

  //Find the Source Clip's start time and length
  aafRational_t clipRate = _parentSlots.top().second;
  double clipStart = (double)axSrcClp.GetSourceReference().startTime  * (double)clipRate.numerator / (double)clipRate.denominator;
  double clipLen = (double)axSrcClp.GetLength() * (double)clipRate.numerator / (double)clipRate.denominator;

  //If the component in the referenced slot ends before or begins
  //after the referenced time, we do not want to add a reference
  //to the graph.

  aafRational_t segmentRate = axMobSlot.GetEditRate();
  AxSegment axSegment( axMobSlot.GetSegment() );

  //We need to determine if this is an Event or a Sequence to properly
  //continue.
  IAAFSequenceSP spSequence;

  if ( AxIsA( axSegment, spSequence ) )
  {
    //Now, we have to consider every event in the sequence.
    AxSequence axSequence( spSequence );
    aafUInt32 numComponents = axSequence.CountComponents();

    for ( unsigned int i = 0; i < numComponents; i++ )
    {
      AxComponent axComp( axSequence.GetComponentAt( i ) );
      AxEvent axEvent( AxQueryInterface<IAAFComponent, IAAFEvent>( axComp ) );

      double eventStart = (double)axEvent.GetPosition() * (double)segmentRate.numerator / (double)segmentRate.denominator;

      //If the event doesn't have a length it is an instantaneous event.
      double eventLen;
      try
      {
        eventLen = (double)axEvent.GetLength() * (double)segmentRate.numerator / (double)segmentRate.denominator;
      }
      catch ( const AxExHResult& ex )
      {
        if ( ex.getHResult() == AAFRESULT_PROP_NOT_PRESENT )
        {
          eventLen = 0.0;
        }
        else
        {
          throw ex;
        }
      }

      if ( !( eventStart + eventLen <= clipStart || clipStart + clipLen <= eventStart ) )
      {
        IAAFSmartPointer2<IAAFComponent> spComp;
        spComp = axComp;
        referencedComponents->insert( spComp );
      }

    }

  }
  else
  {
    AxEvent axEvent( AxQueryInterface<IAAFSegment, IAAFEvent>( axSegment ) );

    double eventStart = (double)axEvent.GetPosition() * (double)segmentRate.numerator / (double)segmentRate.denominator;

    //If the event doesn't have a length it is an instantaneous event.
    double eventLen;
    try
    {
      eventLen = (double)axEvent.GetLength() * (double)segmentRate.numerator / (double)segmentRate.denominator;
    }
    catch ( const AxExHResult& ex )
    {
      if ( ex.getHResult() == AAFRESULT_PROP_NOT_PRESENT )
      {
        eventLen = 0.0;
      }
      else
      {
        throw ex;
      }
    }

    if ( !( eventStart + eventLen <= clipStart || clipStart + clipLen <= eventStart ) )
    {
      IAAFSmartPointer2<IAAFComponent> spComp;
      spComp = AxQueryInterface<IAAFSegment, IAAFComponent>( axSegment );
      referencedComponents->insert( spComp );
    }

  }

  return referencedComponents;

}

shared_ptr<set<IAAFSmartPointer2<IAAFComponent> > > ResolveRefVisitor::GetComponentsToFromGenericSlot( AxSegment& axSegment )
{

  shared_ptr<set<IAAFSmartPointer2<IAAFComponent> > > referencedComponents( new set<IAAFSmartPointer2<IAAFComponent> > );

  //We need to references this segment.  If it is not a sequence,
  //this is trivial.
  IAAFSequenceSP spSequence;
  if ( AxIsA( axSegment, spSequence ) )
  {
    //Now, we have to consider every component in the sequence.
    AxSequence axSequence( spSequence );
    aafUInt32 numComponents = axSequence.CountComponents();
    for ( unsigned int i = 0; i < numComponents; i++ )
    {
      AxComponent axComp( axSequence.GetComponentAt( i ) );

      if ( AxIsA( axComp, spSequence ) )
      {
        AxSequence childSequence( spSequence );
        shared_ptr<set<IAAFSmartPointer2<IAAFComponent> > > childComponents =
          GetComponentsToFromGenericSlot( childSequence );
        set<IAAFSmartPointer2<IAAFComponent> >::const_iterator iter;
        for ( iter = childComponents->begin(); iter != childComponents->end(); iter++ )
        {
          referencedComponents->insert( *iter );
        }
      }
      else
      {
        IAAFSmartPointer2<IAAFComponent> spComp;
        spComp = axComp;
        referencedComponents->insert( spComp );
      }
    }
  }
  else
  {
    IAAFSmartPointer2<IAAFComponent> spComp;
    spComp = AxQueryInterface<IAAFSegment, IAAFComponent>( axSegment );
    referencedComponents->insert( spComp );
  }

  return referencedComponents;

}

shared_ptr<const DetailLevelTestResult> ResolveRefVisitor::GetTestResult() const
{
  return _spResult;
}

}
