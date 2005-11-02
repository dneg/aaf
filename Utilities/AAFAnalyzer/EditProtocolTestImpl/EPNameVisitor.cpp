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

//AAF files
#include <AAFResult.h>

namespace {

} // end of namespace

//======================================================================

namespace aafanalyzer {

using namespace boost;

EPNameVisitor::EPNameVisitor( wostream& log )
    : EPTypedVisitor(),
      _log( log ),
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

bool EPNameVisitor::PreOrderVisit( EPTypedObjNode<IAAFCompositionMob, EPTopLevelComposition>& node )
{
    AxCompositionMob axCompMob( node.GetAAFObjectOfType() );
    if ( !this->VisitComposition( L"Top-Level Composition", L"REQ_EP_027", axCompMob ) )
    {
        return false;
    }
    _topLevelNames.insert( axCompMob.GetName() );
    return true;
}

bool EPNameVisitor::PreOrderVisit( EPTypedObjNode<IAAFCompositionMob, EPLowerLevelComposition>& node )
{
    AxCompositionMob axCompMob( node.GetAAFObjectOfType() );
    if ( !this->VisitComposition( L"Lower-Level Composition", L"REQ_EP_032", axCompMob ) )
    {
        return false;
    }
    _lowerLevelNames.insert( axCompMob.GetName() );
    return true;
}

bool EPNameVisitor::PreOrderVisit( EPTypedObjNode<IAAFCompositionMob, EPSubClipComposition>& node )
{
    AxCompositionMob axCompMob( node.GetAAFObjectOfType() );
    return this->VisitComposition( L"Sub-Clip Composition", L"REQ_EP_038", axCompMob );
}

bool EPNameVisitor::PreOrderVisit( EPTypedObjNode<IAAFCompositionMob, EPAdjustedClipComposition>& node )
{
    AxCompositionMob axCompMob( node.GetAAFObjectOfType() );
    return this->VisitComposition( L"Adjusted Clip Composition", L"REQ_EP_047", axCompMob );
}

bool EPNameVisitor::PreOrderVisit( EPTypedObjNode<IAAFMasterMob, EPTemplateClip>& node )
{
    AxMasterMob axMastMob( node.GetAAFObjectOfType() );
    return this->VisitNonComposition( L"Template Clip", L"REQ_EP_052", axMastMob );
}

bool EPNameVisitor::PreOrderVisit( EPTypedObjNode<IAAFMasterMob, EPClip>& node )
{
    AxMasterMob axMastMob( node.GetAAFObjectOfType() );
    return this->VisitNonComposition( L"Clip", L"REQ_EP_057", axMastMob );
}

bool EPNameVisitor::PreOrderVisit( EPTypedObjNode<IAAFSourceMob, EPRecordingSource>& node )
{
    AxSourceMob axSrcMob( node.GetAAFObjectOfType() );
    return this->VisitNonComposition( L"Recording Source", L"REQ_EP_073", axSrcMob );
}

bool EPNameVisitor::PreOrderVisit( EPTypedObjNode<IAAFSourceMob, EPTapeSource>& node )
{
    AxSourceMob axSrcMob( node.GetAAFObjectOfType() );
    return this->VisitNonComposition( L"Tape Source", L"REQ_EP_081", axSrcMob );
}

bool EPNameVisitor::PreOrderVisit( EPTypedObjNode<IAAFSourceMob, EPFilmSource>& node )
{
    AxSourceMob axSrcMob( node.GetAAFObjectOfType() );
    return this->VisitNonComposition( L"Film Source", L"REQ_EP_086", axSrcMob );
}

bool EPNameVisitor::EdgeVisit(AAFComponentReference& edge)
{
    return false;
}

bool EPNameVisitor::EdgeVisit(AAFSlotReference& edge)
{
    return false;
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
    this->CheckForUniqueNames( _topLevelNames, L"REQ_EP_027", L"Top-Level Composition" );
    this->CheckForUniqueNames( _lowerLevelNames, L"REQ_EP_032", L"Lower-Level Composition" );
}

void EPNameVisitor::CheckForUniqueNames( NameSet& names, const AxString& reqId, const AxString& type )
{
    
    NameSet::const_iterator iter;

    for ( iter = names.begin(); iter != names.end(); iter++ )
    {

        if ( _compositionNames[*iter] != 1 )
        {
           
            shared_ptr<const Requirement> failingReq = RequirementRegistry::GetInstance().GetRequirement( reqId );
            Requirement::RequirementMapSP reqMapSP(new Requirement::RequirementMap);
            (*reqMapSP)[reqId] = failingReq;
                
            AxString explain( type + L" \"" + *iter + L"\" does not have a unique composition name." );
            shared_ptr<DetailLevelTestResult> spFailure( new DetailLevelTestResult( _spResult->GetName(),
                                        L"-", // desc
                                        explain,
                                        L"-", // docref
                                        TestResult::FAIL,
                                        reqMapSP ) );
            spFailure->SetRequirementStatus( TestResult::FAIL, failingReq );
                
            _spResult->AppendSubtestResult( spFailure );
            _spResult->SetResult( _spResult->GetAggregateResult() );
            _spResult->SetRequirementStatus( TestResult::FAIL, failingReq );
                       
        }
    }
}

} // end of namespace aafanalyzer
