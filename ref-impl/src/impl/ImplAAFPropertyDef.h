//@doc
//@class    AAFPropertyDef | Implementation class for AAFPropertyDef
#ifndef __ImplAAFPropertyDef_h__
#define __ImplAAFPropertyDef_h__

/***********************************************\
*                                               *
* Advanced Authoring Format                     *
*                                               *
* Copyright (c) 1998-1999 Avid Technology, Inc. *
* Copyright (c) 1998-1999 Microsoft Corporation *
*                                               *
\***********************************************/

class ImplEnumAAFPropertyValues;
class ImplAAFPropertyValue;
class ImplAAFTypeDef;

#ifndef __ImplAAFDefObject_h__
#include "ImplAAFDefObject.h"
#endif


class ImplAAFPropertyDef : public ImplAAFDefObject
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
  // GetIsSearchable()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetIsSearchable
        // @parm [out, retval] pointer to the result
        (aafBool *  pIsSearchable);


  //****************
  // SetIsSearchable()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetIsSearchable
        // @parm [in] is searchable value
        (aafBool  IsSearchable);


  //****************
  // GetDefaultValue()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetDefaultValue
        // @parm [out, retval] Pointer to default data value
        (ImplAAFPropertyValue ** ppDataValue);


  //****************
  // SetDefaultValue()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetDefaultValue
        // @parm [in] default data value
        (ImplAAFPropertyValue * pDataValue);


  //
  // Non-published methods (yet still public)
  //

  //****************
  // Initialize()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Initialize
       (// @parm [in] auid to be used to identify this property definition
        const aafUID_t * pPropertyAuid,
			
        // @parm [in] OM pid (small integer) to be used to identify
		// this property definition
        OMPropertyId omPid,
			
        // @parm [in, string] friendly name of this property
	    wchar_t * pPropName,
	
        // @parm [in] Type definition of this property definition,
	    ImplAAFTypeDef * pTypeDef,

        // @parm [in] Is this property optional? (mandatory, if not)
		aafBool isOptional);



  OMPropertyId OmPid (void) const;
  //
  // Returns the OM pid (small integer) identifying this property

public:
  // Declare this class to be storable.
  //
  OMDECLARE_STORABLE(ImplAAFPropertyDef)

private:

  // OMWeakReferenceProperty<ImplAAFTypeDef> _Type;
  OMFixedSizeProperty<aafUID_t>              _Type;

  OMFixedSizeProperty<aafBool>               _IsOptional;

  OMFixedSizeProperty<OMPropertyId>          _pid;

  // don't use shortcut in an attempt to avoid including typeDef header
  ImplAAFSmartPointer<ImplAAFTypeDef> _cachedType;
};

//
// smart pointer
//

#ifndef __ImplAAFSmartPointer_h__
// caution! includes assert.h
#include "ImplAAFSmartPointer.h"
#endif

typedef ImplAAFSmartPointer<ImplAAFPropertyDef> ImplAAFPropertyDefSP;

#endif // ! __ImplAAFPropertyDef_h__
