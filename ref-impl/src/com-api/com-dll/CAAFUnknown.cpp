//=--------------------------------------------------------------------------=
// @doc
//
// @module AAFUnknown.cpp |
// 
// Implementation for various things in the unknown object that supports
// aggregation.
//
// @end
//=--------------------------------------------------------------------------=
// Copyright 1995 - 1998 Microsoft Corporation.  All Rights Reserved.
//
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF 
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO 
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A 
// PARTICULAR PURPOSE.
//=--------------------------------------------------------------------------=
//
//
#ifndef __AAFTypes_h__
#include "AAFTypes.h"
#endif

#include "CAAFUnknown.h"
#include "CAAFServer.h"
#include <stddef.h>


//=--------------------------------------------------------------------------=
// CAAFUnknown::CAAFUnknown
//=--------------------------------------------------------------------------=
// @mfunc
// Constructor 
//
// @comm
// If the given outer unknown is null then use the private non-delegating
// implementation. This method also increments the active COM object count
// so that COM server will not attempt to unload (see <f DllCanUnloadNow>).
//
CAAFUnknown::CAAFUnknown
(
 IUnknown *pUnkOuter	// @parm Controlling unknown from <om IClassFactory.CreateInstance>, 
						//		  may be null.
) 
	: m_pUnkOuter((pUnkOuter) ? pUnkOuter : &m_UnkPrivate)
{
	g_pAAFServer->IncrementActiveObjects();
}

//=--------------------------------------------------------------------------=
// CAAFUnknown::~CAAFUnknown
//=--------------------------------------------------------------------------=
// @mfunc
// Destructor
//
// @comm
// Decrements the active COM object count so that the COM server can unload if 
// the count is zero see <f DllCanUnloadNow>)
CAAFUnknown::~CAAFUnknown()
{
	g_pAAFServer->DecrementActiveObjects();
}



//=--------------------------------------------------------------------------=
// CAAFUnknown::CAAFPrivateUnknown::This()
//=--------------------------------------------------------------------------=
// @mfunc CAAFUnknown::CAAFPrivateUnknown::This() |
// Return pointer to outer object's this pointer.
//
// @comm
// This method is used when we're sitting in the private unknown object, and we 
// need to get at the pointer for the main unknown. It is slightly better to do 
// this pointer arithmetic than have to store a pointer to the parent.
//
inline CAAFUnknown *CAAFUnknown::CAAFPrivateUnknown::This
(
    void
)
{
    return (CAAFUnknown *)((LPBYTE)this - offsetof(CAAFUnknown, m_UnkPrivate));
}

//=--------------------------------------------------------------------------=
// CAAFUnknown::CAAFPrivateUnknown::QueryInterface
//=--------------------------------------------------------------------------=
// @mfunc
// This is the non-delegating internal QI routine.
//
// @rdesc
// Returns one of the following values:
//
// @flag S_OK |
// If objects supports the requested interface.
//
// @flag E_NOINTERFACE  | 
// If object does not implement the requeste interface.
//
// @comm
// Since <c CAAFPrivateUnknown> only implements <i IUnknown> methods all other 
// interface requests are delegated to the virtual method <mf CAAFUnknown::InternalQueryInteface> 
// which must be overridden by every derived that implements a new interface. 
//
// @devnote
// Remember to call the base class's implementation of InternalQueryInterface.	
//
STDMETHODIMP CAAFUnknown::CAAFPrivateUnknown::QueryInterface
(
    REFIID riid,			// @parm [in] interface they want
    void **ppvObjOut		// @parm [out] where they want to put the resulting object ptr.
)
{
    if (NULL == ppvObjOut)
		return E_INVALIDARG;

    // if they're asking for IUnknown, then we have to pass them ourselves.
    // otherwise defer to the inheriting object's InternalQueryInterface
    //
    if (IsEqualIID(riid, IID_IUnknown))
	{
        AddRef();
        *ppvObjOut = (IUnknown *)this;
        return S_OK;
    } 
	else
	{
        return This()->InternalQueryInterface(riid, ppvObjOut);
	}

    // dead code    
}

//=--------------------------------------------------------------------------=
// CAAFUnknown::CAAFPrivateUnknown::AddRef
//=--------------------------------------------------------------------------=
// @mfunc
// Adds a tick to the current reference count.
//
// @rdesc
// The new reference count
//
ULONG CAAFUnknown::CAAFPrivateUnknown::AddRef
(
	void
)
{
    return InterlockedIncrement(reinterpret_cast<long *>(&m_cRef));
}

//=--------------------------------------------------------------------------=
// CAAFUnknown::CAAFPrivateUnknown::Release
//=--------------------------------------------------------------------------=
// @mfunc
// Removes a tick from the count, and delets the object if necessary
//
// @rdesc
// Remaining refs
//
// Notes:
//
ULONG CAAFUnknown::CAAFPrivateUnknown::Release
(
    void
)
{
    ULONG cRef = InterlockedDecrement(reinterpret_cast<long *>(&m_cRef));

    if (0 == m_cRef)
        delete This();

    return cRef;
}


//=--------------------------------------------------------------------------=
// CAAFUnknown::InternalQueryInterface
//=--------------------------------------------------------------------------=
// @mfunc
// Objects that are aggregated use this to support additional interfaces.
// they should call this method on their parent so that any of it's interfaces
// are queried.

// @rdesc
// Returns one of the following values:
//
// @flag S_OK | 
//	If objects supports the requested interface.
// @flag E_NOINTERFACE  | 
//	If object does not implement the requeste interface.
//
HRESULT CAAFUnknown::InternalQueryInterface
(
    REFIID  riid,			// @parm [in] interface they want
    void  **ppvObjOut		// @parm [out] where they want to put the resulting object ptr.
)
{
    *ppvObjOut = NULL;

    return E_NOINTERFACE;
}

