//@doc
//@class    AAFCodecDef | Implementation class for AAFCodecDef
#ifndef __ImplAAFCodecDef_h__
#define __ImplAAFCodecDef_h__


/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/



class ImplAAFClassDef;

class ImplEnumAAFCodecFlavours;



#ifndef __ImplAAFDefObject_h__
#include "ImplAAFDefObject.h"
#endif

#include "ImplEnumAAFDataDefs.h"

class ImplAAFCodecDef : public ImplAAFDefObject
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFCodecDef ();

protected:
  virtual ~ImplAAFCodecDef ();

public:


  //****************
  // IsEssenceKindSupported()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    IsEssenceKindSupported
        (// @parm [in,ref] The essence kind
         aafUID_t *pEssenceKind,

         // @parm [out,retval] Is this type supported
         aafBool*  pIsSupported);
 
   //****************
  // AppendEssenceKind()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    AppendEssenceKind
        // @parm [in] The essence kind
        (aafUID_t *  pEssenceKind);



  //****************
  // AreThereFlavours()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    AreThereFlavours
        // @parm [out,retval] True if there are flavours of this codec
        (aafBool *  pResult);


  //****************
  // GetFileDescriptorClass()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetFileDescriptorClass
        // @parm [out] Returned file descriptor class object
        (ImplAAFClassDef ** ppClass);


  //****************
  // SetFileDescriptorClass()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetFileDescriptorClass
        // @parm [in] File descriptor class object
        (ImplAAFClassDef * pClass);


  //****************
  // EnumCodecFlavours()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    EnumCodecFlavours
        // @parm [out,retval] Codec flavour Enumeration
        (ImplEnumAAFCodecFlavours ** ppEnum);
  

public:
	// SDK-private
  //****************
  // GetParameterDefinitions()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetDataDefinitions
        // @parm [out,retval] Parameter definition enumeration
        (ImplEnumAAFDataDefs ** ppEnum);

public:
  // Declare this class to be storable.
  //
  OMDECLARE_STORABLE(ImplAAFCodecDef)

  // Declare the module test method. The implementation of the will be be
  // in /test/ImplAAFCodecDefTest.cpp.
  static AAFRESULT test();
private:
	DataDefWeakRefArrayProp_t						_dataDefs;
};

#endif // ! __ImplAAFCodecDef_h__


