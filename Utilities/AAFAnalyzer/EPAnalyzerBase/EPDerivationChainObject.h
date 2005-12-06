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

#ifndef __EPDerivationChainObject_h_
#define __EPDerivationChainObject_h_

//Edit Protocol Analyzer Base files
#include "EPObjects.h"

//Ax files
#include <AxTypes.h>

namespace aafanalyzer {

class EPDerivationChainObject : public EPObject
{
 public:
  ~EPDerivationChainObject();
  static const AxString GetName();

 protected:
  EPDerivationChainObject();

 private:
  //prohibited
  EPDerivationChainObject( const EPDerivationChainObject& );
  EPDerivationChainObject& operator=( const EPDerivationChainObject& );
};

//======================================================================

class EPTopLevelComposition : public EPDerivationChainObject
{
 public:
  EPTopLevelComposition();
  ~EPTopLevelComposition();
  static const AxString GetName();

 private:

  //prohibited
  EPTopLevelComposition( const EPTopLevelComposition& );
  EPTopLevelComposition& operator=( const EPTopLevelComposition& );
};

//======================================================================

class EPLowerLevelComposition : public EPDerivationChainObject
{
 public:
  EPLowerLevelComposition();
  ~EPLowerLevelComposition();
  static const AxString GetName();

 private:

  //prohibited
  EPLowerLevelComposition( const EPLowerLevelComposition& );
  EPLowerLevelComposition& operator=( const EPLowerLevelComposition& );
};

//======================================================================

class EPSubClipComposition : public EPDerivationChainObject
{
 public:
  EPSubClipComposition();
  ~EPSubClipComposition();
  static const AxString GetName();

 private:

  //prohibited
  EPSubClipComposition( const EPSubClipComposition& );
  EPSubClipComposition& operator=( const EPSubClipComposition& );
};

//======================================================================

class EPAdjustedClipComposition : public EPDerivationChainObject
{
 public:
  EPAdjustedClipComposition();
  ~EPAdjustedClipComposition();
  static const AxString GetName();

 private:

  //prohibited
  EPAdjustedClipComposition( const EPAdjustedClipComposition& );
  EPAdjustedClipComposition& operator=( const EPAdjustedClipComposition& );
};

//======================================================================

class EPTemplateClip : public EPDerivationChainObject
{
 public:
  EPTemplateClip();
  ~EPTemplateClip();
  static const AxString GetName();

 private:

  //prohibited
  EPTemplateClip( const EPTemplateClip& );
  EPTemplateClip& operator=( const EPTemplateClip& );
};

//======================================================================

class EPClip : public EPDerivationChainObject
{
 public:
  EPClip();
  ~EPClip();
  static const AxString GetName();

 private:

  //prohibited
  EPClip( const EPClip& );
  EPClip& operator=( const EPClip& );
};

//======================================================================

class EPFileSource : public EPDerivationChainObject
{
 public:
  EPFileSource();
  ~EPFileSource();
  static const AxString GetName();

 private:

  //prohibited
  EPFileSource( const EPFileSource& );
  EPFileSource& operator=( const EPFileSource& );
};

//======================================================================

class EPAudioFileSource : public EPFileSource
{
 public:
  ~EPAudioFileSource();
  static const AxString GetName();

 protected:
  EPAudioFileSource();

 private:

  //prohibited
  EPAudioFileSource( const EPAudioFileSource& );
  EPAudioFileSource& operator=( const EPAudioFileSource& );
};

//======================================================================

class EPMonoAudioFileSource : public EPAudioFileSource
{
 public:
  EPMonoAudioFileSource();
  ~EPMonoAudioFileSource();
  static const AxString GetName();

 private:

  //prohibited
  EPMonoAudioFileSource( const EPMonoAudioFileSource& );
  EPMonoAudioFileSource& operator=( const EPMonoAudioFileSource& );
};

//======================================================================

class EPMultiChannelAudioFileSource : public EPAudioFileSource
{
 public:
  EPMultiChannelAudioFileSource();
  ~EPMultiChannelAudioFileSource();
  static const AxString GetName();

 private:

  //prohibited
  EPMultiChannelAudioFileSource( const EPMultiChannelAudioFileSource& );
  EPMultiChannelAudioFileSource& operator=( const EPMultiChannelAudioFileSource& );
};

//======================================================================


class EPImageFileSource : public EPFileSource
{
 public:
  EPImageFileSource();
  ~EPImageFileSource();
  static const AxString GetName();

 private:

  //prohibited
  EPImageFileSource( const EPImageFileSource& );
  EPImageFileSource& operator=( const EPImageFileSource& );
};

//======================================================================

class EPRGBAImageFileSource : public EPImageFileSource
{
 public:
  EPRGBAImageFileSource();
  ~EPRGBAImageFileSource();
  static const AxString GetName();

 private:

  //prohibited
  EPRGBAImageFileSource( const EPRGBAImageFileSource& );
  EPRGBAImageFileSource& operator=( const EPRGBAImageFileSource& );
};

//======================================================================

class EPCDCIImageFileSource : public EPImageFileSource
{
 public:
  EPCDCIImageFileSource();
  ~EPCDCIImageFileSource();
  static const AxString GetName();

 private:

  //prohibited
  EPCDCIImageFileSource( const EPCDCIImageFileSource& );
  EPCDCIImageFileSource& operator=( const EPCDCIImageFileSource& );
};

//======================================================================

class EPRecordingSource : public EPDerivationChainObject
{
 public:
  EPRecordingSource();
  ~EPRecordingSource();
  static const AxString GetName();

 private:

  //prohibited
  EPRecordingSource( const EPRecordingSource& );
  EPRecordingSource& operator=( const EPRecordingSource& );
};

//======================================================================

class EPImportSource : public EPDerivationChainObject
{
 public:
  EPImportSource();
  ~EPImportSource();
  static const AxString GetName();

 private:

  //prohibited
  EPImportSource( const EPImportSource& );
  EPImportSource& operator=( const EPImportSource& );
};

//======================================================================

class EPTapeSource : public EPDerivationChainObject
{
 public:
  EPTapeSource();
  ~EPTapeSource();
  static const AxString GetName();

 private:

  //prohibited
  EPTapeSource( const EPImportSource& );
  EPTapeSource& operator=( const EPImportSource& );
};

//======================================================================

class EPFilmSource : public EPDerivationChainObject
{
 public:
  EPFilmSource();
  ~EPFilmSource();
  static const AxString GetName();

 private:

  //prohibited
  EPFilmSource( const EPFilmSource& );
  EPFilmSource& operator=( const EPFilmSource& );
};

//======================================================================

class EPAuxiliarySource : public EPDerivationChainObject
{
 public:
  EPAuxiliarySource();
  ~EPAuxiliarySource();
  static const AxString GetName();

 private:

  //prohibited
  EPAuxiliarySource( const EPAuxiliarySource& );
  EPAuxiliarySource& operator=( const EPAuxiliarySource& );
};

} // end of namespace diskstream

#endif /*__EPDerivationChainObject_h_*/
