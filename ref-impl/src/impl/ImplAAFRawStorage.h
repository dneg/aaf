#ifndef __ImplAAFRawStorage_h__
#define __ImplAAFRawStorage_h__
//=---------------------------------------------------------------------=
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
// The Original Code of this file is Copyright 1998-2001, Licensor of the
// AAF Association.
// 
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

#ifndef __ImplAAFRoot_h__
#include "ImplAAFRoot.h"
#endif

class OMRawStorage;

class ImplAAFRawStorage : public ImplAAFRoot
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFRawStorage ();

protected:
  virtual ~ImplAAFRawStorage ();

public:

  //****************
  // IsReadable()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    IsReadable
        (
		 // set to true if readable
		 aafBoolean_t *  pResult);


  //****************
  // Read()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Read
        (// @parm [out, size_is(bufSize), length_is(*pNumRead)] Buffer into which data is read
         aafMemPtr_t  buf,

         // @parm [in] Size of buf in bytes
         aafUInt32  bufSize,

         // @parm [out] Number of bytes read
         aafUInt32 *  pNumRead);


  //****************
  // IsWriteable()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    IsWriteable
        (
		 // set to true if writeable
		 aafBoolean_t *  pResult);


  //****************
  // Write()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Write
        (// @parm [in, size_is(bufSize)] Buffer from which data is written
         aafMemConstPtr_t  buf,

         // @parm [in] Number of bytes to write
         aafUInt32  bufSize,

		 // Number of bytes written
		 aafUInt32 * pNumWritten);


  //****************
  // Synchronize()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Synchronize ();


  //
  // Methods which are not exported from the toolkit
  //

  //****************
  // Initialize()
  //
  void Initialize (OMRawStorage * prs,
				   aafFileAccess_t access);

  //
  // Returns the underlying OMRawStorage
  //
  OMRawStorage * GetOMStorage();

private:
  OMRawStorage *  _rep;
  aafFileAccess_t _access;

  bool pvtIsReadable () const;
  bool pvtIsWriteable () const;
};

#endif // ! __ImplAAFRawStorage_h__
