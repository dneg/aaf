//@doc
//@class    AAFFindSourceInfo | Implementation class for AAFFindSourceInfo
#ifndef __ImplAAFFindSourceInfo_h__
#define __ImplAAFFindSourceInfo_h__

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

#ifndef __AAFTypes_h__
#include "AAFTypes.h"
#endif

#ifndef __ImplAAFRoot_h__
#include "ImplAAFRoot.h"
#endif

class ImplAAFMob;
class ImplAAFComponent;
class ImplAAFOperationGroup;
class ImplAAFPulldown;

class ImplAAFFindSourceInfo : public ImplAAFRoot
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFFindSourceInfo ();

protected:
  virtual ~ImplAAFFindSourceInfo ();

  	ImplAAFMob		*_mob;
	aafSlotID_t		_slotID;
	aafPosition_t	_position;
	aafRational_t	_editRate;
	aafLength_t		_length;
	ImplAAFComponent *_cpnt;
	ImplAAFOperationGroup *_operationGroup;

public:

	virtual AAFRESULT STDMETHODCALLTYPE
		Init(ImplAAFMob *mob, aafSlotID_t slotID, aafPosition_t position,
							aafRational_t editRate, aafLength_t length,
							ImplAAFComponent *cpnt);
	virtual AAFRESULT STDMETHODCALLTYPE
		AddPulldown(ImplAAFPulldown *pdwn);
	virtual AAFRESULT STDMETHODCALLTYPE
		SetOperationGroup(ImplAAFOperationGroup *OperationGroup);
	virtual AAFRESULT STDMETHODCALLTYPE
		SetComponent(ImplAAFComponent *cpnt);

	//	virtual AAFRESULT STDMETHODCALLTYPE
	//		Duplicate(ImplAAFFindSourceInfo *info);
	virtual AAFRESULT STDMETHODCALLTYPE
		Clear(void);
	virtual AAFRESULT STDMETHODCALLTYPE
		GetMob(ImplAAFMob **ppMob);
	virtual AAFRESULT STDMETHODCALLTYPE
		GetSourceReference(aafSourceRef_t *pSourceRef);
	virtual AAFRESULT STDMETHODCALLTYPE
		GetEditRate(aafRational_t *pEditRate);
	virtual AAFRESULT STDMETHODCALLTYPE
		GetLength(aafLength_t *pLength);
//	virtual AAFRESULT STDMETHODCALLTYPE
//		GetEnclosingOperationGroup(ImplAAFOperationGroup *pSourceRef);
//	virtual AAFRESULT STDMETHODCALLTYPE
//		GetEnclosingComponent(ImplAAFOperationGroup *pSourceRef);
};

#endif // ! __ImplAAFFindSourceInfo_h__

