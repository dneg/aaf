//@doc
//@class    AAFComponent | Implementation class for AAFComponent
#ifndef __ImplAAFComponent_h__
#define __ImplAAFComponent_h__

#ifndef __ImplAAFDataDef_h__
#include "ImplAAFDataDef.h"
#endif

class ImplAAFMob;
class ImplAAFMobSlot;
class ImplAAFOperationDef;
class ImplAAFOperationGroup;
class ImplAAFScopeStack;

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




#ifndef __ImplAAFObject_h__
#include "ImplAAFObject.h"
#endif

#include "OMWeakRefProperty.h"
#include "OMStrongRefVectorProperty.h"

#ifndef __ImplAAFKLVData_h__
#include "ImplAAFKLVData.h"
#endif

#ifndef __ImplEnumAAFKLVData_h__
#include "ImplEnumAAFKLVData.h"
#endif

typedef 
enum _implCompType_t
    {
		kTransition	= 0,
		kSegment	= 1,
		kComponent	=2
	} implCompType_t;

class ImplAAFComponent : public ImplAAFObject
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFComponent ();
  ~ImplAAFComponent ();

  //****************
  // SetLength()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetLength
        (const aafLength_t & length);  //@parm [in] Length of this object


  //****************
  // GetLength()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetLength
        (aafLength_t *  length);  //@parm [retval][out] Length of this component

	
  //****************
  // SetDataDef()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetDataDef
        (ImplAAFDataDef * pDataDef);  //@parm [in] DataDef of this object


  //****************
  // GetDataDef()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetDataDef
        (ImplAAFDataDef ** ppDataDef);  //@parm [retval][out] DataDef of this object

  //****************
  // AppendKLVData()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    AppendKLVData
        (ImplAAFKLVData * pData);  //@parm [in,ref] Data

  //****************
  // RemoveKLVData()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    RemoveKLVData
        (ImplAAFKLVData * pData);

  //****************
  // CountKLVData()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    CountKLVData
        (aafUInt32 *  pNumComments);  //@parm [out,retval] Number  of KLVData


  //****************
  // GetKLVData()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetKLVData
        (ImplEnumAAFKLVData ** ppEnum);  //@parm [out,retval] KLVData

public:
	AAFRESULT SetNewProps(
				aafLength_t length,		// IN - Length  property value
				ImplAAFDataDef * pDataDef);		// IN - DataDef property value
	virtual AAFRESULT AccumulateLength(aafLength_t *length);
	virtual AAFRESULT GetMinimumBounds(aafPosition_t rootPos, aafLength_t rootLen,
										ImplAAFMob *mob, ImplAAFMobSlot *track,
										aafMediaCriteria_t *mediaCrit,
										aafPosition_t currentObjPos,
										aafOperationChoice_t *effectChoice,
										ImplAAFComponent	*prevObject,
										ImplAAFComponent *nextObject,
										ImplAAFScopeStack *scopeStack,
										aafPosition_t *diffPos, aafLength_t *minLength,
										ImplAAFOperationGroup **groupObject, aafInt32	*nestDepth,
										ImplAAFComponent **found, aafBool *foundTransition);

	virtual AAFRESULT GetComponentType(implCompType_t* pType) {*pType = kComponent; return AAFRESULT_SUCCESS;}
	virtual AAFRESULT ChangeContainedReferences(aafMobID_constref from,
												aafMobID_constref to);


  // OM deep copy notification
  virtual void onCopy(void* clientContext) const;

private:
	OMWeakReferenceProperty<ImplAAFDataDef>		_dataDef;
	OMFixedSizeProperty<aafLength_t>	_length;
    OMStrongReferenceVectorProperty<ImplAAFKLVData> _KLVData;
};

#endif // ! __ImplAAFComponent_h__

