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

EPNonEssenceTrack::EPNonEssenceTrack()
{}

EPNonEssenceTrack::~EPNonEssenceTrack()
{}

const AxString EPNonEssenceTrack::GetName()
{
    return L"Non-Essence Track";
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
        IAAFSourceClipSP spSrcClip;
        if ( AxIsA( axSegment, spSrcClip ) )
        {
            //3. Ensure the source clip contains  a source mob.
            AxSourceClip axSrcClip( spSrcClip );
            
            //4. Make sure that the referenced source mob is in this AAF file.
            if ( IsReferenceInFile( axSrcClip ) )
            {
                AxMob axMob( axSrcClip.ResolveRef() );
                IAAFSourceMobSP spSourceMob;
                if ( AxIsA( axMob, spSourceMob ) )
                {
                    //4. Ensure the source mob is a file source mob.
                    AxSourceMob axSrcMob( spSourceMob );
                    AxEssenceDescriptor descriptor( axSrcMob.GetEssenceDescriptor() );
                    IAAFFileDescriptorSP spFileDes;
                    if ( AxIsA( descriptor, spFileDes ) )
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
        IAAFSourceClipSP spSrcClip;
        if ( AxIsA( axSegment, spSrcClip ) )
        {
            //3. Ensure the source clip contains  a source mob.
            AxSourceClip axSrcClip( spSrcClip );

            //4. Make sure that the referenced source mob is in this AAF file.
            if ( IsReferenceInFile( axSrcClip ) )
            {
                AxMob axMob( axSrcClip.ResolveRef() );
                IAAFSourceMobSP spSrcMob;
                if ( AxIsA( axMob, spSrcMob ) )
                {
                    //5. Ensure the source mob is a file source mob.
                    AxSourceMob axSrcMob( spSrcMob );
                    AxEssenceDescriptor descriptor( axSrcMob.GetEssenceDescriptor() );
                    IAAFFileDescriptorSP spFileDes;
                    if ( AxIsA( descriptor, spFileDes ) )
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
