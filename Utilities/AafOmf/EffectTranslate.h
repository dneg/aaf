#ifndef _EFFECT_TRANSLATE_
#define _EFFECT_TRANSLATE_
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

class EffectTranslate
{
public:
	EffectTranslate();
	virtual ~EffectTranslate();

public:
	virtual bool isPrivateEffect(aafUID_t& uid);


	virtual HRESULT GetEffectIDs(IAAFOperationGroup *effect,
									   omfUniqueNamePtr_t effectID,
									   omfUniqueNamePtr_t MCEffectID);

	virtual HRESULT GetAAFEffectID(	omfUniqueNamePtr_t OMFEffectIDPtr,
						omfUniqueNamePtr_t MCEffectIDPtr,
						aafUID_t	*aafUID);
	virtual bool RequiresNestedScope(aafUID_t &effectDefAUID);
};

typedef struct
{
	const aafUID_t	*aafID;
	const char		*omfEffectID;
	const char		*ExtendedEffectID;
} effectXlate_t;

extern effectXlate_t	stdXlateTable[];
extern long				numStdEntries;
#endif

