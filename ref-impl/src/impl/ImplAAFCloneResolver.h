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

#ifndef __ImplAAFCloneResolver_h__
#define __ImplAAFCloneResolver_h__

#include "AAFResult.h"

#include "ImplAAFDictionary.h"

#include "OMWeakRefProperty.h"
#include "OMWeakRefSetProperty.h"
#include "OMWeakReferenceSetIter.h"
#include "OMWeakRefVectorProperty.h"
#include "OMWeakReferenceVectorIter.h"

class ImplAAFFile;
class OMClassFactory;

//=---------------------------------------------------------------------=

#define _AAFCLONE_CHECK_HRESULT(EXPR) \
  { \
    HRESULT _hr = (EXPR); \
    if ( AAFRESULT_SUCCESS != _hr ) { \
       throw ImplAAFCloneResolverEx( _hr ); \
    } \
  }

//=---------------------------------------------------------------------=

class ImplAAFCloneResolverEx
{
 public:
  ImplAAFCloneResolverEx( HRESULT hr )
    : _hr(hr)
    {}

  HRESULT GetHResult() const
    { return _hr; }

 private:
  HRESULT _hr;
};

//=---------------------------------------------------------------------=

// These template functions only have explicit instances.  The provide a
// polymorphic interface to lookup and register definitions in a
// dictionary. The template is used to avoid the need to declare N
// different versions of the function (N is the number of definition
// types).
//
// Template instances exist for:
//   ImplAAFClassDef
//   ImplAAFTypeDef
//   ImplAAFDataDef
//   ImplAAFOperationDef
//   ImplAAFParameterDef
//   ImplAAFCodecDef
//   ImplAAFContainerDef
//   ImplAAFInterpolationDef
//   ImplAAFPluginDef

template <class Type>
HRESULT ImplAAFCloneResolverLookupDef( ImplAAFDictionary* pDict, const aafUID_t& auid, Type** pDef );
template <class Type>
HRESULT ImplAAFCloneResolverRegisterDef( ImplAAFDictionary* pDict, Type* ppDef );

//=---------------------------------------------------------------------=

class ImplAAFCloneResolver {
 public:
  ImplAAFCloneResolver( ImplAAFFile* pDstFile );
  ImplAAFCloneResolver( ImplAAFDictionary* pDstDict );
  ~ImplAAFCloneResolver();

  template <class Type>
  void ResolveWeakReference( const OMWeakReferenceProperty<Type>& weakRef )
  {
	if ( !weakRef.isOptional() || weakRef.isPresent() ) {
		const Type* obj = weakRef;
		CloneAndRegister( obj );
	}
  }

  template <class Type>
  void ResolveWeakReference( const OMWeakReferenceVectorProperty<Type>& weakRefVec )
  {
	if ( !weakRefVec.isOptional() || weakRefVec.isPresent() ) {
		OMWeakReferenceVectorIterator<Type> iterator( weakRefVec );
		while( ++iterator ) {
			const Type *obj = iterator.value();
			CloneAndRegister( obj );
		}
	}
  }

  template <class Type>
  void ResolveWeakReference( const OMWeakReferenceSetProperty<Type>& weakRefSet )
  {
	if ( !weakRefSet.isOptional() || weakRefSet.isPresent() ) {
	  OMWeakReferenceSetIterator<Type> iterator( weakRefSet );
	  while( ++iterator ) {
	    const Type *obj = iterator.value();
	    CloneAndRegister( obj );
	  }
	}
  }

private:

  // prohibited
  ImplAAFCloneResolver();
  ImplAAFCloneResolver( const ImplAAFCloneResolver& );
  ImplAAFCloneResolver& operator==( const ImplAAFCloneResolver& );

  template <class Type>
  void CloneAndRegister( const Type* pSrcDef )
  {
    aafUID_t auid;

    _AAFCLONE_CHECK_HRESULT( pSrcDef->GetAUID( &auid ) );

    Type* pDstDef;
    HRESULT hr = ImplAAFCloneResolverLookupDef( _pDstDict, auid, &pDstDef );

    if ( AAFRESULT_NO_MORE_OBJECTS == hr ) {
    
	  OMStorable* pDstStorable = pSrcDef->shallowCopy( _pDstDict );
      ImplAAFCloneResolver context( _pDstDict );
      pSrcDef->deepCopyTo( pDstStorable, &context );

      pDstDef = dynamic_cast<Type*>( pDstStorable );
      if ( !pDstStorable ) {
		_AAFCLONE_CHECK_HRESULT( AAFRESULT_BAD_TYPE );
      }
    
      _AAFCLONE_CHECK_HRESULT( ImplAAFCloneResolverRegisterDef( _pDstDict, pDstDef ) );
    
      return;
    }
	else {
		_AAFCLONE_CHECK_HRESULT( hr );
		pDstDef->ReleaseReference();
	}

  }

  ImplAAFDictionary* _pDstDict;
};

#endif
