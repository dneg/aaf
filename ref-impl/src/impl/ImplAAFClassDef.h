//@doc
//@class    AAFClassDef | Implementation class for AAFClassDef
#ifndef __ImplAAFClassDef_h__
#define __ImplAAFClassDef_h__

/***********************************************\
*                                               *
* Advanced Authoring Format                     *
*                                               *
* Copyright (c) 1998-1999 Avid Technology, Inc. *
*                                               *
\***********************************************/

class ImplEnumAAFPropertyDefs;
class ImplAAFDefObject;
class ImplAAFTypeDef;
class ImplAAFPropertyDef;

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
  // Initialize()
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
         ImplAAFPropertyDef ** ppPropDef) const;


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


  //****************
  // LookupPropertyDefbyOMPid()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    LookupPropertyDefbyOMPid
        (// @parm [in] pid reprepresenting property to look up
         OMPropertyId omPid,

         // @parm [out] resulting property definition
         ImplAAFPropertyDef ** ppPropDef) const;


  // Returns the AUID of the parent class.  Returns the NULL auid if
  // this is the end of the line.
  void pvtGetParentAUID (aafUID_t & result);


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


  //
  // Private implementation classes to share lookup code between
  // methods which lookup by auid, and ones which look up by OMPid.
  //
  // Abstract base class which allows comparison of a property
  // identifier to a property definition.
  class pvtPropertyIdentifier
  {
  public:
	// Returns AAFTrue if this property identifier matches the given
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


  //
  // The generalized lookup method which uses a pvtPropertyIdentifier
  // as the property ID.
  //
  AAFRESULT STDMETHODCALLTYPE
    generalLookupPropertyDef (
      const pvtPropertyIdentifier & propId,
      ImplAAFPropertyDef ** ppPropDef);


  // OMWeakReferenceProperty<ImplAAFClassDef> _ParentClass;
  OMFixedSizeProperty<aafUID_t>                       _ParentClass;

  OMStrongReferenceVectorProperty<ImplAAFPropertyDef> _Properties;

  // didn't use shorthand here in an attempt to avoid circular references
  ImplAAFSmartPointer<ImplAAFClassDef> _cachedParentClass;
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
