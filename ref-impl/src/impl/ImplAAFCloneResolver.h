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
// The Initial Developer of the Original Code of this file and the
// licensor of the Advanced Media Workflow Association is
// Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

#ifndef __ImplAAFCloneResolver_h__
#define __ImplAAFCloneResolver_h__

// Temporary switch to turn ImplAAFCloneResolver
// functionality on and off.
// If DISABLE_CLONE_RESOLVER defined, ImplAAFCloneResolver
// methods and related routins do nothing.
#define DISABLE_CLONE_RESOLVER

#include "AAFResult.h"
#include "AAFUtils.h"

#include "ImplAAFDictionary.h"
#include "ImplAAFMetaDictionary.h"
#include "ImplAAFPropertyDef.h"
#include "ImplAAFTypeDef.h"
#include "ImplAAFClassDef.h"

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

// ImplAAFCloneResovler is used to implement ImplAAFCloneExternal().
// A pointer to an instance of this class is passed to "onCopy()"
// implementations during OM traversals of the object/property graph.
// It is used to resolve weak references to definition objects stored
// in the dictionary, and to maintain a set of source references
// (i.e. the id's of referenced mobs).  Finally, a set of type ids
// referenced by cloned meta definitions is stored.
//
// The ResolveWeakReference(...) methods will resolve weak references
// to any definition object that is derived from OMStorable and stored
// in the dictionary.  The set of such classes is:
//
//   ImplAFClassDefinition
//   ImplAAFTypeDef
//   ImplAAFDataDef
//   ImplAAFOperationDef
//   ImplAAFParameterDef
//   ImplAAFCodecDef
//   ImplAAFContainerDef
//   ImplAAFInterpolationDef
//   ImplAAFPluginDef
//
// The AddSourceReference() and GetSourceReferences() methods are used
// to collect the set of mob id's for all ImplAAFSourceReference
// objects visited by an OM deepCopy() traversal.  These are the ids
// of mobs that form a mob chain.
//
// The AddTypeReference() and GetTypeReferences() methods are used to
// collect the set of type ids referenced by ImplAAFPropertyDef
// objects visited by an OM deepCopy() traversal.  The
// CloneAndRegister() method uses this to resolve extended types
// referenced in the course of cloning a class definition.
// 

class ImplAAFCloneResolver {
 public:

  // CopyClassDef is a small utility function to initiate a copy of
  // the class definition identified by "id".
  static void CloneClassDef( const OMClassId& id,
			     OMClassFactory* pDstFactory,
			     ImplAAFMetaDictionary* pSrcDict );

  ImplAAFCloneResolver( ImplAAFFile* pDstFile );
  ImplAAFCloneResolver( ImplAAFDictionary* pDstDict );
  ~ImplAAFCloneResolver();

  template <class Key, class Type>
    void ResolveWeakReference( const OMWeakReferenceProperty<Key, Type>& weakRef )
  {
#ifndef DISABLE_CLONE_RESOLVER
	if ( !weakRef.isOptional() || weakRef.isPresent() ) {
		const Type* obj = weakRef;
		CloneAndRegister( obj );
	}
#endif
  }

  template <class Key, class Type>
  void ResolveWeakReference( const OMWeakReferenceVectorProperty<Key, Type>& weakRefVec )
  {
#ifndef DISABLE_CLONE_RESOLVER
	if ( !weakRefVec.isOptional() || weakRefVec.isPresent() ) {
		OMWeakReferenceVectorIterator<Key, Type> iterator( weakRefVec );
		while( ++iterator ) {
			const Type *obj = iterator.value();
			CloneAndRegister( obj );
		}
	}
#endif
  }

  template <class Key, class Type>
  void ResolveWeakReference( const OMWeakReferenceSetProperty<Key, Type>& weakRefSet )
  {
#ifndef DISABLE_CLONE_RESOLVER
	if ( !weakRefSet.isOptional() || weakRefSet.isPresent() ) {
	  OMWeakReferenceSetIterator<Key, Type> iterator( weakRefSet );
	  while( ++iterator ) {
	    const Type *obj = iterator.value();
	    CloneAndRegister( obj );
	  }
	}
#endif
  }

  // Add a mobID to the list, and get the list.  AddSourceReference
  // ignores mobID's that are already in the source reference list.
  // The user is responsible for copying the referenced mobs.
  void AddSourceReference( const aafMobID_t mobID );
  const OMVector<aafMobID_t>& GetSourceReferences() const;

  // Add the uid of a type to the type id list.  Ignores ids that are
  // already in the list.  The user calls GetTypeReferences() and is
  // responsible for initiating new copy operation on the referenced
  // type definitions.
  void AddTypeReference( const aafUID_t typeID );
  const OMVector<aafUID_t>& GetTypeReferences() const;

  // Clone and register any type definition.  The method depends on
  // the ImplAAFCloneResolverLookupDef and
  // ImplAAFCloneResolverRegisterDef methods, hence, support the the
  // same set of definition types.
  // 
  // The method also clones referenced types not present in the
  // destination dictionary.

  template <class Type>
  void CloneAndRegister( const Type* pSrcDef )
  {
#ifndef DISABLE_CLONE_RESOLVER
    aafUID_t auid;

    _AAFCLONE_CHECK_HRESULT( pSrcDef->GetAUID( &auid ) );

    ImplAAFSmartPointer<Type> spDstDef;
    HRESULT hr = ImplAAFCloneResolverLookupDef( _pDstDict, auid,
						&spDstDef );

    if ( AAFRESULT_NO_MORE_OBJECTS == hr ) {

      Type* pDstDef;

      // If this is a meta definition, then pass the meta dictionary
      // to shallowCopy(), else pass the ordinary dictionary.
      OMClassFactory* pOMDstFactory;
      if ( dynamic_cast<const ImplAAFPropertyDef*>(pSrcDef) ||
		   dynamic_cast<const ImplAAFClassDef*>(pSrcDef) ||
		   dynamic_cast<const ImplAAFTypeDef*>(pSrcDef) ) {
		pOMDstFactory = _pDstDict->metaDictionary();
      }
      else {
		pOMDstFactory = _pDstDict;
      }

      OMStorable* pDstStorable = pSrcDef->shallowCopy( pOMDstFactory );
	  
      pDstDef = dynamic_cast<Type*>( pDstStorable );
      if ( !pDstDef ) {
		_AAFCLONE_CHECK_HRESULT( AAFRESULT_BAD_TYPE );
      }

      pSrcDef->onCopy( this );

      ImplAAFCloneResolver context( _pDstDict );
      pSrcDef->deepCopyTo( pDstStorable, &context );

      // Check if the copied definition referenced any types that must
      // also be cloned and registered with the destination
      // dictionary.  This must be done before pDstDef is registered
      // or else the registration will fail due to the undefined
      // types.  This is only any issue when cloneing class
      // definitions, the type id come from the property definitions
      // encountered during the deepCopy traversal of the class
      // definition.

      const OMVector<aafUID_t>& referencedTypes = context.GetTypeReferences();
      unsigned int i;
      for( i = 0; i < referencedTypes.count(); ++i ) {
		aafUID_t typeID = referencedTypes.valueAt(i);

		ImplAAFSmartPointer<ImplAAFDictionary> spSrcDict;
		_AAFCLONE_CHECK_HRESULT( pSrcDef->GetDictionary( &spSrcDict ) );

		ImplAAFSmartPointer<ImplAAFTypeDef> spSrcTypeDef;
		_AAFCLONE_CHECK_HRESULT( ImplAAFCloneResolverLookupDef( spSrcDict, typeID,
									&spSrcTypeDef ) );

		ImplAAFCloneResolver typeContext( _pDstDict );
		typeContext.CloneAndRegister( static_cast<ImplAAFTypeDef*>(spSrcTypeDef) );
      }	
	  
      _AAFCLONE_CHECK_HRESULT( ImplAAFCloneResolverRegisterDef( _pDstDict, pDstDef ) );
    }
#endif
  }

private:

  // prohibited
  ImplAAFCloneResolver();
  ImplAAFCloneResolver( const ImplAAFCloneResolver& );
  ImplAAFCloneResolver& operator==( const ImplAAFCloneResolver& );

  OMVector<aafMobID_t> _sourceIDList;
  OMVector<aafUID_t> _typeIDList;
  ImplAAFDictionary* _pDstDict;
};

#endif
