//@doc
//@class    EnumAAFMobs | Implementation class for EnumAAFMobs
#ifndef __ImplEnumAAFMobs_h__
#define __ImplEnumAAFMobs_h__


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

class ImplAAFMob;







#ifndef __ImplAAFRoot_h__
#include "ImplAAFRoot.h"
#endif

#include "ImplAAFContentStorage.h"

class ImplEnumAAFMobs : public ImplAAFRoot
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplEnumAAFMobs ();
  ~ImplEnumAAFMobs ();


  //****************
  // NextOne()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    NextOne
        (ImplAAFMob ** ppMob);  //@parm [out,retval] The Next Mob


  //****************
  // Next()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Next
        (aafUInt32  count,   //@parm [in] number of mobs requested
		 ImplAAFMob ** ppMobs,   //@parm [out, size_is(count), length_is(*pFetched)] array to receive mobs
         aafUInt32 *  pFetched);  //@parm [out,ref] number of actual Mobs fetched into ppMobs array


  //****************
  // Skip()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Skip
        (aafUInt32  count);  //@parm [in] Number of elements to skip


  //****************
  // Reset()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Reset ();


  //****************
  // Clone()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Clone
        (ImplEnumAAFMobs ** ppEnum);  //@parm [out,retval] new enumeration

public:
// Internal to the toolkit
AAFRESULT
    SetContentStorage(ImplAAFContentStorage *pCStore);
AAFRESULT
    SetCriteria(aafSearchCrit_t *pCriteria);

private:
	aafInt32				_current;
	ImplAAFContentStorage	*_cStorage;
	aafSearchCrit_t			_criteria;
};

#endif // ! __ImplEnumAAFMobs_h__

