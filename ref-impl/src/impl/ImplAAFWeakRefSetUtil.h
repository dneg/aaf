#ifndef __ImplAAFWRSUtil_h__
#define __ImplAAFWRSUtil_h__

//=---------------------------------------------------------------------=
//
// $Id$ $Name$
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
// The Original Code of this file is Copyright 1998-2004, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

#include "ImplAAFDictionary.h"
#include "ImplAAFMetaDictionary.h"

#include "AAFResult.h"
#include "AAFTypes.h"
#include "aafErr.h"

#include "OMWeakRefSetProperty.h"

// ReferencedType was ImplAAFParameterDef
// EnumReferencedType was ImplEnumAAFParameterDef

// _paramDefined is now OMWeakReferenceSetProperty<ReferencedType>& wrs

// template <class ReferencedType>
// template <class EnumReferencedType>

// This code was factored out of ImplAAFOperationDef.  The were the
// original the methods that accessed ImplAAFOperationDef::_paramDefined

class AAFWeakRefSetUtil {
 public:

  // Requires specialized implementations.
  // Implementations currentl exist for:
  // 
  template <class ReferencedType>
  static bool LookupDef( ImplAAFObject* pParentObj, ReferencedType* pDef )
  {
    return false;
  }

  // Add pDef to the WeakReferenceSet.  It must be found in the
  // dictionary (or metadictionary) as reported by LookupDef().
  // The weak refernce is owned by another strong reference set, so we
  // do not need to reference count it when it is added to wrs.

  template <class ReferencedType>
  static AAFRESULT Add( ReferencedType *pDef,
			ImplAAFObject* pParentObject,
			OMWeakReferenceSetProperty<ReferencedType>& wrs )
    {
      if (NULL == pDef)
	return AAFRESULT_NULL_PARAM;
      
      // Check if given definition is in the dict.
      if( !LookupDef( pParentObject, pDef ) ) {
	return AAFRESULT_INVALID_OBJ;
      }

      wrs.appendValue(pDef);
  
      return(AAFRESULT_SUCCESS);
    }

  // Create an enumerator for a weak reference set.
  template <class ReferencedType, class EnumReferencedType>
  static AAFRESULT Get( EnumReferencedType** ppEnum,
			const aafUID_t& enumCLSID,
			ImplAAFObject* pParentObject,
			OMWeakReferenceSetProperty<ReferencedType>& wrs )
  {
    EnumReferencedType	*theEnum;
    if(ppEnum == NULL)
      return(AAFRESULT_NULL_PARAM);
      
    *ppEnum = NULL;
    theEnum = (EnumReferencedType *)CreateImpl( enumCLSID );
    if(theEnum == NULL)
      return(AAFRESULT_NOMEMORY);
    XPROTECT()
    {
      OMWeakReferenceSetIterator<ReferencedType>* iter = 
	new OMWeakReferenceSetIterator<ReferencedType>(wrs);
      if(iter == 0)
	RAISE(AAFRESULT_NOMEMORY);
      CHECK(theEnum->Initialize(&enumCLSID, pParentObject, iter));
      *ppEnum = theEnum;
    }
    XEXCEPT
    {
      if (theEnum)
      {
	theEnum->ReleaseReference();
	theEnum = 0;
      }
    }
    XEND;
    
    return(AAFRESULT_SUCCESS);
  }


  // Return the size of the weak reference set.
  template <class ReferencedType>
  static AAFRESULT Count ( aafUInt32 * pResult,
			   OMWeakReferenceSetProperty<ReferencedType>& wrs )
  {
    if (! pResult)
      return AAFRESULT_NULL_PARAM;
    *pResult = wrs.count();
    return AAFRESULT_SUCCESS;
  }

  // FIXME - Currently unused.  Remove if not used.

  // Lookup a definition ID in the weak reference set.
  template <class ReferencedType>
  static AAFRESULT Lookup ( const aafUID_t & defId,
			    ReferencedType ** ppDef,
			    OMWeakReferenceSetProperty<ReferencedType>& wrs )
  {
    if (!ppDef) return AAFRESULT_NULL_PARAM;
  
    AAFRESULT result = AAFRESULT_SUCCESS;

    if (wrs.find((*reinterpret_cast<const OMObjectIdentification *>(&defId)),
		 *ppDef))
    {
      assert(NULL != *ppDef);
      (*ppDef)->AcquireReference();
    }
    else
    {
      // no recognized class guid in dictionary
      result = AAFRESULT_NO_MORE_OBJECTS;
    }
    
    return (result);
  }

}; // end class AAFWeakRefSetUtil

#endif
