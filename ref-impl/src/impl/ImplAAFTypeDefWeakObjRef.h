//@doc
//@class    AAFTypeDefWeakObjRef | Implementation class for AAFTypeDefWeakObjRef
#ifndef __ImplAAFTypeDefWeakObjRef_h__
#define __ImplAAFTypeDefWeakObjRef_h__

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


class ImplAAFPropertyValue;

class ImplAAFClassDef;

#ifndef __ImplAAFTypeDefObjectRef_h__
#include "ImplAAFTypeDefObjectRef.h"
#endif
#include "OMWeakRefProperty.h"


class ImplAAFTypeDefWeakObjRef : public ImplAAFTypeDefObjectRef
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFTypeDefWeakObjRef ();

protected:
  virtual ~ImplAAFTypeDefWeakObjRef ();

public:

  //****************
  // Initialize()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Initialize
        (// @parm [in] auid to be used to identify this type
         const aafUID_t & id,

         // @parm [in] class def of objects permitted to be referenced
         ImplAAFClassDef * pObjType,

         // @parm [in, string] friendly name of this type definition
         const aafCharacter * pTypeName,

         // @parm [in] Number of property def IDs in pTargetSet
         aafUInt32  ids,

         // @parm [in, size_is(ids)] List of property definition IDs indicating the property where
         // the target is to be found.
         aafUID_constptr  pTargetSet);

  // Override from AAFTypeDefObjectRef
  virtual AAFRESULT STDMETHODCALLTYPE
    CreateValue (/*[in]*/ ImplAAFRoot * pObj,
      /*[out]*/ ImplAAFPropertyValue ** ppPropVal);

  // Override from AAFTypeDefObjectRef
  virtual AAFRESULT STDMETHODCALLTYPE
    SetObject (/*[in]*/ ImplAAFPropertyValue * pPropVal,
      /*[in]*/ ImplAAFRoot * ppObject);

  // Override from AAFTypeDefObjectRef
  virtual AAFRESULT STDMETHODCALLTYPE
    GetObject (/*[in]*/ ImplAAFPropertyValue * pPropVal,
      /*[out]*/ ImplAAFRoot ** ppObject);

  // Override from AAFTypeDefObjectRef
  virtual AAFRESULT STDMETHODCALLTYPE
    GetObjectType (/*[out]*/ ImplAAFClassDef ** ppObjType);

  // Override from AAFTypeDef
  virtual AAFRESULT STDMETHODCALLTYPE
    GetTypeCategory (/*[out]*/ eAAFTypeCategory_t *  pTid);


private:
  // Synchronize the array of OM pids with the current TargetSet property.
  AAFRESULT SyncTargetPidsFromTargetSet(void);

public:


  // Override from AAFTypeDefObjectRef
  AAFRESULT STDMETHODCALLTYPE
    pvtInitialize
        (const aafUID_t & id,
         const ImplAAFClassDef *pType,
         const aafCharacter * pTypeName,
         aafUInt32  ids,
         aafUID_constptr  pTargetSet,
         OMPropertyId * _targetPids = NULL,
         OMPropertyId _uniqueIdentifierPid = 0);

  aafUInt32 GetTargetPidCount(void) const;
  const OMPropertyId * GetTargetPids(void) const;
  OMPropertyId GetUniqueIdentifierPid(void) const;


  // overrides from ImplAAFTypeDef
  //
  aafBool IsFixedSize (void) const;
  size_t PropValSize (void) const;
  aafBool IsRegistered (void) const;
  size_t NativeSize (void) const;

  virtual OMProperty * 
    pvtCreateOMProperty (OMPropertyId pid,
							const wchar_t * name) const;

  // Allocate and initialize the correct subclass of ImplAAFPropertyValue 
  // for the given OMProperty.
  virtual AAFRESULT STDMETHODCALLTYPE
    CreatePropertyValue(OMProperty *property, 
                        ImplAAFPropertyValue ** pPropertyValue) const;


  // override from OMStorable.
  virtual const OMClassId& classId(void) const;

  // Override callbacks from OMStorable
  virtual void onSave(void* clientContext) const;
  virtual void onRestore(void* clientContext) const;

  // Method is called after class has been added to MetaDictionary.
  // If this method fails the class is removed from the MetaDictionary and the
  // registration method will fail.
  virtual HRESULT CompleteClassRegistration(void);

private:
  // Persistent member properties
  OMWeakReferenceProperty<ImplAAFClassDef> _referencedType;
  OMVariableSizeProperty<aafUID_t> _targetSet; // array of property definition ids
  
  // Transient members
  OMPropertyId * _targetPids;
  aafUInt32 _targetPidCount;
  OMPropertyId _uniqueIdentifierPid;
};


#endif // ! __ImplAAFTypeDefWeakObjRef_h__
