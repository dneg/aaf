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
#include "EPMultiChannelAudioVisitor.h"
#include "EPMultiChannelAudioTest.h"

//Test/Result files
#include <DetailLevelTestResult.h>
#include <TestRegistry.h>

//Ax files
#include <AxMob.h>

namespace {

using namespace aafanalyzer;
    
} // end of namespace

//======================================================================

namespace aafanalyzer {

using namespace boost;
 
EPMultiChannelAudioVisitor::EPMultiChannelAudioVisitor( wostream& log,
							shared_ptr<TestLevelTestResult> spTestResult )
  : EPTypedVisitor( Visitor::FOLLOW_REFERENCES ),
    _log( log ),
    _spTestResult( spTestResult ),
    _level( 0 )
{}
    
EPMultiChannelAudioVisitor::~EPMultiChannelAudioVisitor()
{}

bool EPMultiChannelAudioVisitor::PreOrderVisit( EPTypedObjNode<IAAFSourceMob, EPMultiChannelAudioFileSource>& node )
{
    // Do not continue this traversal if the path has already been
    // traversed from a ancestor composition or master mob OR if the
    // parent of this node does not fall under a composition or master
    // mob in the current traversal path.
    if ( this->HaveVisited( node.GetLID() )  || _level == 0 )
    {
        return false;
    }
    
    // This node has not yet been visited, but is now being visited
    // and has an ancestor that is a master mob or a composition mob,
    // therefore REQ_EP_110 has been violated.

    AxSourceMob axMob( node.GetAAFObjectOfType() );

    _spTestResult->AddSingleResult(
        L"REQ_EP_110", 
        this->GetMobName( axMob, EPMultiChannelAudioFileSource::GetName() ) +
            L" is referenced from a Composition or Master Mob.",
        TestResult::FAIL ) ;
        
    //Record this visitation
    this->RecordVisit( node.GetLID() );

    //The test failed, so stop the traversal
    return false;
}

bool EPMultiChannelAudioVisitor::PreOrderVisit( AAFTypedObjNode<IAAFCompositionMob>& node )
{
    //Only continue traversal if this path has yet to be traversed.
    if ( this->HaveVisited( node.GetLID() ) )
    {
        return false;
    }
    
    //Increment the level of Composition and Master mobs.
    _level++;
    
    //Record this visitation
    this->RecordVisit( node.GetLID() );
    
    return true;
}

bool EPMultiChannelAudioVisitor::PreOrderVisit( AAFTypedObjNode<IAAFMasterMob>& node )
{
    //Only continue traversal if this path has yet to be traversed.
    if ( this->HaveVisited( node.GetLID() ) )
    {
        return false;
    }
    
    //Increment the level of Composition and Master mobs.
    _level++;
    
    //Record this visitation
    this->RecordVisit( node.GetLID() );
    
    return true;
}

bool EPMultiChannelAudioVisitor::PreOrderVisit( AAFTypedObjNode<IAAFSourceMob>& node )
{
    //Note: There is special code to handle the case of a Multi-Channel Audio
    //      FileSource.

    //Do not continue this traversal if the path has already been traversed from
    //a ancestor composition or master mob OR if the parent of this node does
    //not fall under a composition or master mob in the current traversal path.
    if ( this->HaveVisited( node.GetLID() ) || _level == 0 )
    {
        return false;
    }
    
    //Record this visitation.
    this->RecordVisit( node.GetLID() );
    
    return true;
}

bool EPMultiChannelAudioVisitor::PostOrderVisit( AAFTypedObjNode<IAAFCompositionMob>& node )
{
    _level--;
    return true;
}

bool EPMultiChannelAudioVisitor::PostOrderVisit( AAFTypedObjNode<IAAFMasterMob>& node )
{
    _level--;
    return true;
}

} // end of namespace aafanalyzer
