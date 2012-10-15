//=---------------------------------------------------------------------=
//
// $Id$ $Name$
//
// The contents of this file are subject to the AAF SDK Public Source
// License Agreement Version 2.0 (the "License"); You may not use this
// file except in compliance with the License.  The License is available
// in AAFSDKPSL.TXT, or you may obtain a copy of the License from the
// Advanced Media Workflow Association, Inc., or its successor.
//
// Software distributed under the License is distributed on an "AS IS"
// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
// the License for the specific language governing rights and limitations
// under the License.  Refer to Section 3.3 of the License for proper use
// of this Exhibit.
//
// WARNING:  Please contact the Advanced Media Workflow Association,
// Inc., for more information about any additional licenses to
// intellectual property covering the AAF Standard that may be required
// to create and distribute AAF compliant products.
// (http://www.amwa.tv/policies).
//
// Copyright Notices:
// The Original Code of this file is Copyright 1998-2009, licensor of the
// Advanced Media Workflow Association.  All rights reserved.
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

 
EPAnnotationVisitor::EPAnnotationVisitor( wostream& log,
                                          boost::shared_ptr<EdgeMap> spEdgeMap,
                                          boost::shared_ptr<TestLevelTestResult> spTestResult )
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
    boost::shared_ptr<AAFTypedObjNode<IAAFComponent> > spGeneric( node.DownCastToAAF<IAAFComponent>() );
    return this->PreOrderVisit( *spGeneric );
}

bool EPAnnotationVisitor::PreOrderVisit( AAFTypedObjNode<IAAFSegment>& node )
{
    boost::shared_ptr<AAFTypedObjNode<IAAFComponent> > spGeneric( node.DownCastToAAF<IAAFComponent>() );
    return this->PreOrderVisit( *spGeneric );
}

bool EPAnnotationVisitor::PreOrderVisit( AAFTypedObjNode<IAAFSequence>& node )
{
    boost::shared_ptr<AAFTypedObjNode<IAAFComponent> > spGeneric( node.DownCastToAAF<IAAFComponent>() );
    return this->PreOrderVisit( *spGeneric );
}

bool EPAnnotationVisitor::PreOrderVisit( AAFTypedObjNode<IAAFFiller>& node )
{
    boost::shared_ptr<AAFTypedObjNode<IAAFComponent> > spGeneric( node.DownCastToAAF<IAAFComponent>() );
    return this->PreOrderVisit( *spGeneric );
}

bool EPAnnotationVisitor::PreOrderVisit( AAFTypedObjNode<IAAFSourceReference>& node )
{
    boost::shared_ptr<AAFTypedObjNode<IAAFComponent> > spGeneric( node.DownCastToAAF<IAAFComponent>() );
    return this->PreOrderVisit( *spGeneric );
}

bool EPAnnotationVisitor::PreOrderVisit( AAFTypedObjNode<IAAFSourceClip>& node )
{
    boost::shared_ptr<AAFTypedObjNode<IAAFComponent> > spGeneric( node.DownCastToAAF<IAAFComponent>() );
    return this->PreOrderVisit( *spGeneric );
}

bool EPAnnotationVisitor::PreOrderVisit( AAFTypedObjNode<IAAFEvent>& node )
{
    boost::shared_ptr<AAFTypedObjNode<IAAFComponent> > spGeneric( node.DownCastToAAF<IAAFComponent>() );
    return this->PreOrderVisit( *spGeneric );
}

bool EPAnnotationVisitor::PreOrderVisit( AAFTypedObjNode<IAAFGPITrigger>& node )
{
    boost::shared_ptr<AAFTypedObjNode<IAAFComponent> > spGeneric( node.DownCastToAAF<IAAFComponent>() );
    return this->PreOrderVisit( *spGeneric );
}

bool EPAnnotationVisitor::PreOrderVisit( AAFTypedObjNode<IAAFEdgecode>& node )
{
    boost::shared_ptr<AAFTypedObjNode<IAAFComponent> > spGeneric( node.DownCastToAAF<IAAFComponent>() );
    return this->PreOrderVisit( *spGeneric );
}

bool EPAnnotationVisitor::PreOrderVisit( AAFTypedObjNode<IAAFTimecode>& node )
{
    boost::shared_ptr<AAFTypedObjNode<IAAFComponent> > spGeneric( node.DownCastToAAF<IAAFComponent>() );
    return this->PreOrderVisit( *spGeneric );
}

bool EPAnnotationVisitor::PreOrderVisit( AAFTypedObjNode<IAAFTimecodeStream>& node )
{
    boost::shared_ptr<AAFTypedObjNode<IAAFComponent> > spGeneric( node.DownCastToAAF<IAAFComponent>() );
    return this->PreOrderVisit( *spGeneric );
}

bool EPAnnotationVisitor::PreOrderVisit( AAFTypedObjNode<IAAFTimecodeStream12M>& node )
{
    boost::shared_ptr<AAFTypedObjNode<IAAFComponent> > spGeneric( node.DownCastToAAF<IAAFComponent>() );
    return this->PreOrderVisit( *spGeneric );
}

bool EPAnnotationVisitor::PreOrderVisit( AAFTypedObjNode<IAAFPulldown>& node )
{
    boost::shared_ptr<AAFTypedObjNode<IAAFComponent> > spGeneric( node.DownCastToAAF<IAAFComponent>() );
    return this->PreOrderVisit( *spGeneric );
}

bool EPAnnotationVisitor::PreOrderVisit( AAFTypedObjNode<IAAFOperationGroup>& node )
{
    boost::shared_ptr<AAFTypedObjNode<IAAFComponent> > spGeneric( node.DownCastToAAF<IAAFComponent>() );
    return this->PreOrderVisit( *spGeneric );
}

bool EPAnnotationVisitor::PreOrderVisit( AAFTypedObjNode<IAAFNestedScope>& node )
{
    boost::shared_ptr<AAFTypedObjNode<IAAFComponent> > spGeneric( node.DownCastToAAF<IAAFComponent>() );
    return this->PreOrderVisit( *spGeneric );
}

bool EPAnnotationVisitor::PreOrderVisit( AAFTypedObjNode<IAAFScopeReference>& node )
{
    boost::shared_ptr<AAFTypedObjNode<IAAFComponent> > spGeneric( node.DownCastToAAF<IAAFComponent>() );
    return this->PreOrderVisit( *spGeneric );
}

bool EPAnnotationVisitor::PreOrderVisit( AAFTypedObjNode<IAAFEssenceGroup>& node )
{
    boost::shared_ptr<AAFTypedObjNode<IAAFComponent> > spGeneric( node.DownCastToAAF<IAAFComponent>() );
    return this->PreOrderVisit( *spGeneric );
}

bool EPAnnotationVisitor::PreOrderVisit( AAFTypedObjNode<IAAFSelector>& node )
{
    boost::shared_ptr<AAFTypedObjNode<IAAFComponent> > spGeneric( node.DownCastToAAF<IAAFComponent>() );
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
            TestResult::PASS,
	    node );
    }
    else
    {
        _spTestResult->AddSingleResult(
            L"REQ_EP_150",
            this->GetMobSlotName( _spEdgeMap, node) + L" is not an Event Mob Slot but contains a CommentMarker.",
            TestResult::FAIL,
	    node );

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
                TestResult::FAIL,
		node );
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
    boost::shared_ptr<EPTypedObjNode<IAAFMobSlot, EPEssenceTrack> > spGeneric( node.DownCast<IAAFMobSlot, EPEssenceTrack>() );
    return this->PreOrderVisit( *spGeneric );
}

bool EPAnnotationVisitor::PreOrderVisit( EPTypedObjNode<IAAFTimelineMobSlot, EPVideoTrack>& node )
{
    boost::shared_ptr<EPTypedObjNode<IAAFMobSlot, EPEssenceTrack> > spGeneric( node.DownCast<IAAFMobSlot, EPEssenceTrack>() );
    return this->PreOrderVisit( *spGeneric );
}

bool EPAnnotationVisitor::PreOrderVisit( EPTypedObjNode<IAAFTimelineMobSlot, EPEssenceTrack>& node )
{
    boost::shared_ptr<EPTypedObjNode<IAAFMobSlot, EPEssenceTrack> > spGeneric( node.DownCast<IAAFMobSlot, EPEssenceTrack>() );
    return this->PreOrderVisit( *spGeneric );
}

bool EPAnnotationVisitor::PreOrderVisit( EPTypedObjNode<IAAFTimelineMobSlot, EPTimecodeTrack>& node )
{
    boost::shared_ptr<EPTypedObjNode<IAAFMobSlot, EPNonEssenceTrack> > spGeneric( node.DownCast<IAAFMobSlot, EPNonEssenceTrack>() );
    return this->PreOrderVisit( *spGeneric );
}

bool EPAnnotationVisitor::PreOrderVisit( EPTypedObjNode<IAAFTimelineMobSlot, EPEdgecodeTrack>& node )
{
    boost::shared_ptr<EPTypedObjNode<IAAFMobSlot, EPNonEssenceTrack> > spGeneric( node.DownCast<IAAFMobSlot, EPNonEssenceTrack>() );
    return this->PreOrderVisit( *spGeneric );
}

bool EPAnnotationVisitor::PreOrderVisit( EPTypedObjNode<IAAFTimelineMobSlot, EPNonEssenceTrack>& node )
{
    boost::shared_ptr<EPTypedObjNode<IAAFMobSlot, EPNonEssenceTrack> > spGeneric( node.DownCast<IAAFMobSlot, EPNonEssenceTrack>() );
    return this->PreOrderVisit( *spGeneric );
}

bool EPAnnotationVisitor::PreOrderVisit( EPTypedObjNode<IAAFStaticMobSlot, EPAudioTrack>& node )
{
    boost::shared_ptr<EPTypedObjNode<IAAFMobSlot, EPEssenceTrack> > spGeneric( node.DownCast<IAAFMobSlot, EPEssenceTrack>() );
    return this->PreOrderVisit( *spGeneric );
}

bool EPAnnotationVisitor::PreOrderVisit( EPTypedObjNode<IAAFStaticMobSlot, EPVideoTrack>& node )
{
    boost::shared_ptr<EPTypedObjNode<IAAFMobSlot, EPEssenceTrack> > spGeneric( node.DownCast<IAAFMobSlot, EPEssenceTrack>() );
    return this->PreOrderVisit( *spGeneric );
}

bool EPAnnotationVisitor::PreOrderVisit( EPTypedObjNode<IAAFStaticMobSlot, EPEssenceTrack>& node )
{
    boost::shared_ptr<EPTypedObjNode<IAAFMobSlot, EPEssenceTrack> > spGeneric( node.DownCast<IAAFMobSlot, EPEssenceTrack>() );
    return this->PreOrderVisit( *spGeneric );
}

bool EPAnnotationVisitor::PreOrderVisit( EPTypedObjNode<IAAFStaticMobSlot, EPTimecodeTrack>& node )
{
    boost::shared_ptr<EPTypedObjNode<IAAFMobSlot, EPNonEssenceTrack> > spGeneric( node.DownCast<IAAFMobSlot, EPNonEssenceTrack>() );
    return this->PreOrderVisit( *spGeneric );
}

bool EPAnnotationVisitor::PreOrderVisit( EPTypedObjNode<IAAFStaticMobSlot, EPEdgecodeTrack>& node )
{
    boost::shared_ptr<EPTypedObjNode<IAAFMobSlot, EPNonEssenceTrack> > spGeneric( node.DownCast<IAAFMobSlot, EPNonEssenceTrack>() );
    return this->PreOrderVisit( *spGeneric );
}

bool EPAnnotationVisitor::PreOrderVisit( EPTypedObjNode<IAAFStaticMobSlot, EPNonEssenceTrack>& node )
{
    boost::shared_ptr<EPTypedObjNode<IAAFMobSlot, EPNonEssenceTrack> > spGeneric( node.DownCast<IAAFMobSlot, EPNonEssenceTrack>() );
    return this->PreOrderVisit( *spGeneric );
}

bool EPAnnotationVisitor::PreOrderVisit( EPTypedObjNode<IAAFEventMobSlot, EPAudioTrack>& node )
{
    boost::shared_ptr<EPTypedObjNode<IAAFEventMobSlot, EPEssenceTrack> > spGeneric( node.DownCast<IAAFEventMobSlot, EPEssenceTrack>() );
    return this->PreOrderVisit( *spGeneric );
}

bool EPAnnotationVisitor::PreOrderVisit( EPTypedObjNode<IAAFEventMobSlot, EPVideoTrack>& node )
{
    boost::shared_ptr<EPTypedObjNode<IAAFEventMobSlot, EPEssenceTrack> > spGeneric( node.DownCast<IAAFEventMobSlot, EPEssenceTrack>() );
    return this->PreOrderVisit( *spGeneric );
}

bool EPAnnotationVisitor::PreOrderVisit( EPTypedObjNode<IAAFEventMobSlot, EPTimecodeTrack>& node )
{
    boost::shared_ptr<EPTypedObjNode<IAAFEventMobSlot, EPNonEssenceTrack> > spGeneric( node.DownCast<IAAFEventMobSlot, EPNonEssenceTrack>() );
    return this->PreOrderVisit( *spGeneric );
}

bool EPAnnotationVisitor::PreOrderVisit( EPTypedObjNode<IAAFEventMobSlot, EPEdgecodeTrack>& node )
{
    boost::shared_ptr<EPTypedObjNode<IAAFEventMobSlot, EPNonEssenceTrack> > spGeneric( node.DownCast<IAAFEventMobSlot, EPNonEssenceTrack>() );
    return this->PreOrderVisit( *spGeneric );
}

bool EPAnnotationVisitor::PreOrderVisit( EPTypedObjNode<IAAFMobSlot, EPAudioTrack>& node )
{
    boost::shared_ptr<EPTypedObjNode<IAAFMobSlot, EPEssenceTrack> > spGeneric( node.DownCast<IAAFMobSlot, EPEssenceTrack>() );
    return this->PreOrderVisit( *spGeneric );
}

bool EPAnnotationVisitor::PreOrderVisit( EPTypedObjNode<IAAFMobSlot, EPVideoTrack>& node )
{
    boost::shared_ptr<EPTypedObjNode<IAAFMobSlot, EPEssenceTrack> > spGeneric( node.DownCast<IAAFMobSlot, EPEssenceTrack>() );
    return this->PreOrderVisit( *spGeneric );
}

bool EPAnnotationVisitor::PreOrderVisit( EPTypedObjNode<IAAFMobSlot, EPTimecodeTrack>& node )
{
    boost::shared_ptr<EPTypedObjNode<IAAFMobSlot, EPNonEssenceTrack> > spGeneric( node.DownCast<IAAFMobSlot, EPNonEssenceTrack>() );
    return this->PreOrderVisit( *spGeneric );
}

bool EPAnnotationVisitor::PreOrderVisit( EPTypedObjNode<IAAFMobSlot, EPEdgecodeTrack>& node )
{
    boost::shared_ptr<EPTypedObjNode<IAAFMobSlot, EPNonEssenceTrack> > spGeneric( node.DownCast<IAAFMobSlot, EPNonEssenceTrack>() );
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
    _taggedValueNames[axTaggedVal.GetName()] = node.GetSharedPointerToNode();
    return false;
}

bool EPAnnotationVisitor::PreOrderVisit( AAFTypedObjNode<IAAFKLVData>& node )
{
    AxKLVData axKLVData( node.GetAAFObjectOfType() );
    _klvDataKeys[axKLVData.GetKey()] = node.GetSharedPointerToNode();
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
    
    //Fail for every unregistered TaggedValue
    map<AxString, boost::shared_ptr<Node> >::const_iterator uIter;
    for ( uIter = _taggedValueNames.begin(); uIter != _taggedValueNames.end(); uIter++ )
    {
        _spTestResult->AddSingleResult(
            L"REQ_EP_151",
            L"TaggedValue \"" + uIter->first + L"\" is not documented in the dictionary.",
            TestResult::FAIL,
	    *uIter->second );
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
    
    //Fail for every unregistered KLV value
    map<aafUID_t, boost::shared_ptr<Node> >::const_iterator uIter;
    for ( uIter = _klvDataKeys.begin(); uIter != _klvDataKeys.end(); uIter++ )
    {
        _spTestResult->AddSingleResult(
            L"REQ_EP_152",
            L"KLVData with key " + AxStringUtil::uid2Str(uIter->first) + L" is not documented in the dictionary.",
            TestResult::FAIL,
	    *uIter->second );
    }
}
   
} // end of namespace aafanalyzer
