//@doc
//@class    AAFComponent | Implementation class for AAFComponent
#ifndef __ImplAAFComponent_h__
#define __ImplAAFComponent_h__

#ifndef __ImplAAFDataDef_h__
#include "ImplAAFDataDef.h"
#endif

#include "ImplEnumAAFTaggedValues.h"

class ImplAAFMob;
class ImplAAFMobSlot;
class ImplAAFOperationDef;
class ImplAAFOperationGroup;
class ImplAAFScopeStack;
class ImplAAFTaggedValue;

//=---------------------------------------------------------------------=
//
// $Id$ $Name$
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
// The Original Code of this file is Copyright 1998-2004, Licensor of the
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

class AAFComponentVisitor;

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



  //***********************************************************
  // METHOD NAME: AppendComment()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFComponent2 | AppendComment |
  // Append and attribute name/value pair to the attribute list. 
  // @end
  // 
  virtual AAFRESULT STDMETHODCALLTYPE
  AppendComment (
    // @parm [in] aafCharacter_constptr | pName | The attribute name.
    aafCharacter_constptr  pName,

    // @parm [in] aafCharacter_constptr | pValue | The attribute value.
    aafCharacter_constptr  pValue
  );

  //***********************************************************
  // METHOD NAME: CountComments()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFComponent2 | CountComments |
  // Return the number of comments contained by this component 
  // @end
  // 
  virtual AAFRESULT STDMETHODCALLTYPE
  CountComments (
    // @parm [out] aafUInt32* | pNumComments | Pointer to comment count.
    aafUInt32*  pNumComments
  );

  //***********************************************************
  // METHOD NAME: GetComments()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFComponent2 | GetComments |
  // Return a comment enumerator for this component. 
  // @end
  // 
  virtual AAFRESULT STDMETHODCALLTYPE
  GetComments (
    // @parm [out] EnumAAFTaggedValues | ppEnum | Pointer to the new enumerator object created by this method.
    ImplEnumAAFTaggedValues ** ppEnum
  );

  //***********************************************************
  // METHOD NAME: RemoveComment()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFComponent2 | RemoveComment |
  // Remove a comment (tagged value).
  // @end
  // 
  virtual AAFRESULT STDMETHODCALLTYPE
  RemoveComment (
    // @parm [in] AAFTaggedValue | pComment | Pointer to the tagged value comment.
    ImplAAFTaggedValue * pComment
  );

  //***********************************************************
  // METHOD NAME: AppendAttribute()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFComponent2 | AppendAttribute |
  // Append and attribute name/value pair to the attribute list. 
  // @end
  // 
  virtual AAFRESULT STDMETHODCALLTYPE
  AppendAttribute (
    // @parm [in] aafCharacter_constptr | pName | The attribute name.
    aafCharacter_constptr  pName,

    // @parm [in] aafCharacter_constptr | pValue | The attribute value.
    aafCharacter_constptr  pValue
  );

  //***********************************************************
  // METHOD NAME: CountAttributes()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFComponent2 | CountAttributes |
  // Return the number of attributes contained by this component 
  // @end
  // 
  virtual AAFRESULT STDMETHODCALLTYPE
  CountAttributes (
    // @parm [out] aafUInt32* | pNumAttributes | Pointer to attribute count.
    aafUInt32*  pNumAttributes
  );

  //***********************************************************
  // METHOD NAME: GetAttributes()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFComponent2 | GetAttributes |
  // Return an attribute enumerator for this component. 
  // @end
  // 
  virtual AAFRESULT STDMETHODCALLTYPE
  GetAttributes (
    // @parm [out] EnumAAFTaggedValues | ppEnum | Pointer to the new enumerator object created by this method.
    ImplEnumAAFTaggedValues ** ppEnum
  );

  //***********************************************************
  // METHOD NAME: RemoveAttribute()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFComponent2 | RemoveAttribute |
  // Remove an attribute (tagged value).
  // @end
  // 
  virtual AAFRESULT STDMETHODCALLTYPE
  RemoveAttribute (
    // @parm [in] AAFTaggedValue | pAttribute | Pointer to the tagged value attribute.
    ImplAAFTaggedValue * pAttribute
  );


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

	// Visit this component and its sub-components
	// with the specified visitor.
	virtual void Accept(AAFComponentVisitor& visitor);


private:
	OMWeakReferenceProperty<ImplAAFDataDef>		_dataDef;
	OMFixedSizeProperty<aafLength_t>	_length;
    OMStrongReferenceVectorProperty<ImplAAFKLVData> _KLVData;
    OMStrongReferenceVectorProperty<ImplAAFTaggedValue> _userComments;
    OMStrongReferenceVectorProperty<ImplAAFTaggedValue> _attributes;

};

#endif // ! __ImplAAFComponent_h__

