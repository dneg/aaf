//@doc
//@class    AAFHeader | Implementation class for AAFHeader
#ifndef __CAAFHeader_h__
#define __CAAFHeader_h__

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


class CAAFHeader
  : public IAAFHeader,
    public IAAFEndian,
    public CAAFObject
{
protected:

  //********
  //
  // Constructor/destructor
  //
  CAAFHeader (IUnknown * pControllingUnknown, aafBool doInit = kAAFTrue);
  virtual ~CAAFHeader ();

public:

  //***********************************************************
  //
  // LookupMob()
  //
  // Looks up the Mob that matches the given mob id and puts it into
  // the ppMob argument.  The returned mob interface is AddRef()ed
  // before it is returned.
  // 
  // Succeeds if all of the following are true:
  // - the ppMob pointer is valid.
  // 
  // If this method fails nothing will be written to *ppMob.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - ppMob is null
  //
  // AAFRESULT_MOB_NOT_FOUND
  //   - the requested mob wasn't found.
  //
  STDMETHOD (LookupMob) (
    // The Mob ID
    /*[in, ref]*/ aafMobID_constref  mobID,

    // Matching Mob
    /*[out,retval]*/ IAAFMob ** ppMob);


  //***********************************************************
  //
  // CountMobs()
  //
  // Writes the number of matches for the given mob kind into the
  // *pNumMobs argument.
  // 
  // Succeeds if all of the following are true:
  // - the pNumMobs pointer is valid.
  // 
  // If this method fails nothing will be written to *pNumMobs.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pNumMobs is null.
  //
  STDMETHOD (CountMobs) (
    // The mob kind to count
    /*[in]*/ aafMobKind_t  mobKind,

    // Total number of mobs of kind mobKind
    /*[out, retval]*/ aafNumSlots_t *  pResult);


  //***********************************************************
  //
  // GetMobs()
  //
  // Places an enumerator for mobs that apply to the criteria into the
  // *ppEnum argument.  If pSearchCriteria is null, all mobs are
  // returned.   The searchTag field of pSearchCriteria, and exactly
  // ONE of the fields in the union (tags.mobID, tags.name, etc. )
  // must be set.  Only one search criterion may be specified.  The
  // returned enumerator is AddRef()ed before it is returned.
  // 
  // Succeeds if all of the following are true:
  // - the ppEnum pointer is valid.
  // 
  // If this method fails nothing will be written to *ppEnum.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - ppEnum is null.
  //
  STDMETHOD (GetMobs) (
    // Search Criteria for enumeration
    /*[in]*/ aafSearchCrit_t *  pSearchCriteria,

    // Mob Enumeration
    /*[out, retval]*/ IEnumAAFMobs ** ppEnum);


  //***********************************************************
  //
  // AddMob()
  //
  // Appends the given mob to the header.  If the given mob is already
  // contained this method will do nothing and will return success.
  // 
  // Succeeds if all of the following are true:
  // - the pMob pointer is valid.
  // - the given mob is not already part of this collection.
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
  //   - pMob is null.
  //
  // AAFRESULT_DUPLICATE_MOBID
  //   - the given mob is already contained.
  //
  STDMETHOD (AddMob) (
    // Mob to add 
    /*[in]*/ IAAFMob * pMob);


  //***********************************************************
  //
  // RemoveMob()
  //
  // // Removes the given mob from the header.
  //
  // Succeeds if all of the following are true:
  // - the pMob pointer is valid.
  // - the given mob is currently in the collection.
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
  //   - pMob is null.
  //
  // AAFRESULT_MOB_NOT_FOUND
  //   - the given mob is not already contained.
  //
  STDMETHOD (RemoveMob) (
    // Mob to remove 
    /*[in]*/ IAAFMob * pMob);


  //***********************************************************
  //
  // CountEssenceData()
  //
  // Writes the total number of essence data into the *pNumEssenceData
  // argument.
  // 
  // Succeeds if all of the following are true:
  // - the pNumEssenceData pointer is valid.
  // 
  // If this method fails nothing will be written to
  // *pNumEssenceData.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pNumEssenceData is null.
  //
  STDMETHOD (CountEssenceData) (
    // Total number of essence data 
    /*[out, retval]*/ aafUInt32 *  pResult);


  //***********************************************************
  //
  // IsEssenceDataPresent()
  //
  // // 
  // Succeeds if all of the following are true:
  // - the pResult pointer is valid.
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
  //   - pResult is null.
  //
  STDMETHOD (IsEssenceDataPresent) (
    // A Unique File Mob ID
    /*[in, ref]*/ aafMobID_constref  fileMobID,

    // The Essence File Format
    /*[in]*/ aafFileFormat_t  fmt,

    // True if the essence is found
    /*[out,retval]*/ aafBoolean_t *  pResult);


  //***********************************************************
  //
  // EnumEssenceData()
  //
  // Places an enumerator for essence that applies to the criteria
  // into the *ppEnum argument.  The returned enumerator is
  // AddRef()ed before it is returned.
  // 
  // Succeeds if all of the following are true:
  // - the pMediaCriteria pointer is valid.
  // - the ppEnum pointer is valid.
  // 
  // If this method fails nothing will be written to *ppEnum.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - either pMediaCriteria or ppEnum is null.
  //
  STDMETHOD (EnumEssenceData) (
    // Essence Enumeration 
    /*[out,retval]*/ IEnumAAFEssenceData ** ppEnum);


  //***********************************************************
  //
  // AddEssenceData()
  //
  // Appends the given essence data object to the header.
  // 
  // NOTE! Stub only.   Implementation not yet added.
  //
  // Succeeds if all of the following are true:
  // - the pEssenceData pointer is valid.
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
  // AAFRESULT_DUPLICATE_MOBID
  //   - The given mob has already been added.  The validation is done by comparing
  //     mobIDs, which should be unique.
  //
  // AAFRESULT_NULL_PARAM
  //   - pEssenceData is null.
  //
  STDMETHOD (AddEssenceData) (
    // Essence data object to append 
    /*[in]*/ IAAFEssenceData * pEssenceData);

  //***********************************************************
  //
  // RemoveEssenceData()
  //
  // // Removes the given EssenceData from the header.
  //
  // Succeeds if all of the following are true:
  // - the pEssenceData pointer is valid.
  // - the given EssenceData is currently in the collection.
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
  //   - pEssenceData is null.
  //
  // AAFRESULT_ESSENCE_NOT_FOUND
  //   - the given EssenceData is not already contained.
  //
  STDMETHOD (RemoveEssenceData) (
    // EssenceData to remove 
    /*[in]*/ IAAFEssenceData * pEssenceData);

  //***********************************************************
  //
  // LookupEssenceData()
  //
  // Looks up the EssenceData that matches the given mob id and puts it into
  // the ppEssenceData argument.  The returned EssenceData interface is AddRef()ed
  // before it is returned.
  // 
  // Succeeds if all of the following are true:
  // - the ppEssenceData pointer is valid.
  // 
  // If this method fails nothing will be written to *ppEssenceData.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - ppEssenceData is null
  //
  // AAFRESULT_MOB_NOT_FOUND
  //   - the requested EssenceData wasn't found.
  //
  STDMETHOD (LookupEssenceData) (
    // The Mob ID
    /*[in, ref]*/ aafMobID_constref  mobID,

    // Matching EssenceData
    /*[out,retval]*/ IAAFEssenceData ** ppEssenceData);

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


  //***********************************************************
  //
  // GetLastIdentification()
  //
  // Places the identification of the last entity that modified the
  // file into the *ppIdentification argument.  The returned
  // identification is AddRef()ed before it is returned.
  // 
  // Succeeds if all of the following are true:
  // - the ppIdentification pointer is valid.
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
  //   - ppIdentification is null.
  //
  STDMETHOD (GetLastIdentification) (
    // Indentification Object 
    /*[out,retval]*/ IAAFIdentification ** ppIdentification);


  //***********************************************************
  //
  // LookupIdentification()
  //
  // Places the Identification that matches the given generation into
  // the *ppIdentification argument.  The returned identification is
  // AddRef()ed before it is returned.
  // 
  // Succeeds if all of the following are true:
  // - the ppIdentification pointer is valid.
  // - the given generation was found.
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
  //   - ppIdentification is null.
  //
  // AAFRESULT_OBJECT_NOT_FOUND
  //   - the given generation was not found..
  //
  STDMETHOD (LookupIdentification) (
    // Unique Generation ID
    /*[in, ref]*/ aafUID_constref  generation,

    // Indentification Object
    /*[out,retval]*/ IAAFIdentification ** ppIdentification);


  //***********************************************************
  //
  // CountIdentifications()
  //
  // Writes the number of identification objects into the *pResult
  // argument.
  // 
  // Succeeds if all of the following are true:
  // - the pResult pointer is valid.
  // 
  // If this method fails nothing will be written to *pResult.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pResult is null.
  //
  STDMETHOD (CountIdentifications) (
    // Total number of identification objects 
    /*[out, retval]*/ aafUInt32 *  pResult);


  //***********************************************************
  //
  // GetIdentifications()
  //
  // Places an enumerator for all Identifications criteria into	the
  // *ppEnum argument.  The returned enumerator is AddRef()ed before
  // it is returned.
  // 
  // NOTE! Stub only.   Implementation not yet added.
  //
  // Succeeds if all of the following are true:
  // - the ppEnum pointer is valid.
  // 
  // If this method fails nothing will be written to *ppEnum.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - ppEnum is null.
  //
  STDMETHOD (GetIdentifications) (
    // Indentification Enumeration 
    /*[out,retval]*/ IEnumAAFIdentifications ** ppEnum);


  //***********************************************************
  //
  // AppendIdentification()
  //
  // Appends the given Identification class to the header.  This
  // method does not attempt to identify duplicate identifications, so
  // it will succeed even if an identical identification has already
  // been appended.
  // 
  // Succeeds if all of the following are true:
  // - the pIdent pointer is valid.
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
  //   - pIdent is null.
  //
  STDMETHOD (AppendIdentification) (
    // Identification to append 
    /*[in]*/ IAAFIdentification * pIdent);


  //***********************************************************
  //
  // GetIdentificationAt()
  //
  // Retrieves the indexed identification from the header.
  // 
  // Succeeds if all of the following are true:
  // - the ppIdentification pointer is valid.
  // - index is less than the value returned by CountIdentifications().
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - ppIdent is null.
  //
  // AAFRESULT_BADINDEX
  //   - index is greater than or equal to result of
  //     CountIdentifications().
  //
  STDMETHOD (GetIdentificationAt) (
    // Index of identification to retrieve
    /*[in]*/ aafUInt32  index,

    // Retrieved identification
    /*[out, retval]*/ IAAFIdentification ** ppIdentification);


  //***********************************************************
  //
  // GetRefImplVersion()
  //
  // Return the version of the Reference Implementation currently
  // running on this machine, which implements these interfaces.
  // 
  // Succeeds if all of the following are true:
  // - the pVersion pointer is valid.
  // 
  // If this method fails nothing is written to *pVersion.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pVersion is null.
  //
  STDMETHOD (GetRefImplVersion) (
    // The Reference Implementation Version 
    /*[out, retval]*/ aafProductVersion_t *  pVersion);


  //***********************************************************
  //
  // GetFileRevision()
  //
  // Return the File Revision property.
  // 
  // NOTE! Stub only.   Implementation not yet added.
  //
  // Succeeds if all of the following are true:
  // - the pRevision pointer is valid.
  // 
  // If this method fails nothing is written to *pRevision.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pRevision is null.
  //
  STDMETHOD (GetFileRevision) (
    // The File Version 
    /*[out, retval]*/ aafVersionType_t *  pRevision);


  //***********************************************************
  //
  // GetLastModified()
  //
  // Return the Last Modified property.
  // 
  // NOTE! Stub only.   Implementation not yet added.
  //
  // Succeeds if all of the following are true:
  // - the pTimeStamp pointer is valid.
  // 
  // If this method fails nothing is written to *pTimeStamp.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pTimeStamp is null.
  //
  STDMETHOD (GetLastModified) (
    // The modification date-time stamp 
    /*[out, retval]*/ aafTimeStamp_t *  pTimeStamp);


  //***********************************************************
  //
  // GetContentStorage()
  //
  // Places the Content Storage object attached to the header into the
  // *ppStorage argument.
  //
  // The returned content storage object is
  // AddRef()ed before it is returned.
  //
  // Succeeds if all of the following are true:
  // - the ppStorage pointer is valid.
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
  //   - ppStorage is null.
  //
  STDMETHOD (GetContentStorage) (
    // Returned Content Storage object 
    /*[out]*/ IAAFContentStorage ** ppStorage);

  //***********************************************************
  // METHOD NAME: GetStoredByteOrder()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFEndian | GetStoredByteOrder |
  // Returns the "Endian-ness" in which the current object was or will
  // be stored.  If this is a transient object (i.e., one which has
  // not been persisted) then it will return the native byte order of
  // the platform on which this is running.
  // 
  // Succeeds if all of the following are true:
  // - the pOrder pointer is valid.
  // 
  // If this method fails nothing is written to *pOrder.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pOrder is null.
  // @end
  // 
  STDMETHOD (GetStoredByteOrder)
   (
    // @parm [out] eAAFByteOrder_t * | pOrder | Pointer to place where byte order is to be put
    eAAFByteOrder_t *  pOrder
  );



  //***********************************************************
  // METHOD NAME: GetNativeByteOrder()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFEndian | GetNativeByteOrder |
  // Returns the native "Endian-ness" of the platform on which this is
  // running.
  // 
  // Succeeds if all of the following are true:
  // - the pOrder pointer is valid.
  // 
  // If this method fails nothing is written to *pOrder.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pOrder is null.
  // @end
  // 
  STDMETHOD (GetNativeByteOrder)
   (
    // @parm [out] eAAFByteOrder_t * | pOrder | Pointer to place where byte order is to be put
    eAAFByteOrder_t *  pOrder
  );


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

#endif // ! __CAAFHeader_h__

