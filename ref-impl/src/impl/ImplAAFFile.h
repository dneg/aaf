//@doc
//@class  AAFFile | Implementation class for AAFFile
#ifndef __ImplAAFFile_h__
#define __ImplAAFFile_h__

/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/

#include "ImplAAFRoot.h"






//
// Forward declaration
//
class OMFile;
class ImplAAFDictionary;
class ImplAAFFile;
class ImplAAFHeader;
class ImplAAFDataDef;

class ImplAAFFile : public ImplAAFRoot
{
public:

  //
  // methods to support API
  //

  virtual AAFRESULT STDMETHODCALLTYPE
	Initialize ();

  virtual AAFRESULT STDMETHODCALLTYPE
	OpenExistingRead (wchar_t * pFileName,
					  aafUInt32 modeFlags);

  virtual AAFRESULT STDMETHODCALLTYPE
	OpenExistingModify (wchar_t * pFileName,
						aafUInt32 modeFlags,
						aafProductIdentification_t * pIdent);

  virtual AAFRESULT STDMETHODCALLTYPE
	OpenNewModify (wchar_t * pFileName,
				   aafUInt32 modeFlags,
				   aafProductIdentification_t * pIdent);

  virtual AAFRESULT STDMETHODCALLTYPE
	OpenTransient (aafProductIdentification_t * pIdent);

  virtual AAFRESULT STDMETHODCALLTYPE
	Close ();

  virtual AAFRESULT STDMETHODCALLTYPE
	Save ();

  virtual AAFRESULT STDMETHODCALLTYPE
	SaveAs (wchar_t * pFileName,
			aafUInt32 modeFlags);

  virtual AAFRESULT STDMETHODCALLTYPE
	Revert ();

  virtual AAFRESULT STDMETHODCALLTYPE
	GetHeader (ImplAAFHeader ** header);

  virtual AAFRESULT STDMETHODCALLTYPE
	GetRevision (aafFileRev_t *  rev);
  

  //
  // Constructor/destructor
  //
  //********
  ImplAAFFile ();
  virtual ~ImplAAFFile ();


public:
  // Declare the module test method. The implementation of the will be be
  // in /test/ImplAAFFileTest.cpp.
  static AAFRESULT test();


private:

  // Private state for this file.
  typedef enum _openType_t
  {
    kOmCreate = 0,
	kOmModify = 1,
	kOmOpenRead = 2,
	kOmTransient = 3,
	kOmUndefined = -1
  } openType_t;
	

  void InternalReleaseObjects();

  aafInt32			_cookie;
  OMFile			*_file;
  ImplAAFDictionary *_factory;
  aafInt16			_byteOrder;
  openType_t			_openType;
  ImplAAFHeader *   _head;		// Needed by Head object
  aafBool   _semanticCheckEnable;	//!!!  /* Used to stop recursion in checks */
  ImplAAFDataDef *	_nilKind;// !!!
  ImplAAFDataDef *	_pictureKind;// !!!
  ImplAAFDataDef *	_soundKind;// !!!
  aafFileRev_t   _setrev;
  aafBool _initialized;
  aafBool _open;
  aafUInt32 _modeFlags;
  aafProductIdentification_t _ident;
};

#endif // ! __ImplAAFFile_h__

