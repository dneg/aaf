//@doc
//@class    AAFDictionary | Implementation class for AAFDictionary
#ifndef __ImplAAFDictionary_h__
#define __ImplAAFDictionary_h__

/***********************************************************************
 *
 *              Copyright (c) 1998-1999 Avid Technology, Inc.
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


class ImplAAFBuiltinClasses;
class ImplAAFBuiltinDefs;
class ImplAAFBuiltinTypes;
class ImplAAFClassDef;
class ImplAAFCodecDef;
class ImplAAFContainerDef;
class ImplAAFDataDef;
class ImplAAFInterpolationDef;
class ImplAAFOperationDef;
class ImplAAFPluginDescriptor;
class ImplAAFTypeDef;
class ImplAAFTypeDefEnum;
class ImplAAFTypeDefRecord;
class ImplEnumAAFClassDefs;
class ImplEnumAAFCodecDefs;
class ImplEnumAAFContainerDefs;
class ImplEnumAAFDataDefs;
class ImplEnumAAFInterpolationDefs;
class ImplEnumAAFOperationDefs;
class ImplEnumAAFPluggableDefs;
class ImplEnumAAFPluginDescriptors;
class ImplEnumAAFTypeDefs;

#ifndef __ImplAAFObject_h__
#include "ImplAAFObject.h"
#endif

#include "OMClassFactory.h"


#include "ImplAAFClassDef.h"
#include "ImplAAFOperationDef.h"
#include "ImplAAFDefObject.h"
#include "ImplAAFCodecDef.h"
#include "ImplAAFContainerDef.h"
#include "ImplAAFInterpolationDef.h"
#include "ImplAAFTypeDef.h"
#include "ImplAAFDataDef.h"

class ImplAAFDictionary :
  public OMClassFactory, 
  public ImplAAFObject
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFDictionary ();

protected:
  virtual ~ImplAAFDictionary ();

public:
  // Create an instance of the appropriate derived class, given the
  // class id.  Initializes the OM properties.
  // 
  // This method implements the OMClassFactory interface.
  //
  OMStorable* create(const OMClassId& classId) const;


  //****************
  // CreateInstance()
  //
  // Creates a single uninitialized AAF object of the class associated 
  // with a specified stored object id.
  virtual AAFRESULT STDMETHODCALLTYPE 
  CreateInstance (
    // ID of the stored object to be created.
    aafUID_constref classId,

    // Address of output variable that receives the 
    // object pointer requested in pAUID
    ImplAAFObject ** ppvObject);


  //****************
  // LookupClassDef()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    LookupClassDef
        (// @parm [in,ref] Class Unique ID
         const aafUID_t & classId,

         // @parm [out,retval] Class Definition
         ImplAAFClassDef ** ppClassDef);


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
         const aafUID_t & typeID,

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
         const aafUID_t & typeID,

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

  //****************
  // RegisterDataDef()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    RegisterDataDef
        // @parm [in] Data Definition Object
        (ImplAAFDataDef * pDataDef);

  //****************
  // LookupDataDef()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    LookupDataDef
        (// @parm [in,ref] Datak Definition Unique ID
         const aafUID_t & dataDefinitionId,

         // @parm [out,retval] Data Definition Object
         ImplAAFDataDef ** ppDataDef);

  //****************
  // GetDataDefs()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetDataDefs
        // @parm [out,retval] Definition Enumeration
        (ImplEnumAAFDataDefs ** ppEnum);


  //****************
  // CountDataDefs()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    CountDataDefs
        // @parm [out, retval] Total number of data definition objects
        (aafUInt32 * pResult);

  //****************
  // RegisterOperationDef()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    RegisterOperationDef
        // @parm [in] Effect Definition Object
        (ImplAAFOperationDef * pOperationDef);

  //****************
  // LookupOperationDef()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    LookupOperationDef
        (// @parm [in,ref] Effect Unique ID
         const aafUID_t & effectId,

         // @parm [out,retval] Effect definition object
         ImplAAFOperationDef ** ppOperationDef);

  //****************
  // GetOperationDefs()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetOperationDefs
        // @parm [out,retval] Definition Enumeration
        (ImplEnumAAFOperationDefs ** ppEnum);


  //****************
  // CountOperationDefs()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    CountOperationDefs
        // @parm [out, retval] Total number of interpolation definition objects
        (aafUInt32 * pResult);


  //****************
  // RegisterParameterDef()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    RegisterParameterDef
        // @parm [in] Parameter Definition Object
        (ImplAAFParameterDef * pParameterDef);

  //****************
  // LookupParameterDef()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    LookupParameterDef
        (// @parm [in,ref] Parameter Unique ID
         const aafUID_t & parameterId,

         // @parm [out,retval] Parameter definition object
         ImplAAFParameterDef ** ppParameterDef);

  //****************
  // GetParameterDefs()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetParameterDefs
        // @parm [out,retval] Definition Enumeration
        (ImplEnumAAFParameterDefs ** ppEnum);

  //****************
  // CountParameterDefs()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    CountParameterDefs
        // @parm [out, retval] Total number of parameter definition objects
        (aafUInt32 * pResult);

  //****************
  // RegisterCodecDef()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    RegisterCodecDef
        // @parm [in] Pluggable Definition
        (ImplAAFCodecDef * pPlugDef);

  //****************
  // GetCodecDefs()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
   GetCodecDefs
      // @parm [out,retval] Definition Enumeration
      (ImplEnumAAFCodecDefs ** ppEnum);


  virtual AAFRESULT STDMETHODCALLTYPE
    LookupCodecDef
      (const aafUID_t & containerId,
	   ImplAAFCodecDef **result);


  //****************
  // CountCodecDefs()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    CountCodecDefs
        // @parm [out, retval] Total number of codec definition objects
        (aafUInt32 * pResult);


  //****************
  // RegisterContainerDef()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    RegisterContainerDef
        // @parm [in] Pluggable Definition
        (ImplAAFContainerDef * pPlugDef);


  //****************
  // GetContainerDefs()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetContainerDefs
        // @parm [out,retval] Definition Enumeration
       (ImplEnumAAFContainerDefs ** ppEnum);


  //****************
  // LookupContainerDef()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
     LookupContainerDef
       (const aafUID_t & containerID,
		ImplAAFContainerDef **result);


  //****************
  // CountContainerDefs()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    CountContainerDefs
        // @parm [out, retval] Total number of container definition objects
        (aafUInt32 * pResult);


  //****************
  // RegisterInterpolationDef()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    RegisterInterpolationDef
        // @parm [in] Interpolation Definition Object
        (ImplAAFInterpolationDef * pInterpolationDef);

  //****************
  // LookupInterpolationDef()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    LookupInterpolationDef
        (// @parm [in,ref] Interpolation Unique ID
         const aafUID_t & interpolationID,

         // @parm [out,retval] Interpolation definition object
         ImplAAFInterpolationDef ** ppInterpolationDef);

  //****************
  // GetInterpolationDefs()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetInterpolationDefs
        // @parm [out,retval] Definition Enumeration
        (ImplEnumAAFInterpolationDefs ** ppEnum);

  //****************
  // CountInterpolationDefs()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    CountInterpolationDefs
        // @parm [out, retval] Total number of interpolation definition objects
        (aafUInt32 * pResult);


  //****************
  // RegisterPluginDef()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    RegisterPluginDef
        (ImplAAFPluginDescriptor * pPluginDesc);

  //****************
  // LookupPluginDef()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    LookupPluginDef
        (// @parm [in,ref] Interpolation Unique ID
         const aafUID_t & interpolationID,

         ImplAAFPluginDescriptor ** ppPluginDesc);

  //****************
  // GetPluginDefs()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetPluginDefs
        (ImplEnumAAFPluginDescriptors ** ppEnum);


  //****************
  // CountPluginDefs()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    CountPluginDefs
        // @parm [out, retval] Total number of plugin definition objects
        (aafUInt32 * pResult);

public:

  //
  // Internal to the SDK
  //
#if USE_NEW_OBJECT_CREATION

  //****************
  // CreateImplClassDef() (not in the public API
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    CreateImplClassDef (
      aafUID_constref classID,
      ImplAAFClassDef * pParentClass,
      aafCharacter_constptr pClassName,
      ImplAAFClassDef ** ppClassDef);


  //****************
  // CreateClassDef()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    CreateClassDef (
      aafUID_constref classID,
      ImplAAFClassDef * pParentClass,
      aafCharacter_constptr pClassName,
      ImplAAFClassDef ** ppClassDef);

#endif // #if USE_NEW_OBJECT_CREATION


  //****************
  // GetNumEssenceData()
  //
  virtual AAFRESULT
    GetNumCodecDefs
        (aafUInt32 *  pNumCodecDefs);  //@parm [out,retval] Total number of pluggable defs

  virtual AAFRESULT
    GetNumContainerDefs
        (aafUInt32 *  pNumContainerDefs);  //@parm [out,retval] Total number of pluggable defs

  // make sure built-in types are initialized.
  void InitBuiltins();

  //
  // Toolkit-internal "Create" methods
  //

  // Factory method for creating a Dictionary.
  static ImplAAFDictionary *CreateDictionary(void);

  //
  // Instantiates an object of the given class and initializes its OM
  // properties.
  //
  ImplAAFObject * CreateAndInit(ImplAAFClassDef * pClassDef) const;

  //
  // Instantiates the most derived class known to the code which
  // represents the given AUID.  Doesn't init its OM properties.
  //
  ImplAAFObject* pvtInstantiate(const aafUID_t & id) const;

  //
  // If the given AUID is known to the code, instantiates an object of
  // that type.  Otherwise, returns NULL.  Doesn't init its OM
  // properties.
  //
  static ImplAAFObject* pvtCreateBaseClassInstance(const aafUID_t & id);


  // Generates an OM PID corresponding to the given property def auid.
  AAFRESULT GenerateOmPid (const aafUID_t & rAuid,
						   OMPropertyId & rOutPid);


  // Like LookupClassDef(), except will only look at classes currently
  // registered in this dictionary; will not attempt to look at
  // builtins which may not have already been entered into the dict.
  AAFRESULT dictLookupClassDef
    (// @parm [in,ref] Class Unique ID
	 const aafUID_t & classID,

	 // @parm [out,retval] Class Definition
	 ImplAAFClassDef ** ppClassDef);

bool PvtIsClassPresent (
      const aafUID_t & classID);

  // Like LookupType(), except will only look at types currently
  // registered in this dictionary; will not attempt to look at
  // builtins which may not have already been entered into the dict.
  AAFRESULT dictLookupTypeDef
    (// @parm [in,ref] Type Unique ID
	 const aafUID_t & typeID,

	 // @parm [out,retval] Type Definition Object
	 ImplAAFTypeDef ** ppTypeDef);

bool PvtIsTypePresent (
      const aafUID_t & typeID);

  // Attempt to register the sizes of this type def if it is a
  // built-in type.  Currently implemented for Enum and Record
  // typedefs.
  void pvtAttemptBuiltinSizeRegistration (ImplAAFTypeDefEnum * ptde) const;
  void pvtAttemptBuiltinSizeRegistration (ImplAAFTypeDefRecord * ptdr) const;

  // Assures that all prop types in all contained classes are present,
  // as well as the prop types in all classes looked up in the
  // future.
  void AssureClassPropertyTypes ();
  void AssurePropertyTypes (ImplAAFClassDef * pcd);

  // Enables/disables registration of definitions.  Used during save()
  // to assure contents of dict remains constant.  Returns current
  // value of flag.  Default is enabled.
  bool SetEnableDefRegistration (bool isEnabled);

  bool IsAxiomaticClass (const aafUID_t & classID) const;

  ImplAAFBuiltinDefs * GetBuiltinDefs ();
  void SetBuiltinClasses(ImplAAFBuiltinClasses *pBuiltinClasses) { _pBuiltinClasses = pBuiltinClasses; };

private:

  bool pvtLookupAxiomaticTypeDef (const aafUID_t & typeID,
							   ImplAAFTypeDef ** ppTypeDef);

  bool pvtLookupAxiomaticClassDef (const aafUID_t & classID,
								   ImplAAFClassDef ** ppClassDef);

  ImplAAFBuiltinClasses * _pBuiltinClasses;
  ImplAAFBuiltinTypes   * _pBuiltinTypes;
  ImplAAFBuiltinDefs    * _pBuiltinDefs;

  OMStrongReferenceSetProperty<ImplAAFCodecDef>				_codecDefinitions;
  OMStrongReferenceSetProperty<ImplAAFContainerDef>			_containerDefinitions;
  OMStrongReferenceSetProperty<ImplAAFOperationDef>			_operationDefinitions;
  OMStrongReferenceSetProperty<ImplAAFParameterDef>			_parameterDefinitions;
  OMStrongReferenceSetProperty<ImplAAFTypeDef>				_typeDefinitions;
  OMStrongReferenceSetProperty<ImplAAFClassDef>				_classDefinitions;
  OMStrongReferenceSetProperty<ImplAAFInterpolationDef>		_interpolationDefinitions;
  OMStrongReferenceSetProperty<ImplAAFDataDef>				_dataDefinitions;
  OMStrongReferenceSetProperty<ImplAAFPluginDescriptor>		_pluginDefinitions;

	ImplAAFTypeDef **_opaqueTypeDefinitions;

  aafInt32 _lastGeneratedPid;	// must be signed!

  static const aafUID_t * sAxiomaticTypeGuids[];
  static const aafUID_t * sAxiomaticClassGuids[];

  ImplAAFTypeDefSP * _axiomaticTypes;

  struct axClassLookupElem
  {
	ImplAAFClassDef * pClassDef;
	aafUID_t          classId;
  };

  axClassLookupElem * _axiomaticClasses;

  // if true, signifies that it's OK to assure that all prop types in
  // all looked-up classes are present.
  bool _OKToAssurePropTypes;

  bool _defRegistrationAllowed;
};

//
// smart pointer
//

#ifndef __ImplAAFSmartPointer_h__
// caution! includes assert.h
#include "ImplAAFSmartPointer.h"
#endif

typedef ImplAAFSmartPointer<ImplAAFDictionary> ImplAAFDictionarySP;

#endif // ! __ImplAAFDictionary_h__
