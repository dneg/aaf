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
#include <AxIterator.h>
#include <AxMetaDef.h>

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
               )                          )
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
        aafSlotID_t slotId = (*iter)->GetSlotID();
        AxClassDef clsDef( (*iter)->GetDefinition() );
        if ( this->IsType( clsDef, kAAFClassID_TimelineMobSlot, kAAFClassID_MobSlot ) )
        {
            AxTimelineMobSlot axMobSlot( AxQueryInterface<IAAFMobSlot, IAAFTimelineMobSlot>( **iter ) );
            aafRational_t editRate = axMobSlot.GetEditRate();
            AxDataDef axDataDef( axMobSlot.GetDataDef() );
            testPassed = TestEditRate( editRate, axDataDef, mobName, slotId );
        }
        else if ( this->IsType( clsDef, kAAFClassID_StaticMobSlot, kAAFClassID_MobSlot ) )
        {
            //Don't bother getting the static mob slot because it does not have
            //an edit rate.
            wstringstream ss;
            ss << L"Mob Slot with ID = " << slotId << L" of " << mobName
               << L" is a StaticMobSlot and does not have an edit rate.";
            _spResult->AddInformationResult( L"REQ_EP_090", ss.str().c_str(), TestResult::FAIL );
        }
        else if ( this->IsType( clsDef, kAAFClassID_EventMobSlot, kAAFClassID_MobSlot ) )
        {
            AxEventMobSlot axMobSlot( AxQueryInterface<IAAFMobSlot, IAAFEventMobSlot>( **iter ) );
            aafRational_t editRate = axMobSlot.GetEditRate();
            AxDataDef axDataDef( axMobSlot.GetDataDef() );
            testPassed = TestEditRate( editRate, axDataDef, mobName, slotId );
        }
        else
        {
            //This is an unknown type of mob slot so there is no way to get an
            //edit rate
            wstringstream ss;
            ss << L"Mob Slot with ID = " << slotId << L" of " << mobName
               << L" is not a known type of MobSlot and its edit rate cannot be accessed.";
            _spResult->AddInformationResult( L"REQ_EP_090", ss.str().c_str(), TestResult::FAIL );
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
        _spResult->AddInformationResult( L"REQ_EP_091", ss.str().c_str(), TestResult::FAIL );
        return false;
    }
    else if ( editRate.denominator < 0 )
    {
        ss << L"has an edit rate with a negative denominator (" << editRate.numerator
           << L"/" << editRate.denominator << L").";
        _spResult->AddInformationResult( L"REQ_EP_091", ss.str().c_str(), TestResult::FAIL );
        return false;
    }
    else if ( editRate.numerator == 0 )
    {
        ss << L"has an edit rate with a zero numerator (" << editRate.numerator
           << L"/" << editRate.denominator << L").";
        _spResult->AddInformationResult( L"REQ_EP_091", ss.str().c_str(), TestResult::FAIL );
        return false;
    }
    else if ( editRate.denominator == 0 )
    {
        ss << L"has an edit rate with a zero denominator (" << editRate.numerator
           << L"/" << editRate.denominator << L").";
        _spResult->AddInformationResult( L"REQ_EP_091", ss.str().c_str(), TestResult::FAIL );
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
        _spResult->AddInformationResult( L"REQ_EP_090", ss.str().c_str(), TestResult::FAIL );
        return false;
    }
    
    return true;
    
}

} // end of namespace aafanalyzer
