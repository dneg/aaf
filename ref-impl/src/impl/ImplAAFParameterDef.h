//@doc
//@class    AAFParameterDef | Implementation class for AAFParameterDef
#ifndef __ImplAAFParameterDef_h__
#define __ImplAAFParameterDef_h__

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


#ifndef __ImplAAFDefObject_h__
#include "ImplAAFDefObject.h"
#endif

#ifndef __ImplAAFTypeDeft_h__
#include "ImplAAFTypeDef.h"
#endif

#include "OMWeakRefProperty.h"
#include "OMWideStringProperty.h"

class ImplAAFParameterDef : public ImplAAFDefObject
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFParameterDef ();


  //****************
  // Initialize()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Initialize
        // @parm [in] Pointer to an AUID reference
        (const aafUID_t & id,
		 const aafCharacter *name,
		 const aafCharacter *description,
     ImplAAFTypeDef * pType);


protected:
  virtual ~ImplAAFParameterDef ();

public:


  //****************
  // GetParameterDataDefID()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetTypeDefinition
        // @parm [retval][out] Pointer to an AUID reference
        (ImplAAFTypeDef **  pParameterDataDefID);

  //****************
  // SetParameterDataDefID()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetTypeDef
        // @parm [in] an AUID reference
        (ImplAAFTypeDef * ParameterDataDefID);

  //****************
  // GetDisplayUnits()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetDisplayUnits
        (// @parm [in,string] DisplayUnits
         aafCharacter *  pDisplayUnits,

         // @parm [in] length of the buffer to hold DisplayUnits
         aafUInt32  bufSize);

  //****************
  // GetDisplayUnitsBufLen()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetDisplayUnitsBufLen
        // @parm [out] DisplayUnits
        (aafUInt32 *  pLen);



  //****************
  // SetDisplayUnits()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetDisplayUnits
        // @parm [in, string] DisplayUnits
        (const aafCharacter *  pDisplayUnits);

  // OM deep copy notification
  virtual void onCopy(void* clientContext) const;

private:
	OMWeakReferenceProperty<ImplAAFTypeDef>					_typeDef;
	OMWideStringProperty									_displayUnits;
};

#endif // ! __ImplAAFParameterDef_h__

