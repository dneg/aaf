//@doc
//@class    AAFTypeDefRename | Implementation class for AAFTypeDefRename
#ifndef __ImplAAFTypeDefRename_h__
#define __ImplAAFTypeDefRename_h__


/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
*                                          *
\******************************************/



#ifndef __ImplAAFTypeDef_h__
#include "ImplAAFTypeDef.h"
#endif


class ImplAAFTypeDefRename : public ImplAAFTypeDef
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFTypeDefRename ();

protected:
  virtual ~ImplAAFTypeDefRename ();

public:

  //****************
  // Initialize()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Initialize
        (// @parm [in] auid to be used to identify this type
         const aafUID_t *  pID,

         // @parm [in] type to which this is an alias
         ImplAAFTypeDef * pBaseType,

         // @parm [in, string] friendly name of this type definition
         wchar_t *  pTypeName);


  //****************
  // GetBaseType()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetBaseType
        // @parm [out] type definition for which this is an alias
        (ImplAAFTypeDef ** ppBaseType) const;


  //****************
  // GetValue()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetValue
        (// @parm [in] property value from which value is to be read
         ImplAAFPropertyValue * pInPropVal,

         // @parm [out] pointer to property value represented by base type
         ImplAAFPropertyValue ** ppOutPropVal);


  //*************************************************************
  //
  // Overrides from OMType, via inheritace through ImplAAFTypeDef
  //
  //*************************************************************

  virtual void reorder(OMByte* externalBytes,
                       size_t externalBytesSize) const;

  virtual size_t externalSize(OMByte* internalBytes,
							  size_t internalBytesSize) const;

  virtual void externalize(OMByte* internalBytes,
                           size_t internalBytesSize,
                           OMByte* externalBytes,
                           size_t externalBytesSize,
                           OMByteOrder byteOrder) const;

  virtual size_t internalSize(OMByte* externalBytes,
							  size_t externalBytesSize) const;

  virtual void internalize(OMByte* externalBytes,
                           size_t externalBytesSize,
                           OMByte* internalBytes,
                           size_t internalBytesSize,
                           OMByteOrder byteOrder) const;


  // overrides from ImplAAFTypeDef
  //
  virtual aafBool IsFixedSize (void) const;
  virtual size_t PropValSize (void) const;
  aafBool IsRegistered (void) const;
  size_t NativeSize (void) const;

  // Override this one, not the ...MBS() one...
  OMProperty * 
    pvtCreateOMProperty (OMPropertyId pid,
						 const aafCharacter * name) const;

public:
  // Declare this class to be storable.
  //
  OMDECLARE_STORABLE(ImplAAFTypeDefRename)

private:
  ImplAAFTypeDefSP BaseType () const;

  // OMWeakReferenceProperty<ImplAAFTypeDef> _RenamedType;
  OMFixedSizeProperty<aafUID_t>           _RenamedType;

  ImplAAFTypeDefSP _cachedBaseType;
};

//
// smart pointer
//

#ifndef __ImplAAFSmartPointer_h__
// caution! includes assert.h
#include "ImplAAFSmartPointer.h"
#endif

typedef ImplAAFSmartPointer<ImplAAFTypeDefRename> ImplAAFTypeDefRenameSP;

#endif // ! __ImplAAFTypeDefRename_h__
