//=---------------------------------------------------------------------=
//
// $Id$ $Name$
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
// The Original Code of this file is Copyright 1998-2004, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

#ifndef __AAFTypes_h__
#include "AAFTypes.h"
#endif

#ifndef __CAAFServer_h__
#include "CAAFServer.h"
#endif


#if defined( OS_WINDOWS )
// Include declarations for InterlockedIncrement() and InterlockcedDecrment().
#include <winbase.h>
#endif

CAAFServer::CAAFServer() :
	_lockCount(0)
{
}

CAAFServer::~CAAFServer()
{
}

void CAAFServer::Lock
(
	aafBool fLock
)
{
	if (fLock)
		InterlockedIncrement(&_lockCount);
	else
		InterlockedDecrement(&_lockCount);
}

// Called by all IUnknown::AddRef implementations in this module
void CAAFServer::IncrementActiveObjects()
{
	InterlockedIncrement(&_activeCount);
}

// Called by all IUnknown::Release implementations in this module
void CAAFServer::DecrementActiveObjects()
{
	InterlockedDecrement(&_activeCount);
}


aafUInt32 CAAFServer::GetLockCount()
{
	return _lockCount;
}
aafUInt32 CAAFServer::GetActiveObjectCount()
{
	return _activeCount;
}


aafUInt32 CAAFServer::InterlockedIncrement(aafUInt32 *value)
{
#if defined( OS_WINDOWS )
	return ::InterlockedIncrement(reinterpret_cast<long *>(value));
#else
	return (++(*value));
#endif
}
aafUInt32 CAAFServer::InterlockedDecrement(aafUInt32 *value)
{
#if defined( OS_WINDOWS )
	return ::InterlockedDecrement(reinterpret_cast<long *>(value));
#else
	return (--(*value));
#endif
}


