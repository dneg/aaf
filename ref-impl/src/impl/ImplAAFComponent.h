//@doc
//@class    AAFComponent | Implementation class for AAFComponent
#ifndef __ImplAAFComponent_h__
#define __ImplAAFComponent_h__


class ImplAAFDataDef;
class ImplAAFMob;
class ImplAAFMobSlot;
class ImplAAFEffectDef;
class ImplAAFEffectInvocation;
class ImplAAFScopeStack;

/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/




#ifndef __ImplAAFObject_h__
#include "ImplAAFObject.h"
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

  OMDECLARE_STORABLE(ImplAAFComponent)

  //****************
  // SetLength()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetLength
        (aafLength_t *  length);  //@parm [in] Length of this object


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
        (aafUID_t *  datadef);  //@parm [in] DataDef of this object


  //****************
  // GetDataDef()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetDataDef
        (aafUID_t *  datadef);  //@parm [retval][out] DataDef of this object


public:
	AAFRESULT SetNewProps(
				aafLength_t length,		// IN - Length  property value
				aafUID_t *dataDef);		// IN - DataDef property value
	virtual AAFRESULT AccumulateLength(aafLength_t *length);
	virtual AAFRESULT GetMinimumBounds(aafPosition_t rootPos, aafLength_t rootLen,
										ImplAAFMob *mob, ImplAAFMobSlot *track,
										aafMediaCriteria_t *mediaCrit,
										aafPosition_t currentObjPos,
										aafEffectChoice_t *effectChoice,
										ImplAAFComponent	*prevObject,
										ImplAAFComponent *nextObject,
										ImplAAFScopeStack *scopeStack,
										aafPosition_t *diffPos, aafLength_t *minLength,
										ImplAAFEffectInvocation **effeObject, aafInt32	*nestDepth,
										ImplAAFComponent **found, aafBool *foundTransition);

	virtual AAFRESULT GetComponentType(implCompType_t* pType) {*pType = kComponent; return AAFRESULT_SUCCESS;}


  // Declare the module test method. The implementation of the will be be
  // in /test/ImplAAFComponentTest.cpp.
  static AAFRESULT test();

private:
	OMFixedSizeProperty<aafUID_t>		_dataDef;
	OMFixedSizeProperty<aafInt64>		_length;
};

#endif // ! __ImplAAFComponent_h__

