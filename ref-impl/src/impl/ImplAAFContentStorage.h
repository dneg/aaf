//@doc
//@class    AAFHeader | Implementation class for AAFHeader
#ifndef __ImplAAFContentStorage_h__
#define __ImplAAFContentStorage_h__

//=---------------------------------------------------------------------=
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

class ImplAAFMob;
class ImplAAFEssenceData;
class ImplAAFDictionary;
class ImplAAFFile;

class ImplEnumAAFMobs;
template <class T> 
class ImplAAFEnumerator;
typedef ImplAAFEnumerator<ImplAAFEssenceData> ImplEnumAAFEssenceData;





#ifndef __ImplAAFObject_h__
#include "ImplAAFObject.h"
#endif

#include "aafErr.h"
#include "ImplAAFObject.h"
#include "ImplAAFMob.h"
#include "ImplAAFEssenceData.h"

#include "OMStrongRefSetProperty.h"
#include "OMDataTypes.h"

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


  //****************
  // LookupMob()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    LookupMob
        (aafMobID_constref  mobID,   //@parm [in,ref] The Mob ID
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
        (aafSearchCrit_t *  pSearchCriteria,   //@parm [in,ref] Search Criteria for enumeration
		 ImplEnumAAFMobs ** ppEnum);  //@parm [out,retval] Mob Enumeration


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
  // EnumEssenceData()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    EnumEssenceData
	    (	 // @parm [out,retval] Essence Enumeration
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



	// Interfaces visible inside the toolkit, but not exposed through the API
	AAFRESULT LookupEssenceData (aafMobID_constref fileMobID, ImplAAFEssenceData **ppEssence);
	AAFRESULT ChangeIndexedMobID (ImplAAFMob *pMob, aafMobID_constref newID);

private:
    OMStrongReferenceSetProperty<OMUniqueMaterialIdentification, ImplAAFMob> _mobs;
    OMStrongReferenceSetProperty<OMUniqueMaterialIdentification, ImplAAFEssenceData> _essenceData;
};

#endif // ! __ImplAAFHeader_h__
