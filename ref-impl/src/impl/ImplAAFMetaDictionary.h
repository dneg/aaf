//@doc
//@class    AAFMetaDictionary | Implementation class for AAFMetaDictionary
#ifndef __ImplAAFMetaDictionary_h__
#define __ImplAAFMetaDictionary_h__

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
 * prior written permission of Avid Technology, Inc.
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

// Forward declarations:


//#include "OMStorable.h"
//#include "OMClassFactory.h"

#include "ImplAAFObject.h"

class ImplAAFMetaDefinition;
class ImplAAFClassDef;
class ImplAAFTypeDef;
class ImplAAFMetaDefinition;
class ImplAAFTypeDefVariableArray;
class ImplAAFTypeDefFixedArray;
class ImplAAFTypeDefRecord;
class ImplAAFTypeDefRename;
class ImplAAFTypeDefStream;
class ImplAAFTypeDefString;
class ImplAAFTypeDefStrongObjRef;
class ImplAAFTypeDefWeakObjRef;
class ImplAAFTypeDefSet;

class ImplEnumAAFClassDefs;
class ImplEnumAAFTypeDefs;

class ImplAAFMetaDictionary :
  public ImplAAFObject
//  public OMClassFactory, 
//  public OMStorable
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFMetaDictionary ();

protected:
  virtual ~ImplAAFMetaDictionary ();

public:
  //
  // Create an instance of the appropriate derived class, given the
  // class id.  Initializes the OM properties.
  // 
  // This method implements the OMClassFactory interface.
  //
  //  OMStorable* create(const OMClassId& classId) const;

  //
  // This method implements the required OMStorable interface method
  //
  //  virtual const OMClassId& classId(void) const;

  // Override callback from OMStorable
  //  virtual void onSave(void* clientContext) const;



  //****************
  // CreateMetaInstance()
  //
  // Creates a single uninitialized AAF meta definition associated 
  // with a specified stored object id.
  virtual AAFRESULT STDMETHODCALLTYPE 
  CreateMetaInstance (
    // Stored Object ID of the meta object to be created.
    aafUID_constref classId,

    // Address of output variable that receives the 
    // object pointer requested in pAUID
    ImplAAFMetaDefinition ** ppMetaObject);


  //
  // Add class property and type access methods ...
  //


  //****************
  // LookupClassDef()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    LookupClassDef
        (// @parm [in,ref] Class Unique ID
         aafUID_constref classId,

         // @parm [out,retval] Class Definition
         ImplAAFClassDef ** ppClassDef);

  //****************
  // CreateForwardClassReference()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    CreateForwardClassReference 
        (// @parm [in] class identification
         aafUID_constref classId);

  //****************
  // IsForwardClassReference()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    HasForwardClassReference 
      (// @parm [in] class identification
       aafUID_constref classId,

       // @parm [out] kAAFTrue if the given class identification is a forward reference.
       aafBoolean_t *pResult);

  //****************
  // RegisterClassDef()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    RegisterClassDef
        (// @parm [in] Class Definition
         ImplAAFClassDef * pClassDef);


  //****************
  // GetClassDefs()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetClassDefs
        // @parm [out,retval] Class Definition Enumeration
        (ImplEnumAAFClassDefs ** ppEnum);


  //****************
  // CountClassDefs()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    CountClassDefs
        // @parm [out, retval] Total number of class definition objects
        (aafUInt32 * pResult);

  //****************
  // RegisterTypeDef()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    RegisterTypeDef
        (// @parm [in] Type Definition Object
         ImplAAFTypeDef * pTypeDef);

  //****************
  // LookupTypeDef()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    LookupTypeDef
        (// @parm [in,ref] Type Unique ID
         aafUID_constref typeID,

         // @parm [out,retval] Type Definition Object
         ImplAAFTypeDef ** ppTypeDef);


  //****************
  // GetTypeDefs()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetTypeDefs
        // @parm [out,retval] Type Def Enumeration
        (ImplEnumAAFTypeDefs ** ppEnum);


  //****************
  // CountTypeDefs()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    CountTypeDefs
        // @parm [out, retval] Total number of type definition objects
        (aafUInt32 * pResult);

  //****************
  // RegisterOpaqueTypeDef()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    RegisterOpaqueTypeDef
        (// @parm [in] Type Definition Object
         ImplAAFTypeDef * pOpaqueTypeDef);

  //****************
  // LookupOpaqueTypeDef()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    LookupOpaqueTypeDef
        (// @parm [in,ref] Type Unique ID
         aafUID_constref typeID,

         // @parm [out,retval] Type Definition Object
         ImplAAFTypeDef ** ppOpaqueTypeDef);

  //****************
  // GetOpaqueTypeDefs()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetOpaqueTypeDefs
        // @parm [out,retval] Type Def Enumeration
        (ImplEnumAAFTypeDefs ** ppEnum);


  //****************
  // CountOpaqueTypeDefs()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    CountOpaqueTypeDefs
        // @parm [out, retval] Total number of type definition objects
        (aafUInt32 * pResult);


  //
  // Meta definition factory methods:
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    CreateClassDef (
      aafUID_constref classID,
      aafCharacter_constptr pClassName,
      aafCharacter_constptr pDescription,
      ImplAAFClassDef * pParentClass,
      ImplAAFClassDef ** ppNewClass);

  virtual AAFRESULT STDMETHODCALLTYPE
   CreateTypeDefVariableArray (
      aafUID_constref typeID,
      aafCharacter_constptr pTypeName,
      aafCharacter_constptr pDescription,
      ImplAAFTypeDef *pElementType,
      ImplAAFTypeDefVariableArray ** ppNewVariableArray);

  virtual AAFRESULT STDMETHODCALLTYPE
    CreateTypeDefFixedArray (
      aafUID_constref typeID,
      aafCharacter_constptr pTypeName,
      aafCharacter_constptr pDescription,
      ImplAAFTypeDef *pElementType,
      aafUInt32  nElements,
      ImplAAFTypeDefFixedArray **pNewFixedArray);

  virtual AAFRESULT STDMETHODCALLTYPE
    CreateTypeDefRecord (
      aafUID_constref typeID,
      aafCharacter_constptr pTypeName,
      aafCharacter_constptr pDescription,
      ImplAAFTypeDef ** ppMemberTypes,
      aafCharacter_constptr * pMemberNames,
      aafUInt32 numMembers,
      ImplAAFTypeDefRecord ** ppNewRecord);

  virtual AAFRESULT STDMETHODCALLTYPE
    CreateTypeDefRename (
      aafUID_constref typeID,
      aafCharacter_constptr pTypeName,
      aafCharacter_constptr pDescription,
      ImplAAFTypeDef *pBaseType,
      ImplAAFTypeDefRename ** ppNewRename);

  virtual AAFRESULT STDMETHODCALLTYPE
   CreateTypeDefStream (
      aafUID_constref typeID,
      aafCharacter_constptr pTypeName,
      aafCharacter_constptr pDescription,
      ImplAAFTypeDef *pElementType,
      ImplAAFTypeDefStream ** ppNewStream);

  virtual AAFRESULT STDMETHODCALLTYPE
   CreateTypeDefString (
      aafUID_constref typeID,
      aafCharacter_constptr pTypeName,
      aafCharacter_constptr pDescription,
      ImplAAFTypeDef *pElementType,
      ImplAAFTypeDefString ** ppNewString);

  virtual AAFRESULT STDMETHODCALLTYPE
   CreateTypeDefStrongObjRef (
      aafUID_constref typeID,
      aafCharacter_constptr pTypeName,
      aafCharacter_constptr pDescription,
      ImplAAFClassDef * pTargetObjType,
      ImplAAFTypeDefStrongObjRef ** ppNewStrongObjRef);

  virtual AAFRESULT STDMETHODCALLTYPE
   CreateTypeDefWeakObjRef (
      aafUID_constref typeID,
      aafCharacter_constptr pTypeName,
      aafCharacter_constptr pDescription,
      ImplAAFClassDef * pTargetObjType,
      aafUID_constptr * pTargetHint,
      aafUInt32 targetHintCount,
      ImplAAFTypeDefWeakObjRef ** ppNewWeakObjRef);

  virtual AAFRESULT STDMETHODCALLTYPE
   CreateTypeDefStrongObjRefVector (
      aafUID_constref typeID,
      aafCharacter_constptr pTypeName,
      aafCharacter_constptr pDescription,
      ImplAAFTypeDefStrongObjRef * pStrongObjRef,
      ImplAAFTypeDefVariableArray ** ppNewStrongObjRefVector);

  virtual AAFRESULT STDMETHODCALLTYPE
   CreateTypeDefWeakObjRefVector (
      aafUID_constref typeID,
      aafCharacter_constptr pTypeName,
      aafCharacter_constptr pDescription,
      ImplAAFTypeDefWeakObjRef * pWeakObjRef,
      ImplAAFTypeDefVariableArray ** ppNewWeakObjRefVector);

  virtual AAFRESULT STDMETHODCALLTYPE
   CreateTypeDefStrongObjRefSet (
      aafUID_constref typeID,
      aafCharacter_constptr pTypeName,
      aafCharacter_constptr pDescription,
      ImplAAFTypeDefStrongObjRef * pStrongObjRef,
      ImplAAFTypeDefSet ** ppNewStrongObjRefSet);

  virtual AAFRESULT STDMETHODCALLTYPE
   CreateTypeDefWeakObjRefSet (
      aafUID_constref typeID,
      aafCharacter_constptr pTypeName,
      aafCharacter_constptr pDescription,
      ImplAAFTypeDefWeakObjRef * pWeakObjRef,
      ImplAAFTypeDefSet ** ppNewWeakObjRefSet);

public:
  // These are low-level OMSet tests for containment.
  bool containsClass(aafUID_constref classId);
  bool containsType(aafUID_constref typeId);


  bool hasForwardClassReference(aafUID_constref classId);
  // If the given classId fromt the set of forward references.
  void RemoveForwardClassReference(aafUID_constref classId);


  ImplAAFTypeDef * findOpaqueTypeDefinition(aafUID_constref typeId);



private:
  OMStrongReferenceSetProperty<OMUniqueObjectIdentification, ImplAAFTypeDef> _typeDefinitions;
  OMStrongReferenceSetProperty<OMUniqueObjectIdentification, ImplAAFClassDef> _classDefinitions;

  // Private class that represents an opaque class definition in an OMSet.
  class OpaqueTypeDefinition
  {
  public:
    OpaqueTypeDefinition();
    OpaqueTypeDefinition(const OpaqueTypeDefinition& rhs);
    OpaqueTypeDefinition(ImplAAFTypeDef * opaqueTypeDef);

    // coersion operator to "transparently" extract the type
    // definition pointer. This will be called when the enumerator
    // attempts to assign an OpaqueTypeDefinition to an ImplAAFTypeDef *.
    operator ImplAAFTypeDef * () const;

    // Methods required by OMSet
    const OMUniqueObjectIdentification identification(void) const;
    OpaqueTypeDefinition& operator= (const OpaqueTypeDefinition& rhs);
    bool operator== (const OpaqueTypeDefinition& rhs);

  private:
    ImplAAFTypeDef * _opaqueTypeDef;
  };

  OMSet<OMUniqueObjectIdentification, OpaqueTypeDefinition> _opaqueTypeDefinitions;

  // Private class that represents a forward class reference.
  class ForwardClassReference
  {
  public:
    ForwardClassReference();
    ForwardClassReference(const ForwardClassReference& rhs);
    ForwardClassReference(aafUID_constref classId);

    // Methods required by OMSet
    const OMUniqueObjectIdentification identification(void) const;
    ForwardClassReference& operator= (const ForwardClassReference& rhs);
    bool operator== (const ForwardClassReference& rhs);

  private:
    aafUID_t _classId;
  };

  OMSet<OMUniqueObjectIdentification, ForwardClassReference> _forwardClassReferences;


};


#endif // #ifndef __ImplAAFMetaDictionary_h__
