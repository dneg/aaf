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
//@doc
//@class    AAFTextLocator | Implementation class for AAFTextLocator
#ifndef __ImplAAFTextLocator_h__
#define __ImplAAFTextLocator_h__

#ifndef __ImplAAFLocator_h__
#include "ImplAAFLocator.h"
#endif

#include "OMWideStringProperty.h"


class ImplAAFTextLocator : public ImplAAFLocator
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFTextLocator ();

protected:
  virtual ~ImplAAFTextLocator ();

public:

  //****************
  // Initialize()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Initialize ();


  //****************
  // GetName()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetName
        (// @parm [out, size_is(bufSize), string] buffer into which Name is written
         aafCharacter *  pNameBuf,

         // @parm [in] The size of the pNameBuf buffer
         aafUInt32  bufSize);


  //****************
  // GetNameBufLen()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetNameBufLen
        // @parm [out] required buffer length
        (aafUInt32 *  pLen);


  //****************
  // SetName()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetName
        // @parm [in, string] the new Name
        (const aafCharacter *  pNameBuf);


  // Persistent data
private:
	OMWideStringProperty                      _name;
};

#endif // ! __ImplAAFTextLocator_h__


