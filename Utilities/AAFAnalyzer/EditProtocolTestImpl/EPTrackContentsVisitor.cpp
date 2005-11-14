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
#include "EPTrackContentsVisitor.h"
#include "EPTrackContentsTest.h"

//Test/Result files
#include <DetailLevelTestResult.h>
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
 
EPTrackContentsVisitor::EPTrackContentsVisitor( wostream& log, shared_ptr<EdgeMap> spEdgeMap )
    : _log(log),
      _spEdgeMap( spEdgeMap ),
      _spResult( new DetailLevelTestResult( L"Edit Protocol Track Contents Visitor",
                                            L"Visit derivation chain material and make sure that their tracks contain the necessary data.",
                                            L"", // explain
                                            L"", // DOCREF REQUIRED
                                            TestResult::PASS,
                                            TestRegistry::GetInstance().GetRequirementsForTest( EPTrackContentsTest::GetTestInfo().GetName() )
               )                          )
{}
    
EPTrackContentsVisitor::~EPTrackContentsVisitor()
{}

bool EPTrackContentsVisitor::PreOrderVisit( EPTypedObjNode<IAAFTimelineMobSlot, EPAudioTrack>& node )
{
    shared_ptr<EPTypedObjNode<IAAFMobSlot, EPEssenceTrack> > spGeneric( node.DownCast<IAAFMobSlot, EPEssenceTrack>() );
    return this->PreOrderVisit( *spGeneric );
}

bool EPTrackContentsVisitor::PreOrderVisit( EPTypedObjNode<IAAFTimelineMobSlot, EPVideoTrack>& node )
{
    shared_ptr<EPTypedObjNode<IAAFMobSlot, EPEssenceTrack> > spGeneric( node.DownCast<IAAFMobSlot, EPEssenceTrack>() );
    return this->PreOrderVisit( *spGeneric );
}

bool EPTrackContentsVisitor::PreOrderVisit( EPTypedObjNode<IAAFTimelineMobSlot, EPEssenceTrack>& node )
{
    shared_ptr<EPTypedObjNode<IAAFMobSlot, EPEssenceTrack> > spGeneric( node.DownCast<IAAFMobSlot, EPEssenceTrack>() );
    return this->PreOrderVisit( *spGeneric );
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
    
    //Find the parent of this mob slot, if it is a File Source mob slot (or
    //an unknown type), there is no requirement that it has a PhysicalTrackNumber.
    shared_ptr<Node> spNode = dynamic_pointer_cast<Node>( node.GetSharedPointerToNode() );
    DepthFirstTraversal dfs( _spEdgeMap, spNode );
    shared_ptr<ParentMobVisitor> spVisitor( new ParentMobVisitor );
    
    dfs.TraverseUp( spVisitor );
    
    if ( spVisitor->NeedsPhysicalTrackNum() )
    {
    
        AxMobSlot axMobSlot( node.GetAAFObjectOfType() );
        
        try
        {
            axMobSlot.GetPhysicalNum();
        }
        catch ( const AxExHResult& ex )
        {
            if ( ex.getHResult() != AAFRESULT_PROP_NOT_PRESENT )
            {
                throw ex;
            }
            AxString mobName = this->GetMobName( _spEdgeMap, node );
    
            wstringstream ss;
            ss << L"Slot with ID ";
            ss << axMobSlot.GetSlotID();
            ss << L" in " << mobName << L" does not have a MobSlot::PhysicalTrackNumber property.";
            
            _spResult->AddInformationResult( L"REQ_EP_103", ss.str().c_str(), TestResult::FAIL );
         
            return false;
            
        }
        
    }
    
    return true;
    
}

shared_ptr<DetailLevelTestResult> EPTrackContentsVisitor::GetResult()
{
    return _spResult;
}

} // end of namespace aafanalyzer
