//=---------------------------------------------------------------------=
//
// $Id$ $Name$
//
// The contents of this file are subject to the AAF SDK Public Source
// License Agreement Version 2.0 (the "License"); You may not use this
// file except in compliance with the License.  The License is available
// in AAFSDKPSL.TXT, or you may obtain a copy of the License from the
// Advanced Media Workflow Association, Inc., or its successor.
//
// Software distributed under the License is distributed on an "AS IS"
// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
// the License for the specific language governing rights and limitations
// under the License.  Refer to Section 3.3 of the License for proper use
// of this Exhibit.
//
// WARNING:  Please contact the Advanced Media Workflow Association,
// Inc., for more information about any additional licenses to
// intellectual property covering the AAF Standard that may be required
// to create and distribute AAF compliant products.
// (http://www.amwa.tv/policies).
//
// Copyright Notices:
// The Original Code of this file is Copyright 1998-2009, licensor of the
// Advanced Media Workflow Association.  All rights reserved.
//
//=---------------------------------------------------------------------=

#include "ImplAAFCloneResolver.h"

#include "ImplAAFFile.h"
#include "ImplAAFDictionary.h"
#include "ImplAAFMetaDictionary.h"
#include "ImplAAFOperationDef.h"

#include "AAFStoredObjectIDs.h"
#include "AAFResult.h"

#include "OMAssertions.h"

//=---------------------------------------------------------------------=

ImplAAFCloneResolver::ImplAAFCloneResolver( ImplAAFFile* pDstFile )
{
  _AAFCLONE_CHECK_HRESULT( pDstFile->GetDictionary( &_pDstDict ) );
}

ImplAAFCloneResolver::ImplAAFCloneResolver( ImplAAFDictionary* pDstDict )
  : _pDstDict( pDstDict )
{}

ImplAAFCloneResolver::~ImplAAFCloneResolver()
{}

void ImplAAFCloneResolver::AddSourceReference( const aafMobID_t mobID )
{
#ifndef DISABLE_CLONE_RESOLVER
  if ( !_sourceIDList.containsValue( mobID ) ) {
    _sourceIDList.insert( mobID );
  }
#endif
}

const OMVector<aafMobID_t>& ImplAAFCloneResolver::GetSourceReferences() const
{
  return _sourceIDList;
}

void ImplAAFCloneResolver::AddTypeReference( const aafUID_t typeID )
{
#ifndef DISABLE_CLONE_RESOLVER
  if ( !_typeIDList.containsValue( typeID ) ) {
    _typeIDList.insert( typeID );
  }
#endif
}

const OMVector<aafUID_t>& ImplAAFCloneResolver::GetTypeReferences() const
{
  return _typeIDList;
}

//=---------------------------------------------------------------------=

#define _DEFINE_LOOKUP_AND_REGISTER( TYPE, LOOKUP_METHOD, REGISTER_METHOD ) \
template <> \
HRESULT ImplAAFCloneResolverLookupDef( ImplAAFDictionary* pDict, const aafUID_t& auid, TYPE** ppDef ) \
{ \
  return pDict->LOOKUP_METHOD( auid, ppDef );  \
} \
template <> \
HRESULT ImplAAFCloneResolverRegisterDef( ImplAAFDictionary* pDict, TYPE* pDef ) \
{ \
  return pDict->REGISTER_METHOD( pDef ); \
}

#define _DEFINE( NAME ) \
	_DEFINE_LOOKUP_AND_REGISTER( ImplAAF##NAME , Lookup##NAME , Register##NAME )

_DEFINE( ClassDef );
_DEFINE( TypeDef );
_DEFINE( DataDef );
_DEFINE( OperationDef );
_DEFINE( ParameterDef );
_DEFINE( CodecDef );
_DEFINE( ContainerDef );
_DEFINE( InterpolationDef );
_DEFINE( PluginDef );

//=---------------------------------------------------------------------=

void ImplAAFCloneResolver::CloneClassDef( const OMClassId& id,
					  OMClassFactory* pDstFactory,
					  ImplAAFMetaDictionary* pSrcDict )
{
#ifndef DISABLE_CLONE_RESOLVER
  // We may be handed an ImplAAFDictionary, or ImplAAFMetaDictionary.
  // In either case, we need to get to the ImplAAFDictionary.

  ImplAAFDictionary* pDstDict = dynamic_cast<ImplAAFDictionary*>(pDstFactory);
  ImplAAFMetaDictionary* pDstMetaDict = dynamic_cast<ImplAAFMetaDictionary*>(pDstFactory);

  if ( !pDstDict && pDstMetaDict ) {
    pDstDict = pDstMetaDict->dataDictionary();
  }

  if ( !pDstDict ) {
    _AAFCLONE_CHECK_HRESULT( AAFRESULT_BAD_TYPE );
  }

  ASSERTU( sizeof(aafUID_t) == sizeof(id) );

  ImplAAFSmartPointer<ImplAAFClassDef> spClassDef;
  _AAFCLONE_CHECK_HRESULT( pSrcDict->LookupClassDef( reinterpret_cast<const aafUID_t&>(id), &spClassDef) );

  ImplAAFCloneResolver resolver(pDstDict);
  resolver.CloneAndRegister( static_cast<ImplAAFClassDef*>(spClassDef) );
#endif
}

//=---------------------------------------------------------------------=
