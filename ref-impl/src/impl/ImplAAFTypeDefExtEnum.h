//@doc
//@class    AAFTypeDefExtEnum | Implementation class for AAFTypeDefExtEnum
#ifndef __ImplAAFTypeDefExtEnum_h__
#define __ImplAAFTypeDefExtEnum_h__


//=---------------------------------------------------------------------=
//
// $Id$ $Name$
//
// The contents of this file are subject to the AAF SDK Public Source
// License Agreement Version 2.0 (the "License"); You may not use this
// file except in compliance with the License.  The License is available
// in AAFSDKPSL.TXT, or you may obtain a copy of the License from the
// Advanced Media Workflow Association, Inc., or its successor.
//
// Software distributed under the License is distributed on an "AS IS"
// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
// the License for the specific language governing rights and limitations
// under the License.  Refer to Section 3.3 of the License for proper use
// of this Exhibit.
//
// WARNING:  Please contact the Advanced Media Workflow Association,
// Inc., for more information about any additional licenses to
// intellectual property covering the AAF Standard that may be required
// to create and distribute AAF compliant products.
// (http://www.amwa.tv/policies).
//
// Copyright Notices:
// The Original Code of this file is Copyright 1998-2009, licensor of the
// Advanced Media Workflow Association.  All rights reserved.
//
// The Initial Developer of the Original Code of this file and the
// licensor of the Advanced Media Workflow Association is
// Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

class ImplAAFPropertyValue;
class OMTypeVisitor;

#ifndef __ImplAAFTypeDef_h__
#include "ImplAAFTypeDef.h"
#endif

#include "AAFUtils.h"
#include "OMArrayProperty.h"
#include "OMExtendibleEnumeratedType.h"


class ImplAAFTypeDefExtEnum : public ImplAAFTypeDef, public OMExtendibleEnumeratedType
{
public:
	//
	// Constructor/destructor
	//
	//********
	ImplAAFTypeDefExtEnum ();
	
protected:
	virtual ~ImplAAFTypeDefExtEnum ();
	
public:
	
	//****************
	// Initialize()
	//
	virtual AAFRESULT STDMETHODCALLTYPE
		Initialize
        (// @parm [in] auid to be used to identify this type
		const aafUID_t & id,
		
		// @parm [in] friendly name of this type definition
		const aafCharacter * pTypeName);
	
	//****************
	// CreateValueFromName()
	//
	virtual AAFRESULT STDMETHODCALLTYPE
		CreateValueFromName (
		// the Name of a valid Enum symbol
		/*[in]*/ aafCharacter_constptr  Name,
		
		// newly created property value
		/*[out]*/ ImplAAFPropertyValue ** ppPropVal);
		

	//****************
	// CountElements()
	//
	virtual AAFRESULT STDMETHODCALLTYPE
		CountElements
		// @parm [out] count of elements within this enumeration
		(aafUInt32 *  pCount);

	
	//****************
	// GetElementValues()
	//
	virtual AAFRESULT STDMETHODCALLTYPE
		GetElementValue
        (// @parm [in] index of element to retrieve
		aafUInt32  numElems,
		
		// @parm [out] requested value
		aafUID_t *  pOutValue);
	
	
	
	//****************
	// GetNameFromValue()
	//
	virtual AAFRESULT STDMETHODCALLTYPE
		GetNameFromValue
        (// @parm [in] value of element to get
		ImplAAFPropertyValue * pValue,
		
		// @parm [out, size_is(bufSize), string] buffer into which the element name is written
		wchar_t *  pName,
		
		// @parm [in] The size of the pName buffer, in bytes
		aafUInt32  bufSize);
	
	
	//****************
	// GetNameBufLenFromValue()
	//
	virtual AAFRESULT STDMETHODCALLTYPE
		GetNameBufLenFromValue
        (// @parm [in] value of element to get
		ImplAAFPropertyValue * pValue,
		
		// @parm [out] required buffer length, in bytes
		aafUInt32 *  pLen);
	
	
	//****************
	// GetNameFromAUID()
	//
	virtual AAFRESULT STDMETHODCALLTYPE
		GetNameFromAUID
        (// @parm [in] value of element to get
		const aafUID_t & value,
		
		// @parm [out, size_is(bufSize), string] buffer into which the element name is written
		wchar_t *  pName,
		
		// @parm [in] The size of the pName buffer, in bytes
		aafUInt32  bufSize);
	
	
	//****************
	// GetNameBufLenFromAUID()
	//
	virtual AAFRESULT STDMETHODCALLTYPE
		GetNameBufLenFromAUID
        (// @parm [in] value of element to get
		const aafUID_t & value,
		
		// @parm [out] required buffer length, in bytes
		aafUInt32 *  pLen);
	
	
	//****************
	// GetAUIDValue()
	//
	virtual AAFRESULT STDMETHODCALLTYPE
		GetAUIDValue
        (// @parm [in] property value to read
		ImplAAFPropertyValue * pPropValIn,
		
		// @parm [out] value of the enum represented by the given input property value
		aafUID_t *  pValueOut);
	
	
	//****************
	// SetAUIDValue()
	//
	virtual AAFRESULT STDMETHODCALLTYPE
		SetAUIDValue
        (// @parm [in] property value to set
		ImplAAFPropertyValue * pPropValToSet,
		
		// @parm [in] new value of the enum represented by the given property value
		const aafUID_t & valueIn);
	
	
	//****************
	// AppendElement()
	//
	virtual AAFRESULT STDMETHODCALLTYPE
		AppendElement
        (// @parm [in] value of appended element
		const aafUID_t & value,
		
		// @parm [in] name of appended element
		const aafCharacter * pName);
	
	
	// Override from AAFTypeDef
	virtual AAFRESULT STDMETHODCALLTYPE
		GetTypeCategory (/*[out]*/ eAAFTypeCategory_t *  pTid);
	
	
	// non-published overrides from AAFTypeDef
	aafBool IsFixedSize (void) const;
	OMUInt32 PropValSize (void) const;
	aafBool IsRegistered (void) const;
	OMUInt32 NativeSize (void) const;
	
	virtual OMProperty * 
		pvtCreateOMProperty (OMPropertyId pid,
		const wchar_t * name) const;
	
	
	//*************************************************************
	//
	// Overrides from OMDefinition
	//
	//*************************************************************

	virtual const OMUniqueObjectIdentification& identification(void) const;

	virtual const wchar_t* name(void) const;

	virtual bool hasDescription(void) const;

	virtual const wchar_t* description(void) const;

	virtual bool isPredefined(void) const;


	//*************************************************************
	//
	// Overrides from OMType, via inheritace through ImplAAFTypeDef
	//
	//*************************************************************
	
	virtual bool isFixedSize(void) const;

	virtual void reorder(OMByte* bytes,
		OMUInt32 bytesSize) const;
	
	virtual OMUInt32 externalSize(const OMByte* internalBytes,
		OMUInt32 internalBytesSize) const;
	
	virtual OMUInt32 externalSize(void) const;

	virtual void externalize(const OMByte* internalBytes,
		OMUInt32 internalBytesSize,
		OMByte* externalBytes,
		OMUInt32 externalBytesSize,
		OMByteOrder byteOrder) const;
	
	virtual OMUInt32 internalSize(const OMByte* externalBytes,
		OMUInt32 externalBytesSize) const;
	
	virtual OMUInt32 internalSize(void) const;

	virtual void internalize(const OMByte* externalBytes,
		OMUInt32 externalBytesSize,
		OMByte* internalBytes,
		OMUInt32 internalBytesSize,
		OMByteOrder byteOrder) const;
	
  virtual void accept(OMTypeVisitor& visitor) const;

	//*************************************************************
	//
	// Overrides from OMExtendibleEnumeratedType
	//
	//*************************************************************

	virtual OMUInt32 elementCount(void) const;

	virtual const wchar_t* elementName(OMUInt32 index) const;

	virtual OMUniqueObjectIdentification elementValue(OMUInt32 index) const;

    virtual wchar_t* elementNameFromValue(OMUniqueObjectIdentification value) const;

    virtual OMUniqueObjectIdentification elementValueFromName(const wchar_t* name) const;
    
    virtual bool isValidValue(OMUniqueObjectIdentification value) const;
    
    virtual bool registerElement(const wchar_t* name, OMUniqueObjectIdentification value);
        

	AAFRESULT STDMETHODCALLTYPE
		GetElementNameBufLen (aafUInt32  index,
						  aafUInt32 * pLen);
	
	AAFRESULT STDMETHODCALLTYPE
		GetElementName (aafUInt32 index,
		wchar_t * pName,
		aafUInt32  bufSize);
	
private:
	// names of elements in this record; stored as single wchar_t array
	// with embedded nulls
	OMVariableSizeProperty<wchar_t>  _ElementNames;
	
	// array of values for elements.
	OMArrayProperty<aafUID_t> _ElementValues;
	
  ImplAAFTypeDef     *_cachedBaseType;
  aafBool            _baseTypeIsCached;
	
        // Lookup a value identifier by name. pName is assumed
        // to be non-null. Returns AAFRESULT_SUCCESS if found.
        // Returns AAFRESULT_INVALID_PARAM if not found.
        AAFRESULT LookupValByName(aafUID_t *pVal, const aafCharacter *pName);

public:
	
	ImplAAFTypeDef* NonRefCountedBaseType (void) const;
	
public:
	// Overrides from ImplAAFTypeDef
	virtual bool IsAggregatable () const;
	virtual bool IsStreamable () const;
	virtual bool IsFixedArrayable () const;
	virtual bool IsVariableArrayable () const;
	virtual bool IsStringable () const;
	
	
	// override from OMStorable.
	virtual const OMClassId& classId(void) const;
	
	// Override callbacks from OMStorable
	virtual void onSave(void* clientContext) const;
	virtual void onRestore(void* clientContext) const;
};

//
// smart pointer
//

#ifndef __ImplAAFSmartPointer_h__
#include "ImplAAFSmartPointer.h"
#endif

typedef ImplAAFSmartPointer<ImplAAFTypeDefExtEnum> ImplAAFTypeDefExtEnumSP;


#endif // ! __ImplAAFTypeDefExtEnum_h__
