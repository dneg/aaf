//@doc
//@class    AAFHeader | Implementation class for AAFHeader
#ifndef __ImplAAFHeader_h__
#define __ImplAAFHeader_h__

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



class ImplAAFMob;
class ImplAAFEssenceData;
class ImplAAFDictionary;
class ImplAAFIdentification;
class ImplAAFFile;

template <class T> 
class ImplAAFEnumerator;
class ImplEnumAAFMobs;
typedef ImplAAFEnumerator<ImplAAFEssenceData> ImplEnumAAFEssenceData;
typedef ImplAAFEnumerator<ImplAAFIdentification> ImplEnumAAFIdentifications;

#ifndef __ImplAAFObject_h__
#include "ImplAAFObject.h"
#endif

#include "OMSetProperty.h"
#include "OMStrongRefProperty.h"
#include "OMStrongRefVectorProperty.h"

#include "aafTable.h"

#include "aafErr.h"
#include "AAFUtils.h"
#include "ImplAAFObject.h"
#include "ImplAAFIdentification.h"
#include "ImplAAFContentStorage.h"
#include "ImplAAFDictionary.h"
#include "ImplAAFFile.h"


class AAFOperationDef;


class ImplAAFHeader : public ImplAAFObject
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFHeader ();

protected:
  ~ImplAAFHeader ();

public:

  //****************
  // LookupMob()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    LookupMob
        (aafMobID_constref mobID,   //@parm [in,ref] The Mob ID
		 ImplAAFMob ** ppMob);  //@parm [out,retval] Matching Mob


  //****************
  // CountMobs()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    CountMobs
        (aafMobKind_t  mobKind,   //@parm [in] The mob kind to count
		 aafNumSlots_t *  pNumMobs);  //@parm [out,retval] Total number of mobs of kind mobKind

  //****************
  // GetMobs()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetMobs
	    (// @parm [in,ref] Search Criteria for enumeration
         aafSearchCrit_t *  pSearchCriteria,

		 // @parm [out,retval] Mob Enumeration
		 ImplEnumAAFMobs ** ppEnum);


  //****************
  // AddMob()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    AddMob
        (ImplAAFMob * pMob);  //@parm [in] Mob to add header


  //****************
  // RemoveMob()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    RemoveMob
        (ImplAAFMob * pMob);  //@parm [in] Mob to remove from header


  //****************
  // CountEssenceData()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    CountEssenceData
        (aafUInt32 *  pNumEssenceData);  //@parm [out,retval] Total number of essence data with type


  //****************
  // IsEssenceDataPresent()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    IsEssenceDataPresent
        (// @parm [in] A Unique File Mob ID
		 aafMobID_constref fileMobID,

		 // @parm [in] The Essence File Format
		 aafFileFormat_t  fmt,

		 // @parm [out,retval] True if the essence is found
         aafBool *  pResult);


  //****************
  // EnumAAFEssenceData()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    EnumEssenceData
	    ( // @parm [out,retval] Essence Enumeration
		 ImplEnumAAFEssenceData ** ppEnum);


  //****************
  // AddEssenceData()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    AddEssenceData
		// @parm [in] Essence data object to append
        (ImplAAFEssenceData * pEssenceData);


  //****************
  // RemoveEssenceData()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    RemoveEssenceData
		// @parm [in] Essence data object to Remove
        (ImplAAFEssenceData * pEssenceData);

  //****************
  // LookupEssenceData()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    LookupEssenceData
        (aafMobID_constref mobID,   //@parm [in,ref] The Mob ID
		 ImplAAFEssenceData ** ppEssenceData);  //@parm [out,retval] Matching EssenceData

  //****************
  // GetContentStorage()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetContentStorage
        (ImplAAFContentStorage ** ppContentStorage);  //@parm [out,retval] The AAF ContentStorage


  //****************
  // GetDictionary()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetDictionary
        (ImplAAFDictionary ** ppDictionary) const;  //@parm [out,retval] The AAF Dictionary



  //****************
  // GetLastIdentification()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetLastIdentification
        (ImplAAFIdentification ** ppIdentification);  //@parm [out,retval] Indentification Object


  //****************
  // LookupIdentification()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    LookupIdentification
	    (// @parm [in,ref] Unique Generation ID
         const aafUID_t & generation,

		 // @parm [out,retval] Indentification Object
		 ImplAAFIdentification ** ppIdentification);


  //****************
  // CountIdents()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    CountIdentifications
        (aafUInt32 *  pNumIdents);  //@parm [out,retval] Total number of identifications


  //****************
  // GetIdentifications()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetIdentifications
		// @parm [out,retval] Indentification Enumeration
        (ImplEnumAAFIdentifications ** ppEnum);



  //****************
  // GetIdentificationAt()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetIdentificationAt
		// @parm [in] index of identification
        (aafUInt32 index,

		// @parm [out,retval] returned Indentification
        ImplAAFIdentification ** ppIdentification);



  //****************
  // AppendIdentification()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    AppendIdentification
		// @parm [in] Identification to append
        (ImplAAFIdentification * pIdent);

  //****************
  // GetRefImplVersion()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetRefImplVersion
		// @parm [out,retval] The Toolkit Version
        (aafProductVersion_t *  pToolkitVersion);


  //****************
  // GetFileRevision()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetFileRevision
        (aafVersionType_t *  pRevision);  //@parm [out,retval] The Toolkit Version

	
  //****************
  // GetLastModified()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetLastModified
        (aafTimeStamp_t *  pLastModified);  //@parm [out,retval] The modification date-time stamp

	
  //****************
  // GetStoredByteOrder()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
  GetStoredByteOrder (eAAFByteOrder_t * pOrder);

	
  //****************
  // GetNativeByteOrder()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
  GetNativeByteOrder (eAAFByteOrder_t * pOrder);


  //****************
  // GetPrimaryMob()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetPrimaryMob (ImplAAFMob** pPrimaryMob);


  //****************
  // SetPrimaryMob()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetPrimaryMob (ImplAAFMob* pPrimaryMob);


  //****************
  // GetOperationalPattern()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetOperationalPattern
        (aafUID_t* pOperationalPattern);


  //****************
  // SetOperationalPattern()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetOperationalPattern
        (const aafUID_t& operationalPatternID);


  //****************
  // UpdateEssenceContainers()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    UpdateEssenceContainers
        ();

  //****************
  // CountEssenceContainers()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    CountEssenceContainers
        (aafUInt32 * pCount);


  //****************
  // GetEssenceContainers()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetEssenceContainers
        (aafUInt32 maxEssenceContainersCount,
         aafUID_t* pEssenceContainers);


  //****************
  // IsEssenceContainerPresent()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    IsEssenceContainerPresent
        (const aafUID_t& essenceContainer,
         aafBoolean_t* pIsPresent);


  //****************
  // CountDescriptiveSchemes()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    CountDescriptiveSchemes
        (aafUInt32 * pCount);


  //****************
  // GetDescriptiveSchemes()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetDescriptiveSchemes
        (aafUInt32 maxDescriptiveSchemesCount,
         aafUID_t* pDescriptiveSchemes);


  //****************
  // IsDescriptiveSchemePresent()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    IsDescriptiveSchemePresent
        (const aafUID_t& descriptiveScheme,
         aafBoolean_t* pIsPresent);


  //****************
  // AddDescriptiveScheme()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    AddDescriptiveScheme
        (const aafUID_t& descriptiveSchemeID);


  //****************
  // RemoveDescriptiveScheme()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    RemoveDescriptiveScheme
        (const aafUID_t& descriptiveSchemeID);


public:
  // Interfaces visible inside the toolkit, but not exposed through the API

  AAFRESULT SetToolkitRevisionCurrent(void);
  // AAFRESULT CountIdentifications (aafInt32 * /*pCount*/);

  AAFRESULT AddIdentificationObject (aafProductIdentification_constptr
									 /*pIdent*/);
  AAFRESULT SetModified(void);		// To NOW

  void SetByteOrder(const aafInt16 byteOrder);
  void SetDictionary(ImplAAFDictionary *pDictionary);
  void SetFileRevision(aafVersionType_t pRevision);

  // Returns true if ObjectModelVersion is present.  Calling
  // SetObjectModelVersion() will make it present.
  bool      IsObjectModelVersionPresent () const;

  // Returns the file format version of this file.  Requires that the
  // property be present.  Call IsObjectModelVersionPresent() to find
  // out.
  aafUInt32 GetObjectModelVersion () const;

  // Makes the ObjectModelVersion present, if it wasn't already; in any
  // case, sets it to the given value.
  void      SetObjectModelVersion (aafUInt32 version);
  void SetFile(ImplAAFFile *file) { _file = file; }

  // Set content store and clean the ident list so that a new set of
  // idents can be appended.  Used by ImplAAFFile::SaveCopyAs()
  void SetContentStorage( ImplAAFContentStorage* pStorage );
  void ClearIdentificationList();

private:
  // These are private accessor methods.
  ImplAAFContentStorage *GetContentStorage(void);
  ImplAAFDictionary *GetDictionary(void) const;

private:

  ImplAAFFile		*_file;

  // Non-table instance variables
  aafProductVersion_t	_toolkitRev;

  // Persistent properties
  //
  OMFixedSizeProperty<aafInt16>                    _byteOrder;
  OMFixedSizeProperty<aafTimeStamp_t>              _lastModified;
  OMStrongReferenceVectorProperty<ImplAAFIdentification>
    _identificationList;
  OMStrongReferenceProperty<ImplAAFContentStorage> _contentStorage;
  OMStrongReferenceProperty<ImplAAFDictionary>     _dictionary;
  OMFixedSizeProperty<aafVersionType_t>            _fileRev;
  OMFixedSizeProperty<aafUInt32>                   _objectModelVersion;
  OMFixedSizeProperty<aafUID_t>                    _operationalPattern;
  OMSetProperty<aafUID_t>                          _essenceContainers;
  OMSetProperty<aafUID_t>                          _descriptiveSchemes;
};

//
// smart pointer
//

#ifndef __ImplAAFSmartPointer_h__
// caution! includes assert.h
#include "ImplAAFSmartPointer.h"
#endif

typedef ImplAAFSmartPointer<ImplAAFHeader> ImplAAFHeaderSP;

#endif // ! __ImplAAFHeader_h__
