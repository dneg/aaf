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

#ifndef __EPOBJECTS_h_
#define __EPOBJECTS_h_

//Ax files
#include <AxTypes.h>

namespace aafanalyzer {

class EPObject
{
 public:
  ~EPObject();
  static const AxString GetName();

 protected:
  EPObject();

 private:
  //prohibited
  EPObject( const EPObject& );
  EPObject& operator=( const EPObject& );
};

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

} // end of namespace diskstream

#endif /*EPOBJECTS_H_*/
