//@doc
//@class    AAFCodecDef | Implementation class for AAFCodecDef
#ifndef __ImplAAFCodecDef_h__
#define __ImplAAFCodecDef_h__

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


class ImplEnumAAFCodecFlavours;


#ifndef __ImplAAFDefObject_h__
#include "ImplAAFDefObject.h"
#endif

#ifndef __ImplAAFClassDef_h__
#include "ImplAAFClassDef.h"
#endif

#include "OMReferenceContainerIter.h"
#include "OMWeakRefProperty.h"

#include "ImplEnumAAFDataDefs.h"

#include "OMWeakRefVectorProperty.h"
typedef OMWeakReferenceVectorProperty<ImplAAFDataDef> DataDefWeakRefArrayProp_t;

class ImplAAFCodecDef : public ImplAAFDefObject
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFCodecDef ();


  //****************
  // Initialize()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Initialize
        // @parm [in] Pointer to an AUID reference
        (const aafUID_t & id,
		 const aafCharacter *name,
		 const aafCharacter *description);

protected:
  virtual ~ImplAAFCodecDef ();

public:


  //****************
  // IsEssenceKindSupported()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    IsEssenceKindSupported
        (// @parm [in,ref] The essence kind
         ImplAAFDataDef * pEssenceKind,

         // @parm [out,retval] Is this type supported
         aafBool*  pIsSupported);
 

  //****************
  // AddEssenceKind()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    AddEssenceKind
        // @parm [in] The essence kind
        (ImplAAFDataDef * pEssenceKind);



  //****************
  // RemoveEssenceKind()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    RemoveEssenceKind
        // @parm [in] The essence kind to remove
        (ImplAAFDataDef * pEssenceKind);



  //****************
  // CountEssenceKinds()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    CountEssenceKinds
        // @parm [out, retval] The returned number of essece kinds
        (aafUInt32 * pResult);


  //****************
  // GetEssenceKinds()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetEssenceKinds
        // @parm [out, retval] The enumerator over essence kinds
        (ImplEnumAAFDataDefs ** ppEnum);


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
  
  // OM deep copy notification
  virtual void onCopy(void* clientContext) const;

private:
	DataDefWeakRefArrayProp_t			_dataDefs;
	OMWeakReferenceProperty<ImplAAFClassDef> _fileDescClass;
};

#endif // ! __ImplAAFCodecDef_h__


