//@doc
//@class    AAFDataDef | Implementation class for AAFDataDef
#ifndef __ImplAAFDataDef_h__
#define __ImplAAFDataDef_h__


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


#ifndef __ImplAAFDefObject_h__
#include "ImplAAFDefObject.h"
#endif


class ImplAAFDataDef : public ImplAAFDefObject
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFDataDef ();

protected:
  virtual ~ImplAAFDataDef ();

public:
  //****************
  // IsPictureKind()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    IsPictureKind
        // @parm [retval,out] pointer to the return value
        (aafBool *  bIsPictureKind);

  //****************
  // IsMatteKind()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    IsMatteKind
        // @parm [retval,out] pointer to the return value
        (aafBool *  bIsMatteKind);

  //****************
  // IsPictureWithMatteKind()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    IsPictureWithMatteKind
        // @parm [retval,out] pointer to the return value
        (aafBool *  bIsPictureWithMatteKind);

  //****************
  // IsSoundKind()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    IsSoundKind
        // @parm [retval,out] pointer to the return value
        (aafBool *  bIsSoundKind);

  //****************
  // DoesDataDefConvertTo()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    DoesDataDefConvertTo
        (// @parm [in,ref] Auid to compare against
         const aafUID_t & id,

         // @parm [retval,out] pointer to result
         aafBool *  bDoesConvertTo);
		   
  //****************
  // IsDataDefOf()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    IsDataDefOf
        (// @parm [in,ref] Auid to compare against
         const aafUID_t & id,

         // @parm [retval,out] pointer to result
         aafBool *  bIsDataDefOf);

  //****************
  // DoesDataDefConvertFrom()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    DoesDataDefConvertFrom
        (// @parm [in,ref] Auid to compare against
         const aafUID_t & id,

         // @parm [retval,out] pointer to result
         aafBool *  bDoesConvertFrom);
};

#endif // ! __ImplAAFDataDef_h__


