//@doc
//@class    AAFClassDef | Implementation class for AAFClassDef
#ifndef __ImplAAFClassDef_h__
#define __ImplAAFClassDef_h__

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

class ImplAAFObject;
class ImplAAFDefObject;
class ImplAAFPropertyDef;

template <class T> 
class ImplAAFEnumerator;
typedef ImplAAFEnumerator<ImplAAFPropertyDef> ImplEnumAAFPropertyDefs;

#ifndef __ImplAAFMetaDefinition_h__
#include "ImplAAFMetaDefinition.h"
#endif

#ifndef __ImplAAFPropertyDef_h__
#include "ImplAAFPropertyDef.h"
#endif

#ifndef __ImplAAFTypeDef_h__
#include "ImplAAFTypeDef.h"
#endif

#include "OMClassDefinition.h"
#include "OMWeakRefProperty.h"
#include "OMStrongRefSetProperty.h"

class ImplAAFClassDef : public ImplAAFMetaDefinition,
                        public OMClassDefinition
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFClassDef ();

protected:
  virtual ~ImplAAFClassDef ();

public:

  //****************
  // Initialize()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Initialize
        // @parm [in] auid to be used to identify this type
        (const aafUID_t & classID,

		// Inheritance parent of this class
		ImplAAFClassDef * pParentClass,

		// Human-legible name
		const aafCharacter * pClassName,

		// Can objects of this class be instantiated
		aafBool isConcrete);


  //****************
  // GetPropertyDefs()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetPropertyDefs
        // @parm [out, retval] Property Definition enumeration
        (ImplEnumAAFPropertyDefs ** ppEnum);


  //****************
  // CountPropertyDefs()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    CountPropertyDefs
        // @parm [out] number of properties contained in this class definition
        (aafUInt32 *  pCount);


  //****************
  // RegisterNewPropertyDef()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    RegisterNewPropertyDef
        (// @parm [in] auid to be used to identify this property
         const aafUID_t & id,

         // @parm [in, string] name of the new property
         const aafCharacter *  pName,

         // @parm [in] type of the new property
         ImplAAFTypeDef * pTypeDef,

         // @parm [in] true if new property is to be optional
         aafBool  isOptional,

         // @parm [in] true if new property is a unique identifier
         aafBool  isUniqueIdentifier,

         // @parm [out] return pointer to newly created property def
         ImplAAFPropertyDef ** ppPropDef);


  //****************
  // RegisterOpionalPropertyDef()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    RegisterOptionalPropertyDef
        (// @parm [in] auid to be used to identify this property
         const aafUID_t & id,

         // @parm [in, string] name of the new property
         const aafCharacter *  pName,

         // @parm [in] type of the new property
         ImplAAFTypeDef * pTypeDef,

         // @parm [out] return pointer to newly created property def
         ImplAAFPropertyDef ** ppPropDef);


  //****************
  // LookupPropertyDef()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    LookupPropertyDef
        (// @parm [in] auid reprepresenting property to look up
         const aafUID_t & propID,

         // @parm [out] resulting property definition
         ImplAAFPropertyDef ** ppPropDef) const;


  //****************
  // GetParent()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetParent
        // @parm [out, retval] parent class definition
        (ImplAAFClassDef ** ppClassDef);


  //****************
  // CreateInstance()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    CreateInstance
        // @parm [out, retval] newly created object
        (ImplAAFObject ** ppobject);

  //****************
  // IsRoot()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    IsRoot
        (aafBool* isRootClass);

  //****************
  // IsConcrete()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    IsConcrete
        (aafBool* pResult);

  //****************
  // IsUniquelyIdentified
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    IsUniquelyIdentified
        (aafBoolean_t * isUniquelyIdentified);

  //****************
  // GetUniqueIdentifier
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetUniqueIdentifier
        (ImplAAFPropertyDef ** ppUniqueIdentifier);

  const OMPropertyDefinition* propertyDefinition(
                                 const OMUniqueObjectIdentification& id) const;

public:

	bool
    PvtIsPropertyDefRegistered (
      aafUID_constref propId);

  //****************
  // SetParent()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetParent
        (ImplAAFClassDef *pClassDef);

  //****************
  // SetBootstrapParent()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetBootstrapParent
        (ImplAAFClassDef *pClassDef);

  //****************
  // pvtInitialize()
  //
  // Same as Initialize(), but takes guid for parent class instead of
  // object pointer.
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    pvtInitialize
        // @parm [in] auid to be used to identify this type
        (const aafUID_t & classID,

		// Inheritance parent of this class
		ImplAAFClassDef * pParentClass,

		// Human-legible name
		const aafCharacter * pClassName,

		// Can objects of this class be instantiated
		aafBool isConcrete);


  // Returns true if this class can be instantiated.
  aafBool pvtIsConcrete () const;

  // Private method to set the "IsConcrete" property (used for bootstrap).
  void pvtSetIsConcrete (aafBoolean_t isConcrete);

  // Private method to unconditionally register a property def (ignoring
  // whether or not property is optional or not, or if this class has
  // already been registered).
  AAFRESULT STDMETHODCALLTYPE
    pvtRegisterPropertyDef
        (const aafUID_t & id,
         const aafCharacter *  pName,
         const aafUID_t & typeId,
         aafBool  isOptional,
         aafBool  isUniqueIdentifier,
         ImplAAFPropertyDef ** ppPropDef);


  // Appends an existing property def object.
  AAFRESULT STDMETHODCALLTYPE
    pvtRegisterExistingPropertyDef
        (// PropertyDef to append
		 ImplAAFPropertyDef * pPropDef);


  //****************
  // LookupPropertyDefbyOMPid()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    LookupPropertyDefbyOMPid
        (// @parm [in] pid reprepresenting property to look up
         OMPropertyId omPid,

         // @parm [out] resulting property definition
         ImplAAFPropertyDef ** ppPropDef) const;

  // Make sure that the type definition of each property definition
  // has been loaded into memory.
  void AssurePropertyTypesLoaded ();

  // Find the unique identifier property defintion for this class or any parent class
  // (RECURSIVE)
  ImplAAFPropertyDef * pvtGetUniqueIdentifier(void); // result is NOT reference counted.


  // override from OMStorable.
  virtual const OMClassId& classId(void) const;

  // Override callbacks from OMStorable
  virtual void onSave(void* clientContext) const;
  virtual void onRestore(void* clientContext) const;


  // Method is called after associated class has been added to MetaDictionary.
  // If this method fails the class is removed from the MetaDictionary and the
  // registration method will fail.
  virtual HRESULT CompleteClassRegistration(void);

private:

  //
  // Private implementation classes to share lookup code between
  // methods which lookup by auid, and ones which look up by OMPid.
  //
  // Abstract base class which allows comparison of a property
  // identifier to a property definition.
  class pvtPropertyIdentifier
  {
  public:
	// Returns kAAFTrue if this property identifier matches the given
	// property definition.
	virtual aafBool DoesMatch
    (const ImplAAFPropertyDef * pTestPropDef) const = 0;
  };

  //
  // Concrete class implementing property identifier as OM PID
  //
  class pvtPropertyIdentifierOMPid : public pvtPropertyIdentifier
  {
  public:
	pvtPropertyIdentifierOMPid (const OMPropertyId & id)
	  : _id (id) {}

	aafBool DoesMatch (const ImplAAFPropertyDef * pTestPropDef) const;
	
  private:
	OMPropertyId _id;
  };

  //
  // Concrete class implementing property identifier as a Property ID
  // AUID.
  //
  class pvtPropertyIdentifierAUID : public pvtPropertyIdentifier
  {
  public:
	pvtPropertyIdentifierAUID (const aafUID_t & id)
	  : _id (id) {}

	aafBool DoesMatch (const ImplAAFPropertyDef * pTestPropDef) const;

  private:
	aafUID_t _id;
  };


  // Provide two level property search routines that are overloaded.

  // Low-level recursive methods the OM find method instead of a linear search
  // that will necessarily load all of the property definitions for the class
  // definition instance.
  AAFRESULT
    generalLookupPropertyDef (
      aafUID_constref propId,
      ImplAAFPropertyDef ** ppPropDef);

  //
  // The generalized lookup method which uses a pvtPropertyIdentifier
  // as the property ID.
  //
  AAFRESULT 
    generalLookupPropertyDef (
      const pvtPropertyIdentifier & propId,
      ImplAAFPropertyDef ** ppPropDef);

  // OMWeakReferenceProperty<ImplAAFClassDef> _ParentClass;
  OMWeakReferenceProperty<ImplAAFClassDef>         _ParentClass;

  OMStrongReferenceSetProperty<OMUniqueObjectIdentification, ImplAAFPropertyDef> _Properties;

  OMFixedSizeProperty<aafBool> _IsConcrete;

  ImplAAFClassDef	*_BootstrapParent;

  bool _propTypesLoaded;
};

//
// smart pointer
//

#ifndef __ImplAAFSmartPointer_h__
// caution! includes assert.h
#include "ImplAAFSmartPointer.h"
#endif

typedef ImplAAFSmartPointer<ImplAAFClassDef> ImplAAFClassDefSP;

#endif // ! __ImplAAFClassDef_h__
