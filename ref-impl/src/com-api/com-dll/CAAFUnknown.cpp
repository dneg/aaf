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



#ifndef __AAFTypes_h__
#include "AAFTypes.h"
#endif

#include "CAAFUnknown.h"
#include "CAAFServer.h"
#include <stddef.h>

#if defined(DONT_DELETE_LAST_REFERENCE)
#include <iostream.h>
#endif

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
    return CAAFServer::InterlockedIncrement(&m_cRef);
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
    ULONG cRef = CAAFServer::InterlockedDecrement(&m_cRef);
#if !defined(DONT_DELETE_LAST_REFERENCE)
    if (0 == m_cRef)
        delete This();
#else
    if (0 > (long)m_cRef)
    {
      cerr << This() << " Error: Releasing an object with reference count:" << (long)(m_cRef) << endl;
    }
#endif

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
    REFIID  /*riid*/,			// @parm [in] interface they want
    void  **ppvObjOut		// @parm [out] where they want to put the resulting object ptr.
)
{
    *ppvObjOut = NULL;

    return E_NOINTERFACE;
}

