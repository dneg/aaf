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
#include "EPTrackContentsVisitor.h"
#include "EPTrackContentsTest.h"

//Test/Result files
#include <DetailLevelTestResult.h>
#include <TestLevelTestResult.h>
#include <TestRegistry.h>

//Analyzer Base files
#include <DepthFirstTraversal.h>
#include <EdgeMap.h>

//Ax files
#include <AxMob.h>
#include <AxDefObject.h>
#include <AxMobSlot.h>
#include <AxComponent.h>
#include <AxMetaDef.h>

//AAF files
#include <AAFResult.h>

//STL files
#include <sstream>

namespace {
    
using namespace aafanalyzer;

class ParentMobVisitor : public EPTypedVisitor
{
    public:
        ParentMobVisitor()
        {}
        
        ~ParentMobVisitor()
        {}
        
        bool PreOrderVisit( AAFTypedObjNode<IAAFCompositionMob>& node )
        {
            _needsPhysicalNum = true;
            return false;
        }
        
        bool PreOrderVisit( AAFTypedObjNode<IAAFMasterMob>& node )
        {
            _needsPhysicalNum = true;
            return false;
        }

        bool PreOrderVisit( EPTypedObjNode<IAAFSourceMob, EPRGBAImageFileSource>& node )
        {
            shared_ptr<EPTypedObjNode<IAAFSourceMob, EPFileSource> > spGeneric( node.DownCast<IAAFSourceMob, EPFileSource>() );
            return this->PreOrderVisit( *spGeneric );
        }
        
        bool PreOrderVisit( EPTypedObjNode<IAAFSourceMob, EPCDCIImageFileSource>& node )
        {
            shared_ptr<EPTypedObjNode<IAAFSourceMob, EPFileSource> > spGeneric( node.DownCast<IAAFSourceMob, EPFileSource>() );
            return this->PreOrderVisit( *spGeneric );
        }
        
        bool PreOrderVisit( EPTypedObjNode<IAAFSourceMob, EPImageFileSource>& node )
        {
            shared_ptr<EPTypedObjNode<IAAFSourceMob, EPFileSource> > spGeneric( node.DownCast<IAAFSourceMob, EPFileSource>() );
            return this->PreOrderVisit( *spGeneric );
        }

        bool PreOrderVisit( EPTypedObjNode<IAAFSourceMob, EPMonoAudioFileSource>& node )
        {
            shared_ptr<EPTypedObjNode<IAAFSourceMob, EPFileSource> > spGeneric( node.DownCast<IAAFSourceMob, EPFileSource>() );
            return this->PreOrderVisit( *spGeneric );
        }
  
        bool PreOrderVisit( EPTypedObjNode<IAAFSourceMob, EPMultiChannelAudioFileSource>& node )
        {
            shared_ptr<EPTypedObjNode<IAAFSourceMob, EPFileSource> > spGeneric( node.DownCast<IAAFSourceMob, EPFileSource>() );
            return this->PreOrderVisit( *spGeneric );
        }

        bool PreOrderVisit( EPTypedObjNode<IAAFSourceMob, EPFileSource>& node )
        {
            _needsPhysicalNum = false;
            return false;
        }
        
        bool PreOrderVisit( AAFTypedObjNode<IAAFSourceMob>& node )
        {
            _needsPhysicalNum = true;
            return false;
        }
        
        bool PreOrderVisit( AAFTypedObjNode<IAAFMob>& node )
        {
            _needsPhysicalNum = false;
            return false;
        }
        
        const bool NeedsPhysicalTrackNum()
        {
            return _needsPhysicalNum;
        }
        
    private:
    
        bool _needsPhysicalNum;
    
        // prohibited
        ParentMobVisitor( const ParentMobVisitor& );
        ParentMobVisitor& operator=( const ParentMobVisitor& );
};

} // end of namespace

//======================================================================

namespace aafanalyzer {

using namespace boost;
 
EPTrackContentsVisitor::EPTrackContentsVisitor( wostream& log,
                                                shared_ptr<EdgeMap> spEdgeMap,
                                                shared_ptr<TestLevelTestResult> spTestResult )
    : _log(log),
      _spEdgeMap( spEdgeMap ),
      _spTestResult( spTestResult )
{}
    
EPTrackContentsVisitor::~EPTrackContentsVisitor()
{}

bool EPTrackContentsVisitor::PreOrderVisit( EPTypedObjNode<IAAFTimelineMobSlot, EPAudioTrack>& node )
{
    bool testPassed;
    shared_ptr<EPTypedObjNode<IAAFMobSlot, EPEssenceTrack> > spGenericEP( node.DownCast<IAAFMobSlot, EPEssenceTrack>() );
    testPassed = this->PreOrderVisit( *spGenericEP );
    shared_ptr<AAFTypedObjNode<IAAFTimelineMobSlot> > spGenericAAF( node.DownCastToAAF<IAAFTimelineMobSlot>() );
    testPassed = this->PreOrderVisit( *spGenericAAF ) && testPassed;
    return testPassed;
}

bool EPTrackContentsVisitor::PreOrderVisit( EPTypedObjNode<IAAFTimelineMobSlot, EPVideoTrack>& node )
{
    bool testPassed;
    shared_ptr<EPTypedObjNode<IAAFMobSlot, EPEssenceTrack> > spGenericEP( node.DownCast<IAAFMobSlot, EPEssenceTrack>() );
    testPassed = this->PreOrderVisit( *spGenericEP );
    shared_ptr<AAFTypedObjNode<IAAFTimelineMobSlot> > spGenericAAF( node.DownCastToAAF<IAAFTimelineMobSlot>() );
    testPassed = this->PreOrderVisit( *spGenericAAF ) && testPassed;
    return testPassed;
}

bool EPTrackContentsVisitor::PreOrderVisit( EPTypedObjNode<IAAFTimelineMobSlot, EPEssenceTrack>& node )
{
    bool testPassed;
    shared_ptr<EPTypedObjNode<IAAFMobSlot, EPEssenceTrack> > spGenericEP( node.DownCast<IAAFMobSlot, EPEssenceTrack>() );
    testPassed = this->PreOrderVisit( *spGenericEP );
    shared_ptr<AAFTypedObjNode<IAAFTimelineMobSlot> > spGenericAAF( node.DownCastToAAF<IAAFTimelineMobSlot>() );
    testPassed = this->PreOrderVisit( *spGenericAAF ) && testPassed;
    return testPassed;
}

bool EPTrackContentsVisitor::PreOrderVisit( AAFTypedObjNode<IAAFTimelineMobSlot>& node )
{
    AxTimelineMobSlot axMobSlot( node.GetAAFObjectOfType() );

    pair<bool,aafPosition_t> markIn  = axMobSlot.ExistsMarkIn();
    pair<bool,aafPosition_t> markOut = axMobSlot.ExistsMarkOut();
    pair<bool,aafPosition_t> userPos  = axMobSlot.ExistsUserPos();

    //
    // Add notation result indicating presence of mark in or mark out.
    //
            
    if ( markIn.first )
    {
      wstringstream ss;
      ss << this->GetMobSlotName( _spEdgeMap, node ) << " has a MarkIn property.";
      _spTestResult->AddSingleResult( L"REQ_EP_108.1", ss.str(), TestResult::NOTED, node );
    }

    if ( markOut.first )
    {
      wstringstream ss;
      ss << this->GetMobSlotName( _spEdgeMap, node ) << " has a MarkOut property.";
      _spTestResult->AddSingleResult( L"REQ_EP_108.2", ss.str(), TestResult::NOTED, node );
    }

    //
    // In both mark in and mark out exist then validate that out is
    // greater than in.
    // 

    if ( markIn.first && markOut.first )
    {
      TestResult::Result result = TestResult::UNDEFINED;
      wstringstream ss;
      ss << this->GetMobSlotName( _spEdgeMap, node );

      if ( markOut.second < markIn.second )
      {
	ss << L" has a marked in point that occurs after the marked out point (IN = "
	   << markIn.second << L"; OUT = " << markOut.second << ").";
	result = TestResult::FAIL;
      }
      else
      {
	ss << L" has valid mark in and mark out points (IN = "
	   << markIn.second << L"; OUT = " << markOut.second << ").";
	result = TestResult::NOTED;
      }

      _spTestResult->AddSingleResult( L"REQ_EP_108.3", ss.str(), result, node );
    }

    //
    // Check for UserPos, add notational result if it exists.
    //
    if ( userPos.first )
    {
      wstringstream ss;
      ss << this->GetMobSlotName( _spEdgeMap, node ) << " has a TimelineMobSlot::UserPos = " << userPos.second;
      _spTestResult->AddSingleResult( L"REQ_EP_108.4", ss.str(), TestResult::NOTED, node );
    }

    return true;    
}

bool EPTrackContentsVisitor::PreOrderVisit( EPTypedObjNode<IAAFStaticMobSlot, EPAudioTrack>& node )
{
    shared_ptr<EPTypedObjNode<IAAFMobSlot, EPEssenceTrack> > spGeneric( node.DownCast<IAAFMobSlot, EPEssenceTrack>() );
    return this->PreOrderVisit( *spGeneric );
}

bool EPTrackContentsVisitor::PreOrderVisit( EPTypedObjNode<IAAFStaticMobSlot, EPVideoTrack>& node )
{
    shared_ptr<EPTypedObjNode<IAAFMobSlot, EPEssenceTrack> > spGeneric( node.DownCast<IAAFMobSlot, EPEssenceTrack>() );
    return this->PreOrderVisit( *spGeneric );
}

bool EPTrackContentsVisitor::PreOrderVisit( EPTypedObjNode<IAAFStaticMobSlot, EPEssenceTrack>& node )
{
    shared_ptr<EPTypedObjNode<IAAFMobSlot, EPEssenceTrack> > spGeneric( node.DownCast<IAAFMobSlot, EPEssenceTrack>() );
    return this->PreOrderVisit( *spGeneric );
}

bool EPTrackContentsVisitor::PreOrderVisit( EPTypedObjNode<IAAFEventMobSlot, EPAudioTrack>& node )
{
    shared_ptr<EPTypedObjNode<IAAFMobSlot, EPEssenceTrack> > spGeneric( node.DownCast<IAAFMobSlot, EPEssenceTrack>() );
    return this->PreOrderVisit( *spGeneric );
}

bool EPTrackContentsVisitor::PreOrderVisit( EPTypedObjNode<IAAFEventMobSlot, EPVideoTrack>& node )
{
    shared_ptr<EPTypedObjNode<IAAFMobSlot, EPEssenceTrack> > spGeneric( node.DownCast<IAAFMobSlot, EPEssenceTrack>() );
    return this->PreOrderVisit( *spGeneric );
}

bool EPTrackContentsVisitor::PreOrderVisit( EPTypedObjNode<IAAFEventMobSlot, EPEssenceTrack>& node )
{
    shared_ptr<EPTypedObjNode<IAAFMobSlot, EPEssenceTrack> > spGeneric( node.DownCast<IAAFMobSlot, EPEssenceTrack>() );
    return this->PreOrderVisit( *spGeneric );
}

bool EPTrackContentsVisitor::PreOrderVisit( EPTypedObjNode<IAAFMobSlot, EPAudioTrack>& node )
{
    shared_ptr<EPTypedObjNode<IAAFMobSlot, EPEssenceTrack> > spGeneric( node.DownCast<IAAFMobSlot, EPEssenceTrack>() );
    return this->PreOrderVisit( *spGeneric );
}

bool EPTrackContentsVisitor::PreOrderVisit( EPTypedObjNode<IAAFMobSlot, EPVideoTrack>& node )
{
    shared_ptr<EPTypedObjNode<IAAFMobSlot, EPEssenceTrack> > spGeneric( node.DownCast<IAAFMobSlot, EPEssenceTrack>() );
    return this->PreOrderVisit( *spGeneric );
}

bool EPTrackContentsVisitor::PreOrderVisit( EPTypedObjNode<IAAFMobSlot, EPEssenceTrack>& node )
{
  // JPT REVIEW - This is awkward way to implement this. There isn't a
  // need to to search up the graph for the parent. Just keep track of
  // in on the way down.  Or, in the mob visitor, check the child
  // slots.Consider rewrite if the ParentMobVisitor ever needs work.

  // Find the parent of this mob slot, if it is a File Source mob
  // slot (or an unknown type), there is no requirement that it has a
  // PhysicalTrackNumber.
  
  shared_ptr<Node> spNode = dynamic_pointer_cast<Node>( node.GetSharedPointerToNode() );
  DepthFirstTraversal dfs( _spEdgeMap, spNode );
  shared_ptr<ParentMobVisitor> spVisitor( new ParentMobVisitor );
  
  dfs.TraverseUp( spVisitor );
  
  if ( spVisitor->NeedsPhysicalTrackNum() )
  {
    AxString mobName = this->GetMobName( _spEdgeMap, node );   

    AxMobSlot axMobSlot( node.GetAAFObjectOfType() );

    wstringstream ss;

    ss << L"Slot with ID ";
    ss << axMobSlot.GetSlotID();
    ss << L" in " << mobName;
    
    pair<bool,aafUInt32> physNum = axMobSlot.ExistsPhysicalNum();
      
    TestResult::Result result(TestResult::UNDEFINED);

    if ( physNum.first )
    {
      ss << L" has MobSlot::PhysicalTrackNumber = " << physNum.second;
      result = TestResult::NOTED;
    }
    else
    {
      ss << L" does not have a MobSlot::PhysicalTrackNumber property.";
      result = TestResult::FAIL;
    }

    _spTestResult->AddSingleResult( L"REQ_EP_103", ss.str(), result, node );
  }
  
  return true;
}

} // end of namespace aafanalyzer
