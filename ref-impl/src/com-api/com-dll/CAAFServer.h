

#ifndef __CAAFServer_h__
#define __CAAFServer_h__


class CAAFServer
{
protected:
	CAAFServer();

public:
	virtual ~CAAFServer();

	void Lock(AAFBool fLock);
	void IncrementActiveObjects();
	void DecrementActiveObjects();

	AAFUInt32 GetLockCount();
	AAFUInt32 GetActiveObjectCount();

protected:
	AAFUInt32 _lockCount;
	AAFUInt32 _activeCount;
};

extern CAAFServer* g_pAAFServer;

#endif // __CAAFServer_h__

