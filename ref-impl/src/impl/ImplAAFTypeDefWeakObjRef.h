//@doc
//@class    AAFTypeDefWeakObjRef | Implementation class for AAFTypeDefWeakObjRef
#ifndef __ImplAAFTypeDefWeakObjRef_h__
#define __ImplAAFTypeDefWeakObjRef_h__

/***********************************************************************
 *
 *              Copyright (c) 1998-2000 Avid Technology, Inc.
 *
 * Permission to use, copy and modify this software and accompanying 
 * documentation, and to distribute and sublicense application software
 * incorporating this software for any purpose is hereby granted, 
 * provided that (i) the above copyright notice and this permission
 * notice appear in all copies of the software and related documentation,
 * and (ii) the name Avid Technology, Inc. may not be used in any
 * advertising or publicity relating to the software without the specific,
 *  prior written permission of Avid Technology, Inc.
 *
 * THE SOFTWARE IS PROVIDED AS-IS AND WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY
 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 * IN NO EVENT SHALL AVID TECHNOLOGY, INC. BE LIABLE FOR ANY DIRECT,
 * SPECIAL, INCIDENTAL, PUNITIVE, INDIRECT, ECONOMIC, CONSEQUENTIAL OR
 * OTHER DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE AND
 * ACCOMPANYING DOCUMENTATION, INCLUDING, WITHOUT LIMITATION, DAMAGES
 * RESULTING FROM LOSS OF USE, DATA OR PROFITS, AND WHETHER OR NOT
 * ADVISED OF THE POSSIBILITY OF DAMAGE, REGARDLESS OF THE THEORY OF
 * LIABILITY.
 *
 ************************************************************************/


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
