//@doc
//@class    AAFSession | Implementation class for AAFSession
#ifndef __ImplAAFSession_h__
#define __ImplAAFSession_h__

/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/

#ifndef __AAFTypes_h__
#include "AAFTypes.h"
#endif

#ifndef __ImplAAFRoot_h__
#include "AAFRoot.h"
#endif

class ImplAAFFile;
class AAFFile;

#include "Container.h"

//
// Forward declaration
//
struct IAAFSession;
class AAFSession;


class ImplAAFSession : public ImplAAFRoot
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFSession ();
  virtual ~ImplAAFSession ();




  //****************
  // EndSession()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    EndSession ();


  //****************
  // CreateFile()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    CreateFile
        (aafDataBuffer_t  filePath,   //@parm [in] File path [replace with object later]
		 aafFileRev_t  rev,   //@parm [in] File revision to create
         ImplAAFFile ** file);  //@parm [out] Current AAF file


  //****************
  // OpenReadFile()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    OpenReadFile
        (aafDataBuffer_t  filePath,   //@parm [in] File path [replace with object later]
		 ImplAAFFile ** file);  //@parm [out] Current AAF file


  //****************
  // OpenModifyFile()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    OpenModifyFile
        (aafDataBuffer_t  filePath,   //@parm [in] File path [replace with object later]
		 ImplAAFFile ** file);  //@parm [out] Current AAF file

  //***********************************************************
  // METHOD NAME: SetCurrentIdentification()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFSession | SetCurrentIdentification |
  // Sets the object which identifies the creator of the file.
  // @end
  // 
  virtual AAFRESULT STDMETHODCALLTYPE
  SetDefaultIdentification (
    // @parm aafProductIdentification | ident | [in] a struct from which it is initialized
    aafProductIdentification_t  *ident
  );


  //***********************************************************
  // METHOD NAME: GetIdentification()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
  GetIdentification (
    aafProductIdentification_t  *ident
  );

public:
  // Declare the module test method. The implementation of the will be be
  // in /test/ImplAAFSessionTest.cpp.
  static AAFRESULT test();

  void InitContainer (void * pContainer);

  void * GetContainer ();

	ImplAAFFile *GetTopFile(void);
	void SetTopFile(ImplAAFFile *file);
	OMLSession	GetContainerSession(void);
	aafProductIdentification_t *GetDefaultIdent(void);

private:

	void * _pContainer;
	ImplAAFFile	*_topFile;
	aafProductIdentification_t	*_defaultIdent;
};

#endif // ! __ImplAAFSession_h__

