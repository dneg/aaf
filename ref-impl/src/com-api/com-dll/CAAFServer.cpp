

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


