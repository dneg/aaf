//@doc
//@class    AAFPropertyDef | Implementation class for AAFPropertyDef
#ifndef __ImplAAFPropertyDef_h__
#define __ImplAAFPropertyDef_h__

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

class ImplEnumAAFPropertyValues;
class ImplAAFPropertyValue;
class ImplAAFTypeDef;

#ifndef __ImplAAFMetaDefinition_h__
#include "ImplAAFMetaDefinition.h"
#endif

#include "OMPropertyDefinition.h"
#include "OMVariableSizeProperty.h"

typedef OMProperty* (*ImplAAFOMPropertyCreateFunc_t)
  (OMPropertyId pid,
   const char * name);

class ImplAAFPropertyDef : public ImplAAFMetaDefinition,
						   public OMPropertyDefinition
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
  // IsUniqueIdentifier()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetIsUniqueIdentifier
      (// @parm [out] pointer to the result
       aafBool * pIsUniqueIdentifier) const;


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
  // pvtInitialize()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    pvtInitialize
       (// @parm [in] auid to be used to identify this property definition
        const aafUID_t & propertyAuid,
			
        // @parm [in] OM pid (small integer) to be used to identify
		// this property definition
        OMPropertyId omPid,
			
        // @parm [in, string] friendly name of this property
	    const aafCharacter * pPropName,
	
        // @parm [in] Type definition of this property definition,
	    const aafUID_t & typeID,

        // @parm [in] Is this property optional? (mandatory, if not)
		  aafBoolean_t isOptional,

        // @parm [in] Is this property a unique identifier
		  aafBoolean_t isUniqueIdentifier);



  OMPropertyId OmPid (void) const;
  //
  // Returns the OM pid (small integer) identifying this property

  //
  // Overrides from OMPropertyDefinition
  //
  const OMType* type(void) const;
  const char* name(void) const;
  OMPropertyId localIdentification(void) const;
  bool isOptional(void) const;

  // Allocates and returns an OMProperty which can represent this
  // property.  
  OMProperty * CreateOMProperty () const;

  // Sets the function which creates OMProperties useful for these
  // properties.
  void SetOMPropCreateFunc (ImplAAFOMPropertyCreateFunc_t pFunc);

private:

  // OMWeakReferenceProperty<ImplAAFTypeDef> _Type;
  OMFixedSizeProperty<aafUID_t>              _Type;

  OMFixedSizeProperty<aafBool>               _IsOptional;

  OMFixedSizeProperty<OMPropertyId>          _pid;

  OMVariableSizeProperty<aafUInt8>			 _DefaultValue;

  OMFixedSizeProperty<aafBool>               _IsUniqueIdentifier;

  ImplAAFTypeDef *                           _cachedType;

  char * _bname;  // name in byte-sized characters

  ImplAAFOMPropertyCreateFunc_t              _OMPropCreateFunc;

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
