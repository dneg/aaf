//@doc
//@class    AAFHeader | Implementation class for AAFHeader
#ifndef __ImplAAFContentStorage_h__
#define __ImplAAFContentStorage_h__

/***********************************************************************
 *
 *              Copyright (c) 1998-1999 Avid Technology, Inc.
 *
 * Permission to use, copy and modify this software and accompanying 
 * documentation, and to distribute and sublicense application software
 * incorporating this software for any purpose is hereby granted, 
 * provided that (i) the above copyright notice and this permission
 * notice appear in all copies of the software and related documentation,
 * and (ii) the name Avid Technology, Inc. may not be used in any
 * advertising or publicity relating to the software without the specific,
 *  prior written permission of Avid Technology, Inc.
 *
 * THE SOFTWARE IS PROVIDED AS-IS AND WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY
 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 * IN NO EVENT SHALL AVID TECHNOLOGY, INC. BE LIABLE FOR ANY DIRECT,
 * SPECIAL, INCIDENTAL, PUNITIVE, INDIRECT, ECONOMIC, CONSEQUENTIAL OR
 * OTHER DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE AND
 * ACCOMPANYING DOCUMENTATION, INCLUDING, WITHOUT LIMITATION, DAMAGES
 * RESULTING FROM LOSS OF USE, DATA OR PROFITS, AND WHETHER OR NOT
 * ADVISED OF THE POSSIBILITY OF DAMAGE, REGARDLESS OF THE THEORY OF
 * LIABILITY.
 *
 ************************************************************************/



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
	virtual AAFRESULT UnlinkMobID(aafMobID_constref mobID);

	AAFRESULT LookupEssence (aafMobID_constref fileMobID, ImplAAFEssenceData **ppEssence);
	AAFRESULT ChangeIndexedMobID (ImplAAFMob *pMob, aafMobID_constref newID);

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
