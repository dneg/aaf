//@doc
//@class    AAFHeader | Implementation class for AAFHeader
#ifndef __ImplAAFContentStorage_h__
#define __ImplAAFContentStorage_h__

/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/



class ImplAAFMob;

class ImplEnumAAFMobs;

class ImplEnumAAFMedia;

class ImplAAFDictionary;

class ImplAAFFile;






#ifndef __ImplAAFObject_h__
#include "ImplAAFObject.h"
#endif

#include "aafTable.h"

#include "aafErr.h"
#include "ImplAAFObject.h"
#include "ImplAAFMob.h"

#include "OMProperty.h"

class AAFDataKind;
class AAFEffectDef;
class ImplAAFSession;

const int PID_CONTENT_STORAGE_MOBS	   = 0;
//!!!const int PID_CONTENT_STORAGE_MEDIA    = 1;

class ImplAAFContentStorage : public ImplAAFObject
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFContentStorage ();
  ~ImplAAFContentStorage ();

	OMDECLARE_STORABLE(ImplAAFContentStorage)

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
  // GetPrimaryMobs()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetPrimaryMobs
        (ImplEnumAAFMobs ** ppEnum);  //@parm [out,retval] Mob Enumeration


  //****************
  // GetMobs()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetMobs
        (aafSearchCrit_t *  pSearchCriteria,   //@parm [in,ref] Search Criteria for enumeration
		 ImplEnumAAFMobs ** ppEnum);  //@parm [out,retval] Mob Enumeration


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
  // GetMedia()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetMedia
        (aafMediaCriteria_t *  pMediaCriteria,   //@parm [in,ref] Media Criteria for enumeration
		 ImplEnumAAFMedia ** ppEnum);  //@parm [out,retval] Media Enumeration

public:
	// Interfaces visible inside the toolkit, but not exposed through the API
	virtual AAFRESULT UnlinkMobID(aafUID_t mobID);

	aafBool IsMediaDataPresent( 	aafUID_t				fileMobUid,	/* IN -- */
									aafFileFormat_t	fmt);
	AAFRESULT AppendDataObject(aafUID_t mobID,      /* IN - Mob ID */
						  AAFObject *dataObj) ;    /* IN - Input Mob */ 
	AAFRESULT ChangeIndexedMobID (ImplAAFMob *pMob, aafUID_t *newID);

AAFRESULT
    GetNthMob (aafInt32 index, ImplAAFMob **ppEnum);

	AAFRESULT BuildMediaCache(void);
AAFRESULT LoadMobTables(void);

private:
	aafTable_t		*_mobIndex;		// Non-persistant
    OMStrongReferenceVectorProperty<ImplAAFMob> _mobs;
//!!!    OMStrongReferenceVectorProperty<ImplAAFMediaData> _mediaData;
};

#endif // ! __ImplAAFHeader_h__
