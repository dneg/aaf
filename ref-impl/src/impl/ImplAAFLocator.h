//@doc
//@class    AAFLocator | Implementation class for AAFLocator
#ifndef __ImplAAFLocator_h__
#define __ImplAAFLocator_h__


//=---------------------------------------------------------------------=
//
// $Id$ $Name$
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
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

#ifndef __ImplAAFObject_h__
#include "ImplAAFObject.h"
#endif


class ImplAAFLocator : public ImplAAFObject
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFLocator ();

protected:
  virtual ~ImplAAFLocator ();

public:

  //****************
  // GetPath()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetPath
        (// @parm [out, size_is(bufSize), string] buffer into which path is written
         aafCharacter *  pPathBuf,

         // @parm [in] The size of the pPathBuf buffer
         aafUInt32  bufSize);


  //****************
  // GetPathBufLen()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetPathBufLen
        // @parm [out] required buffer length
        (aafUInt32 *  pLen);


  //****************
  // SetPath()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetPath
        // @parm [in, string] the new path
        (const aafCharacter *  pPathBuf);
};

#endif // ! __ImplAAFLocator_h__


