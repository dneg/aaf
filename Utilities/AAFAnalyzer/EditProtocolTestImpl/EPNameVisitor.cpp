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
#include "EPNameVisitor.h"
#include "EPNameTest.h"

//Test/Result files
#include <DetailLevelTestResult.h>
#include <TestRegistry.h>

//Requirement files
#include <RequirementRegistry.h>
#include <Requirement.h>

//Ax files
#include <AxUtil.h>
#include <AxMobSlot.h>
#include <AxIterator.h>
#include <AxDefObject.h>
#include <AxDictionary.h>

//AAF files
#include <AAFResult.h>

//STL files
#include <sstream>

namespace {

} // end of namespace

//======================================================================

namespace aafanalyzer {

using namespace boost;

EPNameVisitor::EPNameVisitor( wostream& log, shared_ptr<EdgeMap> spEdgeMap )
    : EPTypedVisitor(),
      _log( log ),
      _spEdgeMap( spEdgeMap ),
      _spResult( new DetailLevelTestResult( L"Edit Protocol Naming Visitor",
                                            L"Visit derivation chain mobs and verify they have valid names.",
                                            L"", // explain
                                            L"", // DOCREF REQUIRED
                                            TestResult::PASS,
                                            TestRegistry::GetInstance().GetRequirementsForTest( EPNameTest::GetTestInfo().GetName() )
               )                          )
{}

EPNameVisitor::~EPNameVisitor()
{}

/*
 * Mob Visitors
 */
bool EPNameVisitor::PreOrderVisit( EPTypedObjNode<IAAFCompositionMob, EPTopLevelComposition>& node )
{
    AxCompositionMob axCompMob( node.GetAAFObjectOfType() );
    if ( !this->VisitComposition( EPTopLevelComposition::GetName(), L"REQ_EP_027", axCompMob ) )
    {
        return false;
    }
    _topLevelNames.insert( axCompMob.GetName() );
    return true;
}

bool EPNameVisitor::PreOrderVisit( EPTypedObjNode<IAAFCompositionMob, EPLowerLevelComposition>& node )
{
    AxCompositionMob axCompMob( node.GetAAFObjectOfType() );
    if ( !this->VisitComposition( EPLowerLevelComposition::GetName(), L"REQ_EP_032", axCompMob ) )
    {
        return false;
    }
    _lowerLevelNames.insert( axCompMob.GetName() );
    return true;
}

bool EPNameVisitor::PreOrderVisit( EPTypedObjNode<IAAFCompositionMob, EPSubClipComposition>& node )
{
    AxCompositionMob axCompMob( node.GetAAFObjectOfType() );
    return this->VisitComposition( EPSubClipComposition::GetName(), L"REQ_EP_038", axCompMob );
}

bool EPNameVisitor::PreOrderVisit( EPTypedObjNode<IAAFCompositionMob, EPAdjustedClipComposition>& node )
{
    AxCompositionMob axCompMob( node.GetAAFObjectOfType() );
    return this->VisitComposition( EPAdjustedClipComposition::GetName(), L"REQ_EP_047", axCompMob );
}

bool EPNameVisitor::PreOrderVisit( EPTypedObjNode<IAAFMasterMob, EPTemplateClip>& node )
{
    AxMasterMob axMastMob( node.GetAAFObjectOfType() );
    return this->VisitNonComposition( EPTemplateClip::GetName(), L"REQ_EP_052", axMastMob );
}

bool EPNameVisitor::PreOrderVisit( EPTypedObjNode<IAAFMasterMob, EPClip>& node )
{
    AxMasterMob axMastMob( node.GetAAFObjectOfType() );
    return this->VisitNonComposition( EPClip::GetName(), L"REQ_EP_057", axMastMob );
}

bool EPNameVisitor::PreOrderVisit( EPTypedObjNode<IAAFSourceMob, EPRecordingSource>& node )
{
    AxSourceMob axSrcMob( node.GetAAFObjectOfType() );
    return this->VisitNonComposition( EPRecordingSource::GetName(), L"REQ_EP_073", axSrcMob );
}

bool EPNameVisitor::PreOrderVisit( EPTypedObjNode<IAAFSourceMob, EPTapeSource>& node )
{
    AxSourceMob axSrcMob( node.GetAAFObjectOfType() );
    return this->VisitNonComposition( EPTapeSource::GetName(), L"REQ_EP_081", axSrcMob );
}

bool EPNameVisitor::PreOrderVisit( EPTypedObjNode<IAAFSourceMob, EPFilmSource>& node )
{
    AxSourceMob axSrcMob( node.GetAAFObjectOfType() );
    return this->VisitNonComposition( EPFilmSource::GetName(), L"REQ_EP_086", axSrcMob );
}

/*
 * Essence Track Visitors:
 *      The behaviour is the same regardless of the type of essence track,
 *      so cast the node to a base essence track (IAAFMobSlot, EPEssenceTrack)
 *      and call the PreOrderVisit on it.
 */
bool EPNameVisitor::PreOrderVisit( EPTypedObjNode<IAAFTimelineMobSlot, EPAudioTrack>& node )
{
    shared_ptr<EPTypedObjNode<IAAFMobSlot, EPEssenceTrack> > spGeneric( node.DownCast<IAAFMobSlot, EPEssenceTrack>() );
    return this->PreOrderVisit( *spGeneric );
}

bool EPNameVisitor::PreOrderVisit( EPTypedObjNode<IAAFTimelineMobSlot, EPVideoTrack>& node )
{
    shared_ptr<EPTypedObjNode<IAAFMobSlot, EPEssenceTrack> > spGeneric( node.DownCast<IAAFMobSlot, EPEssenceTrack>() );
    return this->PreOrderVisit( *spGeneric );
}

bool EPNameVisitor::PreOrderVisit( EPTypedObjNode<IAAFTimelineMobSlot, EPEssenceTrack>& node )
{
    shared_ptr<EPTypedObjNode<IAAFMobSlot, EPEssenceTrack> > spGeneric( node.DownCast<IAAFMobSlot, EPEssenceTrack>() );
    return this->PreOrderVisit( *spGeneric );
}

bool EPNameVisitor::PreOrderVisit( EPTypedObjNode<IAAFStaticMobSlot, EPAudioTrack>& node )
{
    shared_ptr<EPTypedObjNode<IAAFMobSlot, EPEssenceTrack> > spGeneric( node.DownCast<IAAFMobSlot, EPEssenceTrack>() );
    return this->PreOrderVisit( *spGeneric );
}

bool EPNameVisitor::PreOrderVisit( EPTypedObjNode<IAAFStaticMobSlot, EPVideoTrack>& node )
{
    shared_ptr<EPTypedObjNode<IAAFMobSlot, EPEssenceTrack> > spGeneric( node.DownCast<IAAFMobSlot, EPEssenceTrack>() );
    return this->PreOrderVisit( *spGeneric );
}

bool EPNameVisitor::PreOrderVisit( EPTypedObjNode<IAAFStaticMobSlot, EPEssenceTrack>& node )
{
    shared_ptr<EPTypedObjNode<IAAFMobSlot, EPEssenceTrack> > spGeneric( node.DownCast<IAAFMobSlot, EPEssenceTrack>() );
    return this->PreOrderVisit( *spGeneric );
}

bool EPNameVisitor::PreOrderVisit( EPTypedObjNode<IAAFEventMobSlot, EPAudioTrack>& node )
{
    shared_ptr<EPTypedObjNode<IAAFMobSlot, EPEssenceTrack> > spGeneric( node.DownCast<IAAFMobSlot, EPEssenceTrack>() );
    return this->PreOrderVisit( *spGeneric );
}

bool EPNameVisitor::PreOrderVisit( EPTypedObjNode<IAAFEventMobSlot, EPVideoTrack>& node )
{
    shared_ptr<EPTypedObjNode<IAAFMobSlot, EPEssenceTrack> > spGeneric( node.DownCast<IAAFMobSlot, EPEssenceTrack>() );
    return this->PreOrderVisit( *spGeneric );
}

bool EPNameVisitor::PreOrderVisit( EPTypedObjNode<IAAFEventMobSlot, EPEssenceTrack>& node )
{
    shared_ptr<EPTypedObjNode<IAAFMobSlot, EPEssenceTrack> > spGeneric( node.DownCast<IAAFMobSlot, EPEssenceTrack>() );
    return this->PreOrderVisit( *spGeneric );
}

bool EPNameVisitor::PreOrderVisit( EPTypedObjNode<IAAFMobSlot, EPAudioTrack>& node )
{
    shared_ptr<EPTypedObjNode<IAAFMobSlot, EPEssenceTrack> > spGeneric( node.DownCast<IAAFMobSlot, EPEssenceTrack>() );
    return this->PreOrderVisit( *spGeneric );
}

bool EPNameVisitor::PreOrderVisit( EPTypedObjNode<IAAFMobSlot, EPVideoTrack>& node )
{
    shared_ptr<EPTypedObjNode<IAAFMobSlot, EPEssenceTrack> > spGeneric( node.DownCast<IAAFMobSlot, EPEssenceTrack>() );
    return this->PreOrderVisit( *spGeneric );
}

bool EPNameVisitor::PreOrderVisit( EPTypedObjNode<IAAFMobSlot, EPEssenceTrack>& node )
{
    AxMobSlot axMobSlot( node.GetAAFObjectOfType() );
    
    try
    {
        AxString trackName = axMobSlot.GetName();
        if ( _essenceTrackNames.find( trackName ) == _essenceTrackNames.end() )
        {
            _essenceTrackNames[trackName] = 1;
        }
        else
        {
            _essenceTrackNames[trackName] = _essenceTrackNames[trackName] + 1;
        }
    }
    catch ( const AxExHResult& ex )
    {
        if ( ex.getHResult() == AAFRESULT_PROP_NOT_PRESENT )
        {
            AxString mobName = GetMobName( _spEdgeMap, node );
            aafSlotID_t slotId = axMobSlot.GetSlotID();
            wstringstream ss;
            ss << L"MobSlot with ID = " << slotId << L" of " << mobName << L" does not have a valid name.";
            _spResult->AddInformationResult( L"REQ_EP_101", ss.str().c_str(), TestResult::FAIL );
            return false;
        }
        else
        {
            throw ex;
        }
    }
    return true;
}

/*
 * Definition Object Visitors
 */
bool EPNameVisitor::PreOrderVisit( AAFTypedObjNode<IAAFDataDef>& node )
{
    shared_ptr<AAFTypedObjNode<IAAFDefObject> > spGeneric( node.DownCast<IAAFDefObject>() );
    return this->PreOrderVisit( *spGeneric );
}

bool EPNameVisitor::PreOrderVisit( AAFTypedObjNode<IAAFParameterDef>& node )
{
    shared_ptr<AAFTypedObjNode<IAAFDefObject> > spGeneric( node.DownCast<IAAFDefObject>() );
    return this->PreOrderVisit( *spGeneric );
}

bool EPNameVisitor::PreOrderVisit( AAFTypedObjNode<IAAFPluginDef>& node )
{
    shared_ptr<AAFTypedObjNode<IAAFDefObject> > spGeneric( node.DownCast<IAAFDefObject>() );
    return this->PreOrderVisit( *spGeneric );
}

bool EPNameVisitor::PreOrderVisit( AAFTypedObjNode<IAAFContainerDef>& node )
{
    shared_ptr<AAFTypedObjNode<IAAFDefObject> > spGeneric( node.DownCast<IAAFDefObject>() );
    return this->PreOrderVisit( *spGeneric );
}

bool EPNameVisitor::PreOrderVisit( AAFTypedObjNode<IAAFInterpolationDef>& node )
{
    shared_ptr<AAFTypedObjNode<IAAFDefObject> > spGeneric( node.DownCast<IAAFDefObject>() );
    return this->PreOrderVisit( *spGeneric );
}

bool EPNameVisitor::PreOrderVisit( AAFTypedObjNode<IAAFTaggedValueDefinition>& node )
{
    shared_ptr<AAFTypedObjNode<IAAFDefObject> > spGeneric( node.DownCast<IAAFDefObject>() );
    return this->PreOrderVisit( *spGeneric );
}

bool EPNameVisitor::PreOrderVisit( AAFTypedObjNode<IAAFOperationDef>& node )
{
    shared_ptr<AAFTypedObjNode<IAAFDefObject> > spGeneric( node.DownCast<IAAFDefObject>() );
    return this->PreOrderVisit( *spGeneric );
}

bool EPNameVisitor::PreOrderVisit( AAFTypedObjNode<IAAFCodecDef>& node )
{
    shared_ptr<AAFTypedObjNode<IAAFDefObject> > spGeneric( node.DownCast<IAAFDefObject>() );
    return this->PreOrderVisit( *spGeneric );
}

bool EPNameVisitor::PreOrderVisit( AAFTypedObjNode<IAAFKLVDataDefinition>& node )
{
    shared_ptr<AAFTypedObjNode<IAAFDefObject> > spGeneric( node.DownCast<IAAFDefObject>() );
    return this->PreOrderVisit( *spGeneric );
}
 
bool EPNameVisitor::PreOrderVisit( AAFTypedObjNode<IAAFDefObject>& node )
{
    
    AxDefObject axDefObj( node.GetAAFObjectOfType() );
    
    try
    {
        axDefObj.GetName();
    }
    catch ( const AxExHResult& ex )
    {
        if ( ex.getHResult() == AAFRESULT_PROP_NOT_PRESENT )
        {
        
            shared_ptr<const Requirement> failingReq = RequirementRegistry::GetInstance().GetRequirement( L"REQ_EP_161" );
            Requirement::RequirementMapSP reqMapSP(new Requirement::RequirementMap);
            (*reqMapSP)[L"REQ_EP_161"] = failingReq;
        
            AxString explain( L"DefinitionObject does not have a valid name." );
            shared_ptr<DetailLevelTestResult> spFailure( new DetailLevelTestResult( _spResult->GetName(),
                                        L"-", // desc
                                        explain,
                                        L"-", // docref
                                        TestResult::FAIL,
                                        reqMapSP ) );
            spFailure->AddDetail( L"AUID: " + AxStringUtil::uid2Str( axDefObj.GetAUID() ) );
            spFailure->SetRequirementStatus( TestResult::FAIL, failingReq );
        
            _spResult->AppendSubtestResult( spFailure );
            _spResult->SetResult( _spResult->GetAggregateResult() );
            _spResult->SetRequirementStatus( TestResult::FAIL, failingReq );
        
            return false;
        }
        else
        {
            throw ex;
        }
    }
    return true;
}

shared_ptr<DetailLevelTestResult> EPNameVisitor::GetResult()
{
    return _spResult;
}

bool EPNameVisitor::VisitComposition( const AxString& type, const AxString& reqId, AxCompositionMob& axCompMob )
{
    AxString nodeName;

    try
    {
        nodeName = axCompMob.GetName();
    }
    catch ( const AxExHResult& ex )
    {
        if ( ex.getHResult() == AAFRESULT_PROP_NOT_PRESENT )
        {
            
            shared_ptr<const Requirement> failingReq = RequirementRegistry::GetInstance().GetRequirement( reqId );
            Requirement::RequirementMapSP reqMapSP(new Requirement::RequirementMap);
            (*reqMapSP)[reqId] = failingReq;
            
            AxString explain( type + L" does not have a valid name." );
            shared_ptr<DetailLevelTestResult> spFailure( new DetailLevelTestResult( _spResult->GetName(),
                                        L"-", // desc
                                        explain,
                                        L"-", // docref
                                        TestResult::FAIL,
                                        reqMapSP ) );
            spFailure->AddDetail( L"Mob ID: " + AxStringUtil::mobid2Str( axCompMob.GetMobID() ) );
            spFailure->SetRequirementStatus( TestResult::FAIL, failingReq );
            
            _spResult->AppendSubtestResult( spFailure );
            _spResult->SetResult( _spResult->GetAggregateResult() );
            _spResult->SetRequirementStatus( TestResult::FAIL, failingReq );
            
            return false;
        }
        else
        {
            throw ex;
        }
    }
    
    if ( _compositionNames.find( nodeName ) != _compositionNames.end() )
    {
        _compositionNames[nodeName] = _compositionNames[nodeName] + 1;
    }
    else
    {
        _compositionNames[nodeName] = 1;
    }
    
    return true;
    
}

bool EPNameVisitor::VisitNonComposition( const AxString& type, const AxString& reqId, AxMob& axMob )
{
    AxString nodeName;

    try
    {
        nodeName = axMob.GetName();
    }
    catch ( const AxExHResult& ex )
    {
        if ( ex.getHResult() == AAFRESULT_PROP_NOT_PRESENT )
        {
            
            shared_ptr<const Requirement> failingReq = RequirementRegistry::GetInstance().GetRequirement( reqId );
            Requirement::RequirementMapSP reqMapSP(new Requirement::RequirementMap);
            (*reqMapSP)[reqId] = failingReq;
            
            AxString explain( type + L" does not have a valid name." );
            shared_ptr<DetailLevelTestResult> spFailure( new DetailLevelTestResult( _spResult->GetName(),
                                        L"-", // desc
                                        explain,
                                        L"-", // docref
                                        TestResult::FAIL,
                                        reqMapSP ) );
            spFailure->AddDetail( L"Mob ID: " + AxStringUtil::mobid2Str( axMob.GetMobID() ) );
            spFailure->SetRequirementStatus( TestResult::FAIL, failingReq );
            
            _spResult->AppendSubtestResult( spFailure );
            _spResult->SetResult( _spResult->GetAggregateResult() );
            _spResult->SetRequirementStatus( TestResult::FAIL, failingReq );
            
            return false;
            
        }
        else
        {
            throw ex;
        }
    }
    
    return true;
    
}

void EPNameVisitor::CheckForUniqueNames()
{
    //Check composition names for uniqueness
    this->CheckForUniqueNames( _topLevelNames, L"REQ_EP_027", EPTopLevelComposition::GetName() );
    this->CheckForUniqueNames( _lowerLevelNames, L"REQ_EP_032", EPLowerLevelComposition::GetName() );
    
    //Check essence track names for uniqueness
    NameMap::const_iterator iter;
    for ( iter = _essenceTrackNames.begin(); iter != _essenceTrackNames.end(); iter++ )
    {
        if ( iter->second != 1 )
        {
            AxString explain( L"Essence Track \"" + iter->first + L"\" does not have a unique essence track name." );
            _spResult->AddInformationResult( L"REQ_EP_101", explain, TestResult::FAIL );
        }
    }
}

void EPNameVisitor::CheckForUniqueNames( NameSet& names, const AxString& reqId, const AxString& type )
{
    NameSet::const_iterator iter;
    for ( iter = names.begin(); iter != names.end(); iter++ )
    {
        if ( _compositionNames[*iter] != 1 )
        {
            AxString explain( type + L" \"" + *iter + L"\" does not have a unique composition name." );
            _spResult->AddInformationResult( reqId, explain, TestResult::FAIL );
        }
    }
}

} // end of namespace aafanalyzer
