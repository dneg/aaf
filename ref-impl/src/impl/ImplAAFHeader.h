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

class ImplAAFMedia;

class ImplEnumAAFMobs;

class ImplEnumAAFMedia;

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
//#include "ImplAAFSession.h"
#include "ImplAAFIdentification.h"
#include "ImplAAFContentStorage.h"

#include "OMProperty.h"

class AAFDataKind;
class AAFEffectDef;
class ImplAAFSession;

const int PID_HEADER_BYTEORDER          = 0;
const int PID_HEADER_LASTMODIFIED       = 1;
const int PID_HEADER_IDENTIFICATIONLIST = 2;
const int PID_HEADER_CONTENTSTORAGE		= 3;

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
  // EnumAAFPrimaryMobs()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    EnumAAFPrimaryMobs
		// @parm [out,retval] Mob Enumeration
        (ImplEnumAAFMobs ** ppEnum);


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
  // IsMediaDataPresent()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    IsMediaDataPresent
        (aafUID_t *  pFileMobID,   //@parm [in,ref] A Unique File Mob ID
		 aafFileFormat_t  fmt,   //@parm [in] The Media File Format
         aafBool *  result);  //@parm [out,retval] True if the media is found


  //****************
  // EnumAAFMediaObjects()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    EnumAAFMediaObjects
	    (// @parm [in,ref] Media Criteria for enumeration
         aafMediaCriteria_t *  pMediaCriteria,

		 // @parm [out,retval] Media Enumeration
		 ImplEnumAAFMedia ** ppEnum);


  //****************
  // AppendMedia()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    AppendMedia
		// @parm [in] Media object to append
        (ImplAAFMedia * pMedia);


  //****************
  // RemoveMedia()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    RemoveMedia
		// @parm [in] Media object to remove
        (ImplAAFMedia * pMedia);


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
  // Declare the module test method. The implementation of the will be be
  // in /test/ImplAAFHeaderTest.cpp.
  static AAFRESULT test();

public:
	// Interfaces visible inside the toolkit, but not exposed through the API
	AAFRESULT AppendDataObject(aafUID_t mobID,      /* IN - Mob ID */
						  ImplAAFObject *dataObj) ;    /* IN - Input Mob */ 

AAFRESULT SetToolkitRevisionCurrent(void);
AAFRESULT IsValidHeadObject(void);

AAFRESULT GetNumIdentifications (aafInt32 * /*pCount*/);

AAFRESULT AddIdentificationObject (aafProductIdentification_t * /*pIdent*/);
AAFRESULT BuildMediaCache(void);
AAFRESULT LoadMobTables(void);

private:
	// These are private accessor methods.
	ImplAAFContentStorage *GetContentStorage(void);
	ImplAAFDictionary *GetDictionary(void);

public:
#if FULL_TOOLKIT
AAFRESULT ReadToolkitRevision(aafProductVersion_t *revision);
AAFRESULT WriteToolkitRevision(aafProductVersion_t revision);
AAFRESULT FinishCreation(void);
AAFRESULT BuildDatakindCache(void);
AAFRESULT BuildEffectDefCache(void);
AAFRESULT UpdateLocalCLSD(void);
AAFRESULT CreateTables(void);
AAFRESULT UpdateFileCLSD(void);
AAFRESULT CreateDatakindCache(void);
#endif

private:

		ImplAAFFile		*_file;
#if FULL_TOOLKIT
		aafTable_t       *_dataObjs;
		aafTable_t       *_datadefs;
		aafTable_t       *_effectDefs;
		aafTable_t       *_mobs;
#endif

		// Non-table instance variables
		aafVersionType_t	_fileRev;
		aafProductVersion_t	_toolkitRev;

		// Persistent properties
    //
		OMFixedSizeProperty<aafInt16>                      _byteOrder;
		OMFixedSizeProperty<aafTimeStamp_t>                _lastModified;
    OMStrongReferenceVectorProperty<ImplAAFIdentification> _identificationList;
		OMStrongReferenceProperty<ImplAAFContentStorage>	_contentStorage;
};

#endif // ! __ImplAAFHeader_h__
