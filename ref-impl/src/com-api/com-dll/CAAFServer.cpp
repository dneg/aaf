/***********************************************************************
*
*              Copyright (c) 1998-1999 Avid Technology, Inc.
*
* Permission to use, copy and modify this software and accompanying
* documentation, and to distribute and sublicense application software
* incorporating this software for any purpose is hereby granted,
* provided that (i) the above copyright notice and this permission
* notice appear in all copies of the software and related documentation,
* and (ii) the name Avid Technology, Inc. may not be used in any
* advertising or publicity relating to the software without the specific,
*  prior written permission of Avid Technology, Inc.
*
* THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND,
* EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY
* WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
* IN NO EVENT SHALL AVID TECHNOLOGY, INC. BE LIABLE FOR ANY DIRECT,
* SPECIAL, INCIDENTAL, PUNITIVE, INDIRECT, ECONOMIC, CONSEQUENTIAL OR
* OTHER DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER ARISING OUT OF
* OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE AND
* ACCOMPANYING DOCUMENTATION, INCLUDING, WITHOUT LIMITATION, DAMAGES
* RESULTING FROM LOSS OF USE, DATA OR PROFITS, AND WHETHER OR NOT
* ADVISED OF THE POSSIBILITY OF DAMAGE, REGARDLESS OF THE THEORY OF
* LIABILITY.
*
************************************************************************/

#ifndef __AAFTypes_h__
#include "AAFTypes.h"
#endif

#ifndef __CAAFServer_h__
#include "CAAFServer.h"
#endif


#if defined(WIN32)
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
#if defined(WIN32)
	return ::InterlockedIncrement(reinterpret_cast<long *>(value));
#else
	return (++(*value));
#endif
}
aafUInt32 CAAFServer::InterlockedDecrement(aafUInt32 *value)
{
#if defined(WIN32)
	return ::InterlockedDecrement(reinterpret_cast<long *>(value));
#else
	return (--(*value));
#endif
}


