//@doc
//@class    MetaDefinition | Implementation class for MetaDefinition
#ifndef __ImplAAFMetaDefinition_h__
#define __ImplAAFMetaDefinition_h__

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

#ifndef __AAFTypes_h__
#include "AAFTypes.h"
#endif

#include "ImplAAFStorable.h"

#include "OMStorable.h"
#include "OMFixedSizeProperty.h"
#include "OMWideStringProperty.h"

class ImplAAFDictionary;
class ImplAAFClassDef;
class ImplAAFTypeDef;
template <typename ReferencedObject>
class OMWeakReferenceProperty;
template <typename ReferencedObject>
class OMWeakReferenceVectorProperty;

class ImplAAFMetaDefinition : 
  public ImplAAFStorable
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFMetaDefinition ();

protected:
  virtual ~ImplAAFMetaDefinition ();

public:

  //****************
  // Initialize()
  //
  AAFRESULT STDMETHODCALLTYPE
    Initialize
        // @parm [in] Pointer to an AUID reference
        (aafUID_constref id,
		 aafCharacter_constptr name,
     aafCharacter_constptr description);


  //****************
  // GetAUID()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetAUID
        // @parm [retval,out] Pointer to an AUID reference
        (aafUID_t * pAuid) const;


  //****************
  // SetName()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetName
        (aafCharacter_constptr  name);  //@parm [in, ref] Definition Name


  //****************
  // GetName()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetName
        (// @parm [out, string, size_is(bufSize)] buffer into which Name is to be written
         aafCharacter *  pName,

         // @parm [in] size of *pName buffer in bytes
         aafUInt32  bufSize);


  //****************
  // GetNameBufLen()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetNameBufLen
        (aafUInt32 *  nameLen);  //@parm [in,out] Definition Name length


  //****************
  // SetDescription()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetDescription
        (aafCharacter_constptr description);  //@parm [in, ref] Definition description


  //****************
  // GetDescription()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetDescription
        (aafCharacter * description,  //@parm [in] Definition Description
		 aafUInt32 bufSize);	  //@parm [in] size of the buffer required to hold Definition Description + terminator


  //****************
  // GetDescriptionBufLen()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetDescriptionBufLen
        (aafUInt32 *  descriptionLen);  //@parm [in,out] Definition description length

public:
  // Private SDK methods.


  // Gets the dictionary used to create this instance.
  virtual AAFRESULT STDMETHODCALLTYPE 
    GetDictionary(ImplAAFDictionary **ppDictionary) const;

  ImplAAFTypeDef* bootstrapTypeWeakReference(
               const OMWeakReferenceProperty<ImplAAFTypeDef>& reference) const;

  ImplAAFTypeDef* bootstrapTypeWeakReferenceVectorElement(
                   const OMWeakReferenceVectorProperty<ImplAAFTypeDef>& vector,
                   aafUInt32 index) const;

  ImplAAFClassDef* bootstrapClassWeakReference(
              const OMWeakReferenceProperty<ImplAAFClassDef>& reference) const;

protected:
  // Associate the existing OMProperties with corresponding property definitions from
  // the given class definition. NOTE: This call is recursive, it calls itself again
  // for the parent class of the given class until current class is a "root" class.
  virtual void InitOMProperties (ImplAAFClassDef * pClassDef);
public:


  virtual const OMUniqueObjectIdentification& identification(void) const;

  // Private method to assign the unique identifier.
  AAFRESULT SetIdentification(aafUID_constref identification);


  // override from OMStorable.
  virtual const OMClassId& classId(void) const;

  // Override callbacks from OMStorable
  virtual void onSave(void* clientContext) const;
  virtual void onRestore(void* clientContext) const;

  
  // Overrides of ImplAAFStorable.
  // Return true if this is a meta object
  // NOTE: These objects will eventually owned by the Object Manager.
  virtual bool metaObject(void) const;
  
  // Return true is this is a data object (Interchange object).
  virtual bool dataObject(void) const;

  // Method is called after associated class has been added to MetaDictionary.
  // If this method fails the class is removed from the MetaDictionary and the
  // registration method will fail.
  virtual HRESULT CompleteClassRegistration(void);

private:

  // friendly name of this definition
  OMWideStringProperty          _name;

  // brief text description of this definition
  OMWideStringProperty          _description;

  // auid to be used to identify this definition
  OMFixedSizeProperty<aafUID_t> _identification;
};

//
// smart pointer
//

#ifndef __ImplAAFSmartPointer_h__
// caution! includes assert.h
#include "ImplAAFSmartPointer.h"
#endif

typedef ImplAAFSmartPointer<ImplAAFMetaDefinition> ImplAAFMetaDefinitionSP;

#endif // ! __ImplAAFMetaDefinition_h__
