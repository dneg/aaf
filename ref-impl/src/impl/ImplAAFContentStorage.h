//@doc
//@class    AAFHeader | Implementation class for AAFHeader
#ifndef __ImplAAFContentStorage_h__
#define __ImplAAFContentStorage_h__

/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
*                                          *
\******************************************/



class ImplAAFMob;

class ImplEnumAAFMobs;

class ImplAAFEssenceData;

class ImplEnumAAFEssenceData;

class ImplAAFDictionary;

class ImplAAFFile;






#ifndef __ImplAAFObject_h__
#include "ImplAAFObject.h"
#endif

#include "aafTable.h"

#include "aafErr.h"
#include "ImplAAFObject.h"
#include "ImplAAFMob.h"
#include "ImplAAFEssenceData.h"

#include "OMProperty.h"


class AAFDataKind;
class AAFOperationDef;


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
  // EnumEssenceData()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    EnumEssenceData
	    (	 // @parm [out,retval] Essence Enumeration
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



	// Interfaces visible inside the toolkit, but not exposed through the API
	virtual AAFRESULT UnlinkMobID(aafUID_t mobID);

	AAFRESULT LookupEssence (aafUID_t *pFileMobID, ImplAAFEssenceData **ppEssence);
	AAFRESULT ChangeIndexedMobID (ImplAAFMob *pMob, aafUID_t *newID);

AAFRESULT
    GetNthMob (aafInt32 index, ImplAAFMob **ppEnum);

AAFRESULT
    GetNthEssenceData (aafInt32 index, ImplAAFEssenceData **ppEnum);

AAFRESULT LoadMobTables(void);

private:
	aafTable_t		*_mobIndex;		// Non-persistant
    OMStrongReferenceVectorProperty<ImplAAFMob> _mobs;
    OMStrongReferenceVectorProperty<ImplAAFEssenceData> _essenceData;
};

#endif // ! __ImplAAFHeader_h__
