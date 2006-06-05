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
#include "EPContainedTrackVisitor.h"
#include "EPContainedTrackTest.h"

//Test/Result files
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
using namespace boost;

class AuxiliarySlotVisitor : public EPTypedVisitor
{
    public:
        AuxiliarySlotVisitor( shared_ptr<EdgeMap> spEdgeMap, shared_ptr<DetailLevelTestResult> spResult )
            : _spEdgeMap( spEdgeMap ), _spResult( spResult ), _testPassed( true )
        {}

        ~AuxiliarySlotVisitor()
        {}

        bool PreOrderVisit( AAFTypedObjNode<IAAFTimelineMobSlot>& node )
        {
            shared_ptr<AAFTypedObjNode<IAAFMobSlot> > spGeneric( node.DownCastToAAF<IAAFMobSlot>() );
            return this->PreOrderVisit( *spGeneric );
        }

        bool PreOrderVisit( AAFTypedObjNode<IAAFEventMobSlot>& node )
        {
            shared_ptr<AAFTypedObjNode<IAAFMobSlot> > spGeneric( node.DownCastToAAF<IAAFMobSlot>() );
            return this->PreOrderVisit( *spGeneric );
        }

        bool PreOrderVisit( AAFTypedObjNode<IAAFStaticMobSlot>& node )
        {
            shared_ptr<AAFTypedObjNode<IAAFMobSlot> > spGeneric( node.DownCastToAAF<IAAFMobSlot>() );
            return this->PreOrderVisit( *spGeneric );
        }

        bool PreOrderVisit( AAFTypedObjNode<IAAFMobSlot>& node )
        {
            AxMobSlot axMobSlot( node.GetAAFObjectOfType() );
            AxSegment axSegment( axMobSlot.GetSegment() );
            AxDataDef dataDef ( axSegment.GetDataDef() );
            if ( dataDef.GetAUID() != kAAFDataDef_Auxiliary  )
            {
                _spResult->AddInformationResult( L"REQ_EP_136", this->GetMobSlotName( _spEdgeMap, node ) + L" has a segment with a Data Definition that is not DataDef_Auxiliary.", TestResult::FAIL );
                _testPassed = false;
            }
            return false;
        }

        bool AreSlotsOk()
        {
            return _testPassed;
        }

    private:

        shared_ptr<EdgeMap> _spEdgeMap;
        shared_ptr<DetailLevelTestResult> _spResult;
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

EPContainedTrackVisitor::EPContainedTrackVisitor( wostream& log, shared_ptr<EdgeMap> spEdgeMap )
    : _log(log),
      _spEdgeMap( spEdgeMap ),
      _spResult( new DetailLevelTestResult( L"Edit Protocol Contained Track Visitor",
                                            L"Visit derivation chain mateiral and ensure they contain the required tracks.",
                                            L"", // explain
                                            L"", // DOCREF REQUIRED
                                            TestResult::PASS,
                                            TestRegistry::GetInstance().GetRequirementsForTest( EPContainedTrackTest::GetTestInfo().GetName() )
               )                          )
{}

EPContainedTrackVisitor::~EPContainedTrackVisitor()
{}

bool EPContainedTrackVisitor::PreOrderVisit( EPTypedObjNode<IAAFCompositionMob, EPTopLevelComposition>& node )
{

    AxCompositionMob axCompMob( node.GetAAFObjectOfType() );

    //Get the name of the mob
    AxString nodeName = this->GetMobName( axCompMob, EPTopLevelComposition::GetName() );

    unsigned int unnumberedtracks;
    shared_ptr<MobSlotNodeSet> timecodeTracks = this->GetTimecodeTracks( _spEdgeMap, node );

    bool testPassed = this->CheckPrimaryTimecodeTracks( timecodeTracks, node );
    shared_ptr<TrackNumberMap> spTrackNumMap( CountTrackCodes( timecodeTracks, unnumberedtracks ) );

    //Ensure all tracks have a physical track number
    if (unnumberedtracks != 0)
    {
        wstringstream ss;
        ss << nodeName << L" has " << unnumberedtracks << " Timecode tracks with no MobSlot::PhysicalTrackNumber property.";
        AxString explain( ss.str().c_str() );
        _spResult->AddInformationResult( L"REQ_EP_028", explain, TestResult::FAIL );
        testPassed = false;
    }

    TrackNumberMap::const_iterator mapIter;

    for ( mapIter = spTrackNumMap->begin(); mapIter != spTrackNumMap->end(); mapIter++ )
    {

        switch (mapIter->first)
        {
            case 1:
                //1. Ensure that there are <=1 primary timecode tracks
                if ( mapIter->second > 1 )
                {
                    _spResult->AddInformationResult( L"REQ_EP_028", nodeName + L" has more than one Primary Timecode track.", TestResult::FAIL );
                    testPassed = false;
                }
                break;
            case 2:
                //2. Ensure that there are <=1 alternative timecode tracks
                if ( mapIter->second > 1 )
                {
                    _spResult->AddInformationResult( L"REQ_EP_028", nodeName + L" has more than one Alternative Timecode track.", TestResult::FAIL );
                    testPassed = false;
                }
                break;
            case 3: case 4: case 5: case 6:
                //3. Ensure there are no reserved timecode tracks
                {
                    wstringstream ss;
                    ss << nodeName << L" has a Reserved Timecode track (MobSlot::PhysicalTrackNumber = " << mapIter->first << L").";
                    AxString explain( ss.str().c_str() );
                    _spResult->AddInformationResult( L"REQ_EP_028", explain, TestResult::FAIL );
                    testPassed = false;
                }
                break;
            default:
                //4. Ensure there are no other timecode tracks
                {
                    wstringstream ss;
                    ss << nodeName << L" has a illegal Timecode track (MobSlot::PhysicalTrackNumber = " << mapIter->first << L").";
                    AxString explain( ss.str().c_str() );
                    _spResult->AddInformationResult( L"REQ_EP_028", explain, TestResult::FAIL );
                    testPassed = false;
                }
                break;
        }

    }

    //5. Ensure there are >= 1 primary timecode tracks
    if ( spTrackNumMap->find( 1 ) == spTrackNumMap->end() )
    {
        _spResult->AddInformationResult( L"REQ_EP_028", nodeName + L" does not have a Primary Timecode track.", TestResult::FAIL );
        testPassed = false;
    }

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
    shared_ptr<MobSlotNodeSet> spEssenceTrack = this->GetEssenceTracks( _spEdgeMap, node );

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
            _spResult->AddInformationResult( L"REQ_EP_037", explain, TestResult::FAIL );
            testPassed = false;
        }
    }

    return testPassed;
}

bool EPContainedTrackVisitor::PreOrderVisit( EPTypedObjNode<IAAFCompositionMob, EPAdjustedClipComposition>& node )
{
    bool testPassed = true;

    AxCompositionMob axCompMob( node.GetAAFObjectOfType() );
    shared_ptr<MobSlotNodeSet> spEssenceTrack = this->GetEssenceTracks( _spEdgeMap, node );

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
            _spResult->AddInformationResult( L"REQ_EP_046", explain, TestResult::FAIL );
            testPassed = false;
        }
    }

    return testPassed;
}

bool EPContainedTrackVisitor::PreOrderVisit( EPTypedObjNode<IAAFSourceMob, EPTapeSource>& node )
{

    AxSourceMob axSrcMob( node.GetAAFObjectOfType() );

    //Get the name of the mob
    AxString nodeName = this->GetMobName( axSrcMob, EPTapeSource::GetName() );

    unsigned int unnumberedtracks;
    shared_ptr<TrackNumberMap> spTrackNumMap( CountTrackCodes( this->GetTimecodeTracks( _spEdgeMap, node ), unnumberedtracks ) );

    TrackNumberMap::const_iterator mapIter;
    bool testPassed = true;

    //Ensure all tracks have a physical track number
    if (unnumberedtracks != 0)
    {
        wstringstream ss;
        ss << nodeName << L" has " << unnumberedtracks << " Timecode tracks with no MobSlot::PhysicalTrackNumber property.";
        AxString explain( ss.str().c_str() );
        _spResult->AddInformationResult( L"REQ_EP_082", explain, TestResult::FAIL );
        testPassed = false;
    }

    for ( mapIter = spTrackNumMap->begin(); mapIter != spTrackNumMap->end(); mapIter++ )
    {

        switch (mapIter->first)
        {
            case 1:
                //1. Ensure that there are <=1 primary timecode tracks
                if ( mapIter->second > 1 )
                {
                    _spResult->AddInformationResult( L"REQ_EP_082", nodeName + L" has more than one Primary timecode track.", TestResult::FAIL );
                    testPassed = false;
                }
                break;
            case 3: case 4: case 5: case 6: case 7:
                //2. Ensure that there are <=1 AuxN tracks
                if ( mapIter->second > 1 )
                {
                    wstringstream ss;
                    ss << nodeName << L" has more than one Aux" << (mapIter->first - 2) << L" Timecode track.";
                    AxString explain( ss.str().c_str() );
                    _spResult->AddInformationResult( L"REQ_EP_082", explain, TestResult::FAIL );
                    testPassed = false;
                }
                break;
            case 2: case 8: case 9: case 10: case 11: case 12:
                //3. Ensure there are no reserved timecode tracks
                {
                    wstringstream ss;
                    ss << nodeName << L" has a Reserved Timecode track (MobSlot::PhysicalTrackNumber = " << mapIter->first << L").";
                    AxString explain( ss.str().c_str() );
                    _spResult->AddInformationResult( L"REQ_EP_082", explain, TestResult::FAIL );
                    testPassed = false;
                }
                break;
            default:
                //4. Ensure there are no other timecode tracks
                {
                    wstringstream ss;
                    ss << nodeName << L" has a illegal Timecode track (MobSlot::PhysicalTrackNumber = " << mapIter->first << L").";
                    AxString explain( ss.str().c_str() );
                    _spResult->AddInformationResult( L"REQ_EP_082", explain, TestResult::FAIL );
                    testPassed = false;
                }
                break;
        }

    }

    return testPassed;
}

bool EPContainedTrackVisitor::PreOrderVisit( EPTypedObjNode<IAAFSourceMob, EPFilmSource>& node )
{

    AxSourceMob axSrcMob( node.GetAAFObjectOfType() );

    //Get the name of the mob
    AxString nodeName = this->GetMobName( axSrcMob, EPFilmSource::GetName() );

    unsigned int unnumberedtracks;
    shared_ptr<TrackNumberMap> spTrackNumMap( CountTrackCodes( this->GetEdgecodeTracks( _spEdgeMap, node ), unnumberedtracks ) );

    TrackNumberMap::const_iterator mapIter;
    bool testPassed = true;

    //Ensure all tracks have a physical track number
    if (unnumberedtracks != 0)
    {
        wstringstream ss;
        ss << nodeName << L" has " << unnumberedtracks << " Edgecode tracks with no MobSlot::PhysicalTrackNumber property.";
        AxString explain( ss.str().c_str() );
        _spResult->AddInformationResult( L"REQ_EP_087", explain, TestResult::FAIL );
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
                    _spResult->AddInformationResult( L"REQ_EP_087", nodeName + L" has more than one Keycode Number Edgecode track.", TestResult::FAIL );
                    testPassed = false;
                }
                break;
            case 2:
                //2. Ensure that there are <=1 Ink Number Edgecode tracks
                if ( mapIter->second > 1 )
                {
                    _spResult->AddInformationResult( L"REQ_EP_087", nodeName + L" has more than one Ink Number Edgecode track.", TestResult::FAIL );
                    testPassed = false;
                }
                break;
            case 3:
                //3. Ensure that there are <=1 Aux. Ink Number Edgecode tracks
                if ( mapIter->second > 1 )
                {
                    _spResult->AddInformationResult( L"REQ_EP_087", nodeName + L" has more than one Aux. Ink Number Edgecode track.", TestResult::FAIL );
                    testPassed = false;
                }
                break;
            default:
                //4. Ensure there are no other Edgecode tracks
                {
                    wstringstream ss;
                    ss << nodeName << L" has a illegal Edgecode track (MobSlot::PhysicalTrackNumber = " << mapIter->first << L").";
                    AxString explain( ss.str().c_str() );
                    _spResult->AddInformationResult( L"REQ_EP_087", explain, TestResult::FAIL );
                    testPassed = false;
                }
                break;
        }

    }

    return testPassed;

}

bool EPContainedTrackVisitor::PreOrderVisit( EPTypedObjNode<IAAFSourceMob, EPAuxiliarySource>& node )
{
    AxSourceMob axSrcMob( node.GetAAFObjectOfType() );

    if ( axSrcMob.CountSlots() < 1 )
    {
        _spResult->AddInformationResult( L"REQ_EP_136", this->GetMobSlotName( _spEdgeMap, node ) + L" does not have any mob slots.", TestResult::FAIL);
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
    shared_ptr<Node> spNode = dynamic_pointer_cast<Node>( node.GetSharedPointerToNode() );
    DepthFirstTraversal dfs( _spEdgeMap, spNode );
    shared_ptr<AuxiliarySlotVisitor> spVisitor( new AuxiliarySlotVisitor( _spEdgeMap, _spResult ) );

    dfs.TraverseDown( spVisitor );
    if ( !spVisitor->AreSlotsOk() )
    {
        return false;
    }

    return true;
}

shared_ptr<DetailLevelTestResult> EPContainedTrackVisitor::GetResult()
{
    return _spResult;
}

shared_ptr<EPContainedTrackVisitor::TrackNumberMap> EPContainedTrackVisitor::CountTrackCodes( shared_ptr<EPTypedVisitor::MobSlotNodeSet> tracks, unsigned int& unnumberedTracks )
{
    shared_ptr<TrackNumberMap> spTrackMap( new TrackNumberMap );
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
    shared_ptr<Node> spNode = mobSlotNode.GetSharedPointerToNode();

    //Use a UniquelyTypedMobSlotVisitor to determine the if this node contains
    //a combination of Sequences and another type of Component, and if it does,
    //the number of components.
    DepthFirstTraversal dfs( _spEdgeMap, spNode );
    shared_ptr<UniquelyTypedSegmentCounter<SegmentType> > spVisitor( new UniquelyTypedSegmentCounter<SegmentType>() );
    dfs.TraverseDown( spVisitor );

    //Determine if there was a single node of the specified type.
    return !(spVisitor->ContainsIllegalNodes()) && ( spVisitor->GetNumSegments() == 1 );


}

bool EPContainedTrackVisitor::CheckPrimaryTimecodeTracks( shared_ptr<EPTypedVisitor::MobSlotNodeSet> tracks, Node& node )
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
                    _spResult->AddInformationResult( L"REQ_EP_131", ss.str().c_str(), TestResult::FAIL );
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

} // end of namespace aafanalyzer