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

#include "AAFTypes.h"
#include "ImplAAFRoot.h"


//
// Forward declaration
//
class ImplAAFFile;
class ImplAAFPluginManager;


class ImplAAFSession : public ImplAAFRoot
{
public:

  //****************
  // GetInstance()
  //
  // Allows clients to get the single instance of this class.
  //
  static ImplAAFSession * GetInstance ();


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
        (aafWChar *  filePath,   //@parm [in] File path [replace with object later]
		 aafFileRev_t  rev,   //@parm [in] File revision to create
         ImplAAFFile ** file);  //@parm [out] Current AAF file


  //****************
  // OpenReadFile()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    OpenReadFile
        (aafWChar *  filePath,   //@parm [in] File path [replace with object later]
		 ImplAAFFile ** file);  //@parm [out] Current AAF file


  //****************
  // OpenModifyFile()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    OpenModifyFile
        (aafWChar *  filePath,   //@parm [in] File path [replace with object later]
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

  virtual AAFRESULT STDMETHODCALLTYPE
  BeginSession (
    aafProductIdentification_t  *ident
  );


public:
  // Declare the module test method. The implementation of the will be be
  // in /test/ImplAAFSessionTest.cpp.
  static AAFRESULT test();

  ImplAAFFile *GetTopFile(void);
  void SetTopFile(ImplAAFFile *file);
  aafProductIdentification_t *GetDefaultIdent(void);
  void InitPluginManager (void);
  ImplAAFPluginManager *GetPluginManager (void);

private:

  //
  // Constructor/destructor
  //
  //********
  ImplAAFSession ();
  virtual ~ImplAAFSession ();

  // single instance of this class
  static ImplAAFSession * _singleton;

  ImplAAFFile	*_topFile;
  aafProductIdentification_t	*_defaultIdent;
  class ImplAAFPluginManager	*_plugins;

};

#endif // ! __ImplAAFSession_h__

