

#ifndef __aaftypes_h__
#include "aaftypes.h"
#endif

#ifndef __CAAFServer_h__
#include "CAAFServer.h"
#endif


#if defined(WIN32)
#include <windows.h>
#else
inline long InterlockedIncrement(long *value)
{
	return (++(*value));
}
inline long InterlockedDecrement(long *value)
{
	return (--(*value));
}
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
	AAFBool fLock
)
{
	if (fLock)
		InterlockedIncrement(reinterpret_cast<long *>(&_lockCount));
	else
		InterlockedDecrement(reinterpret_cast<long *>(&_lockCount));
}

// Called by all IUnknown::AddRef implementations in this module
void CAAFServer::IncrementActiveObjects()
{
	InterlockedIncrement(reinterpret_cast<long *>(&_activeCount));
}

// Called by all IUnknown::Release implementations in this module
void CAAFServer::DecrementActiveObjects()
{
	InterlockedDecrement(reinterpret_cast<long *>(&_activeCount));
}


AAFUInt32 CAAFServer::GetLockCount()
{
	return _lockCount;
}
AAFUInt32 CAAFServer::GetActiveObjectCount()
{
	return _activeCount;
}


