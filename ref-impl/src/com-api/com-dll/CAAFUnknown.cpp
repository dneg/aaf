//=---------------------------------------------------------------------=
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
// The Original Code of this file is Copyright 1998-2001, Licensor of the
// AAF Association.
// 
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

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
#include <string.h>

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
) : 
	m_cRef(0), 
	m_pUnkOuter(NULL)
{
	m_pUnkOuter = (pUnkOuter) ? pUnkOuter : GetPrivateUnknown();
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

    // We only support the IID_IUnknown interface 
    //
    // Hack: We have to use aafIsEqualIID from AAFUtils.cpp here,
    // but since this file is linked with some external apps
    // let's use memcmp until we have user utilities library. - Alex,4-Oct-00
    if (memcmp((void*)&riid, (void*)&IID_IUnknown, sizeof(IID)) == 0)
    { 
        *ppvObjOut = static_cast<IUnknown *>(GetPrivateUnknown()); 
        ((IUnknown *)*ppvObjOut)->AddRef();
        return S_OK;
    }

    return E_NOINTERFACE;
}


//=--------------------------------------------------------------------------=
// CAAFUnknown::InternalAddRef
//=--------------------------------------------------------------------------=
// @mfunc
// Adds a tick to the current reference count.
//
// @rdesc
// The new reference count
//
ULONG CAAFUnknown::InternalAddRef
(
	void
)
{
    return CAAFServer::InterlockedIncrement(&m_cRef);
}

//=--------------------------------------------------------------------------=
// CAAFUnknown::InternalRelease
//=--------------------------------------------------------------------------=
// @mfunc
// Removes a tick from the count, and delets the object if necessary
//
// @rdesc
// Remaining refs
//
// Notes:
//
ULONG CAAFUnknown::InternalRelease
(
    void
)
{
    ULONG cRef = CAAFServer::InterlockedDecrement(&m_cRef);
    if (0 == m_cRef)
        delete this;
    return cRef;
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

