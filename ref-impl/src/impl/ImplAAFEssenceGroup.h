//@doc
//@class    AAFEssenceGroup | Implementation class for AAFEssenceGroup
#ifndef __ImplAAFEssenceGroup_h__
#define __ImplAAFEssenceGroup_h__


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


/***********************************************\
*	Stub only.   Implementation not yet added	*
\***********************************************/



class ImplAAFSegment;

class ImplAAFDataDef;

class ImplAAFSourceClip;

#ifndef __ImplAAFSegment_h__
#include "ImplAAFSegment.h"
#endif

#include "OMStrongRefProperty.h"
#include "OMStrongRefVectorProperty.h"

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



  //****************
  // SetStillFrame()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetStillFrame
        // @parm [in] Still Frame source clip 
        (ImplAAFSourceClip * stillFrame);
    //@comm Essence group choices should be added with the AddChoice() function.
    

  //****************
  // GetStillFrame()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetStillFrame
        // @parm [out] Still Frame source clip 
        (ImplAAFSourceClip **stillFrame);
    //@comm Essence group choices should be added with the AddChoice() function.
    

  //****************
  // AppendChoice()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    AppendChoice
        // @parm [in] Source clip to add as a choice 
        (ImplAAFSegment * choice);


  //****************
  // PrependChoice()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    PrependChoice
        // @parm [in] Source clip to add as a choice 
        (ImplAAFSegment * choice);


  //****************
  // InsertChoiceAt()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    InsertChoiceAt
        // @parm [in] place to put new choice
        (aafUInt32 index,

		 // @parm [in] Source clip to add as a choice
		 ImplAAFSegment * choice);


  //****************
  // CountChoices()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    CountChoices
        (aafUInt32  *result);


  //****************
  // GetChoiceAt()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetChoiceAt
        (// @parm [in] The 0-based index into the array
         aafUInt32  index,

         // @parm [out] The representation at that index
         ImplAAFSegment  ** result);


  //****************
  // RemoveChoiceAt()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    RemoveChoiceAt
        (// @parm [in] The 0-based index into the array
         aafUInt32  index);

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

	AAFRESULT GetCriteriaSegment(
			aafMediaCriteria_t *criteria,
			ImplAAFSegment		**retSrcClip);
	virtual AAFRESULT ChangeContainedReferences(aafMobID_constref from,
												aafMobID_constref to);
	virtual AAFRESULT ValidateChoice(ImplAAFSegment *choice);

private:
	OMStrongReferenceVectorProperty<ImplAAFSegment>	_choices;
	OMStrongReferenceProperty<ImplAAFSourceClip>		_stillFrame;
};	

#endif // ! __ImplAAFEssenceGroup_h__
