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
	kOmCreate, kOmModify, kOmOpenRead
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
class ImplAAFDataDefinition;

class ImplAAFFile : public ImplAAFRoot
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFFile ();
  virtual ~ImplAAFFile ();

  //***********************************************************
  // METHOD NAME: Close()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFFile | Close |
  // Closes an AAF file, saving the result.
  // @end
  virtual AAFRESULT STDMETHODCALLTYPE
    Close ();

  //***********************************************************
  // METHOD NAME: GetRevision()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFFile | GetRevision |
  // Get the revision of the ciurrent AAF file.
  // @end
  // 
  virtual AAFRESULT STDMETHODCALLTYPE
  GetRevision (
    // @parm aafFileRev_t * | rev | [out] Revision of the current file
    aafFileRev_t *  rev
  );

public:
  // Declare the module test method. The implementation of the will be be
  // in /test/ImplAAFFileTest.cpp.
  static AAFRESULT test();

AAFRESULT Create(
			aafDataBuffer_t		stream, 
			ImplAAFSession *	session, 
			aafFileRev_t		rev);
AAFRESULT OpenRead(
			aafDataBuffer_t		stream, 
			ImplAAFSession *	session);
AAFRESULT OpenModify(
			aafDataBuffer_t		stream, 
			ImplAAFSession *	session);
private:

	AAFRESULT InternOpenFile(aafDataBuffer_t stream, 
								   ImplAAFSession * session,
								   OMLContainerUseMode useMode, 
								   openType_t type);

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
		ImplAAFDataDefinition *	_nilKind;// !!!
		ImplAAFDataDefinition *	_pictureKind;// !!!
		ImplAAFDataDefinition *	_soundKind;// !!!
#ifdef AAF_ERROR_TRACE
		char			*_stackTrace;	//!!!	
		aafInt32		_stackTraceSize;	//!!!	
#endif
		aafFileRev_t     _setrev;
};

#endif // ! __ImplAAFFile_h__

