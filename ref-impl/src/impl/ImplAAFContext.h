//@doc
//@class    AAFContext | Implementation class for AAFContext
#ifndef __ImplAAFContext_h__
#define __ImplAAFContext_h__

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


class ImplAAFContext : public ImplAAFRoot
{
public:

  //****************
  // GetInstance()
  //
  // Allows clients to get the single instance of this class.
  //
  static ImplAAFContext * GetInstance ();


public:
  // Declare the module test method. The implementation of the will be be
  // in /test/ImplAAFContextTest.cpp.
  static AAFRESULT test();


  void InitPluginManager (void);
  ImplAAFPluginManager *GetPluginManager (void);

private:

  //
  // Constructor/destructor
  //
  //********
  ImplAAFContext ();
  virtual ~ImplAAFContext ();

  // single instance of this class
  static ImplAAFContext * _singleton;

  ImplAAFPluginManager	*_plugins;

  // Make private helper class a friend so that it
  // may call the destructor. This helper class is used
  // to ensure that the singleton context instance is 
  // cleaned up properly.
  friend class ImplAAFContextHelper;
};

#endif // ! __ImplAAFContext_h__

