#ifndef __ImplAAFContext_h__
#define __ImplAAFContext_h__

//=---------------------------------------------------------------------=
//
// $Id$ $Name$
//
// The contents of this file are subject to the AAF SDK Public Source
// License Agreement Version 2.0 (the "License"); You may not use this
// file except in compliance with the License.  The License is available
// in AAFSDKPSL.TXT, or you may obtain a copy of the License from the
// Advanced Media Workflow Association, Inc., or its successor.
//
// Software distributed under the License is distributed on an "AS IS"
// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
// the License for the specific language governing rights and limitations
// under the License.  Refer to Section 3.3 of the License for proper use
// of this Exhibit.
//
// WARNING:  Please contact the Advanced Media Workflow Association,
// Inc., for more information about any additional licenses to
// intellectual property covering the AAF Standard that may be required
// to create and distribute AAF compliant products.
// (http://www.amwa.tv/policies).
//
// Copyright Notices:
// The Original Code of this file is Copyright 1998-2009, licensor of the
// Advanced Media Workflow Association.  All rights reserved.
//
// The Initial Developer of the Original Code of this file and the
// licensor of the Advanced Media Workflow Association is
// Avid Technology.
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
struct IAAFProgress;

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

  // Pass in NULL to revert to no-callback
  HRESULT SetProgressCallback(IAAFProgress * pProgress);

  // Succeeds, returning NULL if no calback
  HRESULT GetProgressCallback(IAAFProgress ** ppProgress);
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
  IAAFProgress			*_progressCallback;

};

#endif // ! __ImplAAFContext_h__

