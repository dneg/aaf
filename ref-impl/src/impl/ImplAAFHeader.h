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

#if OM_PRESENT
#include "aafErr.h"
#include "ImplAAFObject.h"
#include "ImplAAFSession.h"
#include "ImplAAFIdentification.h"

#include "OMProperty.h"

class AAFDataKind;
class AAFEffectDef;

const int CLSID_AAFHEADER = 43;

const int PID_HEADER_BYTEORDER          = 0;
const int PID_HEADER_LASTMODIFIED       = 1;
const int PID_HEADER_IDENTIFICATIONLIST = 2;
#endif

class ImplAAFHeader : public ImplAAFObject
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFHeader ();
  ~ImplAAFHeader ();


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
  // GetIdentification()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetIdentification
        (aafUID_t *  pGeneration,   //@parm [in,ref] Unique Generation ID
		 ImplAAFIdentification ** ppIdentification);  //@parm [out,retval] Indentification Object


  //****************
  // GetIdentificationList()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetIdentificationList
        (ImplEnumAAFIdentifications ** ppEnum);  //@parm [out,retval] Indentification Enumeration



  //****************
  // GetToolkitVersion()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetToolkitVersion
        (aafProductVersion_t *  pToolkitVersion);  //@parm [out,retval] The Toolkit Version


  //****************
  // GetFileRevision()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetFileRevision
        (aafVersionType_t *  pRevision);  //@parm [out,retval] The Toolkit Version

	
  //****************
  // GetByteOrder()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetByteOrder
        (aafInt16 *  pByteOrder);  //@parm [out,retval] The Byte order


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

	aafBool IsMediaDataPresent( 	aafUID_t				fileMobUid,	/* IN -- */
									aafFileFormat_t	fmt);
	AAFRESULT AppendDataObject(aafUID_t mobID,      /* IN - Mob ID */
						  AAFObject *dataObj) ;    /* IN - Input Mob */ 

AAFRESULT SetToolkitRevisionCurrent(void);
AAFRESULT IsValidHeadObject(void);

AAFRESULT GetNumIdentifications (aafInt32 * /*pCount*/);

AAFRESULT AddIdentificationObject (aafProductIdentification_t * /*pIdent*/);
AAFRESULT BuildMediaCache(void);
AAFRESULT LoadMobTables(void);

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
virtual int classId(void) const;

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

#if OM_PRESENT
		// Persistent properties
    //
		OMFixedSizeProperty<aafInt16>                      _byteOrder;
		OMFixedSizeProperty<aafTimeStamp_t>                _lastModified;
    OMStrongReferenceVectorProperty<AAFIdentification> _identificationList;
#else
		aafInt16	_byteOrder;
#endif
};

#endif // ! __ImplAAFHeader_h__
