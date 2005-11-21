//=---------------------------------------------------------------------=
//
// $Id$
//
// The contents of this file are subject to the AAF SDK Public
// Source License Agreement (the "License") You may not use this file
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
#include "EPTrack.h"

//AAF Analyzer Base files
#include <MobNodeMap.h>

//Ax files
#include <AxDefObject.h>
#include <AxMetaDef.h>
#include <AxEssence.h>

//AAF files
#include <AAFClassDefUIDs.h>

namespace {

using namespace aafanalyzer;

bool IsType( AxClassDef& clsDef, aafUID_t type, aafUID_t parentType )
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

bool IsReferenceInFile( AxSourceClip& axSrcClip )
{
    aafSourceRef_t srcRef = axSrcClip.GetSourceReference();
    aafMobID_t mobid = srcRef.sourceID;   
    shared_ptr<Node> spNode;
    spNode = MobNodeMap::GetInstance().GetMobNode(mobid);
    return spNode != NULL;
}

} // end of namespace


//======================================================================
//======================================================================
//======================================================================

namespace aafanalyzer {
    
using namespace boost;

EPTrackObject::EPTrackObject()
{}

EPTrackObject::~EPTrackObject()
{}

const AxString EPTrackObject::GetName()
{
    return L"Edit Protocol Track Object";
}

//======================================================================

EPEssenceTrack::EPEssenceTrack()
{}

EPEssenceTrack::~EPEssenceTrack()
{}

const AxString EPEssenceTrack::GetName()
{
    return L"Essence Track";
}

//======================================================================

EPTimecodeTrack::EPTimecodeTrack()
{}

EPTimecodeTrack::~EPTimecodeTrack()
{}

const AxString EPTimecodeTrack::GetName()
{
    return L"Timecode Track";
}

//======================================================================

EPEdgecodeTrack::EPEdgecodeTrack()
{}

EPEdgecodeTrack::~EPEdgecodeTrack()
{}

const AxString EPEdgecodeTrack::GetName()
{
    return L"Edgecode Track";
}

//======================================================================

AxMobSlot& EPExtendedEssenceTrack::GetMobSlot()
{
    return _mobSlot;
}

AxSourceClip& EPExtendedEssenceTrack::GetSourceClip()
{
    return _sourceClip;
}

AxMob& EPExtendedEssenceTrack::GetMob()
{
    return _mob;
}
        
EPExtendedEssenceTrack::EPExtendedEssenceTrack( IAAFMobSlotSP mobSlot, IAAFSourceClipSP clip, IAAFMobSP mob )
    : _mobSlot( mobSlot ),
      _sourceClip( clip ),
      _mob( mob )
{}

const AxString EPExtendedEssenceTrack::GetName()
{
    return L"Edit Protocol Extended Essence Track";
}

//======================================================================

EPAudioTrack::EPAudioTrack( IAAFMobSlotSP mobSlot, IAAFSourceClipSP clip, IAAFMobSP mob )
    : EPExtendedEssenceTrack( mobSlot, clip, mob )
{}

shared_ptr<EPAudioTrack> EPAudioTrack::CreateAudioTrack( AxMobSlot& axMobSlot )
{

    //1. Ensure the slot's data def is either picture or sound
    AxDataDef axDataDef( axMobSlot.GetDataDef() );
    if ( axDataDef.IsSoundKind() )
    {
        //2. Ensure the segment is a source clip.
        AxSegment axSegment( axMobSlot.GetSegment() );
        AxClassDef segClsDef( axSegment.GetDefinition() );
        if ( IsType( segClsDef, kAAFClassID_SourceClip, kAAFClassID_Segment ) )
        {
            //3. Ensure the source clip contains  a source mob.
            AxSourceClip axSrcClip( AxQueryInterface<IAAFSegment, IAAFSourceClip>( axSegment ) );
            
            //4. Make sure that the referenced source mob is in this AAF file.
            if ( IsReferenceInFile( axSrcClip ) )
            {
                AxMob axMob( axSrcClip.ResolveRef() );
                AxClassDef mobClsDef( axMob.GetDefinition() );
                if ( IsType( mobClsDef, kAAFClassID_SourceMob, kAAFClassID_Mob ) )
                {
                    //4. Ensure the source mob is a file source mob.
                    AxSourceMob axSrcMob( AxQueryInterface<IAAFMob, IAAFSourceMob>( axMob ) );
                    AxEssenceDescriptor descriptor( axSrcMob.GetEssenceDescriptor() );
                    AxClassDef desClsDef( descriptor.GetDefinition() );
                    if ( IsType( desClsDef, kAAFClassID_FileDescriptor, kAAFClassID_EssenceDescriptor ) )
                    {
                        shared_ptr<EPAudioTrack> spTrack( new EPAudioTrack( axMobSlot, axSrcClip, axSrcMob ) );
                        return spTrack;
                    }
                }
            }
        }
    }
    
    //Return a null shared pointer if the mob slot does not contain an essence
    //track.
    return shared_ptr<EPAudioTrack>();
    
}

const AxString EPAudioTrack::GetName()
{
    return L"Audio Track";
}

//======================================================================

EPVideoTrack::EPVideoTrack( IAAFMobSlotSP mobSlot, IAAFSourceClipSP clip, IAAFMobSP mob )
    : EPExtendedEssenceTrack( mobSlot, clip, mob )
{}

shared_ptr<EPVideoTrack> EPVideoTrack::CreateVideoTrack( AxMobSlot& axMobSlot )
{

    //1. Ensure the slot's data def is either picture or sound
    AxDataDef axDataDef( axMobSlot.GetDataDef() );
    if ( axDataDef.IsPictureKind() )
    {
        //2. Ensure the segment is a source clip.
        AxSegment axSegment( axMobSlot.GetSegment() );
        AxClassDef segClsDef( axSegment.GetDefinition() );
        if ( IsType( segClsDef, kAAFClassID_SourceClip, kAAFClassID_Segment ) )
        {
            //3. Ensure the source clip contains  a source mob.
            AxSourceClip axSrcClip( AxQueryInterface<IAAFSegment, IAAFSourceClip>( axSegment ) );

            //4. Make sure that the referenced source mob is in this AAF file.
            if ( IsReferenceInFile( axSrcClip ) )
            {
                AxMob axMob( axSrcClip.ResolveRef() );
                AxClassDef mobClsDef( axMob.GetDefinition() );
                if ( IsType( mobClsDef, kAAFClassID_SourceMob, kAAFClassID_Mob ) )
                {
                    //5. Ensure the source mob is a file source mob.
                    AxSourceMob axSrcMob( AxQueryInterface<IAAFMob, IAAFSourceMob>( axMob ) );
                    AxEssenceDescriptor descriptor( axSrcMob.GetEssenceDescriptor() );
                    AxClassDef desClsDef( descriptor.GetDefinition() );
                    if ( IsType( desClsDef, kAAFClassID_FileDescriptor, kAAFClassID_EssenceDescriptor ) )
                    {
                        shared_ptr<EPVideoTrack> spTrack( new EPVideoTrack( axMobSlot, axSrcClip, axSrcMob ) );
                        return spTrack;
                    }
                }
            }
        }
    }
    
    //Return a null shared pointer if the mob slot does not contain an essence
    //track.
    return shared_ptr<EPVideoTrack>();
    
}

const AxString EPVideoTrack::GetName()
{
    return L"Video Track";
}

} // end of namespace diskstream
