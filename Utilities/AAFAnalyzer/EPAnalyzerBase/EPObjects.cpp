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
#include "EPObjects.h"

namespace {

using namespace aafanalyzer;

} // end of namespace


//======================================================================
//======================================================================
//======================================================================

namespace aafanalyzer {

EPObject::EPObject()
{}

EPObject::~EPObject()
{}

const AxString EPObject::GetName()
{
    return L"Edit Protocol Object";
}

//======================================================================

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


} // end of namespace diskstream
