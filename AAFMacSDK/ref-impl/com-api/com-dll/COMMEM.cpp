#include <new.h>
#include <compobj.h>
#include <iostream.h>


// Helper class.
class ComMem
{
public:
  ComMem() :
    _old_new_handler(0),
    _pMalloc(0),
    _count(0),
    _bytes_allocated(0)    
  {
  	// Install a new handler.
  	_old_new_handler = set_new_handler(NewHandler);		

  	CoGetMalloc(MEMCTX_TASK, &_pMalloc);
  	//CoGetMalloc(MEMCTX_SHARED, &_pMalloc);
  }
  
  ~ComMem()
  {
  	if (_pMalloc)
  	{
  	  _pMalloc->Release();
  	  _pMalloc = 0;
  	} 
  	
   	// Restore the old handler.
  	set_new_handler(_old_new_handler);		
  }

  void * Alloc(size_t nSize);
  void Free(void *pv);
  
  static void NewHandler();
  
  new_handler _old_new_handler;
  IMalloc *_pMalloc;
  unsigned long _count;
  unsigned long _bytes_allocated;
};

void * ComMem::Alloc(size_t nSize)
{
	++_count;
	_bytes_allocated += nSize;
	
	if (_pMalloc)
		return _pMalloc->Alloc(nSize);
	else
		return CoTaskMemAlloc(nSize);
}

void ComMem::Free(void *pv)
{
	--_count;

	if (_pMalloc)
	{
		if (!_pMalloc->DidAlloc(pv))
			cerr << "Attempting to delete non-allocated pointer!." << endl;

		_pMalloc->Free(pv);
	}
	else
		CoTaskMemFree(pv);
}

void ComMem::NewHandler()
{
	throw bad_alloc();
}


#if !defined(__cdecl) && !defined(MSC_VER)
#define __cdecl
#endif

// Create our global allocator object.
ComMem g_ComMem;

//#define USE_GLOBAL_NEW

#if defined(USE_GLOBAL_NEW)

void * __cdecl operator new(size_t nSize)
{
    return g_ComMem.Alloc(nSize);
}

void __cdecl operator delete(void *pv)
{
    g_ComMem.Free(pv);
}
#endif

int __cdecl _purecall(void);
int __cdecl _purecall(void)
{
#ifdef _DEBUG
    Debugger();
#endif
    return 0;
}
