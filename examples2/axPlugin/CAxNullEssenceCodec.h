#ifndef __CAxUnknownCodec_h__
#define __CAxUnknownCodec_h__

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

#include "CAxUnknown.h"
#include "CAxPlugin.h"
#include "CAxEssenceCodec.h"

#include "AxImplNullEssenceCodec.h"
#include "AxImplPlugin.h"
#include "AxPlugin.h"

// CAxNullEssenceCodec - sample implementation of an AAF Plugin.
//
// The IUknown implementation is provided by CAxUnknown.
//
// The IAAFPlugin implementation is provided by CAxPlugin and AxImplPlugin.
// CAxPlugin implements the IAAFPlugin COM interface by wrapping an instance of
// AxImplPlugin.  CAxPlugin catches exceptions and returns HRESULT's to the caller.
// AxImplPlugin has a set of methods that match CAxPlugin.  However, these methods al
// use a void return type.  AxImplPlugin reports routine errors by throwing AxExHResult.
// The CAxPlugin and AxImplPlugins are generic and resusable.  The CAxNullEssenceCodec
// illustrates how to initialize AxImplePlugin.
//
// CAxEssenceCodec implements the IAAFEssenceCodec COM interface by wrapping an instance
// AxImplEssenceCodec.  CAxEssenceCodec catches exceptions and returns HRESULT's to the
// caller.  CAxEssenceCodec is generic and reusable.
// AxImplNullEssenceCodec has a set of methods that match CAxEssenceCodec except they all
// have a void return type.  AxImplNullEssenceCodec is an example codec implementation
// that is not reusable without modification.

class CAxNullEssenceCodec
: public CAxEssenceCodec<AxImplNullEssenceCodec>,
  public CAxPlugin<AxImplPlugin>,
  public CAxUnknown
{
public:

	CAxNullEssenceCodec( IUnknown * pUnkOuter );
	virtual ~CAxNullEssenceCodec();

	CAXUNKNOWN_DECLARE_IUNKNOWN_METHODS
	
	// Override CAxUnknown::NondelegatingQueryInterface() in order to add
	// support for the interfaces supported by this class.
	STDMETHOD( NondelegatingQueryInterface(const IID& iid, void** ppv) );
};

#endif
