//@doc
//@class    AAFObject | Implementation class for AAFObject
#ifndef __CAAFObject_h__
#define __CAAFObject_h__

//=---------------------------------------------------------------------=
//
// This file was GENERATED for the AAF SDK
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



#ifndef __AAF_h__
#include "AAF.h"
#endif













#ifndef __CAAFRoot_h__
#include "CAAFRoot.h"
#endif


class CAAFObject
  : public IAAFObject,
    public CAAFRoot
{
protected:

  //********
  //
  // Constructor/destructor
  //
  CAAFObject (IUnknown * pControllingUnknown, aafBool doInit = kAAFTrue);
  virtual ~CAAFObject ();

public:


  //***********************************************************
  //
  // GetGeneration()
  //
  // Gets the generation of this object, which is represented by an
  // AAFIdentification object.  This is used to detect when an object
  // has been modified.
  //
  // This method will succeed if generation tracking is enabled for
  // this object.  Call EnableGenerationTrackint() and
  // DisableGenerationTrackint() to control generation tracking for
  // this object.  Call IsGenerationTracked() to determine if
  // generation tracking is currently enabled for this object.
  //
  // Succeeds if all of the following are true:
  // - the given pGeneration pointer is valid.
  // - generation tracking is enabled for this object.
  // - this object is attached to a file.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - ppGeneration arg is NULL.
  //
  // AAFRESULT_INVALID_PARAM
  //   - Generation tracking is not enabled for this object.
  //
  // AAFRESULT_OBJECT_NOT_ATTACHED
  //   - This object is not attached to a file from which generation
  //     information can be obtained..
  //
  STDMETHOD (GetGeneration) (
    // AAFIdentification object corresponding to this object's
    // Generation ID 
    /*[out]*/ IAAFIdentification ** ppGeneration);


  //***********************************************************
  //
  // GetGenerationAUID()
  //
  // Shortcut to get the AUID representing the Identification
  // representing this object's generation.  (Can also be obtained by
  // calling this->GetGeneration(&pIdent), and using its result to
  // call pIdent->GetAuid()).
  //
  // This method will succeed if generation tracking is enabled for
  // this object.  Call EnableGenerationTrackint() and
  // DisableGenerationTrackint() to control generation tracking for
  // this object.  Call IsGenerationTracked() to determine if
  // generation tracking is currently enabled for this object.
  //
  // Succeeds if all of the following are true:
  // - the given pResult pointer is valid.
  // - generation tracking is enabled for this object.
  // - this object is attached to a file.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pResult arg is NULL.
  //
  // AAFRESULT_INVALID_PARAM
  //   - Generation tracking is not enabled for this object.
  //
  // AAFRESULT_OBJECT_NOT_ATTACHED
  //   - This object is not attached to a file from which generation
  //     information can be obtained..
  //
  STDMETHOD (GetGenerationAUID) (
    // AUID corresponding to this object's Generation ID 
    /*[out]*/ aafUID_t *  pResult);


  //***********************************************************
  //
  // GetDefinition()
  //
  // Returns the class definition which describes this object
  // instance.
  //
  // Succeeds if all of the following are true:
  // - the given ppClassDef pointer is valid.
  //
  // Note! Use care when dealing with the object class.  Among the
  // pitfalls to be avoided is that tests for equality will not
  // reflect inheritance.  This becomes important if an unknown
  // non-builtin (that is, user defined) object class ID is obtained.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - ppClassDef arg is NULL.
  //
  STDMETHOD (GetDefinition) (
    // class definition of which this object is an instance. 
    /*[out]*/ IAAFClassDef ** ppClassDef);


  //***********************************************************
  //
  // GetProperties()
  //
  // Returns an enumerator across all properties actually contained in
  // this object.  Each property is represented by an IAAFProperty
  // interface.
  // 
  //
  // *ppEnum is AddRef()ed before it is returned.
  // Succeeds if all of the following are true:
  // - the ppEnum pointer is valid.
  // 
  // If this method fails nothing will be written to *ppEnum.
  // 
  // Note! This is a low-level method which allows direct access to
  // properties.  If such access is done, any semantic checking (such
  // as that which is performed in all other named property Get/Set
  // methods) is not done here.  Users must use this method at their
  // own risk.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - ppEnum arg is NULL.
  //
  STDMETHOD (GetProperties) (
    // Property Enumeration 
    /*[out, retval]*/ IEnumAAFProperties ** ppEnum);


  //***********************************************************
  //
  // CountProperties()
  //
  // Returns the number of properties currently present in this
  // object.  This is the same number as will be accessed through
  // GetProperties().
  // 
  // Note! This is a low-level method which allows direct access to
  // properties.  If such access is done, any semantic checking (such
  // as that which is performed in all other named property Get/Set
  // methods) is not done here.  Users must use this method at their
  // own risk.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pCount arg is NULL.
  //
  STDMETHOD (CountProperties) (
    // count of properties present in this object 
    /*[out]*/ aafUInt32 *  pCount);


  //***********************************************************
  //
  // GetPropertyValue()
  //
  // Returns the requested Property Value.  The desired property data
  // is identified by the given property definition.
  // 
  // Note! This is a low-level method which allows direct access to
  // properties.  If such access is done, any semantic checking (such
  // as that which is performed in all other named property Get/Set
  // methods) is not done here.  Users must use this method at their
  // own risk.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pPropDef or ppPropVal arg is NULL.
  //
  // AAFRESULT_ILLEGAL_PROPERTY
  //   - named property illegal for this object's class.
  //
  // AAFRESULT_PROP_NOT_PRESENT
  //   - named property is optional, but not present in this class.
  //
  STDMETHOD (GetPropertyValue) (
    // property definition indentifying desired property
    /*[in]*/ IAAFPropertyDef * pPropDef,

    // returned AAFPropertyValue
    /*[out]*/ IAAFPropertyValue ** ppPropVal);


  //***********************************************************
  //
  // SetPropertyValue()
  //
  // Sets the value of the given property to the given value.  If the
  // selected property is optional but not yet present, will make the
  // property present before setting its value.
  //
  // Note! This is a low-level method which allows direct access to
  // properties.  If such access is done, any semantic checking (such
  // as that which is performed in all other named property Get/Set
  // methods) is not done here.  Users must use this method at their
  // own risk.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pPropDef or ppPropVal arg is NULL.
  //
  // AAFRESULT_ILLEGAL_PROPERTY
  //   - named property illegal for this object's class.
  //
  // AAFRESULT_PROP_NOT_PRESENT
  //   - named property is optional, but not present in this class.
  //
  STDMETHOD (SetPropertyValue) (
    // property definition indentifying desired property
    /*[in]*/ IAAFPropertyDef * pPropDef,

    // value to set.
    /*[in]*/ IAAFPropertyValue * pPropVal);


  //***********************************************************
  //
  // IsPropertyPresent()
  //
  // Sets *pResultReturns true in if named property is legal and is
  // present; sets it to false if it is legal and is absent.
  // 
  // Note! This is a low-level method which allows direct access to
  // properties.  If such access is done, any semantic checking (such
  // as that which is performed in all other named property Get/Set
  // methods) is not done here.  Users must use this method at their
  // own risk.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pResult arg is NULL.
  //
  // AAFRESULT_ILLEGAL_PROPERTY
  //   - named property illegal for this object's class.
  //
  STDMETHOD (IsPropertyPresent) (
    // property definition indentifying desired property
    /*[in]*/ IAAFPropertyDef * pPropDef,

    // true if present; false if not present
    /*[out]*/ aafBoolean_t*  pResult);


  //***********************************************************
  //
  // RemoveOptionalProperty()
  //
  // Removes the property if named property is legal and is
  // optional and present.
  // 
  // Note! This is a low-level method which allows direct access to
  // properties.  If such access is done, any semantic checking (such
  // as that which is performed in all other named property Get/Set
  // methods) is not done here.  Users must use this method at their
  // own risk.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pResult arg is NULL.
  //
  // AAFRESULT_ILLEGAL_PROPERTY
  //   - named property illegal for this object's class.
  //
  STDMETHOD (RemoveOptionalProperty) (
    // property definition indentifying desired property 
    /*[in]*/ IAAFPropertyDef * pPropDef);


  //***********************************************************
  //
  // CreateOptionalPropertyValue()
  //
  // Returns the requested Property Value.  The desired property data
  // is identified by the given optional property definition.
  // 
  // Note! This is a low-level method which allows direct access to
  // properties.  If such access is done, any semantic checking (such
  // as that which is performed in all other named property Get/Set
  // methods) is not done here.  Users must use this method at their
  // own risk.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pPropDef or ppPropVal arg is NULL.
  //
  // AAFRESULT_ILLEGAL_PROPERTY
  //   - named property illegal for this object's class.
  //
  // AAFRESULT_PROP_ALREADY_PRESENT
  //   - named property is optional, but already present in this class.
  //
  STDMETHOD (CreateOptionalPropertyValue) (
    // property definition indentifying desired property
    /*[in]*/ IAAFPropertyDef * pPropDef,

    // returned AAFPropertyValue
    /*[out]*/ IAAFPropertyValue ** ppPropVal);


  //***********************************************************
  //
  // GetDictionary()
  //
  // Returns the dictionary for this object instance.
  //
  // Succeeds if all of the following are true:
  // - the given ppDictionary pointer is valid.
  //
  // Note! Use care when dealing with the object class.  Among the
  // pitfalls to be avoided is that tests for equality will not
  // reflect inheritance.  This becomes important if an unknown
  // non-builtin (that is, user defined) object class ID is obtained.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - ppClassDef arg is NULL.
  //
  STDMETHOD (GetDictionary) (
    // A pointer to an Dictionary pointer 
    /*[out, retval]*/ IAAFDictionary ** ppDictionary);


  //***********************************************************
  //
  // EnableGenerationTracking()
  //
  // Calling this method will cause generation tracking to be enabled
  // for this object.  Generation information will then be available
  // through the GetGeneration() and GetGenerationAUID() methods.
  // 
  // Calling the DisableGenerationTracking() method will disable
  // generation tracking for this object.
  // 
  // Generation tracking is disabled by default.
  //
  // This method always succeeds, even if generation tracking is
  // already enabled.
  // 
  // This method will return the following result:
  //  - AAFRESULT_SUCCESS
  //
  STDMETHOD (EnableGenerationTracking)
     ();



  //***********************************************************
  //
  // DisableGenerationTracking()
  //
  // Calling this method will cause generation tracking to be disabled
  // for this object.  Generation information will then not be
  // available through the GetGeneration() and GetGenerationAUID()
  // methods.
  // 
  // Calling the EnableGenerationTracking() method will enable
  // generation tracking for this object.
  // 
  // Generation tracking is disabled by default.
  //
  // This method always succeeds, even if generation tracking is
  // already disbled.
  // 
  // This method will return the following result:
  //  - AAFRESULT_SUCCESS
  //
  STDMETHOD (DisableGenerationTracking)
     ();



  //***********************************************************
  //
  // IsGenerationTracked()
  //
  // This method will set *pResult to AAFTrue if generations are
  // being tracked for this object.  A result of AAFTrue indicates
  // that generation information will be available through the
  // GetGeneration() and GetGenerationAUID() methods.
  // 
  // Calling the EnableGenerationTracking() method will enable
  // generation tracking for this object; calling the
  // DisableGenerationTracking() method will disable generation
  // tracking for this object.
  // 
  // Generation tracking is disabled by default.
  //
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pResult arg is NULL.
  //
  STDMETHOD (IsGenerationTracked) (
    // set to true if generations are tracked; false otherwise 
    /*[out]*/ aafBoolean_t *  pResult);

private:
  //
  // Private class to encapsulate a list of extensions.
  //
  class Extension
  {
  public:
    Extension(REFCLSID rclsid);
    ~Extension();
    
    // Return the class id of this extension.
    REFCLSID GetCLSID(void) const;

    // Set or get the next extension.
    void SetNext(Extension *next);
    Extension * GetNext(void) const;

    // Attempt to create the given plugin extension.
    HRESULT InitializeExtension(IUnknown *pControllingUnknown);

    // 
    // Declare the QI that delegates to the QI of the pExtensionUnknown.
    // 
    HRESULT QueryInterface(REFIID riid, void **ppvObjOut);

  private:
    // The next extension in the list.
    Extension *pNext;

    // The code class id for this plugin extension. This
    // value is used to make sure that we do not attempt to
    // create duplicate extension objects.
    CLSID clsid;

    // Non-deleting unknown pointer for a plugin class extension.
    IUnknown *pExtensionUnknown;

    // State flag used by this class' associated extension's
    // QueryInterface has been called. (Used to stop a possibly
    // endless QI cycle: Oobject::QI -> Extension::QI -> Object::QI.)
    bool bExtensionQueryInterfaceInProgress;
  };

  // The first extension.
  CAAFObject::Extension *pExtension;

public:
  //
  // Intialize class extensions. This method is called after the
  // contained Impl object has been completely initialized. This
  // allows the aggregated extension object access to all of the
  // interfaces and property data of its controlling unknown.
  //
  STDMETHOD(InitializeExtension)(REFCLSID clsid);

protected:
  // 
  // Declare the QI that implements for the interfaces
  // for this module. This will be called by CAAFUnknown::QueryInterface().
  // 
  STDMETHOD(InternalQueryInterface)(REFIID riid, void **ppvObjOut);


public:
  //
  // This class as concrete. All AAF objects can be constructed from
  // a CLSID. This will allow subclassing all "base-classes" by
  // aggreggation.
  // 
  AAF_DECLARE_CONCRETE();
  //
  //********
};

#endif // ! __CAAFObject_h__


