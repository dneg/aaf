//@doc
//@class    AAFEssenceGroup | Implementation class for AAFEssenceGroup
#ifndef __ImplAAFEssenceGroup_h__
#define __ImplAAFEssenceGroup_h__


/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/

/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/

/***********************************************\
*	Stub only.   Implementation not yet added	*
\***********************************************/



class ImplAAFSegment;

class ImplAAFDataDef;

class ImplAAFSourceClip;

#ifndef __ImplAAFSegment_h__
#include "ImplAAFSegment.h"
#endif


class ImplAAFEssenceGroup : public ImplAAFSegment
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFEssenceGroup ();

protected:
  virtual ~ImplAAFEssenceGroup ();

public:


/****/
  //****************
  // SetStillFrame()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetStillFrame
        // @parm [in] Still Frame source clip 
        (ImplAAFSourceClip * stillFrame);
    //@comm Essence group choices should be added with the AddChoice() function.
    
/****/
  //****************
  // AddChoice()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    AppendChoice
        // @parm [in] Source clip to add as a choice 
        (ImplAAFSourceClip * choice);

/****/
  //****************
  // GetNumRepresentations()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetNumRepresentations
        (aafUInt32  *result);

  //****************
  // GetIndexedRepresentation()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetIndexedRepresentation
        (// @parm [in] The 0-based index into the array
         aafUInt32  index,

         // @parm [out] The representation at that index
         ImplAAFSourceClip  ** result);

//	private:
//		AAFSourceClip			*_stillFrame;
//		vector<AAFSourceClip *>	_choices;


public:
	//SDK-private methods
	AAFRESULT GetMinimumBounds(aafPosition_t rootPos, aafLength_t rootLen,
										ImplAAFMob *mob, ImplAAFMobSlot *track,
										aafMediaCriteria_t *mediaCrit,
										aafPosition_t currentObjPos,
										aafOperationChoice_t *effectChoice,
										ImplAAFComponent	*prevObject,
										ImplAAFComponent *nextObject,
										ImplAAFScopeStack *scopeStack,
										aafPosition_t *diffPos, aafLength_t *minLength,
										ImplAAFOperationGroup **effeObject, aafInt32	*nestDepth,
										ImplAAFComponent **foundObj, aafBool *foundTransition);


public:
  // Declare this class to be storable.
  //
  OMDECLARE_STORABLE(ImplAAFEssenceGroup)

  // Declare the module test method. The implementation of the will be be
  // in /test/ImplAAFEssenceGroupTest.cpp.
  static AAFRESULT test();
private:
	OMStrongReferenceVectorProperty<ImplAAFSourceClip>	_choices;
	OMStrongReferenceProperty<ImplAAFSourceClip>		_stillFrame;
};	

#endif // ! __ImplAAFEssenceGroup_h__


