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



#include "AxPlugin.h"

#include "AxPluginRegistry.h"
#include "CAxClassFactory.h"
#include "CAxUnknown.h"

bool AxPlugin::CanUnloadNow()
{
	// CanUnload if:
	//	- no CAxUnknown objects still exist
	//	- no factory objects still exist
	//	- the lock count is zero

	if ( 0 == CAxUnknown::GetActiveComponents() &&
		 0 == CAxClassFactoryCounters::GetInstanceCount() &&
		 0 == CAxClassFactoryCounters::GetLockCount() ) {
		return true;
	}

	return false;
}

unsigned int AxPlugin::GetClassCount()
{
	return AxPluginRegistry::GetInstance().GetClassCount();
}

HRESULT AxPlugin::GetClassObjectID(ULONG index, CLSID *pClassID)
{
	return AxPluginRegistry::GetInstance().GetClassObjectID( index, pClassID );
}

HRESULT AxPlugin::GetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
	return AxPluginRegistry::GetInstance().GetClassObject( rclsid, riid, ppv );
}
