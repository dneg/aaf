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
//=---------------------------------------------------------------------=

#include "AxInit.h"

#include "AxSmartPointer.h"
#include "AxEx.h"
#include "AxHrMap.h"
#include "AxPluginMgr.h"

#include <AAF.h>

#include <stdlib.h>

bool AxInit::once = false;

AxInit::AxInit()
{
  Init( getenv("AX_AAF_COMAPI") );
}


AxInit::AxInit(const char* dllname)
{
  Init( dllname );
}

void AxInit::Init( const char* dllname )
{  
  if ( once ) {
    throw AxEx( L"Multiple AxInit instances not permited." );
  }
  
  // Force the AxHrMap singleton to init so that it doesn't happen
  // the first time an exception is handled.
  AxHrMap::getInstance();
  
  CHECK_HRESULT( AAFLoad( dllname ) );
  
  AxPluginMgr mgr;
  mgr.RegisterSharedPlugins();
  
  once = true;
}

AxInit::~AxInit()
{
	AAFUnload();

	once = false;
}
