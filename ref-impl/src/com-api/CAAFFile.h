//@doc
//@class    AAFFile | Implementation class for AAFFile
#ifndef __CAAFFile_h__
#define __CAAFFile_h__

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






#ifndef __CAAFRoot_h__
#include "CAAFRoot.h"
#endif


class CAAFFile
  : public IAAFFile,
    public CAAFRoot
{
protected:

  //********
  //
  // Constructor/destructor
  //
  CAAFFile (IUnknown * pControllingUnknown, aafBool doInit = kAAFTrue);
  virtual ~CAAFFile ();

public:


  //***********************************************************
  //
  // Open()
  //
  // Associates this IAAFFile-supporting object with the storage to
  // which it is to be associated.  It will then be possible to access
  // objects within this file.
  //
  // This method will succeed only if all of the following are true:
  // - This object has not already been opened.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_ALREADY_OPEN
  //   - This object has already been Open()ed.
  //
  STDMETHOD (Open)
     ();



  //***********************************************************
  //
  // SaveCopyAs()
  //
  // Copies the contents of this file to *pDestFile.  Requires that
  // *pDestFile be open, writeable or modifiable, and not yet
  // contain any objects.
  //
  // NOTE! Stub only.   Implementation not yet added.
  //
  // Succeeds if:
  // - This object is currently open.
  // - The named filesystem file is writable.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NOT_OPEN
  //   - This object is not open.
  //
  // AAFRESULT_NOT_WRITEABLE
  //   - The named file cannot be written.
  //
  STDMETHOD (SaveCopyAs) (
    // File to recieve contents 
    /*[in]*/ IAAFFile * pDestFile);


  //***********************************************************
  //
  // GetHeader()
  //
  // Returns the IAAFHeader-supporting object associated with this
  // file.  If this object has never been associated with a file, a
  // new empty IAAFHeader-supporting object will be created and
  // returned.  The returned header is AddRef()ed before it is
  // returned.  Note that the header is automatically created when the
  // file object is created.
  //
  // Succeeds if:
  // - This object has already been Initialize()d.
  // - This file object is open.
  // - the given header pointer is valid.
  // - this object contains a header.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NOT_INITIALIZED
  //   - This object has not yet had Initialize() called on it.
  //
  // AAFRESULT_NOT_OPEN
  //   - This object is not open.
  //
  // AAFRESULT_NULL_PARAM
  //   - the header pointer arg is NULL.
  //
  // AAFRESULT_NO_HEADER
  //   - this object contains no header.
  //
  STDMETHOD (GetHeader) (
    // Set to header of the current file 
    /*[out, retval]*/ IAAFHeader ** ppHeader);


  //***********************************************************
  //
  // GetRevision()
  //
  // Get the revision of the current AAF file, and returns it in pRev.
  //
  // Succeeds if:
  // - This object has already been Initialize()d.
  // - the given revision pointer is valid.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NOT_INITIALIZED
  //   - This object has not yet had Initialize() called on it.
  //
  // AAFRESULT_NULL_PARAM
  //   - the revision pointer arg is NULL.
  //
  STDMETHOD (GetRevision) (
    // Revision of the current file 
    /*[out, retval]*/ aafFileRev_t *  pRev);


  //***********************************************************
  //
  // Save()
  //
  // If this IAAFFile-supporting object is open, all unsaved changes
  // made to the contents of this object are saved.
  //
  // This method will succeed only if all of the following are true:
  // - This object is currently open.
  // - Sufficient space remains in the storage for the data to be
  //   written.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NOT_OPEN
  //   - This object is not open.
  //
  // AAFRESULT_WRONG_OPENMODE
  //   - This object is not open for writing or modification.
  //
  // AAFRESULT_INSUFFICIENT_SPACE
  //   - There is insufficient space in the storage to save the
  //     contents of this object.
  //
  STDMETHOD (Save)
     ();



  //***********************************************************
  //
  // Close()
  //
  // Dissociates this IAAFFile-supporting object from its
  // currently-associated storage.  Any AAF objects which were
  // contained within this object will then no longer be available to
  // the client.
  //
  // If the client is maintaining any references to any objects which
  // are contained in this object, those objects will be marked as
  // dissociated, and methods on them will fail.
  //
  // Any unsaved changes will be written to the file before the Close
  // operation is complete.
  //
  // This method will succeed only if all of the following are true:
  // - This object is currently open.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NOT_OPEN
  //   - This object is not already open.
  //
  STDMETHOD (Close)
     ();


  //***********************************************************
  //
  // GetDictionary()
  //
  // Places the dictionary that contains all types of aaf definition
  // objects into the *ppDictionary argument.  The returned dictionary
  // is AddRef()ed before it is returned.  Note that the dictionary
  // is automatically created when the header object is created.
  // 
  // Succeeds if all of the following are true:
  // - the ppDictionary pointer is valid.
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
  //   - ppDictionary is null.
  //
  STDMETHOD (GetDictionary) (
    // The AAF Dictionary 
    /*[out, retval]*/ IAAFDictionary ** ppDictionary);


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

#endif // ! __CAAFFile_h__


