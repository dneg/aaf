//@doc
//@class    AAFPropertyDef | Implementation class for AAFPropertyDef
#ifndef __ImplAAFPropertyDef_h__
#define __ImplAAFPropertyDef_h__

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

class ImplEnumAAFPropertyValues;
class ImplAAFPropertyValue;
class ImplAAFTypeDef;

#ifndef __ImplAAFMetaDefinition_h__
#include "ImplAAFMetaDefinition.h"
#endif

#include "OMPropertyDefinition.h"
#include "OMVariableSizeProperty.h"

typedef OMProperty* (*ImplAAFOMPropertyCreateFunc_t)
  (OMPropertyId pid,
   const wchar_t * name);

class ImplAAFPropertyDef : public ImplAAFMetaDefinition,
						   public OMPropertyDefinition
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFPropertyDef ();

protected:
  virtual ~ImplAAFPropertyDef ();

public:

  //****************
  // GetTypeDef()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetTypeDef
        // @parm [out] definition of type contained by this property
        (ImplAAFTypeDef ** ppTypeDef) const;


  //****************
  // IsOptional()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetIsOptional
      (// @parm [out] pointer to the result
       aafBool * pIsOptional) const;


  //****************
  // IsUniqueIdentifier()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetIsUniqueIdentifier
      (// @parm [out] pointer to the result
       aafBool * pIsUniqueIdentifier) const;

  //
  // Non-published methods (yet still public)
  //

  //****************
  // pvtInitialize()
  //
  AAFRESULT pvtInitialize
       (// @parm [in] auid to be used to identify this property definition
        const aafUID_t & propertyAuid,
			
        // @parm [in] OM pid (small integer) to be used to identify
		// this property definition
        OMPropertyId omPid,
			
        // @parm [in, string] friendly name of this property
	    const aafCharacter * pPropName,
	
        // @parm [in] Type definition of this property definition,
	    const aafUID_t & typeID,

        // @parm [in] Is this property optional? (mandatory, if not)
		  aafBoolean_t isOptional,

        // @parm [in] Is this property a unique identifier
		  aafBoolean_t isUniqueIdentifier,

        // @parm [in] The class definition to which this property belongs
		  OMClassDefinition* pContainingClass);


  //****************
  // pvtInitialize()
  //
  AAFRESULT pvtInitialize
       (// @parm [in] auid to be used to identify this property definition
        aafUID_constref propertyAuid,
			
        // @parm [in] OM pid (small integer) to be used to identify
		// this property definition
        OMPropertyId omPid,
			
        // @parm [in, string] friendly name of this property
	aafCharacter_constptr pPropName,
	
        // @parm [in] Type definition of this property definition,
	ImplAAFTypeDef *pType,

        // @parm [in] Is this property optional? (mandatory, if not)
	aafBoolean_t isOptional,

        // @parm [in] Is this property a unique identifier
	aafBoolean_t isUniqueIdentifier,

        // @parm [in] The class definition to which this property belongs
	OMClassDefinition* pContainingClass);


  OMPropertyId OmPid (void) const;
  //
  // Returns the OM pid (small integer) identifying this property

  //
  // Overrides from OMPropertyDefinition
  //
  virtual OMClassDefinition* containingClass(void) const;
  virtual const OMUniqueObjectIdentification& identification(void) const;
  virtual const wchar_t* name(void) const;
  virtual bool hasDescription(void) const;
  virtual const wchar_t* description(void) const;
  virtual bool isPredefined(void) const;
  virtual const OMType* type(void) const;
  virtual OMPropertyId localIdentification(void) const;
  // PdN: TODO: this is not used by the OM layer but by newProperty
  void setLocalIdentification(OMPropertyId propertyId);
  virtual bool isOptional(void) const;
  virtual OMUniqueObjectIdentification typeId(void) const;
  virtual bool isUniqueIdentifier(void) const;

  // Allocates and returns an OMProperty which can represent this
  // property.  
  OMProperty * CreateOMProperty () const;

  // Sets the function which creates OMProperties useful for these
  // properties.
  void SetOMPropCreateFunc (ImplAAFOMPropertyCreateFunc_t pFunc);


  // override from OMStorable.
  virtual const OMClassId& classId(void) const;

  // Override callbacks from OMStorable
  virtual void onSave(void* clientContext) const;
  virtual void onRestore(void* clientContext) const;

  // Method is called after class has been added to MetaDictionary.
  // If this method fails the class is removed from the MetaDictionary and the
  // registration method will fail.
  virtual HRESULT CompleteClassRegistration(void);

  virtual AAFRESULT MergeTo( ImplAAFClassDef* pDestClassDef );

private:

  // OMWeakReferenceProperty<ImplAAFTypeDef> _Type;
  OMFixedSizeProperty<aafUID_t>              _Type;

  OMFixedSizeProperty<aafBool>               _IsOptional;

  OMFixedSizeProperty<OMPropertyId>          _pid;

  OMFixedSizeProperty<aafBool>               _IsUniqueIdentifier;

  ImplAAFTypeDef *                           _cachedType;

  ImplAAFOMPropertyCreateFunc_t              _OMPropCreateFunc;

  // The class to which this property belongs
  OMClassDefinition*                         _containingClass;

};

//
// smart pointer
//

#ifndef __ImplAAFSmartPointer_h__
#include "ImplAAFSmartPointer.h"
#endif

typedef ImplAAFSmartPointer<ImplAAFPropertyDef> ImplAAFPropertyDefSP;

#endif // ! __ImplAAFPropertyDef_h__
