//@doc
//@class    AAFFile | Implementation class for AAFFile
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

#include "AAFTypes.h"
#include "ImplAAFRoot.h"

#include "Container.h"

typedef enum
{
	kOmCreate, kOmModify, kOmOpenRead, kOmUndefined = -1
}				openType_t;

//
// Forward declaration
//
struct IAAFFile;
class AAFFile;
struct IAAFMedia;
class AAFMedia;
class ImplAAFHeader;
struct IAAFSession;
class ImplAAFSession;
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
	OpenTransient ();

  virtual AAFRESULT STDMETHODCALLTYPE
	Close ();

  virtual AAFRESULT STDMETHODCALLTYPE
	Save ();

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
#if 0
  OMDECLARE_STORABLE(ImplAAFFile)
#endif


public:
  // Declare the module test method. The implementation of the will be be
  // in /test/ImplAAFFileTest.cpp.
  static AAFRESULT test();

AAFRESULT Create(
			aafWChar*		stream, 
			ImplAAFSession *	session, 
			aafFileRev_t		rev);
AAFRESULT OpenRead(
			aafWChar*		stream, 
			ImplAAFSession *	session);
AAFRESULT OpenModify(
			aafWChar*		stream, 
			ImplAAFSession *	session);
private:

	AAFRESULT InternOpenFile(aafWChar* stream, 
								   ImplAAFSession * session,
								   OMLContainerUseMode useMode, 
								   openType_t type);
  
  void InternalReleaseObjects();

		aafInt32       	_cookie;
		aafFileFormat_t _fmt;
		OMContainer     *_container;
		aafInt16           _byteOrder;
		openType_t		_openType;
		ImplAAFFile			*_prevFile;
		ImplAAFHeader *     _head;		// Needed by Head object
#if FULL_TOOLKIT
		aafCloseMediaPtr _closeMediaProc;
		aafBool			_customStreamFuncsExist;	//!!!	
		struct aafCodecStreamFuncs _streamFuncs;	//!!!	
		aafRawStream_t	*_rawFile;	//!!!	
		aafLocatorFailureCB _locatorFailureCallback;	//!!!	
		aafCodecID_t	_rawCodecID;	//!!!	
		void           	*_rawFileDesc;		//!!!/* If non-omfi file */
		AAFMedia *   _topMedia;	//!!!
#endif
		aafBool         _semanticCheckEnable;	//!!!  /* Used to stop recursion in checks */
		ImplAAFSession		 *_session;		//!!!  used by file checker
		ImplAAFDataDef *	_nilKind;// !!!
		ImplAAFDataDef *	_pictureKind;// !!!
		ImplAAFDataDef *	_soundKind;// !!!
#ifdef AAF_ERROR_TRACE
		char			*_stackTrace;	//!!!	
		aafInt32		_stackTraceSize;	//!!!	
#endif
		aafFileRev_t     _setrev;

  aafBool _initialized;
  aafBool _open;
  aafProductIdentification_t _ident;
};

#endif // ! __ImplAAFFile_h__

