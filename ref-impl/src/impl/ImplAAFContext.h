//@doc
//@class    AAFContext | Implementation class for AAFContext
#ifndef __ImplAAFContext_h__
#define __ImplAAFContext_h__

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

  static void DeleteInstance(void);

public:

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

};

#endif // ! __ImplAAFContext_h__

