#ifndef _EFFECT_TRANSLATE_
#define _EFFECT_TRANSLATE_
/***********************************************************************
 *
 *              Copyright (c) 1998-1999 Avid Technology, Inc.
 *
 * Permission to use, copy and modify this software and accompanying 
 * documentation, and to distribute and sublicense application software
 * incorporating this software for any purpose is hereby granted, 
 * provided that (i) the above copyright notice and this permission
 * notice appear in all copies of the software and related documentation,
 * and (ii) the name Avid Technology, Inc. may not be used in any
 * advertising or publicity relating to the software without the specific,
 *  prior written permission of Avid Technology, Inc.
 *
 * THE SOFTWARE IS PROVIDED AS-IS AND WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY
 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 * IN NO EVENT SHALL AVID TECHNOLOGY, INC. BE LIABLE FOR ANY DIRECT,
 * SPECIAL, INCIDENTAL, PUNITIVE, INDIRECT, ECONOMIC, CONSEQUENTIAL OR
 * OTHER DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE AND
 * ACCOMPANYING DOCUMENTATION, INCLUDING, WITHOUT LIMITATION, DAMAGES
 * RESULTING FROM LOSS OF USE, DATA OR PROFITS, AND WHETHER OR NOT
 * ADVISED OF THE POSSIBILITY OF DAMAGE, REGARDLESS OF THE THEORY OF
 * LIABILITY.
 *
 ************************************************************************/

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

