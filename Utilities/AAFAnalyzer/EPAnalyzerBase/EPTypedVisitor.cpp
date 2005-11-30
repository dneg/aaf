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

//Edit Protocol Analyzer Base files
#include "EPTypedVisitor.h"

//Analyzer Base files
#include <DepthFirstTraversal.h>
#include <EdgeMap.h>

//Ax files
#include <AxMob.h>
#include <AxEx.h>
#include <AxMetaDef.h>
#include <AxMobSlot.h>
#include <AxDefObject.h>

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
        
        bool PreOrderVisit( EPTypedObjNode<IAAFCompositionMob, EPTopLevelComposition>& node )
        {
            AxCompositionMob axMob( node.GetAAFObjectOfType() );
            _parentName = this->GetMobName( axMob, EPTopLevelComposition::GetName() );
            return false;
        }
        
        bool PreOrderVisit( EPTypedObjNode<IAAFCompositionMob, EPLowerLevelComposition>& node )
        {
            AxCompositionMob axMob( node.GetAAFObjectOfType() );
            _parentName = this->GetMobName( axMob, EPLowerLevelComposition::GetName() );
            return false;
        }
        
        bool PreOrderVisit( EPTypedObjNode<IAAFCompositionMob, EPSubClipComposition>& node )
        {
            AxCompositionMob axMob( node.GetAAFObjectOfType() );
            _parentName = this->GetMobName( axMob, EPSubClipComposition::GetName() );
            return false;
        }
        
        bool PreOrderVisit( EPTypedObjNode<IAAFCompositionMob, EPAdjustedClipComposition>& node )
        {
            AxCompositionMob axMob( node.GetAAFObjectOfType() );
            _parentName = this->GetMobName( axMob, EPAdjustedClipComposition::GetName() );
            return false;
        }
        
        bool PreOrderVisit( EPTypedObjNode<IAAFMasterMob, EPTemplateClip>& node )
        {
            AxMasterMob axMob( node.GetAAFObjectOfType() );
            _parentName = this->GetMobName( axMob, EPTemplateClip::GetName() );
            return false;
        }
        
        bool PreOrderVisit( EPTypedObjNode<IAAFMasterMob, EPClip>& node )
        {
            AxMasterMob axMob( node.GetAAFObjectOfType() );
            _parentName = this->GetMobName( axMob, EPClip::GetName() );
            return false;
        }
        
        bool PreOrderVisit( EPTypedObjNode<IAAFSourceMob, EPFileSource>& node )
        {
            AxSourceMob axMob( node.GetAAFObjectOfType() );
            _parentName = this->GetMobName( axMob, EPFileSource::GetName() );
            return false;
        }
        
        bool PreOrderVisit( EPTypedObjNode<IAAFSourceMob, EPRecordingSource>& node )
        {
            AxSourceMob axMob( node.GetAAFObjectOfType() );
            _parentName = this->GetMobName( axMob, EPRecordingSource::GetName() );
            return false;
        }
        
        bool PreOrderVisit( EPTypedObjNode<IAAFSourceMob, EPImportSource>& node )
        {
            AxSourceMob axMob( node.GetAAFObjectOfType() );
            _parentName = this->GetMobName( axMob, EPImportSource::GetName() );
            return false;
        }
        
        bool PreOrderVisit( EPTypedObjNode<IAAFSourceMob, EPTapeSource>& node )
        {
            AxSourceMob axMob( node.GetAAFObjectOfType() );
            _parentName = this->GetMobName( axMob, EPTapeSource::GetName() );
            return false;
        }
        
        bool PreOrderVisit( EPTypedObjNode<IAAFSourceMob, EPFilmSource>& node )
        {
            AxSourceMob axMob( node.GetAAFObjectOfType() );
            _parentName = this->GetMobName( axMob, EPFilmSource::GetName() );
            return false;
        }
        
        bool PreOrderVisit( EPTypedObjNode<IAAFSourceMob, EPAuxiliarySource>& node )
        {
            AxSourceMob axMob( node.GetAAFObjectOfType() );
            _parentName = this->GetMobName( axMob, EPAuxiliarySource::GetName() );
            return false;
        }
        
        bool PreOrderVisit( AAFTypedObjNode<IAAFCompositionMob>& node )
        {
            AxCompositionMob axMob( node.GetAAFObjectOfType() );
            _parentName = this->GetMobName( axMob, L"Composition Mob" );
            return false;
        }
        
        bool PreOrderVisit( AAFTypedObjNode<IAAFMasterMob>& node )
        {
            AxMasterMob axMob( node.GetAAFObjectOfType() );
            _parentName = this->GetMobName( axMob, L"Master Mob" );
            return false;
        }
        
        bool PreOrderVisit( AAFTypedObjNode<IAAFSourceMob>& node )
        {
            AxSourceMob axMob( node.GetAAFObjectOfType() );
            _parentName = this->GetMobName( axMob, L"Source Mob" );
            return false;
        }
        
        bool PreOrderVisit( AAFTypedObjNode<IAAFMob>& node )
        {
            AxMob axMob( node.GetAAFObjectOfType() );
            _parentName = this->GetMobName( axMob, L"Mob" );
            return false;
        }
        
        bool PreOrderVisit( AAFTypedObjNode<IAAFTimelineMobSlot>& node )
        {
            shared_ptr<AAFTypedObjNode<IAAFMobSlot> > spGeneric( node.DownCast<IAAFMobSlot>() );
            return this->PreOrderVisit( *spGeneric );
        }
        
        bool PreOrderVisit( AAFTypedObjNode<IAAFEventMobSlot>& node )
        {
            shared_ptr<AAFTypedObjNode<IAAFMobSlot> > spGeneric( node.DownCast<IAAFMobSlot>() );
            return this->PreOrderVisit( *spGeneric );
        }
        
        bool PreOrderVisit( AAFTypedObjNode<IAAFStaticMobSlot>& node )
        {
            shared_ptr<AAFTypedObjNode<IAAFMobSlot> > spGeneric( node.DownCast<IAAFMobSlot>() );
            return this->PreOrderVisit( *spGeneric );
        }
        
        bool PreOrderVisit( AAFTypedObjNode<IAAFMobSlot>& node )
        {
            AxMobSlot axMobSlot( node.GetAAFObjectOfType() );
            aafSlotID_t slotId = axMobSlot.GetSlotID();
            
            wstringstream ss;
            ss << L"Mob slot with ID = " << slotId << " of ";
            _slotName = ss.str().c_str();
            return true;
        }
        
        const AxString GetParentMobName()
        {
            return _parentName;
        }
        
        const AxString GetParentSlotName()
        {
            return _slotName + _parentName;
        }
        
    private:
    
        AxString _parentName;
        AxString _slotName;
    
        // prohibited
        ParentMobVisitor( const ParentMobVisitor& );
        ParentMobVisitor& operator=( const ParentMobVisitor& );
};

class ChildMobSlotVisitor : public EPTypedVisitor
{
    public:
        ChildMobSlotVisitor()
            : _spEssenceTracks( new EPTypedVisitor::MobSlotSet ),
              _spAudioTracks( new EPTypedVisitor::MobSlotSet ),
              _spVideoTracks( new EPTypedVisitor::MobSlotSet ),
              _spTimecodeTracks( new EPTypedVisitor::MobSlotSet ),
              _spEdgecodeTracks( new EPTypedVisitor::MobSlotSet )
        {}
        
        ~ChildMobSlotVisitor()
        {}
        
        bool PreOrderVisit( EPTypedObjNode<IAAFTimelineMobSlot, EPEssenceTrack>& node )
        {
            shared_ptr<AxMobSlot> spSlot( new AxTimelineMobSlot( node.GetAAFObjectOfType() ) );
            _spEssenceTracks->insert( spSlot );
            return false;
        }
        
        bool PreOrderVisit( EPTypedObjNode<IAAFTimelineMobSlot, EPAudioTrack>& node )
        {
            shared_ptr<AxMobSlot> spSlot( new AxTimelineMobSlot( node.GetAAFObjectOfType() ) );
            _spEssenceTracks->insert( spSlot );
            _spAudioTracks->insert( spSlot );            
            return false;
        }
        
        bool PreOrderVisit( EPTypedObjNode<IAAFTimelineMobSlot, EPVideoTrack>& node )
        {
            shared_ptr<AxMobSlot> spSlot( new AxTimelineMobSlot( node.GetAAFObjectOfType() ) );
            _spEssenceTracks->insert( spSlot );
            _spVideoTracks->insert( spSlot );
            return false;
        }
        
        bool PreOrderVisit( EPTypedObjNode<IAAFTimelineMobSlot, EPTimecodeTrack>& node )
        {
            shared_ptr<AxMobSlot> spSlot( new AxTimelineMobSlot( node.GetAAFObjectOfType() ) );
            _spTimecodeTracks->insert( spSlot );
            return false;
        }
        
        bool PreOrderVisit( EPTypedObjNode<IAAFTimelineMobSlot, EPEdgecodeTrack>& node )
        {
            shared_ptr<AxMobSlot> spSlot( new AxTimelineMobSlot( node.GetAAFObjectOfType() ) );
            _spEdgecodeTracks->insert( spSlot );
            return false;
        }

        bool PreOrderVisit( EPTypedObjNode<IAAFStaticMobSlot, EPEssenceTrack>& node )
        {
            shared_ptr<AxMobSlot> spSlot( new AxStaticMobSlot( node.GetAAFObjectOfType() ) );
            _spEssenceTracks->insert( spSlot );
            return false;
        }
        
        bool PreOrderVisit( EPTypedObjNode<IAAFStaticMobSlot, EPAudioTrack>& node )
        {
            shared_ptr<AxMobSlot> spSlot( new AxStaticMobSlot( node.GetAAFObjectOfType() ) );
            _spEssenceTracks->insert( spSlot );
            _spAudioTracks->insert( spSlot );            
            return false;
        }
        
        bool PreOrderVisit( EPTypedObjNode<IAAFStaticMobSlot, EPVideoTrack>& node )
        {
            shared_ptr<AxMobSlot> spSlot( new AxStaticMobSlot( node.GetAAFObjectOfType() ) );
            _spEssenceTracks->insert( spSlot );
            _spVideoTracks->insert( spSlot );
            return false;
        }
        
        bool PreOrderVisit( EPTypedObjNode<IAAFStaticMobSlot, EPTimecodeTrack>& node )
        {
            shared_ptr<AxMobSlot> spSlot( new AxStaticMobSlot( node.GetAAFObjectOfType() ) );
            _spTimecodeTracks->insert( spSlot );
            return false;
        }
        
        bool PreOrderVisit( EPTypedObjNode<IAAFStaticMobSlot, EPEdgecodeTrack>& node )
        {
            shared_ptr<AxMobSlot> spSlot( new AxStaticMobSlot( node.GetAAFObjectOfType() ) );
            _spEdgecodeTracks->insert( spSlot );
            return false;
        }
        
        bool PreOrderVisit( EPTypedObjNode<IAAFEventMobSlot, EPEssenceTrack>& node )
        {
            shared_ptr<AxMobSlot> spSlot( new AxEventMobSlot( node.GetAAFObjectOfType() ) );
            _spEssenceTracks->insert( spSlot );
            return false;
        }
        
        bool PreOrderVisit( EPTypedObjNode<IAAFEventMobSlot, EPAudioTrack>& node )
        {
            shared_ptr<AxMobSlot> spSlot( new AxEventMobSlot( node.GetAAFObjectOfType() ) );
            _spEssenceTracks->insert( spSlot );
            _spAudioTracks->insert( spSlot );            
            return false;
        }
        
        bool PreOrderVisit( EPTypedObjNode<IAAFEventMobSlot, EPVideoTrack>& node )
        {
            shared_ptr<AxMobSlot> spSlot( new AxEventMobSlot( node.GetAAFObjectOfType() ) );
            _spEssenceTracks->insert( spSlot );
            _spVideoTracks->insert( spSlot );
            return false;
        }
        
        bool PreOrderVisit( EPTypedObjNode<IAAFEventMobSlot, EPTimecodeTrack>& node )
        {
            shared_ptr<AxMobSlot> spSlot( new AxEventMobSlot( node.GetAAFObjectOfType() ) );
            _spTimecodeTracks->insert( spSlot );
            return false;
        }
        
        bool PreOrderVisit( EPTypedObjNode<IAAFEventMobSlot, EPEdgecodeTrack>& node )
        {
            shared_ptr<AxMobSlot> spSlot( new AxEventMobSlot( node.GetAAFObjectOfType() ) );
            _spEdgecodeTracks->insert( spSlot );
            return false;
        }
        
        bool PreOrderVisit( EPTypedObjNode<IAAFMobSlot, EPEssenceTrack>& node )
        {
            shared_ptr<AxMobSlot> spSlot( new AxMobSlot( node.GetAAFObjectOfType() ) );
            _spEssenceTracks->insert( spSlot );
            return false;
        }
        
        bool PreOrderVisit( EPTypedObjNode<IAAFMobSlot, EPAudioTrack>& node )
        {
            shared_ptr<AxMobSlot> spSlot( new AxMobSlot( node.GetAAFObjectOfType() ) );
            _spEssenceTracks->insert( spSlot );
            _spAudioTracks->insert( spSlot );            
            return false;
        }
        
        bool PreOrderVisit( EPTypedObjNode<IAAFMobSlot, EPVideoTrack>& node )
        {
            shared_ptr<AxMobSlot> spSlot( new AxMobSlot( node.GetAAFObjectOfType() ) );
            _spEssenceTracks->insert( spSlot );
            _spVideoTracks->insert( spSlot );
            return false;
        }
        
        bool PreOrderVisit( EPTypedObjNode<IAAFMobSlot, EPTimecodeTrack>& node )
        {
            shared_ptr<AxMobSlot> spSlot( new AxMobSlot( node.GetAAFObjectOfType() ) );
            _spTimecodeTracks->insert( spSlot );
            return false;
        }
        
        bool PreOrderVisit( EPTypedObjNode<IAAFMobSlot, EPEdgecodeTrack>& node )
        {
            shared_ptr<AxMobSlot> spSlot( new AxMobSlot( node.GetAAFObjectOfType() ) );
            _spEdgecodeTracks->insert( spSlot );
            return false;
        }
        
        shared_ptr<EPTypedVisitor::MobSlotSet> GetEssenceTracks()
        {
            return _spEssenceTracks;
        }

        shared_ptr<EPTypedVisitor::MobSlotSet> GetAudioTracks()
        {
            return _spAudioTracks;
        }
        
        shared_ptr<EPTypedVisitor::MobSlotSet> GetVideoTracks()
        {
            return _spVideoTracks;
        }
        
        shared_ptr<EPTypedVisitor::MobSlotSet> GetTimecodeTracks()
        {
            return _spTimecodeTracks;
        }
        
        shared_ptr<EPTypedVisitor::MobSlotSet> GetEdgecodeTracks()
        {
            return _spEdgecodeTracks;
        }
       
    private:
    
        shared_ptr<EPTypedVisitor::MobSlotSet> _spEssenceTracks;
        shared_ptr<EPTypedVisitor::MobSlotSet> _spAudioTracks;
        shared_ptr<EPTypedVisitor::MobSlotSet> _spVideoTracks;
        shared_ptr<EPTypedVisitor::MobSlotSet> _spTimecodeTracks;
        shared_ptr<EPTypedVisitor::MobSlotSet> _spEdgecodeTracks;
        
        // prohibited
        ChildMobSlotVisitor( const ChildMobSlotVisitor& );
        ChildMobSlotVisitor& operator=( const ChildMobSlotVisitor& );
};

} // end of namespace

//======================================================================
//======================================================================
//======================================================================

namespace aafanalyzer
{

using namespace std;
using namespace boost;

EPTypedVisitor::EPTypedVisitor()
{
}

EPTypedVisitor::~EPTypedVisitor()
{ 
}

AxString EPTypedVisitor::GetMobName( AxMob& axMob, const AxString& type )
{
    //Get the name of the mob
    AxString nodeName;
    try
    {
        nodeName = type + L": \"" + axMob.GetName() + L"\"";
    }
    catch ( const AxExHResult& ex )
    {
      if ( ex.getHResult() == AAFRESULT_PROP_NOT_PRESENT )
      {
        nodeName = L"Unnamed " + type;
      }
      else
      {
        throw ex;
      }
    }
    
    return nodeName;
    
}

AxString EPTypedVisitor::GetMobName( shared_ptr<EdgeMap> spEdgeMap, Node& node )
{
    shared_ptr<Node> spNode = dynamic_pointer_cast<Node>( node.GetSharedPointerToNode() );
    DepthFirstTraversal dfs( spEdgeMap, spNode );
    shared_ptr<ParentMobVisitor> spVisitor( new ParentMobVisitor );
    
    dfs.TraverseUp( spVisitor );
    return spVisitor->GetParentMobName();
}

AxString EPTypedVisitor::GetMobSlotName( shared_ptr<EdgeMap> spEdgeMap, Node& node )
{
    shared_ptr<Node> spNode = dynamic_pointer_cast<Node>( node.GetSharedPointerToNode() );
    DepthFirstTraversal dfs( spEdgeMap, spNode );
    shared_ptr<ParentMobVisitor> spVisitor( new ParentMobVisitor );
    
    dfs.TraverseUp( spVisitor );
    return spVisitor->GetParentSlotName();
}

bool EPTypedVisitor::IsType( AxClassDef& clsDef, aafUID_t type, aafUID_t parentType )
{
    aafUID_t auid = clsDef.GetAUID();
    
    if ( auid == type )
    {
        return true;
    }
    else if ( auid == parentType )
    {
        return false;
    }

    AxClassDef parentDef( clsDef.GetParent() );
    return IsType( parentDef, type, parentType );
}

shared_ptr<EPTypedVisitor::MobSlotSet> EPTypedVisitor::GetEssenceTracks( shared_ptr<EdgeMap> spEdgeMap, Node& node )
{
    shared_ptr<Node> spNode = dynamic_pointer_cast<Node>( node.GetSharedPointerToNode() );
    DepthFirstTraversal dfs( spEdgeMap, spNode );
    shared_ptr<ChildMobSlotVisitor> spVisitor( new ChildMobSlotVisitor );
    
    dfs.TraverseDown( spVisitor );
    return spVisitor->GetEssenceTracks();
}

shared_ptr<EPTypedVisitor::MobSlotSet> EPTypedVisitor::GetAudioTracks( shared_ptr<EdgeMap> spEdgeMap, Node& node )
{
    shared_ptr<Node> spNode = dynamic_pointer_cast<Node>( node.GetSharedPointerToNode() );
    DepthFirstTraversal dfs( spEdgeMap, spNode );
    shared_ptr<ChildMobSlotVisitor> spVisitor( new ChildMobSlotVisitor );
    
    dfs.TraverseDown( spVisitor );
    return spVisitor->GetAudioTracks();
}

shared_ptr<EPTypedVisitor::MobSlotSet> EPTypedVisitor::GetVideoTracks( shared_ptr<EdgeMap> spEdgeMap, Node& node )
{
    shared_ptr<Node> spNode = dynamic_pointer_cast<Node>( node.GetSharedPointerToNode() );
    DepthFirstTraversal dfs( spEdgeMap, spNode );
    shared_ptr<ChildMobSlotVisitor> spVisitor( new ChildMobSlotVisitor );
    
    dfs.TraverseDown( spVisitor );
    return spVisitor->GetVideoTracks();
}

shared_ptr<EPTypedVisitor::MobSlotSet> EPTypedVisitor::GetTimecodeTracks( shared_ptr<EdgeMap> spEdgeMap, Node& node )
{
    shared_ptr<Node> spNode = dynamic_pointer_cast<Node>( node.GetSharedPointerToNode() );
    DepthFirstTraversal dfs( spEdgeMap, spNode );
    shared_ptr<ChildMobSlotVisitor> spVisitor( new ChildMobSlotVisitor );
    
    dfs.TraverseDown( spVisitor );
    return spVisitor->GetTimecodeTracks();
}

shared_ptr<EPTypedVisitor::MobSlotSet> EPTypedVisitor::GetEdgecodeTracks( shared_ptr<EdgeMap> spEdgeMap, Node& node )
{
    shared_ptr<Node> spNode = dynamic_pointer_cast<Node>( node.GetSharedPointerToNode() );
    DepthFirstTraversal dfs( spEdgeMap, spNode );
    shared_ptr<ChildMobSlotVisitor> spVisitor( new ChildMobSlotVisitor );
    
    dfs.TraverseDown( spVisitor );
    return spVisitor->GetEdgecodeTracks();
}


} // end of namespace diskstream

