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
#include "EPAnnotationVisitor.h"
#include "EPAnnotationTest.h"

//Test/Result files
#include <DetailLevelTestResult.h>
#include <TestLevelTestResult.h>
#include <TestRegistry.h>

//Ax files
#include <AxComponent.h>
#include <AxEx.h>
#include <AxTaggedValue.h>
#include <AxKLVData.h>
#include <AxDefObject.h>

//AAF files
#include <AAFResult.h>

namespace {

} // end of namespace

//======================================================================

namespace aafanalyzer {

using namespace boost;
 
EPAnnotationVisitor::EPAnnotationVisitor( wostream& log,
                                          shared_ptr<EdgeMap> spEdgeMap,
                                          shared_ptr<TestLevelTestResult> spTestResult )
    : _log(log),
      _spEdgeMap( spEdgeMap ),
      _spTestResult( spTestResult )
{
    _isAncestorEssenceTrack.push( false );
    _isAncestorEventMobSlot.push( false );
    _isParentMobSlot.push( false );
}
    
EPAnnotationVisitor::~EPAnnotationVisitor()
{}

/*
 * 
 * Components
 * 
 */
 
bool EPAnnotationVisitor::PreOrderVisit( AAFTypedObjNode<IAAFTransition>& node )
{
    shared_ptr<AAFTypedObjNode<IAAFComponent> > spGeneric( node.DownCastToAAF<IAAFComponent>() );
    return this->PreOrderVisit( *spGeneric );
}

bool EPAnnotationVisitor::PreOrderVisit( AAFTypedObjNode<IAAFSegment>& node )
{
    shared_ptr<AAFTypedObjNode<IAAFComponent> > spGeneric( node.DownCastToAAF<IAAFComponent>() );
    return this->PreOrderVisit( *spGeneric );
}

bool EPAnnotationVisitor::PreOrderVisit( AAFTypedObjNode<IAAFSequence>& node )
{
    shared_ptr<AAFTypedObjNode<IAAFComponent> > spGeneric( node.DownCastToAAF<IAAFComponent>() );
    return this->PreOrderVisit( *spGeneric );
}

bool EPAnnotationVisitor::PreOrderVisit( AAFTypedObjNode<IAAFFiller>& node )
{
    shared_ptr<AAFTypedObjNode<IAAFComponent> > spGeneric( node.DownCastToAAF<IAAFComponent>() );
    return this->PreOrderVisit( *spGeneric );
}

bool EPAnnotationVisitor::PreOrderVisit( AAFTypedObjNode<IAAFSourceReference>& node )
{
    shared_ptr<AAFTypedObjNode<IAAFComponent> > spGeneric( node.DownCastToAAF<IAAFComponent>() );
    return this->PreOrderVisit( *spGeneric );
}

bool EPAnnotationVisitor::PreOrderVisit( AAFTypedObjNode<IAAFSourceClip>& node )
{
    shared_ptr<AAFTypedObjNode<IAAFComponent> > spGeneric( node.DownCastToAAF<IAAFComponent>() );
    return this->PreOrderVisit( *spGeneric );
}

bool EPAnnotationVisitor::PreOrderVisit( AAFTypedObjNode<IAAFEvent>& node )
{
    shared_ptr<AAFTypedObjNode<IAAFComponent> > spGeneric( node.DownCastToAAF<IAAFComponent>() );
    return this->PreOrderVisit( *spGeneric );
}

bool EPAnnotationVisitor::PreOrderVisit( AAFTypedObjNode<IAAFGPITrigger>& node )
{
    shared_ptr<AAFTypedObjNode<IAAFComponent> > spGeneric( node.DownCastToAAF<IAAFComponent>() );
    return this->PreOrderVisit( *spGeneric );
}

bool EPAnnotationVisitor::PreOrderVisit( AAFTypedObjNode<IAAFEdgecode>& node )
{
    shared_ptr<AAFTypedObjNode<IAAFComponent> > spGeneric( node.DownCastToAAF<IAAFComponent>() );
    return this->PreOrderVisit( *spGeneric );
}

bool EPAnnotationVisitor::PreOrderVisit( AAFTypedObjNode<IAAFTimecode>& node )
{
    shared_ptr<AAFTypedObjNode<IAAFComponent> > spGeneric( node.DownCastToAAF<IAAFComponent>() );
    return this->PreOrderVisit( *spGeneric );
}

bool EPAnnotationVisitor::PreOrderVisit( AAFTypedObjNode<IAAFTimecodeStream>& node )
{
    shared_ptr<AAFTypedObjNode<IAAFComponent> > spGeneric( node.DownCastToAAF<IAAFComponent>() );
    return this->PreOrderVisit( *spGeneric );
}

bool EPAnnotationVisitor::PreOrderVisit( AAFTypedObjNode<IAAFTimecodeStream12M>& node )
{
    shared_ptr<AAFTypedObjNode<IAAFComponent> > spGeneric( node.DownCastToAAF<IAAFComponent>() );
    return this->PreOrderVisit( *spGeneric );
}

bool EPAnnotationVisitor::PreOrderVisit( AAFTypedObjNode<IAAFPulldown>& node )
{
    shared_ptr<AAFTypedObjNode<IAAFComponent> > spGeneric( node.DownCastToAAF<IAAFComponent>() );
    return this->PreOrderVisit( *spGeneric );
}

bool EPAnnotationVisitor::PreOrderVisit( AAFTypedObjNode<IAAFOperationGroup>& node )
{
    shared_ptr<AAFTypedObjNode<IAAFComponent> > spGeneric( node.DownCastToAAF<IAAFComponent>() );
    return this->PreOrderVisit( *spGeneric );
}

bool EPAnnotationVisitor::PreOrderVisit( AAFTypedObjNode<IAAFNestedScope>& node )
{
    shared_ptr<AAFTypedObjNode<IAAFComponent> > spGeneric( node.DownCastToAAF<IAAFComponent>() );
    return this->PreOrderVisit( *spGeneric );
}

bool EPAnnotationVisitor::PreOrderVisit( AAFTypedObjNode<IAAFScopeReference>& node )
{
    shared_ptr<AAFTypedObjNode<IAAFComponent> > spGeneric( node.DownCastToAAF<IAAFComponent>() );
    return this->PreOrderVisit( *spGeneric );
}

bool EPAnnotationVisitor::PreOrderVisit( AAFTypedObjNode<IAAFEssenceGroup>& node )
{
    shared_ptr<AAFTypedObjNode<IAAFComponent> > spGeneric( node.DownCastToAAF<IAAFComponent>() );
    return this->PreOrderVisit( *spGeneric );
}

bool EPAnnotationVisitor::PreOrderVisit( AAFTypedObjNode<IAAFSelector>& node )
{
    shared_ptr<AAFTypedObjNode<IAAFComponent> > spGeneric( node.DownCastToAAF<IAAFComponent>() );
    return this->PreOrderVisit( *spGeneric );
}

//Base Cases:

bool EPAnnotationVisitor::PreOrderVisit( AAFTypedObjNode<IAAFDescriptiveMarker>& node )
{
    //This is a comment marker subclass, there is nothing that needs to be
    //checked, but state must be updated.
    _isParentMobSlot.push( false );
    return true;
}

bool EPAnnotationVisitor::PreOrderVisit( AAFTypedObjNode<IAAFCommentMarker>& node )
{
    
    bool testPassed = true;
    
    //Need to ensure that parent is event mob slot
    if ( _isAncestorEventMobSlot.top() )
    {
        _spTestResult->AddSingleResult(
            L"REQ_EP_150",
            this->GetMobSlotName( _spEdgeMap, node) + L" is an Event Mob Slot that contains a CommentMarker.",
            TestResult::PASS );
    }
    else
    {
        _spTestResult->AddSingleResult(
            L"REQ_EP_150",
            this->GetMobSlotName( _spEdgeMap, node) + L" is not an Event Mob Slot but contains a CommentMarker.",
            TestResult::FAIL );
        testPassed = false;
    }
    
    //Update State
    //If the traversal is going to stop, then the parent should not change as
    //this nodes children will not be visited, and this node will not be post
    //order visited.
    if ( testPassed )
    {
        _isParentMobSlot.push( false );
    }
    
    return testPassed;
}

bool EPAnnotationVisitor::PreOrderVisit( AAFTypedObjNode<IAAFComponent>& node )
{
    
    AxComponent axComponent( node.GetAAFObjectOfType() );
    
    //Need to ensure that parent is a comment marker or mob slot
    if ( axComponent.CountComments() != 0 )
    {
        //Component::UserComments is being used
        if ( !_isParentMobSlot.top() )
        {
            //Parent is not a MobSlot and this is not a CommentMarker so fail
            //the test.
            _spTestResult->AddSingleResult(
                L"REQ_EP_147",
                this->GetMobSlotName( _spEdgeMap, node ) + L" contains a Component that illegally uses the Component::UserComments property.",
                TestResult::FAIL );
            return false;
        }
    }

    //Update state
    _isParentMobSlot.push( false );
    return true;
}

/*
 * 
 * Mob Slot/Tracks
 * 
 */

bool EPAnnotationVisitor::PreOrderVisit( EPTypedObjNode<IAAFTimelineMobSlot, EPAudioTrack>& node )
{
    shared_ptr<EPTypedObjNode<IAAFMobSlot, EPEssenceTrack> > spGeneric( node.DownCast<IAAFMobSlot, EPEssenceTrack>() );
    return this->PreOrderVisit( *spGeneric );
}

bool EPAnnotationVisitor::PreOrderVisit( EPTypedObjNode<IAAFTimelineMobSlot, EPVideoTrack>& node )
{
    shared_ptr<EPTypedObjNode<IAAFMobSlot, EPEssenceTrack> > spGeneric( node.DownCast<IAAFMobSlot, EPEssenceTrack>() );
    return this->PreOrderVisit( *spGeneric );
}

bool EPAnnotationVisitor::PreOrderVisit( EPTypedObjNode<IAAFTimelineMobSlot, EPEssenceTrack>& node )
{
    shared_ptr<EPTypedObjNode<IAAFMobSlot, EPEssenceTrack> > spGeneric( node.DownCast<IAAFMobSlot, EPEssenceTrack>() );
    return this->PreOrderVisit( *spGeneric );
}

bool EPAnnotationVisitor::PreOrderVisit( EPTypedObjNode<IAAFTimelineMobSlot, EPTimecodeTrack>& node )
{
    shared_ptr<EPTypedObjNode<IAAFMobSlot, EPNonEssenceTrack> > spGeneric( node.DownCast<IAAFMobSlot, EPNonEssenceTrack>() );
    return this->PreOrderVisit( *spGeneric );
}

bool EPAnnotationVisitor::PreOrderVisit( EPTypedObjNode<IAAFTimelineMobSlot, EPEdgecodeTrack>& node )
{
    shared_ptr<EPTypedObjNode<IAAFMobSlot, EPNonEssenceTrack> > spGeneric( node.DownCast<IAAFMobSlot, EPNonEssenceTrack>() );
    return this->PreOrderVisit( *spGeneric );
}

bool EPAnnotationVisitor::PreOrderVisit( EPTypedObjNode<IAAFTimelineMobSlot, EPNonEssenceTrack>& node )
{
    shared_ptr<EPTypedObjNode<IAAFMobSlot, EPNonEssenceTrack> > spGeneric( node.DownCast<IAAFMobSlot, EPNonEssenceTrack>() );
    return this->PreOrderVisit( *spGeneric );
}

bool EPAnnotationVisitor::PreOrderVisit( EPTypedObjNode<IAAFStaticMobSlot, EPAudioTrack>& node )
{
    shared_ptr<EPTypedObjNode<IAAFMobSlot, EPEssenceTrack> > spGeneric( node.DownCast<IAAFMobSlot, EPEssenceTrack>() );
    return this->PreOrderVisit( *spGeneric );
}

bool EPAnnotationVisitor::PreOrderVisit( EPTypedObjNode<IAAFStaticMobSlot, EPVideoTrack>& node )
{
    shared_ptr<EPTypedObjNode<IAAFMobSlot, EPEssenceTrack> > spGeneric( node.DownCast<IAAFMobSlot, EPEssenceTrack>() );
    return this->PreOrderVisit( *spGeneric );
}

bool EPAnnotationVisitor::PreOrderVisit( EPTypedObjNode<IAAFStaticMobSlot, EPEssenceTrack>& node )
{
    shared_ptr<EPTypedObjNode<IAAFMobSlot, EPEssenceTrack> > spGeneric( node.DownCast<IAAFMobSlot, EPEssenceTrack>() );
    return this->PreOrderVisit( *spGeneric );
}

bool EPAnnotationVisitor::PreOrderVisit( EPTypedObjNode<IAAFStaticMobSlot, EPTimecodeTrack>& node )
{
    shared_ptr<EPTypedObjNode<IAAFMobSlot, EPNonEssenceTrack> > spGeneric( node.DownCast<IAAFMobSlot, EPNonEssenceTrack>() );
    return this->PreOrderVisit( *spGeneric );
}

bool EPAnnotationVisitor::PreOrderVisit( EPTypedObjNode<IAAFStaticMobSlot, EPEdgecodeTrack>& node )
{
    shared_ptr<EPTypedObjNode<IAAFMobSlot, EPNonEssenceTrack> > spGeneric( node.DownCast<IAAFMobSlot, EPNonEssenceTrack>() );
    return this->PreOrderVisit( *spGeneric );
}

bool EPAnnotationVisitor::PreOrderVisit( EPTypedObjNode<IAAFStaticMobSlot, EPNonEssenceTrack>& node )
{
    shared_ptr<EPTypedObjNode<IAAFMobSlot, EPNonEssenceTrack> > spGeneric( node.DownCast<IAAFMobSlot, EPNonEssenceTrack>() );
    return this->PreOrderVisit( *spGeneric );
}

bool EPAnnotationVisitor::PreOrderVisit( EPTypedObjNode<IAAFEventMobSlot, EPAudioTrack>& node )
{
    shared_ptr<EPTypedObjNode<IAAFEventMobSlot, EPEssenceTrack> > spGeneric( node.DownCast<IAAFEventMobSlot, EPEssenceTrack>() );
    return this->PreOrderVisit( *spGeneric );
}

bool EPAnnotationVisitor::PreOrderVisit( EPTypedObjNode<IAAFEventMobSlot, EPVideoTrack>& node )
{
    shared_ptr<EPTypedObjNode<IAAFEventMobSlot, EPEssenceTrack> > spGeneric( node.DownCast<IAAFEventMobSlot, EPEssenceTrack>() );
    return this->PreOrderVisit( *spGeneric );
}

bool EPAnnotationVisitor::PreOrderVisit( EPTypedObjNode<IAAFEventMobSlot, EPTimecodeTrack>& node )
{
    shared_ptr<EPTypedObjNode<IAAFEventMobSlot, EPNonEssenceTrack> > spGeneric( node.DownCast<IAAFEventMobSlot, EPNonEssenceTrack>() );
    return this->PreOrderVisit( *spGeneric );
}

bool EPAnnotationVisitor::PreOrderVisit( EPTypedObjNode<IAAFEventMobSlot, EPEdgecodeTrack>& node )
{
    shared_ptr<EPTypedObjNode<IAAFEventMobSlot, EPNonEssenceTrack> > spGeneric( node.DownCast<IAAFEventMobSlot, EPNonEssenceTrack>() );
    return this->PreOrderVisit( *spGeneric );
}

bool EPAnnotationVisitor::PreOrderVisit( EPTypedObjNode<IAAFMobSlot, EPAudioTrack>& node )
{
    shared_ptr<EPTypedObjNode<IAAFMobSlot, EPEssenceTrack> > spGeneric( node.DownCast<IAAFMobSlot, EPEssenceTrack>() );
    return this->PreOrderVisit( *spGeneric );
}

bool EPAnnotationVisitor::PreOrderVisit( EPTypedObjNode<IAAFMobSlot, EPVideoTrack>& node )
{
    shared_ptr<EPTypedObjNode<IAAFMobSlot, EPEssenceTrack> > spGeneric( node.DownCast<IAAFMobSlot, EPEssenceTrack>() );
    return this->PreOrderVisit( *spGeneric );
}

bool EPAnnotationVisitor::PreOrderVisit( EPTypedObjNode<IAAFMobSlot, EPTimecodeTrack>& node )
{
    shared_ptr<EPTypedObjNode<IAAFMobSlot, EPNonEssenceTrack> > spGeneric( node.DownCast<IAAFMobSlot, EPNonEssenceTrack>() );
    return this->PreOrderVisit( *spGeneric );
}

bool EPAnnotationVisitor::PreOrderVisit( EPTypedObjNode<IAAFMobSlot, EPEdgecodeTrack>& node )
{
    shared_ptr<EPTypedObjNode<IAAFMobSlot, EPNonEssenceTrack> > spGeneric( node.DownCast<IAAFMobSlot, EPNonEssenceTrack>() );
    return this->PreOrderVisit( *spGeneric );
}

//Base cases:
bool EPAnnotationVisitor::PreOrderVisit( EPTypedObjNode<IAAFEventMobSlot, EPEssenceTrack>& node )
{
    _isAncestorEventMobSlot.push( true );
    _isParentMobSlot.push( true );
    _isAncestorEssenceTrack.push( true );
    return true;
}

bool EPAnnotationVisitor::PreOrderVisit( EPTypedObjNode<IAAFEventMobSlot, EPNonEssenceTrack>& node )
{
    _isAncestorEventMobSlot.push( true );
    _isParentMobSlot.push( true );
    _isAncestorEssenceTrack.push( false );
    return true;
}

bool EPAnnotationVisitor::PreOrderVisit( EPTypedObjNode<IAAFMobSlot, EPEssenceTrack>& node )
{
    _isAncestorEventMobSlot.push( false );
    _isParentMobSlot.push( true );
    _isAncestorEssenceTrack.push( true );
    return true;
}

bool EPAnnotationVisitor::PreOrderVisit( EPTypedObjNode<IAAFMobSlot, EPNonEssenceTrack>& node )
{
    _isAncestorEventMobSlot.push( false );
    _isParentMobSlot.push( true );
    _isAncestorEssenceTrack.push( false );
    return true;
}

/*
 * 
 * Post-Order Visits
 * 
 */
 
bool EPAnnotationVisitor::PostOrderVisit( AAFTypedObjNode<IAAFComponent>& node )
{
    _isParentMobSlot.pop();
    return true;
}

bool EPAnnotationVisitor::PostOrderVisit( AAFTypedObjNode<IAAFTransition>& node )
{
    _isParentMobSlot.pop();
    return true;
}

bool EPAnnotationVisitor::PostOrderVisit( AAFTypedObjNode<IAAFSegment>& node )
{
    _isParentMobSlot.pop();
    return true;
}

bool EPAnnotationVisitor::PostOrderVisit( AAFTypedObjNode<IAAFSequence>& node )
{
    _isParentMobSlot.pop();
    return true;
}

bool EPAnnotationVisitor::PostOrderVisit( AAFTypedObjNode<IAAFFiller>& node )
{
    _isParentMobSlot.pop();
    return true;
}

bool EPAnnotationVisitor::PostOrderVisit( AAFTypedObjNode<IAAFSourceReference>& node )
{
    _isParentMobSlot.pop();
    return true;
}

bool EPAnnotationVisitor::PostOrderVisit( AAFTypedObjNode<IAAFSourceClip>& node )
{
    _isParentMobSlot.pop();
    return true;
}

bool EPAnnotationVisitor::PostOrderVisit( AAFTypedObjNode<IAAFEvent>& node )
{
    _isParentMobSlot.pop();
    return true;
}

bool EPAnnotationVisitor::PostOrderVisit( AAFTypedObjNode<IAAFGPITrigger>& node )
{
    _isParentMobSlot.pop();
    return true;
}

bool EPAnnotationVisitor::PostOrderVisit( AAFTypedObjNode<IAAFCommentMarker>& node )
{
    _isParentMobSlot.pop();
    return true;
}

bool EPAnnotationVisitor::PostOrderVisit( AAFTypedObjNode<IAAFDescriptiveMarker>& node )
{
    _isParentMobSlot.pop();
    return true;
}

bool EPAnnotationVisitor::PostOrderVisit( AAFTypedObjNode<IAAFEdgecode>& node )
{
    _isParentMobSlot.pop();
    return true;
}

bool EPAnnotationVisitor::PostOrderVisit( AAFTypedObjNode<IAAFTimecode>& node )
{
    _isParentMobSlot.pop();
    return true;
}

bool EPAnnotationVisitor::PostOrderVisit( AAFTypedObjNode<IAAFTimecodeStream>& node )
{
    _isParentMobSlot.pop();
    return true;
}

bool EPAnnotationVisitor::PostOrderVisit( AAFTypedObjNode<IAAFTimecodeStream12M>& node )
{
    _isParentMobSlot.pop();
    return true;
}

bool EPAnnotationVisitor::PostOrderVisit( AAFTypedObjNode<IAAFPulldown>& node )
{
    _isParentMobSlot.pop();
    return true;
}

bool EPAnnotationVisitor::PostOrderVisit( AAFTypedObjNode<IAAFOperationGroup>& node )
{
    _isParentMobSlot.pop();
    return true;
}

bool EPAnnotationVisitor::PostOrderVisit( AAFTypedObjNode<IAAFNestedScope>& node )
{
    _isParentMobSlot.pop();
    return true;
}

bool EPAnnotationVisitor::PostOrderVisit( AAFTypedObjNode<IAAFScopeReference>& node )
{
    _isParentMobSlot.pop();
    return true;
}

bool EPAnnotationVisitor::PostOrderVisit( AAFTypedObjNode<IAAFEssenceGroup>& node )
{
    _isParentMobSlot.pop();
    return true;
}

bool EPAnnotationVisitor::PostOrderVisit( AAFTypedObjNode<IAAFSelector>& node )
{
    _isParentMobSlot.pop();
    return true;
}

bool EPAnnotationVisitor::PostOrderVisit( AAFTypedObjNode<IAAFTimelineMobSlot>& node )
{
    return PopStacks();
}

bool EPAnnotationVisitor::PostOrderVisit( AAFTypedObjNode<IAAFStaticMobSlot>& node )
{
    return PopStacks();
}

bool EPAnnotationVisitor::PostOrderVisit( AAFTypedObjNode<IAAFEventMobSlot>& node )
{
    return PopStacks();
}

bool EPAnnotationVisitor::PostOrderVisit( AAFTypedObjNode<IAAFMobSlot>& node )
{
    return PopStacks();
}

/*
 * 
 * TaggedValue/KLVData/Definitions
 * 
 */
 
bool EPAnnotationVisitor::PreOrderVisit( AAFTypedObjNode<IAAFTaggedValue>& node )
{
    AxTaggedValue axTaggedVal( node.GetAAFObjectOfType() );
    _taggedValueNames.insert( axTaggedVal.GetName() );
    return false;
}

bool EPAnnotationVisitor::PreOrderVisit( AAFTypedObjNode<IAAFKLVData>& node )
{
    AxKLVData axKLVData( node.GetAAFObjectOfType() );
    _klvDataKeys.insert( axKLVData.GetKey() );
    return false;
}

bool EPAnnotationVisitor::PreOrderVisit( AAFTypedObjNode<IAAFTaggedValueDefinition>& node )
{
    AxTaggedValueDef axTaggedValDef( node.GetAAFObjectOfType() );
    _taggedValueDefs.insert( axTaggedValDef.GetName() );
    return false;
}

bool EPAnnotationVisitor::PreOrderVisit( AAFTypedObjNode<IAAFKLVDataDefinition>& node )
{
    AxKLVDataDef axKLVDataDef( node.GetAAFObjectOfType() );
    _klvDataDefs.insert( axKLVDataDef.GetAUID() );
    return false;
}

/*
 * 
 * Helper Functions
 * 
 */

    
bool EPAnnotationVisitor::PopStacks()
{
    _isAncestorEssenceTrack.pop();
    _isAncestorEventMobSlot.pop();
    _isParentMobSlot.pop();

    return true;
}

/*
 * 
 * Check Functions
 * 
 */
void EPAnnotationVisitor::CheckForTaggedValueDefinitions()
{    
    //Remove all registered TaggedValues from the registered set.
    set<AxString>::const_iterator dIter;
    for ( dIter = _taggedValueDefs.begin(); dIter != _taggedValueDefs.end(); dIter++ )
    {
        _taggedValueNames.erase( *dIter );
    }
    
    //Fail for every unregistered TaggedValue.
    set<AxString>::const_iterator uIter;
    for ( uIter = _taggedValueNames.begin(); uIter != _taggedValueNames.end(); uIter++ )
    {
        _spTestResult->AddSingleResult(
            L"REQ_EP_151",
            L"TaggedValue \"" + *uIter + L"\" is not documented in the dictionary.",
            TestResult::FAIL );
    }
}

void EPAnnotationVisitor::CheckForKLVValueDefinitions()
{
    //Remove all registered TaggedValues from the registered set.
    set<aafUID_t>::const_iterator dIter;
    for ( dIter = _klvDataDefs.begin(); dIter != _klvDataDefs.end(); dIter++ )
    {
        _klvDataKeys.erase( *dIter );
    }
    
    //Fail for every unregistered TaggedValue.
    set<aafUID_t>::const_iterator uIter;
    for ( uIter = _klvDataKeys.begin(); uIter != _klvDataKeys.end(); uIter++ )
    {
        _spTestResult->AddSingleResult(
            L"REQ_EP_152",
            L"KLVData with key " + AxStringUtil::uid2Str(*uIter) + L" is not documented in the dictionary.",
            TestResult::FAIL );
    }
}
   
} // end of namespace aafanalyzer
