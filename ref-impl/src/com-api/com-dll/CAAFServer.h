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

#ifndef __CAAFServer_h__
#define __CAAFServer_h__

#ifndef __AAFTypes_h__
#include "AAFTypes.h"
#endif


class CAAFServer
{
protected:
	CAAFServer();

public:
	virtual ~CAAFServer();

	void Lock(aafBool fLock);
	void IncrementActiveObjects();
	void DecrementActiveObjects();

	aafUInt32 GetLockCount();
	aafUInt32 GetActiveObjectCount();

	static aafUInt32 InterlockedIncrement(aafUInt32 *value);
	static aafUInt32 InterlockedDecrement(aafUInt32 *value);

protected:
	aafUInt32 _lockCount;
	aafUInt32 _activeCount;
};

extern CAAFServer* g_pAAFServer;

#endif // __CAAFServer_h__

