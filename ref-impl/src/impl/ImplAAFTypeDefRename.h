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
         aafUID_t *  pID,

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
  // GetName()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetName
        (// @parm [out, size_is(bufSize), string] buffer into which the name is written
         wchar_t *  pName,

         // @parm [in] The size of the pName buffer, in bytes
         aafInt32  bufSize);


  //****************
  // GetNameBufLen()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetNameBufLen
        // @parm [out] required buffer length, in bytes
        (aafInt32 *  pLen);


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

  /*
  virtual void reorder(OMByte* bytes,
                       size_t bytesSize) const;

  virtual size_t externalSize(void) const;

  virtual void externalize(OMByte* internalBytes,
                           size_t internalBytesSize,
                           OMByte* externalBytes,
                           size_t externalBytesSize,
                           OMByteOrder byteOrder) const;

  virtual size_t internalSize(void) const;

  virtual void internalize(OMByte* externalBytes,
                           size_t externalBytesSize,
                           OMByte* internalBytes,
                           size_t internalBytesSize,
                           OMByteOrder byteOrder) const;
						   */

public:
  // Declare this class to be storable.
  //
  OMDECLARE_STORABLE(ImplAAFTypeDefRename)

  // Declare the module test method. The implementation of the will be be
  // in /test/ImplAAFTypeDefRenameTest.cpp.
  static AAFRESULT test();
};

#endif // ! __ImplAAFTypeDefRename_h__


