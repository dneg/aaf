#ifndef __ImplAAFWAVEDescriptor_h__
#define __ImplAAFWAVEDescriptor_h__

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


#include "ImplAAFFileDescriptor.h"

#include "OMVariableSizeProperty.h"

class ImplAAFWAVEDescriptor : public ImplAAFFileDescriptor
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFWAVEDescriptor ();

protected:
  virtual ~ImplAAFWAVEDescriptor ();

public:
  virtual AAFRESULT STDMETHODCALLTYPE
	Initialize ();


  //****************
  // GetSummary()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetSummary
	    (// [in] Size of preallocated buffer
         aafUInt32  size,

		 // [out, size_is(size)] Preallocated buffer to hold the WAVE file information
		 aafDataValue_t  pSummary);

  //****************
  // GetSummaryBufferSize()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetSummaryBufferSize
		// [out] required buffer size
        (aafUInt32 *  pSize);

  //****************
  // SetSummary()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetSummary
	    (// [in] Size of preallocated buffer
         aafUInt32  size,

		 // [in, size_is(size)] buffer containing value
		 aafDataValue_t  pSummary);


private:
	OMVariableSizeProperty<aafUInt8> _summary;

};

#endif // ! __ImplAAFWAVEDescriptor_h__

