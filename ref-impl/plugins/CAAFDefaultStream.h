//@doc
//@class    AAFEssenceStream | Implementation class for AAFEssenceStream
#ifndef __CAAFDefaultStream_h__
#define __CAAFDefaultStream_h__

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

#ifndef __AAFPlugin_h__
#include "AAFPlugin.h"
#endif

#ifndef __CAAFUnknown_h__
#include "CAAFUnknown.h"
#endif


EXTERN_C const CLSID CLSID_AAFDefaultStream;

class CAAFDefaultStream
  : public IAAFEssenceStream,
    public CAAFUnknown
{
protected:

  //********
  //
  // Constructor/destructor
  //
  CAAFDefaultStream (IUnknown * pControllingUnknown, aafBool doInit = kAAFTrue);
  virtual ~CAAFDefaultStream ();

public:


  // Write some number of bytes to the stream exactly and with no formatting or compression.
  STDMETHOD (Write)
    (/*[in,size_is(buflen)]*/ aafDataBuffer_t  buffer, // to a buffer
     /*[in]*/ aafInt32  buflen); // of this size 

  // Read some number of bytes from the stream exactly and with no formatting or compression.
  STDMETHOD (Read)
    (/*[in]*/ aafUInt32  buflen, // to a buffer of this size
     /*[out, size_is(buflen), length_is(*bytesRead)]*/ aafDataBuffer_t  buffer, // here is the buffer
     /*[out,ref]*/ aafUInt32 *  bytesRead); // Return bytes actually read 

  // Seek to the absolute byte offset into the stream.
  STDMETHOD (Seek)
    (/*[in]*/ aafInt64  byteOffset); // The absolute byte offset into the stream. 

  // Seek forward or backward the given byte count.
  STDMETHOD (SeekRelative)
    (/*[in]*/ aafInt32  byteOffset); // The relative byte offset into the stream. 

  // Returns kAAFTrue if the byte offset is within the stream.
  STDMETHOD (IsPosValid)
    (/*[in]*/ aafInt64  byteOffset, // The absolute byte offset into the stream.
     /*[out]*/ aafBool *  isValid); // The result. 

  // Returns the position within the stream.
  STDMETHOD (GetPosition)
    (/*[out]*/ aafInt64 *  position); // The position within the stream. 

  // Returns the length of the stream.
  STDMETHOD (GetLength)
    (/*[out]*/ aafInt64 *  position); // The length of the stream. 

  // Ensure that all bits are written.
  STDMETHOD (omcFlushCache)
     ();


  // Sets the size of the cache buffer used for further operations.
			// Destroys the current contents of the cache.
  STDMETHOD (SetCacheSize)
    (/*[in]*/ aafInt32  itsSize); // The size of the cache buffer. 


protected:
  // 
  // Declare the QI that implements for the interfaces
  // for this module. This will be called by CAAFUnknown::QueryInterface().
  // 
  virtual HRESULT InternalQueryInterface(REFIID riid, void **ppvObjOut);


public:
  //
  // This class as concrete. All AAF objects can be constructed from
  // a CLSID. This will allow subclassing all "base-classes" by
  // aggreggation.
  // 
  AAF_DECLARE_CONCRETE();
  //
  //********

  // Declare the module test method. The implementation of the will be be
  // in /test/CAAFEssenceStreamTest.cpp.
  static HRESULT test();
};

#endif // ! __CAAFEssenceStream_h__


