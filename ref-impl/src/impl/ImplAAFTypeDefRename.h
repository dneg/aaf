//@doc
//@class    AAFTypeDefRename | Implementation class for AAFTypeDefRename
#ifndef __ImplAAFTypeDefRename_h__
#define __ImplAAFTypeDefRename_h__


/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
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
        (ImplAAFTypeDef ** ppBaseType);


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


public:
  // Declare this class to be storable.
  //
  OMDECLARE_STORABLE(ImplAAFTypeDefRename)

private:
  // OMWeakReferenceProperty<ImplAAFTypeDef> _RenamedType;
  OMFixedSizeProperty<aafUID_t>           _RenamedType;
};

#endif // ! __ImplAAFTypeDefRename_h__


