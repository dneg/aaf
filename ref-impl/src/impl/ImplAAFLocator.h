//@doc
//@class    AAFLocator | Implementation class for AAFLocator
#ifndef __ImplAAFLocator_h__
#define __ImplAAFLocator_h__


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






#ifndef __ImplAAFObject_h__
#include "ImplAAFObject.h"
#endif


class ImplAAFLocator : public ImplAAFObject
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFLocator ();

protected:
  virtual ~ImplAAFLocator ();

public:

  //****************
  // GetPath()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetPath
        (// @parm [out, size_is(bufSize), string] buffer into which path is written
         aafCharacter *  pPathBuf,

         // @parm [in] The size of the pPathBuf buffer
         aafInt32  bufSize);


  //****************
  // GetPathBufLen()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetPathBufLen
        // @parm [out] required buffer length
        (aafInt32 *  pLen);


  //****************
  // SetPath()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetPath
        // @parm [in, string] the new path
        (aafCharacter *  pPathBuf);
};

#endif // ! __ImplAAFLocator_h__


