//@doc
//@class    AAFHeader | Implementation class for AAFHeader
#ifndef __ImplAAFHeader_h__
#define __ImplAAFHeader_h__

/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/



class ImplAAFMob;

class ImplAAFEssenceData;

class ImplEnumAAFMobs;

class ImplEnumAAFEssenceData;

class ImplAAFDictionary;

class ImplAAFIdentification;

class ImplEnumAAFIdentifications;

class ImplAAFFile;






#ifndef __ImplAAFObject_h__
#include "ImplAAFObject.h"
#endif

#include "aafTable.h"

#include "aafErr.h"
#include "ImplAAFObject.h"
#include "ImplAAFIdentification.h"
#include "ImplAAFContentStorage.h"
#include "ImplAAFDictionary.h"


class AAFDataKind;
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
  OMDECLARE_STORABLE(ImplAAFHeader)

  //****************
  // LookupMob()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    LookupMob
        (aafUID_t *  mobID,   //@parm [in,ref] The Mob ID
		 ImplAAFMob ** ppMob);  //@parm [out,retval] Matching Mob


  //****************
  // GetNumMobs()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetNumMobs
        (aafMobKind_t  mobKind,   //@parm [in] The mob kind to count
		 aafNumSlots_t *  pNumMobs);  //@parm [out,retval] Total number of mobs of kind mobKind

  //****************
  // EnumAAFAllMobs()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    EnumAAFAllMobs
	    (// @parm [in,ref] Search Criteria for enumeration
         aafSearchCrit_t *  pSearchCriteria,

		 // @parm [out,retval] Mob Enumeration
		 ImplEnumAAFMobs ** ppEnum);


  //****************
  // AppendMob()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    AppendMob
        (ImplAAFMob * pMob);  //@parm [in] Mob to add header


  //****************
  // RemoveMob()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    RemoveMob
        (ImplAAFMob * pMob);  //@parm [in] Mob to remove from header


  //****************
  // GetNumEssenceData()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetNumEssenceData
        (aafUInt32 *  pNumEssenceData);  //@parm [out,retval] Total number of essence data with type


  //****************
  // IsEssenceDataPresent()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    IsEssenceDataPresent
        (// @parm [in] A Unique File Mob ID
		 aafUID_t *  pFileMobID,

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
  // AppendEssenceData()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    AppendEssenceData
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
        (ImplAAFDictionary ** ppDictionary);  //@parm [out,retval] The AAF Dictionary



  //****************
  // GetLastIdentification()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetLastIdentification
        (ImplAAFIdentification ** ppIdentification);  //@parm [out,retval] Indentification Object


  //****************
  // GetIdentificationByGen()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetIdentificationByGen
	    (// @parm [in,ref] Unique Generation ID
         aafUID_t *  pGeneration,

		 // @parm [out,retval] Indentification Object
		 ImplAAFIdentification ** ppIdentification);


  //****************
  // GetNumIdents()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetNumIdents
        (aafUInt32 *  pNumIdents);  //@parm [out,retval] Total number of identifications


  //****************
  // EnumAAFIdents()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    EnumAAFIdents
		// @parm [out,retval] Indentification Enumeration
        (ImplEnumAAFIdentifications ** ppEnum);



  //****************
  // AppendIdentification()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    AppendIdentification
		// @parm [in] Identification to append
        (ImplAAFIdentification * pIdent);


  //****************
  // RemoveIdentification()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    RemoveIdentification
		// @parm [in] Identification to remove
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

public:
	// Interfaces visible inside the toolkit, but not exposed through the API

AAFRESULT SetToolkitRevisionCurrent(void);
AAFRESULT GetNumIdentifications (aafInt32 * /*pCount*/);

AAFRESULT AddIdentificationObject (aafProductIdentification_t * /*pIdent*/);
AAFRESULT LoadMobTables(void);
AAFRESULT SetModified(void);		// To NOW

  void SetByteOrder(const aafInt16 byteOrder);
  void SetDictionary(ImplAAFDictionary *pDictionary);
  void SetFileRevision(aafVersionType_t pRevision);

private:
	// These are private accessor methods.
	ImplAAFContentStorage *GetContentStorage(void);
	ImplAAFDictionary *GetDictionary(void);

private:

		ImplAAFFile		*_file;

		// Non-table instance variables
		aafProductVersion_t	_toolkitRev;

		// Persistent properties
    //
		OMFixedSizeProperty<aafInt16>						_byteOrder;
		OMFixedSizeProperty<aafTimeStamp_t>					_lastModified;
		OMStrongReferenceVectorProperty<ImplAAFIdentification> _identificationList;
		OMStrongReferenceProperty<ImplAAFContentStorage>	_contentStorage;
		OMStrongReferenceProperty<ImplAAFDictionary>		_dictionary;
		OMFixedSizeProperty<aafVersionType_t>               _fileRev;
};

#endif // ! __ImplAAFHeader_h__
