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

//Ax files
#include <AxMob.h>
#include <AxMetaDef.h>
#include <AxMobSlot.h>
#include <AxIterator.h>
#include <AxComponent.h>

//AAF files
#include <AAFResult.h>

//STL files
#include <sstream>

namespace {

} // end of namespace

//======================================================================

namespace aafanalyzer {

using namespace boost;
 
EPContainedTrackVisitor::EPContainedTrackVisitor( wostream& log )
    : _log(log),
      _spResult( new DetailLevelTestResult( L"Edit Protocol Contained Track Visitor",
                                            L"Visit derivation chain mateiral and ensure they contain the required tracks.",
                                            L"", // explain
                                            L"", // DOCREF REQUIRED
                                            TestResult::PASS,
                                            TestRegistry::GetInstance().GetRequirementsForTest( EPContainedTrackTest::GetTestInfo().GetName() )
               )                          )
{
}
    
EPContainedTrackVisitor::~EPContainedTrackVisitor()
{
}

bool EPContainedTrackVisitor::PreOrderVisit( EPTypedObjNode<IAAFCompositionMob, EPTopLevelComposition>& node )
{
  
    AxCompositionMob axCompMob( node.GetAAFObjectOfType() );
    
    //Get the name of the mob
    AxString nodeName = this->GetMobName( axCompMob, L"Top-Level Composition");
    
    unsigned int unnumberedtracks;
    shared_ptr<TrackMap> spTrackMap( CountTracks( axCompMob, kAAFClassID_Timecode, unnumberedtracks ) );
    
    TrackMap::const_iterator mapIter;
    bool testPassed = true;
    
    //Ensure all tracks have a physical track number
    if (unnumberedtracks != 0)
    {
        wstringstream ss;
        ss << nodeName << L" has " << unnumberedtracks << " Timecode tracks with no MobSlot::PhysicalTrackNumber property.";
        AxString explain( ss.str().c_str() );
        _spResult->AddInformationResult( L"REQ_EP_028", explain, TestResult::FAIL );
        testPassed = false;
    }    
    
    for ( mapIter = spTrackMap->begin(); mapIter != spTrackMap->end(); mapIter++ )
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
    if ( spTrackMap->find( 1 ) == spTrackMap->end() )
    {
        _spResult->AddInformationResult( L"REQ_EP_028", nodeName + L" does not have a Primary Timecode track.", TestResult::FAIL );
        testPassed = false;
    }
    
    return testPassed;
    
}

bool EPContainedTrackVisitor::PreOrderVisit( EPTypedObjNode<IAAFSourceMob, EPTapeSource>& node )
{
  
    AxSourceMob axSrcMob( node.GetAAFObjectOfType() );
    
    //Get the name of the mob
    AxString nodeName = this->GetMobName( axSrcMob, L"Tape Source" );
    
    unsigned int unnumberedtracks;
    shared_ptr<TrackMap> spTrackMap( CountTracks( axSrcMob, kAAFClassID_Timecode, unnumberedtracks ) );
    
    TrackMap::const_iterator mapIter;
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
    
    for ( mapIter = spTrackMap->begin(); mapIter != spTrackMap->end(); mapIter++ )
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
    AxString nodeName = this->GetMobName( axSrcMob, L"Film Source" );
    
    unsigned int unnumberedtracks;
    shared_ptr<TrackMap> spTrackMap( CountTracks( axSrcMob, kAAFClassID_EdgeCode, unnumberedtracks ) );
    
    TrackMap::const_iterator mapIter;
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
    
    for ( mapIter = spTrackMap->begin(); mapIter != spTrackMap->end(); mapIter++ )
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

bool EPContainedTrackVisitor::EdgeVisit(AAFComponentReference& edge)
{
    return false;
}

bool EPContainedTrackVisitor::EdgeVisit(AAFSlotReference& edge)
{
    return false;
}
    
shared_ptr<DetailLevelTestResult> EPContainedTrackVisitor::GetResult()
{
    return _spResult;
}

shared_ptr<EPContainedTrackVisitor::TrackMap> EPContainedTrackVisitor::CountTracks( AxMob& axMob, aafUID_t trackType, unsigned int& unnumberedTracks )
{
    
    shared_ptr<TrackMap> spTrackMap( new TrackMap );
    unnumberedTracks = 0;
    
    //Get a mob slot iterator
    AxMobSlotIter slotIter( axMob.GetSlots() );

    //Storage for value returned by iterator.
    IAAFSmartPointer2<IAAFMobSlot> spMobSlot; 

    //Repeat until the iterator is out of mob slots.
    while ( slotIter.NextOne( spMobSlot ) )
    {
        AxMobSlot axMobSlot( spMobSlot );
        AxClassDef slotClsDef( axMobSlot.GetDefinition() );
        //Find out if it's a timeline mob slot
        if ( this->IsType( slotClsDef, kAAFClassID_TimelineMobSlot, kAAFClassID_MobSlot ) )
        {
            AxSegment axSegment( axMobSlot.GetSegment() );
            AxClassDef segClsDef( axSegment.GetDefinition() );
            //Find out if this is a trackType track.
            if ( this->IsType( segClsDef, trackType, kAAFClassID_Segment ) )
            {
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
        }
    }
    
    return spTrackMap;
    
}
    
} // end of namespace aafanalyzer
