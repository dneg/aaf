//@doc
//@class    AAFHeader | Implementation class for AAFHeader
#ifndef __ImplAAFHeader_h__
#define __ImplAAFHeader_h__

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

  //****************
  // LookupMob()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    LookupMob
        (const aafUID_t & mobID,   //@parm [in,ref] The Mob ID
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
		 const aafUID_t & fileMobID,

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
  // AAFRESULT CountIdentifications (aafInt32 * /*pCount*/);

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

//
// smart pointer
//

#ifndef __ImplAAFSmartPointer_h__
// caution! includes assert.h
#include "ImplAAFSmartPointer.h"
#endif

typedef ImplAAFSmartPointer<ImplAAFHeader> ImplAAFHeaderSP;

#endif // ! __ImplAAFHeader_h__
