//@doc
//@class    AAFTypeDefExtEnum | Implementation class for AAFTypeDefExtEnum
#ifndef __ImplAAFTypeDefExtEnum_h__
#define __ImplAAFTypeDefExtEnum_h__


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

#ifndef __ImplAAFTypeDef_h__
#include "ImplAAFTypeDef.h"
#endif


class ImplAAFTypeDefExtEnum : public ImplAAFTypeDef
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
	size_t PropValSize (void) const;
	aafBool IsRegistered (void) const;
	size_t NativeSize (void) const;
	
	virtual OMProperty * 
		pvtCreateOMProperty (OMPropertyId pid,
		const wchar_t * name) const;
	
	
	//*************************************************************
	//
	// Overrides from OMType, via inheritace through ImplAAFTypeDef
	//
	//*************************************************************
	
	virtual void reorder(OMByte* bytes,
		size_t bytesSize) const;
	
	virtual size_t externalSize(const OMByte* internalBytes,
		size_t internalBytesSize) const;
	
	virtual void externalize(const OMByte* internalBytes,
		size_t internalBytesSize,
		OMByte* externalBytes,
		size_t externalBytesSize,
		OMByteOrder byteOrder) const;
	
	virtual size_t internalSize(const OMByte* externalBytes,
		size_t externalBytesSize) const;
	
	virtual void internalize(const OMByte* externalBytes,
		size_t externalBytesSize,
		OMByte* internalBytes,
		size_t internalBytesSize,
		OMByteOrder byteOrder) const;
	
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
	OMVariableSizeProperty<aafUID_t> _ElementValues;
	
	ImplAAFTypeDefSP _cachedBaseType;
	

public:
	
	ImplAAFTypeDefSP BaseType (void) const;
	
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
// caution! includes assert.h
#include "ImplAAFSmartPointer.h"
#endif

typedef ImplAAFSmartPointer<ImplAAFTypeDefExtEnum> ImplAAFTypeDefExtEnumSP;


#endif // ! __ImplAAFTypeDefExtEnum_h__
