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
// All rights reserved.
//
//=---------------------------------------------------------------------=

#include "ImplAAFCloneResolver.h"

#include "ImplAAFFile.h"
#include "ImplAAFDictionary.h"
#include "ImplAAFOperationDef.h"
#include "AAFResult.h"

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
  if ( !_sourceIDList.containsValue( mobID ) ) {
    _sourceIDList.insert( mobID );
  }
}

const OMVector<aafMobID_t>& ImplAAFCloneResolver::GetSourceReferences() const
{
  return _sourceIDList;
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

bool operator==( const aafMobID_t& lhs, const aafMobID_t& rhs )
{
   return (memcmp( &rhs, &lhs, sizeof(aafMobID_t) ) == 0) ? true : false;
}
