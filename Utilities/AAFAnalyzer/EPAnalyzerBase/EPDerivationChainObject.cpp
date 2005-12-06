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
#include "EPDerivationChainObject.h"

namespace {

using namespace aafanalyzer;

} // end of namespace


//======================================================================
//======================================================================
//======================================================================

namespace aafanalyzer {

EPDerivationChainObject::EPDerivationChainObject()
{}

EPDerivationChainObject::~EPDerivationChainObject()
{}

const AxString EPDerivationChainObject::GetName()
{
    return L"Edit Protocol Derivation Chain Object";
}

//======================================================================

EPTopLevelComposition::EPTopLevelComposition()
{}

EPTopLevelComposition::~EPTopLevelComposition()
{}

const AxString EPTopLevelComposition::GetName()
{
    return L"Top-Level Composition";
}

//======================================================================

EPLowerLevelComposition::EPLowerLevelComposition()
{}

EPLowerLevelComposition::~EPLowerLevelComposition()
{}

const AxString EPLowerLevelComposition::GetName()
{
    return L"Lower-Level Composition";
}
  
//======================================================================

EPSubClipComposition::EPSubClipComposition()
{}

EPSubClipComposition::~EPSubClipComposition()
{}

const AxString EPSubClipComposition::GetName()
{
    return L"Sub-Clip Composition";
}
  
//======================================================================

EPAdjustedClipComposition::EPAdjustedClipComposition()
{}

EPAdjustedClipComposition::~EPAdjustedClipComposition()
{}

const AxString EPAdjustedClipComposition::GetName()
{
    return L"Adjusted-Clip Composition";
}
  
//======================================================================

EPTemplateClip::EPTemplateClip()
{}

EPTemplateClip::~EPTemplateClip()
{}

const AxString EPTemplateClip::GetName()
{
    return L"Template Clip";
}
  
//======================================================================

EPClip::EPClip()
{}

EPClip::~EPClip()
{}

const AxString EPClip::GetName()
{
    return L"Clip";
}
  
//======================================================================

EPFileSource::EPFileSource()
{}

EPFileSource::~EPFileSource()
{}

const AxString EPFileSource::GetName()
{
    return L"File Source";
}

//======================================================================

EPAudioFileSource::EPAudioFileSource()
{}

EPAudioFileSource::~EPAudioFileSource()
{}

const AxString EPAudioFileSource::GetName()
{
    return L"Audio File Source";
}

//======================================================================

EPMonoAudioFileSource::EPMonoAudioFileSource()
{}

EPMonoAudioFileSource::~EPMonoAudioFileSource()
{}

const AxString EPMonoAudioFileSource::GetName()
{
    return L"Mono Audio File Source";
}

//======================================================================

EPMultiChannelAudioFileSource::EPMultiChannelAudioFileSource()
{}

EPMultiChannelAudioFileSource::~EPMultiChannelAudioFileSource()
{}

const AxString EPMultiChannelAudioFileSource::GetName()
{
    return L"Multi-Channel Audio File Source";
}

//======================================================================

EPImageFileSource::EPImageFileSource()
{}

EPImageFileSource::~EPImageFileSource()
{}

const AxString EPImageFileSource::GetName()
{
    return L"Digital Image File Source";
}

//======================================================================

EPRGBAImageFileSource::EPRGBAImageFileSource()
{}

EPRGBAImageFileSource::~EPRGBAImageFileSource()
{}

const AxString EPRGBAImageFileSource::GetName()
{
    return L"RGBA Digital Image File Source";
}

//======================================================================

EPCDCIImageFileSource::EPCDCIImageFileSource()
{}

EPCDCIImageFileSource::~EPCDCIImageFileSource()
{}

const AxString EPCDCIImageFileSource::GetName()
{
    return L"CDCI Digital Image File Source";
}
  
//======================================================================

EPRecordingSource::EPRecordingSource()
{}

EPRecordingSource::~EPRecordingSource()
{}

const AxString EPRecordingSource::GetName()
{
    return L"Recording Source";
}
  
//======================================================================

EPImportSource::EPImportSource()
{}

EPImportSource::~EPImportSource()
{}

const AxString EPImportSource::GetName()
{
    return L"Import Source";
}
  
//======================================================================

EPTapeSource::EPTapeSource()
{}

EPTapeSource::~EPTapeSource()
{}

const AxString EPTapeSource::GetName()
{
    return L"Tape Source";
}
  
//======================================================================

EPFilmSource::EPFilmSource()
{}

EPFilmSource::~EPFilmSource()
{}

const AxString EPFilmSource::GetName()
{
    return L"Film Source";
}

//======================================================================

EPAuxiliarySource::EPAuxiliarySource()
{}

EPAuxiliarySource::~EPAuxiliarySource()
{}

const AxString EPAuxiliarySource::GetName()
{
    return L"Auxiliary Source";
}


} // end of namespace diskstream
