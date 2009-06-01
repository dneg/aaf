//@doc
//@class    AAFNetworkLocator | Implementation class for AAFNetworkLocator
#ifndef __ImplAAFNetworkLocator_h__
#define __ImplAAFNetworkLocator_h__

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
// The Initial Developer of the Original Code of this file and the
// licensor of the Advanced Media Workflow Association is
// Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=





#ifndef __ImplAAFLocator_h__
#include "ImplAAFLocator.h"
#endif

#include "OMWideStringProperty.h"



class ImplAAFNetworkLocator : public ImplAAFLocator
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFNetworkLocator ();
  virtual ~ImplAAFNetworkLocator ();


  virtual AAFRESULT STDMETHODCALLTYPE
	Initialize ();


  // Override from AAFLocator
  virtual AAFRESULT STDMETHODCALLTYPE
    GetPath (/*[out, size_is(bufSize), string]*/ aafCharacter *  pPathBuf,
	  /*[in]*/ aafUInt32  bufSize);



  // Override from AAFLocator
  virtual AAFRESULT STDMETHODCALLTYPE
    GetPathBufLen (/*[out]*/ aafUInt32 *  pLen);



  // Override from AAFLocator
  virtual AAFRESULT STDMETHODCALLTYPE
    SetPath (/*[in, string]*/ const aafCharacter *  pPathBuf);

private:
  OMWideStringProperty                             _path;
};

#endif // ! __ImplAAFNetworkLocator_h__

