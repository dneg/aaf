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

namespace aafanalyzer {

class EPDerivationChainObject
{
 public:
  EPDerivationChainObject();
  ~EPDerivationChainObject();

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

 private:

  //prohibited
  EPFilmSource( const EPFilmSource& );
  EPFilmSource& operator=( const EPFilmSource& );

};

} // end of namespace diskstream

#endif /*EPOBJECTS_H_*/
