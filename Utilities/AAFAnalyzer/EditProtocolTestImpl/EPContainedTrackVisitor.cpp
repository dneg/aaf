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
#include "EPContainedTrackVisitor.h"
#include "EPContainedTrackTest.h"

//Test/Result files
#include <TestLevelTestResult.h>
#include <DetailLevelTestResult.h>
#include <TestRegistry.h>

//Analyzer Base files
#include <DepthFirstTraversal.h>

//Ax files
#include <AxMob.h>
#include <AxMetaDef.h>
#include <AxMobSlot.h>
#include <AxComponent.h>
#include <AxDefObject.h>

//AAF files
#include <AAFResult.h>
#include <AAFDataDefs.h>

//STL files
#include <sstream>

namespace {

using namespace aafanalyzer;

class AuxiliarySlotVisitor : public EPTypedVisitor
{
    public:
        AuxiliarySlotVisitor( boost::shared_ptr<EdgeMap> spEdgeMap,
                              boost::shared_ptr<TestLevelTestResult> spTestResult )
            : _spEdgeMap( spEdgeMap ),
              _spTestResult( spTestResult ),
              _testPassed( true )
        {}

        ~AuxiliarySlotVisitor()
        {}

        bool PreOrderVisit( AAFTypedObjNode<IAAFTimelineMobSlot>& node )
        {
            boost::shared_ptr<AAFTypedObjNode<IAAFMobSlot> > spGeneric( node.DownCastToAAF<IAAFMobSlot>() );
            return this->PreOrderVisit( *spGeneric );
        }

        bool PreOrderVisit( AAFTypedObjNode<IAAFEventMobSlot>& node )
        {
            boost::shared_ptr<AAFTypedObjNode<IAAFMobSlot> > spGeneric( node.DownCastToAAF<IAAFMobSlot>() );
            return this->PreOrderVisit( *spGeneric );
        }

        bool PreOrderVisit( AAFTypedObjNode<IAAFStaticMobSlot>& node )
        {
            boost::shared_ptr<AAFTypedObjNode<IAAFMobSlot> > spGeneric( node.DownCastToAAF<IAAFMobSlot>() );
            return this->PreOrderVisit( *spGeneric );
        }

        bool PreOrderVisit( AAFTypedObjNode<IAAFMobSlot>& node )
        {
            AxMobSlot axMobSlot( node.GetAAFObjectOfType() );
            AxSegment axSegment( axMobSlot.GetSegment() );
            AxDataDef dataDef ( axSegment.GetDataDef() );
            if ( dataDef.GetAUID() != kAAFDataDef_Auxiliary  )
            {
                _spTestResult->AddSingleResult(
                    L"REQ_EP_136",
                    this->GetMobSlotName( _spEdgeMap, node ) + L" has a segment with a Data Definition that is not DataDef_Auxiliary.",
                    TestResult::FAIL,
		    node );
                _testPassed = false;
            }
            return false;
        }

        bool AreSlotsOk()
        {
            return _testPassed;
        }

    private:

        boost::shared_ptr<EdgeMap> _spEdgeMap;
        boost::shared_ptr<TestLevelTestResult> _spTestResult;
        bool _testPassed;

        // prohibited
        AuxiliarySlotVisitor();
        AuxiliarySlotVisitor( const AuxiliarySlotVisitor& );
        AuxiliarySlotVisitor& operator=( const AuxiliarySlotVisitor& );
};

template<typename SegmentType>
class UniquelyTypedSegmentCounter : public TypedVisitor
{
    public:
        UniquelyTypedSegmentCounter():
            _segments( 0 ), _illegalNodes( false )
        {}

        ~UniquelyTypedSegmentCounter()
        {}

        //The first node passed to this visitor should be a MobSlot, so accept
        //MobSlots
        bool PreOrderVisit( AAFTypedObjNode<IAAFMobSlot>& node )
        {
            return true;
        }

        bool PreOrderVisit( AAFTypedObjNode<IAAFTimelineMobSlot>& node )
        {
            return true;
        }

        bool PreOrderVisit( AAFTypedObjNode<IAAFEventMobSlot>& node )
        {
            return true;
        }

        bool PreOrderVisit( AAFTypedObjNode<IAAFStaticMobSlot>& node )
        {
            return true;
        }

        bool PreOrderVisit( AAFTypedObjNode<IAAFSequence>& node )
        {
            //Sequences are ok, continue the traversal as long as no illegal
            //nodes have been encountered.
            return !_illegalNodes;
        }

        bool PreOrderVisit( AAFTypedObjNode<SegmentType>& node )
        {
            //Increment the number of typed segments, and stop traversing down.
            _segments++;

            /*
             *
             * Note: Currently this is being used for SourceClips, Timecodes and
             *       OperationGroups.  SourceClips and Timecodes do not contain
             *       any components so this should not cause a problem.
             *       OperationGroups do contain Components.  Until other
             *       information is provided, we will assume that we don't have
             *       to count Components contained within the Component we are
             *       looking for.  If this visitor is used for any other type
             *       of Component, the ramifications of sub-Components should
             *       be considered
             *
             */
             return false;
        }

        unsigned int GetNumSegments()
        {
            return _segments;
        }

        bool ContainsIllegalNodes()
        {
            return _illegalNodes;
        }

    private:
        unsigned int _segments;
        bool _illegalNodes;

        bool DefaultPreOrderBehaviour()
        {
            //We have encountered an illegal type of node, so stop the
            //traversal.
            _illegalNodes = true;
            return false;
        }

        // prohibited
        UniquelyTypedSegmentCounter( const UniquelyTypedSegmentCounter& );
        UniquelyTypedSegmentCounter& operator=( const UniquelyTypedSegmentCounter& );

};

} // end of namespace

//======================================================================

namespace aafanalyzer {

using namespace boost;

EPContainedTrackVisitor::EPContainedTrackVisitor( wostream& log,
                                                  boost::shared_ptr<EdgeMap> spEdgeMap,
                                                  boost::shared_ptr<TestLevelTestResult> spTestResult )
    : _log(log),
      _spEdgeMap( spEdgeMap ),
      _spTestResult( spTestResult )
{}

EPContainedTrackVisitor::~EPContainedTrackVisitor()
{}

bool EPContainedTrackVisitor::PreOrderVisit( EPTypedObjNode<IAAFCompositionMob, EPTopLevelComposition>& node )
{
  AxCompositionMob axCompMob( node.GetAAFObjectOfType() );
  
  AxString nodeName = this->GetMobName( axCompMob, EPTopLevelComposition::GetName() );

  bool testPassed = CheckTimecodeTrackPhysicalNumbers( node, 6, nodeName, L"REQ_EP_028" );
  
  return testPassed;
}

bool EPContainedTrackVisitor::PreOrderVisit( EPTypedObjNode<IAAFCompositionMob, EPLowerLevelComposition>& node )
{
    return this->CheckPrimaryTimecodeTracks( this->GetTimecodeTracks( _spEdgeMap, node ), node );
}

bool EPContainedTrackVisitor::PreOrderVisit( EPTypedObjNode<IAAFCompositionMob, EPSubClipComposition>& node )
{
    bool testPassed = true;

    AxCompositionMob axCompMob( node.GetAAFObjectOfType() );
    boost::shared_ptr<MobSlotNodeSet> spEssenceTrack = this->GetEssenceTracks( _spEdgeMap, node );

    MobSlotNodeSet::const_iterator iter;
    for ( iter = spEssenceTrack->begin(); iter != spEssenceTrack->end(); iter++ )
    {
        if ( !CheckForSingleSegment<IAAFSourceClip>( **iter ) )
        {
            AxString mobName = this->GetMobName( axCompMob, EPSubClipComposition::GetName() );
            AxObject axObj( (*iter)->GetAAFObject() );
            AxMobSlot axMobSlot( AxQueryInterface<IAAFObject, IAAFMobSlot>( axObj ) );

            wstringstream ss;
            ss << L"Slot with ID ";
            ss << axMobSlot.GetSlotID();
            ss << L" in " << mobName << L" does not have exactly one SourceClip.";

            AxString explain( ss.str().c_str() );
            _spTestResult->AddSingleResult( L"REQ_EP_037", explain, TestResult::FAIL, node );
            testPassed = false;
        }
    }

    return testPassed;
}

bool EPContainedTrackVisitor::PreOrderVisit( EPTypedObjNode<IAAFCompositionMob, EPAdjustedClipComposition>& node )
{
    bool testPassed = true;

    AxCompositionMob axCompMob( node.GetAAFObjectOfType() );
    boost::shared_ptr<MobSlotNodeSet> spEssenceTrack = this->GetEssenceTracks( _spEdgeMap, node );

    MobSlotNodeSet::const_iterator iter;
    for ( iter = spEssenceTrack->begin(); iter != spEssenceTrack->end(); iter++ )
    {
        if ( !CheckForSingleSegment<IAAFOperationGroup>( **iter ) )
        {
            AxString mobName = this->GetMobName( axCompMob, EPAdjustedClipComposition::GetName() );
            AxObject axObj( (*iter)->GetAAFObject() );
            AxMobSlot axMobSlot( AxQueryInterface<IAAFObject, IAAFMobSlot>( axObj ) );

            wstringstream ss;
            ss << L"Slot with ID ";
            ss << axMobSlot.GetSlotID();
            ss << L" in " << mobName << L" does not have exactly one OperationGroup.";

            AxString explain( ss.str().c_str() );
            _spTestResult->AddSingleResult( L"REQ_EP_046", explain, TestResult::FAIL, node );
            testPassed = false;
        }
    }

    return testPassed;
}

bool EPContainedTrackVisitor::PreOrderVisit( EPTypedObjNode<IAAFSourceMob, EPTapeSource>& node )
{
  AxSourceMob axSrcMob( node.GetAAFObjectOfType() );

  AxString nodeName = this->GetMobName( axSrcMob, EPTapeSource::GetName() );

  bool testPassed = CheckTimecodeTrackPhysicalNumbers( node, 12, nodeName, L"REQ_EP_082" );

  return testPassed;
}

bool EPContainedTrackVisitor::PreOrderVisit( EPTypedObjNode<IAAFSourceMob, EPFilmSource>& node )
{
    AxSourceMob axSrcMob( node.GetAAFObjectOfType() );

    AxString nodeName = this->GetMobName( axSrcMob, EPFilmSource::GetName() );

    unsigned int unnumberedtracks;
    boost::shared_ptr<TrackNumberMap> spTrackNumMap( CountTrackCodes( this->GetEdgecodeTracks( _spEdgeMap, node ), unnumberedtracks ) );

    TrackNumberMap::const_iterator mapIter;
    bool testPassed = true;

    //Ensure all tracks have a physical track number
    if (unnumberedtracks != 0)
    {
        wstringstream ss;
        ss << nodeName << L" has " << unnumberedtracks << " Edgecode tracks with no MobSlot::PhysicalTrackNumber property.";
        AxString explain( ss.str().c_str() );
        _spTestResult->AddSingleResult( L"REQ_EP_087", explain, TestResult::FAIL, node );
        testPassed = false;
    }

    for ( mapIter = spTrackNumMap->begin(); mapIter != spTrackNumMap->end(); mapIter++ )
    {

        switch (mapIter->first)
        {
            case 1:
                //1. Ensure that there are <=1 Keycode Number Edgecode tracks
                if ( mapIter->second > 1 )
                {
                    _spTestResult->AddSingleResult( L"REQ_EP_087", nodeName + L" has more than one Keycode Number Edgecode track.", TestResult::FAIL, node );
                    testPassed = false;
                }
                break;
            case 2:
                //2. Ensure that there are <=1 Ink Number Edgecode tracks
                if ( mapIter->second > 1 )
                {
                    _spTestResult->AddSingleResult( L"REQ_EP_087", nodeName + L" has more than one Ink Number Edgecode track.", TestResult::FAIL, node );
                    testPassed = false;
                }
                break;
            case 3:
                //3. Ensure that there are <=1 Aux. Ink Number Edgecode tracks
                if ( mapIter->second > 1 )
                {
                    _spTestResult->AddSingleResult( L"REQ_EP_087", nodeName + L" has more than one Aux. Ink Number Edgecode track.", TestResult::FAIL, node );
                    testPassed = false;
                }
                break;
            default:
                //4. Ensure there are no other Edgecode tracks
                {
                    wstringstream ss;
                    ss << nodeName << L" has a illegal Edgecode track (MobSlot::PhysicalTrackNumber = " << mapIter->first << L").";
                    AxString explain( ss.str().c_str() );
                    _spTestResult->AddSingleResult( L"REQ_EP_087", explain, TestResult::FAIL, node );
                    testPassed = false;
                }
                break;
        }

    }

    // The absence of physical track numbers in a file source is
    // valid, however, we want to add the result in order to identify
    // physical track numbers as present, and tested.  Therefore only
    // add the pass result if the track map is not empty (ie. there
    // are tracks present to test). This is different from the test
    // behavior of top level composition and tape sources becasue each
    // of those are always, minimally, be tested to ensure they
    // contain a primary timecode track (identified by physical track
    // number).
    if ( !spTrackNumMap->empty() && testPassed )
    {
      wstringstream ss;
      ss << nodeName << L" has valid physical track numbers.";
      _spTestResult->AddSingleResult( L"REQ_EP_087",
				      ss.str(),
				      TestResult::PASS,
				      node );
    }

    return testPassed;
}

bool EPContainedTrackVisitor::PreOrderVisit( EPTypedObjNode<IAAFSourceMob, EPAuxiliarySource>& node )
{
    AxSourceMob axSrcMob( node.GetAAFObjectOfType() );

    if ( axSrcMob.CountSlots() < 1 )
    {
        _spTestResult->AddSingleResult( L"REQ_EP_136",
					this->GetMobSlotName( _spEdgeMap, node ) + L" does not have any mob slots.",
					TestResult::FAIL,
					node );
        return false;
    }

    /*
     *
     * Loop through all the slots, making sure they:
     *
     *  (1) Are Timeline, Event or Static
     *  (2) Have auxiliary segments
     *
     */
    boost::shared_ptr<Node> spNode = boost::dynamic_pointer_cast<Node>( node.GetSharedPointerToNode() );
    DepthFirstTraversal dfs( _spEdgeMap, spNode );
    boost::shared_ptr<AuxiliarySlotVisitor> spVisitor( new AuxiliarySlotVisitor( _spEdgeMap, _spTestResult ) );

    dfs.TraverseDown( spVisitor );
    if ( !spVisitor->AreSlotsOk() )
    {
        return false;
    }

    // It's valid, add a pass result.
    _spTestResult->AddSingleResult( L"REQ_EP_136",
				    this->GetMobSlotName( _spEdgeMap, node ) + L" is a valid auxiliary source.",
				    TestResult::PASS,
				    node );

    return true;
}

boost::shared_ptr<EPContainedTrackVisitor::TrackNumberMap> EPContainedTrackVisitor::CountTrackCodes( boost::shared_ptr<EPTypedVisitor::MobSlotNodeSet> tracks, unsigned int& unnumberedTracks )
{
    boost::shared_ptr<TrackNumberMap> spTrackMap( new TrackNumberMap );
    unnumberedTracks = 0;

    EPTypedVisitor::MobSlotNodeSet::const_iterator iter;

    for ( iter = tracks->begin(); iter != tracks->end(); iter++ )
    {

        AxObject axObj( (*iter)->GetAAFObject() );
        AxMobSlot axMobSlot( AxQueryInterface<IAAFObject, IAAFMobSlot>( axObj ) );

        try {
            aafUInt32 physicalTrackNumber = axMobSlot.GetPhysicalNum();
            if ( spTrackMap->find( physicalTrackNumber ) == spTrackMap->end() )
            {
                (*spTrackMap)[physicalTrackNumber] = 1;
            }
            else
            {
                (*spTrackMap)[physicalTrackNumber] = (*spTrackMap)[physicalTrackNumber] + 1;
            }
        }
        catch ( const AxExHResult& ex )
        {
            if ( ex.getHResult() == AAFRESULT_PROP_NOT_PRESENT )
            {
                unnumberedTracks++;
            }
            else
            {
                throw ex;
            }
        }
    }

    return spTrackMap;

}

template<typename SegmentType>
bool EPContainedTrackVisitor::CheckForSingleSegment( Node& mobSlotNode )
{

    //Get a shared pointer to the MobSlot node.
    boost::shared_ptr<Node> spNode = mobSlotNode.GetSharedPointerToNode();

    //Use a UniquelyTypedMobSlotVisitor to determine the if this node contains
    //a combination of Sequences and another type of Component, and if it does,
    //the number of components.
    DepthFirstTraversal dfs( _spEdgeMap, spNode );
    boost::shared_ptr<UniquelyTypedSegmentCounter<SegmentType> > spVisitor( new UniquelyTypedSegmentCounter<SegmentType>() );
    dfs.TraverseDown( spVisitor );

    //Determine if there was a single node of the specified type.
    return !(spVisitor->ContainsIllegalNodes()) && ( spVisitor->GetNumSegments() == 1 );


}

bool EPContainedTrackVisitor::CheckPrimaryTimecodeTracks( boost::shared_ptr<EPTypedVisitor::MobSlotNodeSet> tracks, Node& node )
{

    bool testPassed = true;

    EPTypedVisitor::MobSlotNodeSet::const_iterator iter;
    for ( iter = tracks->begin(); iter != tracks->end(); iter++ )
    {
        AxObject axObj( (*iter)->GetAAFObject() );
        AxMobSlot axMobSlot( AxQueryInterface<IAAFObject, IAAFMobSlot>( axObj ) );

        try {

            aafUInt32 physicalTrackNumber = axMobSlot.GetPhysicalNum();
            if ( physicalTrackNumber == 1 )
            {
                if ( !CheckForSingleSegment<IAAFTimecode>( **iter ) )
                {
                    wstringstream ss;
                    ss << L"Mob slot with ID = " << axMobSlot.GetSlotID()
                       << L" of " << this->GetMobSlotName( _spEdgeMap, node )
                       << L" is a timecode track that does not consist of a single Timecode object.";
                    _spTestResult->AddSingleResult( L"REQ_EP_131", ss.str().c_str(), TestResult::FAIL, node );
                    testPassed = false;
                }
            }
        }
        catch ( const AxExHResult& ex )
        {
            if ( ex.getHResult() != AAFRESULT_PROP_NOT_PRESENT )
            {
                throw ex;
            }
        }
    }

    return testPassed;

}

bool EPContainedTrackVisitor::CheckTimecodeTrackPhysicalNumbers( Node& node,
								 unsigned int maxPhysTrackNum,
								 wstring nodeName, wstring reqId )
{
  bool testPassed = true;
  
  unsigned int unnumberedtracks;
  boost::shared_ptr<MobSlotNodeSet> timecodeTracks = this->GetTimecodeTracks( _spEdgeMap, node );
  
  testPassed = this->CheckPrimaryTimecodeTracks( timecodeTracks, node );
  boost::shared_ptr<TrackNumberMap> spTrackNumMap( CountTrackCodes( timecodeTracks, unnumberedtracks ) );
  
  //Ensure all tracks have a physical track number
  if (unnumberedtracks != 0)
  {
    wstringstream ss;
    ss << nodeName << L" has " << unnumberedtracks << " timecode tracks with no MobSlot::PhysicalTrackNumber property.";
    AxString explain( ss.str().c_str() );
    _spTestResult->AddSingleResult( reqId, explain, TestResult::FAIL, node );
    testPassed = false;
  }

  TrackNumberMap::const_iterator mapIter;

  // Check for unique physical track numbers, and ensure they are in
  // the range of value numbers.  Check actual track number values
  // in order to specialize the error message.
  for ( mapIter = spTrackNumMap->begin(); mapIter != spTrackNumMap->end(); mapIter++ )
  {
    // mapIter->first is the track number
    // mapIter-second is the number of tracks with that number
    
    // 1 to maxPhysTrackNum are valid physical track numbers
    if ( !(1 <= mapIter->first && mapIter->first <= maxPhysTrackNum) )
    {
      wstringstream ss;
      ss << nodeName << L" has a timecode track with an illegal MobSlot::PhysicalTrackNumber value of " << mapIter->first;
      boost::shared_ptr<DetailLevelTestResult>
	spDetailResult = _spTestResult->AddSingleResult( reqId, ss.str(), TestResult::FAIL, node );
      ss.str( L"" );
      ss << "Valid values are 1 <= PhysicalTrackNumber <= " << maxPhysTrackNum << L".";
      spDetailResult->AddDetail( ss.str() );
      testPassed = false;
    }

    // A physical track number cannot be used more than once in a mob.
    if ( mapIter->second > 1 )
    {
      wstring what;
      
      wstringstream ss;
      ss << nodeName << L" has " << mapIter->second
	 << L" time code tracks with a MobSlot::PhysicalTrackNumber value of " << mapIter->first << L".";
      boost::shared_ptr<DetailLevelTestResult>
	spDetailResult = _spTestResult->AddSingleResult( reqId, ss.str(), TestResult::FAIL, node );
      spDetailResult->AddDetail( L"Physical track number must be unique within a Mob." );
      testPassed = false;
    }
  }
  
  //  Ensure there is one primary timecode track.
  if ( spTrackNumMap->find( 1 ) == spTrackNumMap->end() )
  {
    _spTestResult->AddSingleResult( reqId,
				    nodeName + L" does not have a Primary Timecode track.",
				    TestResult::FAIL,
				    node );
    testPassed = false;
  }
  
  if ( testPassed )
  {
    wstringstream ss;
    ss << nodeName << L" has valid physical track numbers.";
    _spTestResult->AddSingleResult( reqId,
				    ss.str(),
				    TestResult::PASS,
				    node );
  }

  return testPassed;
}

} // end of namespace aafanalyzer
