//@doc
//@class    AAFEssenceData | Implementation class for AAFEssenceData
#ifndef __CAAFEssenceData_h__
#define __CAAFEssenceData_h__

//=---------------------------------------------------------------------=
//
// This file was GENERATED for the AAF SDK
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



#ifndef __AAF_h__
#include "AAF.h"
#endif







#ifndef __CAAFObject_h__
#include "CAAFObject.h"
#endif


class CAAFEssenceData
  : public IAAFEssenceData,
    public CAAFObject
{
protected:

  //********
  //
  // Constructor/destructor
  //
  CAAFEssenceData (IUnknown * pControllingUnknown, aafBool doInit = kAAFTrue);
  virtual ~CAAFEssenceData ();

public:



  //***********************************************************
  //
  // Initialize()
  //
  // Associates a weak reference to the given file mob with the
  // essence data.
  //
  // Succeeds if all of the following are true:
  // - the pFileMob pointer is valid and points to 
  // a file mob (contains a file descriptor).
  // 
  // If this method fails no state will be changed.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pFileMob is null.
  //
  STDMETHOD (Initialize) (
    // reference to a file mob 
    /*[in]*/ IAAFSourceMob * pFileMob);

  //***********************************************************
  //
  // Write()
  //
  // Write pre-interleaved data to a essence stream.
  //
  STDMETHOD (Write) (
    // write this many bytes
    /*[in]*/ aafUInt32  bytes,

    // here is the buffer
    /*[out, size_is(bytes)]*/ aafDataBuffer_t  buffer,

    // 
    /*[out,ref]*/ aafUInt32 *  bytesWritten);


  //***********************************************************
  //
  // Read()
  //
  // Read pre-interleaved data from a essence stream.
  //
  STDMETHOD (Read) (
    // read this many bytes
    /*[in]*/ aafUInt32  bytes,

    // here is the buffer
    /*[out, size_is(bytes), length_is(*bytesRead)]*/ aafDataBuffer_t  buffer,

    // 
    /*[out,ref]*/ aafUInt32 *  bytesRead);


  //***********************************************************
  //
  // SetPosition()
  //
  // Seek to absolute position within the essence data.
  //
  STDMETHOD (SetPosition) (
    // offset from beginning of essence 
    /*[in]*/ aafPosition_t  offset);


  //***********************************************************
  //
  // GetPosition()
  //
  // Get the absolute position within the essence data.
  //
  STDMETHOD (GetPosition) (
    // offset from beginning of essence 
    /*[out]*/ aafPosition_t*  pOffset);


  //***********************************************************
  //
  // GetSize()
  //
  // Return the total size of the essence data.
  //
  STDMETHOD (GetSize) (
    // size of essence data 
    /*[out]*/ aafLength_t *  pSize );


  //***********************************************************
  //
  // SetFileMob()
  //
  // Associates a weak reference to the given file mob with the
  // essence data.
  //
  // Succeeds if all of the following are true:
  // - the pFileMob pointer is valid and points to 
  // a file mob (contains a file descriptor).
  // 
  // If this method fails no state will be changed.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pFileMob is null.
  //
  STDMETHOD (SetFileMob) (
    // reference to a file mob 
    /*[in]*/ IAAFSourceMob * pFileMob);


  //***********************************************************
  //
  // GetFileMob()
  //
  // Associates a weak reference to the given file mob with the
  // essence data.
  //
  // Succeeds if all of the following are true:
  // - the ppFileMob pointer is valid and a weak
  // reference to the associated file mob can be
  // resolved.
  // 
  // If this method fails no state will be changed.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - ppFileMob is null.
  //
  STDMETHOD (GetFileMob) (
    // reference to a file mob 
    /*[in]*/ IAAFSourceMob ** ppFileMob);


  //***********************************************************
  //
  // GetFileMobID()
  //
  // Return the mob id used to find the file mob associated with this
  // essence.  The file mob must exist in the same file as this
  // essence data.
  //
  STDMETHOD (GetFileMobID) (
    // the file mob id associated with essence 
    /*[out]*/ aafMobID_t *  pFileMobID);


protected:
  // 
  // Declare the QI that implements for the interfaces
  // for this module. This will be called by CAAFUnknown::QueryInterface().
  // 
  STDMETHOD(InternalQueryInterface)(REFIID riid, void **ppvObjOut);


public:
  //
  // This class as concrete. All AAF objects can be constructed from
  // a CLSID. This will allow subclassing all "base-classes" by
  // aggreggation.
  // 
  AAF_DECLARE_CONCRETE();
  //
  //********
};

#endif // ! __CAAFEssenceData_h__


