//=---------------------------------------------------------------------=
//
// $Id$ $Name$
//
// The contents of this file are subject to the AAF SDK Public Source
// License Agreement Version 2.0 (the "License"); You may not use this
// file except in compliance with the License.  The License is available
// in AAFSDKPSL.TXT, or you may obtain a copy of the License from the
// Advanced Media Workflow Association, Inc., or its successor.
//
// Software distributed under the License is distributed on an "AS IS"
// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
// the License for the specific language governing rights and limitations
// under the License.  Refer to Section 3.3 of the License for proper use
// of this Exhibit.
//
// WARNING:  Please contact the Advanced Media Workflow Association,
// Inc., for more information about any additional licenses to
// intellectual property covering the AAF Standard that may be required
// to create and distribute AAF compliant products.
// (http://www.amwa.tv/policies).
//
// Copyright Notices:
// The Original Code of this file is Copyright 1998-2009, licensor of the
// Advanced Media Workflow Association.  All rights reserved.
//
//=---------------------------------------------------------------------=

#ifndef __EPEffect__
#define __EPEffect__

//Edit Protocol Analyzer Base files
#include "EPObjects.h"

//Ax files

//Boost files

namespace aafanalyzer {

class EPEffect : public EPObject
{
    public:
        EPEffect();
        ~EPEffect();
        static const AxString GetName();

    private:
        //prohibited
        EPEffect( const EPEffect& );
        EPEffect& operator=( const EPEffect& );
};

//======================================================================

class EPVideoDissolveEffect : public EPEffect
{
    public:
        EPVideoDissolveEffect();
        ~EPVideoDissolveEffect();
        static const AxString GetName();

    private:

        //prohibited
        EPVideoDissolveEffect( const EPVideoDissolveEffect& );
        EPVideoDissolveEffect& operator=( const EPVideoDissolveEffect& );
};

//======================================================================

class EPSMPTEVideoWipeEffect : public EPEffect
{
    public:
        EPSMPTEVideoWipeEffect();
        ~EPSMPTEVideoWipeEffect();
        static const AxString GetName();

    private:

        //prohibited
        EPSMPTEVideoWipeEffect( const EPSMPTEVideoWipeEffect& );
        EPSMPTEVideoWipeEffect& operator=( const EPSMPTEVideoWipeEffect& );
};

//======================================================================

class EPVideoSpeedControlEffect : public EPEffect
{
    public:
        EPVideoSpeedControlEffect();
        ~EPVideoSpeedControlEffect();
        static const AxString GetName();

    private:

        //prohibited
        EPVideoSpeedControlEffect( const EPVideoSpeedControlEffect& );
        EPVideoSpeedControlEffect& operator=( const EPVideoSpeedControlEffect& );
};

//======================================================================

class EPVideoRepeatEffect : public EPEffect
{
    public:
        EPVideoRepeatEffect();
        ~EPVideoRepeatEffect();
        static const AxString GetName();

    private:

        //prohibited
        EPVideoRepeatEffect( const EPVideoRepeatEffect& );
        EPVideoRepeatEffect& operator=( const EPVideoRepeatEffect& );
};

//======================================================================

class EPVideoFlipEffect : public EPEffect
{
    public:
        EPVideoFlipEffect();
        ~EPVideoFlipEffect();
        static const AxString GetName();

    private:

        //prohibited
        EPVideoFlipEffect( const EPVideoFlipEffect& );
        EPVideoFlipEffect& operator=( const EPVideoFlipEffect& );
};

//======================================================================

class EPVideoFlopEffect : public EPEffect
{
    public:
        EPVideoFlopEffect();
        ~EPVideoFlopEffect();
        static const AxString GetName();

    private:

        //prohibited
        EPVideoFlopEffect( const EPVideoFlopEffect& );
        EPVideoFlopEffect& operator=( const EPVideoFlopEffect& );
};

//======================================================================

class EPVideoFlipFlopEffect : public EPEffect
{
    public:
        EPVideoFlipFlopEffect();
        ~EPVideoFlipFlopEffect();
        static const AxString GetName();

    private:

        //prohibited
        EPVideoFlipFlopEffect( const EPVideoFlipFlopEffect& );
        EPVideoFlipFlopEffect& operator=( const EPVideoFlipFlopEffect& );
};

//======================================================================

class EPVideoPositionEffect : public EPEffect
{
    public:
        EPVideoPositionEffect();
        ~EPVideoPositionEffect();
        static const AxString GetName();

    private:

        //prohibited
        EPVideoPositionEffect( const EPVideoPositionEffect& );
        EPVideoPositionEffect& operator=( const EPVideoPositionEffect& );
};

//======================================================================

class EPVideoCropEffect : public EPEffect
{
    public:
        EPVideoCropEffect();
        ~EPVideoCropEffect();
        static const AxString GetName();

    private:

        //prohibited
        EPVideoCropEffect( const EPVideoCropEffect& );
        EPVideoCropEffect& operator=( const EPVideoCropEffect& );
};

//======================================================================

class EPVideoScaleEffect : public EPEffect
{
    public:
        EPVideoScaleEffect();
        ~EPVideoScaleEffect();
        static const AxString GetName();

    private:

        //prohibited
        EPVideoScaleEffect( const EPVideoScaleEffect& );
        EPVideoScaleEffect& operator=( const EPVideoScaleEffect& );
};

//======================================================================

class EPVideoRotateEffect : public EPEffect
{
    public:
        EPVideoRotateEffect();
        ~EPVideoRotateEffect();
        static const AxString GetName();

    private:

        //prohibited
        EPVideoRotateEffect( const EPVideoRotateEffect& );
        EPVideoRotateEffect& operator=( const EPVideoRotateEffect& );
};

//======================================================================

class EPVideoCornerPinningEffect : public EPEffect
{
    public:
        EPVideoCornerPinningEffect();
        ~EPVideoCornerPinningEffect();
        static const AxString GetName();

    private:

        //prohibited
        EPVideoCornerPinningEffect( const EPVideoCornerPinningEffect& );
        EPVideoCornerPinningEffect& operator=( const EPVideoCornerPinningEffect& );
};

//======================================================================

class EPAlphaWithVideoKeyEffect : public EPEffect
{
    public:
        EPAlphaWithVideoKeyEffect();
        ~EPAlphaWithVideoKeyEffect();
        static const AxString GetName();

    private:

        //prohibited
        EPAlphaWithVideoKeyEffect( const EPAlphaWithVideoKeyEffect& );
        EPAlphaWithVideoKeyEffect& operator=( const EPAlphaWithVideoKeyEffect& );
};

//======================================================================

class EPSeparateAlphaKeyEffect : public EPEffect
{
    public:
        EPSeparateAlphaKeyEffect();
        ~EPSeparateAlphaKeyEffect();
        static const AxString GetName();

    private:

        //prohibited
        EPSeparateAlphaKeyEffect( const EPSeparateAlphaKeyEffect& );
        EPSeparateAlphaKeyEffect& operator=( const EPSeparateAlphaKeyEffect& );
};

//======================================================================

class EPLuminanceKeyEffect : public EPEffect
{
    public:
        EPLuminanceKeyEffect();
        ~EPLuminanceKeyEffect();
        static const AxString GetName();

    private:

        //prohibited
        EPLuminanceKeyEffect( const EPLuminanceKeyEffect& );
        EPLuminanceKeyEffect& operator=( const EPLuminanceKeyEffect& );
};

//======================================================================

class EPChromaKeyEffect : public EPEffect
{
    public:
        EPChromaKeyEffect();
        ~EPChromaKeyEffect();
        static const AxString GetName();

    private:

        //prohibited
        EPChromaKeyEffect( const EPChromaKeyEffect& );
        EPChromaKeyEffect& operator=( const EPChromaKeyEffect& );
};

//======================================================================

class EPMonoAudioGainEffect : public EPEffect
{
    public:
        EPMonoAudioGainEffect();
        ~EPMonoAudioGainEffect();
        static const AxString GetName();

    private:

        //prohibited
        EPMonoAudioGainEffect( const EPMonoAudioGainEffect& );
        EPMonoAudioGainEffect& operator=( const EPMonoAudioGainEffect& );
};

//======================================================================

class EPMonoAudioPanEffect : public EPEffect
{
    public:
        EPMonoAudioPanEffect();
        ~EPMonoAudioPanEffect();
        static const AxString GetName();

    private:

        //prohibited
        EPMonoAudioPanEffect( const EPMonoAudioPanEffect& );
        EPMonoAudioPanEffect& operator=( const EPMonoAudioPanEffect& );
};

//======================================================================

class EPSingleParameterAudioDissolveEffect : public EPEffect
{
    public:
        EPSingleParameterAudioDissolveEffect();
        ~EPSingleParameterAudioDissolveEffect();
        static const AxString GetName();

    private:

        //prohibited
        EPSingleParameterAudioDissolveEffect( const EPSingleParameterAudioDissolveEffect& );
        EPSingleParameterAudioDissolveEffect& operator=( const EPSingleParameterAudioDissolveEffect& );
};

//======================================================================

class EPTwoParameterAudioDissolveEffect : public EPEffect
{
    public:
        EPTwoParameterAudioDissolveEffect();
        ~EPTwoParameterAudioDissolveEffect();
        static const AxString GetName();

    private:

        //prohibited
        EPTwoParameterAudioDissolveEffect( const EPTwoParameterAudioDissolveEffect& );
        EPTwoParameterAudioDissolveEffect& operator=( const EPTwoParameterAudioDissolveEffect& );
};

} // end of namespace diskstream

#endif/*__EPEffect__*/

