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

#ifndef __EPTrack__
#define __EPTrack__

//Ax files
#include <AxMobSlot.h>
#include <AxComponent.h>
#include <AxMob.h>

//Boost files
#include<boost/shared_ptr.hpp>

class AxClassDef;

namespace aafanalyzer {

using namespace boost;

/*
 * This interface abstracts the idea of a track within the context of the Edit
 * Protocol.  Currently a track is considered to be:
 * 
 *      a) A MobSlot
 *      b) The SourceClip contained within the MobSlot
 *      c) The Mob referenced by the SourceClip
 * 
 * (Note: This does not match with the definition of an Essence Track, so it
 *        has not been included as a derived class of EPTrack).
 * 
 */

class EPTrack
{
    public:
       
        AxMobSlot& GetMobSlot();
        AxSourceClip& GetSourceClip();
        AxMob& GetMob();
        
    protected:
    
        EPTrack( IAAFMobSlotSP mobSlot, IAAFSourceClipSP clip, IAAFMobSP mob );

    private:

        AxMobSlot       _mobSlot;
        AxSourceClip    _sourceClip;
        AxMob           _mob;

        //prohibited
        EPTrack();
        EPTrack( const EPTrack& );
        EPTrack& operator=( const EPTrack& );
};

//======================================================================

/*
 * This class strengthens the requirements of a track as follows:
 * 
 *      a) The mob slot must have an Sound kind data definition.
 *      c) The Mob referenced by the SourceClip must be a file source mob.
 * 
 */

class EPAudioTrack : public EPTrack
{
    public:

        static shared_ptr<EPAudioTrack> CreateAudioTrack( AxMobSlot& axMobSlot );

    private:

        EPAudioTrack( IAAFMobSlotSP mobSlot, IAAFSourceClipSP clip, IAAFMobSP mob );

        //prohibited
        EPAudioTrack();
        EPAudioTrack( const EPAudioTrack& );
        EPAudioTrack& operator=( const EPAudioTrack& );
};

//======================================================================

/*
 * This class strengthens the requirements of a track as follows:
 * 
 *      a) The mob slot must have an Picture kind data definition.
 *      c) The Mob referenced by the SourceClip must be a file source mob.
 * 
 */

class EPVideoTrack : public EPTrack
{
    public:

        static shared_ptr<EPVideoTrack> CreateVideoTrack( AxMobSlot& axMobSlot );

    private:

        EPVideoTrack( IAAFMobSlotSP mobSlot, IAAFSourceClipSP clip, IAAFMobSP mob );

        //prohibited
        EPVideoTrack();
        EPVideoTrack( const EPVideoTrack& );
        EPVideoTrack& operator=( const EPVideoTrack& );
};

} // end of namespace diskstream

#endif/*__EPTrack__*/
