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
//=---------------------------------------------------------------------=

// Credits:
//
// The CAxUnknown implementation is a slighly modified version
// of the IUnknown implementation presented in:
// "Inside COM", Dale Rogerson, Microsoft Press
//
// The CAxUnknown class methods have been defined using the AAF SDK
// STDMETHODCALLTYPE macro, and the windows specific InterlockedDecrement and
// InterlockedIncrement call are not used.  Other than that, it is
// identical to the code presented in the "Inside COM" book.
//
// Concerning the non-use of Interlocked{Decrement,Increment} calls by this code:
// these Windows functions (macros?..) provide thread safe access to the static instance
// count, and per object referenct count.  The AAF SDK, is not thread safe hence an AAF 
// plugin COM object should never be accessed by multiple threads. Hence, there is no need
// to introduce platform dependent atomic counter access code.

///////////////////////////////////////////////////////////
//
// CAxUnknown.cpp 
//   - Implementation of IUnknown base class
//
#include "CAxUnknown.h"

///////////////////////////////////////////////////////////
//
// Count of active objects
//   - Use to determine if we can unload the DLL.
//
long CAxUnknown::s_cActiveComponents = 0 ;

///////////////////////////////////////////////////////////
//
// Constructor
//
CAxUnknown::CAxUnknown(IUnknown* pUnknownOuter)
: m_cRef(1)
{
	// Set m_pUnknownOuter pointer.
	if (pUnknownOuter == NULL)
	{
		m_pUnknownOuter = reinterpret_cast<IUnknown*>
		                     (static_cast<INondelegatingUnknown*>
		                     (this)) ;  // notice cast
	}
	else
	{
		m_pUnknownOuter = pUnknownOuter ;
	}

	// Increment count of active components.
	s_cActiveComponents++;
}

//
// Destructor
//
CAxUnknown::~CAxUnknown()
{
	s_cActiveComponents--;
}

//
// FinalRelease - called by Release before it deletes the component
//
void CAxUnknown::FinalRelease()
{
	m_cRef = 1 ;
}

//
// Nondelegating IUnknown
//   - Override to handle custom interfaces.
//
HRESULT STDMETHODCALLTYPE
	CAxUnknown::NondelegatingQueryInterface(const IID& iid, void** ppv)
{
	// CAxUnknown supports only IUnknown.
	if (iid == IID_IUnknown)
	{
		return FinishQI(reinterpret_cast<IUnknown*>
		                   (static_cast<INondelegatingUnknown*>(this)), ppv) ;
	}	
	else
	{
		*ppv = NULL ;
		return E_NOINTERFACE ;
	}
}

//
// AddRef
//
ULONG STDMETHODCALLTYPE CAxUnknown::NondelegatingAddRef()
{
	m_cRef++;
	return m_cRef;
}

//
// Release
//
ULONG STDMETHODCALLTYPE CAxUnknown::NondelegatingRelease()
{
	m_cRef--;
	if (m_cRef == 0)
	{
		FinalRelease() ;
		delete this ;
		return 0 ;
	}
	return m_cRef ;
}

//
// FinishQI
//   - Helper function to simplify overriding
//     NondelegatingQueryInterface
//
HRESULT CAxUnknown::FinishQI(IUnknown* pI, void** ppv) 
{
	*ppv = pI ;
	pI->AddRef() ;
	return S_OK ;
}

