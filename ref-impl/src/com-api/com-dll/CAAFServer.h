

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

