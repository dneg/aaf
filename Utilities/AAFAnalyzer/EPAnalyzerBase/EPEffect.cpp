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
#include "EPEffect.h"

//AAF files
#include <AAFOperationDefs.h>

namespace {

using namespace aafanalyzer;

} // end of namespace


//======================================================================
//======================================================================
//======================================================================

namespace aafanalyzer {
    
EPEffect::EPEffect()
{}

EPEffect::~EPEffect()
{}

const AxString EPEffect::GetName()
{
    return L"Unknown Effect";
}

//======================================================================

EPVideoDissolveEffect::EPVideoDissolveEffect()
{}

EPVideoDissolveEffect::~EPVideoDissolveEffect()
{}

const AxString EPVideoDissolveEffect::GetName()
{
    return L"Video Dissolve Effect";
}

//======================================================================

EPSMPTEVideoWipeEffect::EPSMPTEVideoWipeEffect()
{}

EPSMPTEVideoWipeEffect::~EPSMPTEVideoWipeEffect()
{}

const AxString EPSMPTEVideoWipeEffect::GetName()
{
    return L"SMPTE Video Wipe Effect";
}

//======================================================================

EPVideoSpeedControlEffect::EPVideoSpeedControlEffect()
{}

EPVideoSpeedControlEffect::~EPVideoSpeedControlEffect()
{}

const AxString EPVideoSpeedControlEffect::GetName()
{
    return L"Video Speed Control Effect";
}

//======================================================================

EPVideoRepeatEffect::EPVideoRepeatEffect()
{}

EPVideoRepeatEffect::~EPVideoRepeatEffect()
{}

const AxString EPVideoRepeatEffect::GetName()
{
    return L"Video Repeat Effect";
}

//======================================================================

EPVideoFlipEffect::EPVideoFlipEffect()
{}

EPVideoFlipEffect::~EPVideoFlipEffect()
{}

const AxString EPVideoFlipEffect::GetName()
{
    return L"Video Flip Effect";
}

//======================================================================

EPVideoFlopEffect::EPVideoFlopEffect()
{}

EPVideoFlopEffect::~EPVideoFlopEffect()
{}

const AxString EPVideoFlopEffect::GetName()
{
    return L"Video Flop Effect";
}

//======================================================================

EPVideoFlipFlopEffect::EPVideoFlipFlopEffect()
{}

EPVideoFlipFlopEffect::~EPVideoFlipFlopEffect()
{}

const AxString EPVideoFlipFlopEffect::GetName()
{
    return L"Video Flip-Flop Effect";
}

//======================================================================

EPVideoPositionEffect::EPVideoPositionEffect()
{}

EPVideoPositionEffect::~EPVideoPositionEffect()
{}

const AxString EPVideoPositionEffect::GetName()
{
    return L"Video Position Effect";
}

//======================================================================

EPVideoCropEffect::EPVideoCropEffect()
{}

EPVideoCropEffect::~EPVideoCropEffect()
{}

const AxString EPVideoCropEffect::GetName()
{
    return L"Video Crop Effect";
}

//======================================================================

EPVideoScaleEffect::EPVideoScaleEffect()
{}

EPVideoScaleEffect::~EPVideoScaleEffect()
{}

const AxString EPVideoScaleEffect::GetName()
{
    return L"Video Scale Effect";
}

//======================================================================

EPVideoRotateEffect::EPVideoRotateEffect()
{}

EPVideoRotateEffect::~EPVideoRotateEffect()
{}

const AxString EPVideoRotateEffect::GetName()
{
    return L"Video Rotate Effect";
}

//======================================================================

EPVideoCornerPinningEffect::EPVideoCornerPinningEffect()
{}

EPVideoCornerPinningEffect::~EPVideoCornerPinningEffect()
{}

const AxString EPVideoCornerPinningEffect::GetName()
{
    return L"Video Corner Pinning Effect";
}

//======================================================================

EPAlphaWithVideoKeyEffect::EPAlphaWithVideoKeyEffect()
{}

EPAlphaWithVideoKeyEffect::~EPAlphaWithVideoKeyEffect()
{}

const AxString EPAlphaWithVideoKeyEffect::GetName()
{
    return L"Alpha With Video Key Effect";
}

//======================================================================

EPSeparateAlphaKeyEffect::EPSeparateAlphaKeyEffect()
{}

EPSeparateAlphaKeyEffect::~EPSeparateAlphaKeyEffect()
{}

const AxString EPSeparateAlphaKeyEffect::GetName()
{
    return L"Separate Alpha Key Effect";
}

//======================================================================

EPLuminanceKeyEffect::EPLuminanceKeyEffect()
{}

EPLuminanceKeyEffect::~EPLuminanceKeyEffect()
{}

const AxString EPLuminanceKeyEffect::GetName()
{
    return L"Luminance Key Effect";
}

//======================================================================

EPChromaKeyEffect::EPChromaKeyEffect()
{}

EPChromaKeyEffect::~EPChromaKeyEffect()
{}

const AxString EPChromaKeyEffect::GetName()
{
    return L"Chroma Key Effect";
}

//======================================================================

EPMonoAudioGainEffect::EPMonoAudioGainEffect()
{}

EPMonoAudioGainEffect::~EPMonoAudioGainEffect()
{}

const AxString EPMonoAudioGainEffect::GetName()
{
    return L"Mono Audio Gain Effect";
}

//======================================================================

EPMonoAudioPanEffect::EPMonoAudioPanEffect()
{}

EPMonoAudioPanEffect::~EPMonoAudioPanEffect()
{}

const AxString EPMonoAudioPanEffect::GetName()
{
    return L"Mono Audio Pan Effect";
}

//======================================================================

EPSingleParameterAudioDissolveEffect::EPSingleParameterAudioDissolveEffect()
{}

EPSingleParameterAudioDissolveEffect::~EPSingleParameterAudioDissolveEffect()
{}

const AxString EPSingleParameterAudioDissolveEffect::GetName()
{
    return L"Single-Parameter Audio Dissolve Effect";
}

//======================================================================

EPTwoParameterAudioDissolveEffect::EPTwoParameterAudioDissolveEffect()
{}

EPTwoParameterAudioDissolveEffect::~EPTwoParameterAudioDissolveEffect()
{}

const AxString EPTwoParameterAudioDissolveEffect::GetName()
{
    return L"Two-Parameter Audio Dissolve Effect";
}

} // end of namespace diskstream
