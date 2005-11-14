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

//Edit Protocol Analyzer Base files
#include "EPObjects.h"

//Ax files
#include <AxMobSlot.h>
#include <AxComponent.h>
#include <AxMob.h>
#include <AxTypes.h>

//Boost files
#include<boost/shared_ptr.hpp>

class AxClassDef;

namespace aafanalyzer {

using namespace boost;

class EPTrackObject : public EPObject
{
    public:
        ~EPTrackObject();
        static const AxString GetName();

    protected:
        EPTrackObject();

    private:
        //prohibited
        EPTrackObject( const EPTrackObject& );
        EPTrackObject& operator=( const EPTrackObject& );
};

//======================================================================

/*
 * This class represents a timecode track, which is currently defined as a
 * MobSlot where IsTimecodeType() true.
 */

class EPTimecodeTrack : public EPTrackObject
{
    public:
        EPTimecodeTrack();
        ~EPTimecodeTrack();
        static const AxString GetName();

    private:

        //prohibited
        EPTimecodeTrack( const EPTimecodeTrack& );
        EPTimecodeTrack& operator=( const EPTimecodeTrack& );
};

//======================================================================

/*
 * This class represents a edgecode track, which is currently defined as a
 * MobSlot where IsEdgecodeType() true.
 */

class EPEdgecodeTrack : public EPTrackObject
{
    public:
        EPEdgecodeTrack();
        ~EPEdgecodeTrack();
        static const AxString GetName();

    private:

        //prohibited
        EPEdgecodeTrack( const EPEdgecodeTrack& );
        EPEdgecodeTrack& operator=( const EPEdgecodeTrack& );
};

//======================================================================

/*
 * This class represents an essence track, which is currently defined as a
 * MobSlot where IsPictureType() or IsAudioType is true.
 */

class EPEssenceTrack : public EPTrackObject
{
    public:
        EPEssenceTrack();
        ~EPEssenceTrack();
        static const AxString GetName();

    private:

        //prohibited
        EPEssenceTrack( const EPEssenceTrack& );
        EPEssenceTrack& operator=( const EPEssenceTrack& );
};

//======================================================================

/*
 * This interface abstracts the idea of a track within the context of the Edit
 * Protocol.  Currently a track is considered to be:
 * 
 *      a) A MobSlot
 *      b) The SourceClip contained within the MobSlot
 *      c) The Mob referenced by the SourceClip
 * 
 */
 
 //TODO: Pending clarificaiton of requirements, the definition in part (b) may
 //      have to be changed (to allow for sequences).

class EPCompleteTrack : public EPTrackObject
{
    public:
       
        AxMobSlot& GetMobSlot();
        AxSourceClip& GetSourceClip();
        AxMob& GetMob();
        static const AxString GetName();
        
    protected:
    
        EPCompleteTrack( IAAFMobSlotSP mobSlot, IAAFSourceClipSP clip, IAAFMobSP mob );

    private:

        AxMobSlot       _mobSlot;
        AxSourceClip    _sourceClip;
        AxMob           _mob;

        //prohibited
        EPCompleteTrack();
        EPCompleteTrack( const EPCompleteTrack& );
        EPCompleteTrack& operator=( const EPCompleteTrack& );
};

//======================================================================

/*
 * This class strengthens the requirements of a track as follows:
 * 
 *      a) The mob slot must have an Sound kind data definition.
 *      c) The Mob referenced by the SourceClip must be a file source mob.
 * 
 */

class EPAudioTrack : public EPCompleteTrack, public EPEssenceTrack
{
    public:

        static shared_ptr<EPAudioTrack> CreateAudioTrack( AxMobSlot& axMobSlot );
        static const AxString GetName();

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

class EPVideoTrack : public EPCompleteTrack, public EPEssenceTrack
{
    public:

        static shared_ptr<EPVideoTrack> CreateVideoTrack( AxMobSlot& axMobSlot );
        static const AxString GetName();

    private:

        EPVideoTrack( IAAFMobSlotSP mobSlot, IAAFSourceClipSP clip, IAAFMobSP mob );

        //prohibited
        EPVideoTrack();
        EPVideoTrack( const EPVideoTrack& );
        EPVideoTrack& operator=( const EPVideoTrack& );
};

} // end of namespace diskstream

#endif/*__EPTrack__*/
