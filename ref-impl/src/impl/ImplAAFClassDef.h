//@doc
//@class    AAFClassDef | Implementation class for AAFClassDef
#ifndef __ImplAAFClassDef_h__
#define __ImplAAFClassDef_h__

/***********************************************\
*                                               *
* Advanced Authoring Format                     *
*                                               *
* Copyright (c) 1998-1999 Avid Technology, Inc. *
* Copyright (c) 1998-1999 Microsoft Corporation *
*                                               *
\***********************************************/

class ImplEnumAAFPropertyDefs;
class ImplAAFDefObject;
class ImplAAFTypeDef;

#ifndef __ImplAAFDefObject_h__
#include "ImplAAFDefObject.h"
#endif

#ifndef __ImplAAFPropertyDef_h__
#include "ImplAAFPropertyDef.h"
#endif


class ImplAAFClassDef : public ImplAAFDefObject
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
  // GetPropertyDefs()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Initialize
        // @parm [in] auid to be used to identify this type
        (const aafUID_t * pID,

		// Inheritance parent of this class
		ImplAAFClassDef * pParentClass,

		// Human-legible name
		const aafCharacter * pClassName);


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
  // AppendNewPropertyDef()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    AppendNewPropertyDef
        (// @parm [in] auid to be used to identify this property
         aafUID_t *  pID,

         // @parm [in, string] name of the new property
         wchar_t *  pName,

         // @parm [in] type of the new property
         ImplAAFTypeDef * pTypeDef,

         // @parm [in] true if new property is to be optional
         aafBool  isOptional,

         // @parm [out] return pointer to newly created property def
         ImplAAFPropertyDef ** ppPropDef);


  //****************
  // AppendOpionalPropertyDef()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    AppendOptionalPropertyDef
        (// @parm [in] auid to be used to identify this property
         aafUID_t *  pID,

         // @parm [in, string] name of the new property
         wchar_t *  pName,

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
         aafUID_t *  pPropID,

         // @parm [out] resulting property definition
         ImplAAFPropertyDef ** ppPropDef);


  //****************
  // GetParent()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetParent
        // @parm [out, retval] parent class definition
        (ImplAAFClassDef ** ppClassDef);


public:

  //
  // Non-published methods
  //

  //****************
  // Similar to AppendNewPropertyDef(), except it does not require
  // that class be unregistered.
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    pvtAppendNewPropertyDef
        (// @parm [in] auid to be used to identify this property
         aafUID_t *  pID,

         // @parm [in, string] name of the new property
         wchar_t *  pName,

         // @parm [in] type of the new property
         ImplAAFTypeDef * pTypeDef,

         // @parm [in] true if new property is to be optional
         aafBool  isOptional,

         // @parm [out] return pointer to newly created property def
         ImplAAFPropertyDef ** ppPropDef);


  // Declare this class to be storable.
  //
  OMDECLARE_STORABLE(ImplAAFClassDef)


private:

  // Private method to unconditionally append a property def (ignoring
  // whether or not property is optional or not, or if this class has
  // already been registered).
  AAFRESULT STDMETHODCALLTYPE
    pvtAppendPropertyDef
        (aafUID_t *  pID,
         wchar_t *  pName,
         ImplAAFTypeDef * pTypeDef,
         aafBool  isOptional,
         ImplAAFPropertyDef ** ppPropDef);


  // OMWeakReferenceProperty<ImplAAFClassDef> _ParentClass;
  OMFixedSizeProperty<aafUID_t>                       _ParentClass;

  OMStrongReferenceVectorProperty<ImplAAFPropertyDef> _Properties;
};

#endif // ! __ImplAAFClassDef_h__
