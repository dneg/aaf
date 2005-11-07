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
#include "EPEditRateVisitor.h"
#include "EPEditRateTest.h"

//Edit Protocol Analyzer Base files
#include <EPTrack.h>

//Test/Result files
#include <DetailLevelTestResult.h>
#include <TestRegistry.h>

//Requirement files
#include <RequirementRegistry.h>
#include <Requirement.h>

//Ax files
#include <AxMob.h>
#include <AxMobSlot.h>
#include <AxDefObject.h>
#include <AxMetaDef.h>
#include <AxEssence.h>

//AAF files
#include <AAFClassDefUIDs.h>

//STL files
#include <sstream>

namespace {

} // end of namespace

//======================================================================

namespace aafanalyzer {

using namespace boost;
 
EPEditRateVisitor::EPEditRateVisitor( wostream& log )
    : _log(log), 
      _spResult( new DetailLevelTestResult( L"Edit Protocol Edit Rate Visitor",
                                            L"Ensure all essence tracks have valid edit rates.",
                                            L"", // explain
                                            L"", // DOCREF REQUIRED
                                            TestResult::PASS,
                                            TestRegistry::GetInstance().GetRequirementsForTest( EPEditRateTest::GetTestInfo().GetName() )
               )                          ),
      _staticAudioTracks( 0 ),
      _unknownAudioTracks( 0 )
{
}

EPEditRateVisitor::~EPEditRateVisitor()
{
}

bool EPEditRateVisitor::PreOrderVisit( AAFTypedObjNode<IAAFCompositionMob>& node )
{
    AxCompositionMob axCompMob( node.GetAAFObjectOfType() );
    return this->VisitMob( axCompMob );
}

bool EPEditRateVisitor::PreOrderVisit( AAFTypedObjNode<IAAFMasterMob>& node )
{
    AxMasterMob axMastMob( node.GetAAFObjectOfType() );
    return this->VisitMob( axMastMob );
}

bool EPEditRateVisitor::PreOrderVisit( AAFTypedObjNode<IAAFSourceMob>& node )
{
    AxSourceMob axSrcMob( node.GetAAFObjectOfType() );
    return this->VisitMob( axSrcMob );
}
       
bool EPEditRateVisitor::EdgeVisit(AAFComponentReference& edge)
{
    return false;
}

bool EPEditRateVisitor::EdgeVisit(AAFSlotReference& edge)
{
    return false;
}
    
 shared_ptr<DetailLevelTestResult> EPEditRateVisitor::GetResult()
 {
    return _spResult;
 }
    
bool EPEditRateVisitor::VisitMob( AxMob& axMob )
{

    AxString mobName = this->GetMobName( axMob, L"Mob" );
    
    shared_ptr<TrackSet> essenceTracks = this->GetEssenceTracks( axMob );
    TrackSet::const_iterator iter;
    
    bool testPassed = true;
    
    for ( iter = essenceTracks->begin(); iter != essenceTracks->end(); iter++ )
    {
        
        //Attempt to convert the essence track to an audio and video track
        //(one will return a null pointer because the data def cannot be sound
        // and picture).
        shared_ptr<EPAudioTrack> spAudioTrack( EPAudioTrack::CreateAudioTrack( **iter ) );
        shared_ptr<EPVideoTrack> spVideoTrack( EPVideoTrack::CreateVideoTrack( **iter ) );
        
        aafSlotID_t slotId = (*iter)->GetSlotID();
        AxClassDef clsDef( (*iter)->GetDefinition() );
        if ( this->IsType( clsDef, kAAFClassID_TimelineMobSlot, kAAFClassID_MobSlot ) )
        {
            AxTimelineMobSlot axMobSlot( AxQueryInterface<IAAFMobSlot, IAAFTimelineMobSlot>( **iter ) );
            aafRational_t editRate = axMobSlot.GetEditRate();
            AxDataDef axDataDef( axMobSlot.GetDataDef() );
            testPassed = TestEditRate( editRate, axDataDef, mobName, slotId );
            if ( spAudioTrack )
            {
                VisitAudioTrack( spAudioTrack, editRate );
            }
            else if ( spVideoTrack )
            {
                testPassed = VisitVideoTrack( spVideoTrack, editRate ) && testPassed;
            }
        }
        else if ( this->IsType( clsDef, kAAFClassID_StaticMobSlot, kAAFClassID_MobSlot ) )
        {
            //Don't bother getting the static mob slot because it does not have
            //an edit rate.
            wstringstream ss;
            ss << L"Mob Slot with ID = " << slotId << L" of " << mobName
               << L" is a StaticMobSlot and does not have an edit rate.";
            _spResult->AddInformationResult( L"REQ_EP_091", ss.str().c_str(), TestResult::FAIL );
            if ( spAudioTrack )
            {
                _staticAudioTracks++;
            }
            else if ( spVideoTrack )
            {
                ss.str( L"" );
                ss << L"Video Track in MobSlot with ID = " << slotId << L" of " << mobName
                   << L" is in a StaticMobSlot and does not have an edit rate to compare with a sample rate.";
                _spResult->AddInformationResult( L"REQ_EP_100", ss.str().c_str(), TestResult::FAIL );
            }
        }
        else if ( this->IsType( clsDef, kAAFClassID_EventMobSlot, kAAFClassID_MobSlot ) )
        {
            AxEventMobSlot axMobSlot( AxQueryInterface<IAAFMobSlot, IAAFEventMobSlot>( **iter ) );
            aafRational_t editRate = axMobSlot.GetEditRate();
            AxDataDef axDataDef( axMobSlot.GetDataDef() );
            testPassed = TestEditRate( editRate, axDataDef, mobName, slotId );
            if ( spAudioTrack )
            {
                VisitAudioTrack( spAudioTrack, editRate );
            }
            else if ( spVideoTrack )
            {
                testPassed = VisitVideoTrack( spVideoTrack, editRate ) && testPassed;
            }
        }
        else
        {
            //This is an unknown type of mob slot so there is no way to get an
            //edit rate
            wstringstream ss;
            ss << L"Mob Slot with ID = " << slotId << L" of " << mobName
               << L" is not a known type of MobSlot and its edit rate cannot be accessed.";
            _spResult->AddInformationResult( L"REQ_EP_091", ss.str().c_str(), TestResult::FAIL );
            if ( spAudioTrack )
            {
                _unknownAudioTracks++;
            }
            else if ( spVideoTrack )
            {
                ss.str( L"" );
                ss << L"Video Track in MobSlot with ID = " << slotId << L" of " << mobName
                   << L" is in an unknown type of MobSlot and does not have an edit rate that can be accessed to compare with a sample rate.";
                _spResult->AddInformationResult( L"REQ_EP_100", ss.str().c_str(), TestResult::FAIL );
            }
        }
    }

    return testPassed;
}

bool EPEditRateVisitor::TestEditRate( aafRational_t editRate, AxDataDef& axDataDef, const AxString& mobName, aafSlotID_t slotId )
{

    wstringstream ss;
    
    ss << L"Mob Slot with ID = " << slotId
       << L" of " << mobName << L" ";
    
    if ( editRate.numerator < 0 )
    {
        ss << L"has an edit rate with a negative numerator (" << editRate.numerator
           << L"/" << editRate.denominator << L").";
        _spResult->AddInformationResult( L"REQ_EP_092", ss.str().c_str(), TestResult::FAIL );
        return false;
    }
    else if ( editRate.denominator < 0 )
    {
        ss << L"has an edit rate with a negative denominator (" << editRate.numerator
           << L"/" << editRate.denominator << L").";
        _spResult->AddInformationResult( L"REQ_EP_092", ss.str().c_str(), TestResult::FAIL );
        return false;
    }
    else if ( editRate.numerator == 0 )
    {
        ss << L"has an edit rate with a zero numerator (" << editRate.numerator
           << L"/" << editRate.denominator << L").";
        _spResult->AddInformationResult( L"REQ_EP_092", ss.str().c_str(), TestResult::FAIL );
        return false;
    }
    else if ( editRate.denominator == 0 )
    {
        ss << L"has an edit rate with a zero denominator (" << editRate.numerator
           << L"/" << editRate.denominator << L").";
        _spResult->AddInformationResult( L"REQ_EP_092", ss.str().c_str(), TestResult::FAIL );
        return false;
    }
    else if ( !_erTable.IsInTable( editRate, axDataDef.IsPictureKind() ) )
    {
        ss << L"has an edit rate that is not allowed for ";
        if ( axDataDef.IsPictureKind() )
        {
            ss << L"video ";
        }
        else
        {
            ss << L"audio ";
        }
        ss << L"tracks (" << editRate.numerator << L"/" << editRate.denominator
           << L" = " 
           << _erTable.Round( (double)editRate.numerator/(double)editRate.denominator)
           << L").";
        _spResult->AddInformationResult( L"REQ_EP_091", ss.str().c_str(), TestResult::FAIL );
        return false;
    }
    
    return true;
    
}

void EPEditRateVisitor::VisitAudioTrack( shared_ptr<EPAudioTrack> spTrack, aafRational_t editRate )
{

    RationalKey erKey( editRate.numerator, editRate.denominator );

    if ( _audioEditRates.find( erKey ) == _audioEditRates.end() )
    {
        _audioEditRates[erKey] = 1;
    }
    else
    {
        _audioEditRates[erKey] = _audioEditRates[erKey]+ 1;
    }
    
    AxSourceMob axSrcMob( AxQueryInterface<IAAFMob, IAAFSourceMob>( spTrack->GetMob() ) );
    AxFileDescriptor axFileDes( AxQueryInterface<IAAFEssenceDescriptor, IAAFFileDescriptor>( axSrcMob.GetEssenceDescriptor() ) );
    aafRational_t sampleRate = axFileDes.GetSampleRate();
    RationalKey srKey( sampleRate.numerator, sampleRate.denominator );
    
    if ( _audioSampleRates.find( srKey ) == _audioSampleRates.end() )
    {
      _audioSampleRates[srKey] = 1;
    }
    else
    {
        _audioSampleRates[srKey] = _audioSampleRates[srKey]+ 1;
    }

}

bool EPEditRateVisitor::VisitVideoTrack( shared_ptr<EPVideoTrack> spTrack, aafRational_t editRate )
{
    AxSourceMob axSrcMob( AxQueryInterface<IAAFMob, IAAFSourceMob>( spTrack->GetMob() ) );
    AxFileDescriptor axFileDes( AxQueryInterface<IAAFEssenceDescriptor, IAAFFileDescriptor>( axSrcMob.GetEssenceDescriptor() ) );
    aafRational_t sampleRate = axFileDes.GetSampleRate();
    
    double dEditRate = (double)editRate.numerator/(double)editRate.denominator;
    double dSampleRate = (double)sampleRate.numerator/(double)sampleRate.denominator;
    
    if ( dEditRate > dSampleRate )
    {
        AxString mobName = this->GetMobName( axSrcMob, L"File Source Mob" );
        wstringstream ss;
        ss << L"Video Track in MobSlot with ID = "
           << spTrack->GetMobSlot().GetSlotID()
           << L" of " << mobName
           << L" has an edit rate (" << editRate.numerator
           << L"/" << editRate.denominator
           << L") that is greater than its sample rate ("
           << sampleRate.numerator << L"/" << sampleRate.denominator
           << L").";
        _spResult->AddInformationResult( L"REQ_EP_100", ss.str().c_str(), TestResult::FAIL );
        return false;
    }

    return true;

}

void EPEditRateVisitor::CheckAudioSampleRates()
{

    //If there is more than 1 edit rate or sample rate or if there are edit or
    //sample rates mixed with static or unknown MobSlots or there are a mixture
    //of static and unknown MobSlots the test must fail.
    
    //Note: An edit rate is added every time a sample rate is added.  Therefore
    //      it is impossible for one map to contain no elements when the other
    //      is not empty.
    
    if ( _audioEditRates.size() > 1 || 
         _audioSampleRates.size() > 1 ||
         (_audioEditRates.size() > 0 && _staticAudioTracks > 0 ) ||
         (_audioEditRates.size() > 0 && _unknownAudioTracks > 0 ) ||
         (_audioSampleRates.size() > 0 && _staticAudioTracks > 0 ) ||
         (_audioSampleRates.size() > 0 && _unknownAudioTracks > 0 ) ||
         (_staticAudioTracks > 0 && _unknownAudioTracks > 0 )
       )
    {
        
        Requirement::RequirementMapSP reqMapSP(new Requirement::RequirementMap);
        shared_ptr<const Requirement> requirement = RequirementRegistry::GetInstance().GetRequirement(L"REQ_EP_099");
        (*reqMapSP)[L"REQ_EP_099"] = requirement;
    
        shared_ptr<DetailLevelTestResult> spSubResult( new DetailLevelTestResult( 
                                    _spResult->GetName(),
                                    L"-", // desc
                                    L"All audio tracks within the file do not have the same edit and sample rates.",
                                    L"-", // docref
                                    TestResult::FAIL,
                                    reqMapSP ) );
        spSubResult->SetRequirementStatus( TestResult::FAIL, requirement );

        RateMap::const_iterator iter;
        
        for ( iter = _audioEditRates.begin(); iter != _audioEditRates.end(); iter++ )
        {
            wstringstream ss;
            ss << iter->second << L" audio track(s) have an edit rate of " << iter->first.first << L"/" << iter->first.second << L".";
            spSubResult->AddDetail( ss.str().c_str() );
        }
        for ( iter = _audioSampleRates.begin(); iter != _audioSampleRates.end(); iter++ )
        {
            wstringstream ss;
            ss << iter->second << L" audio track(s) have an sample rate of " << iter->first.first << L"/" << iter->first.second << L".";
            spSubResult->AddDetail( ss.str().c_str() );
        }
        if ( _staticAudioTracks > 0 )
        {
            wstringstream ss;
            ss << _staticAudioTracks << L" audio track(s) are static and have no edit rate.";
            spSubResult->AddDetail( ss.str().c_str() );
        }
        if ( _unknownAudioTracks > 0 )
        {
            wstringstream ss;
            ss << _unknownAudioTracks << L" audio track(s) are of an unknown type for which an edit rate cannot be accessed.";
            spSubResult->AddDetail( ss.str().c_str() );
        }
        
        _spResult->AppendSubtestResult( spSubResult );
        _spResult->SetResult( _spResult->GetAggregateResult() );
        _spResult->SetRequirementStatus( TestResult::FAIL, requirement );
               
    }
    //Verify the edit rate matches the sample rate
    else if ( _audioEditRates.size() == 1 &&  _audioSampleRates.size() == 1 )
    {
        RateMap::const_iterator iter;
        RationalKey kEditRate;
        RationalKey kSampleRate;
        
        for ( iter = _audioEditRates.begin(); iter != _audioEditRates.end(); iter++ )
        {
            kEditRate = iter->first;
        }
        for ( iter = _audioSampleRates.begin(); iter != _audioSampleRates.end(); iter++ )
        {
            kSampleRate = iter->first;
        }
        
        double dEditRate = (double)kEditRate.first/(double)kEditRate.second;
        double dSampleRate = (double)kSampleRate.first/(double)kSampleRate.second;
        
        if ( dEditRate != dSampleRate )
        {
            wstringstream ss;
            ss << L"All audio tracks have an edit rate of " << kEditRate.first
               << L"/" << kEditRate.second << L" but a sample rate of "
               << kSampleRate.first << L"/" << kSampleRate.second << L".";
            _spResult->AddInformationResult( L"REQ_EP_099", ss.str().c_str(), TestResult::FAIL );
        }
        
    }
}

} // end of namespace aafanalyzer
