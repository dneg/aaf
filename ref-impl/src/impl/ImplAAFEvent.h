//@doc
//@class    AAFEvent | Implementation class for AAFEvent
#ifndef __ImplAAFEvent_h__
#define __ImplAAFEvent_h__


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


#ifndef __ImplAAFSegment_h__
#include "ImplAAFSegment.h"
#endif


class ImplAAFEvent : public ImplAAFSegment
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFEvent ();

protected:
  virtual ~ImplAAFEvent ();

public:


  //****************
  // GetPosition()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetPosition
        // @parm [out,retval] Position property value
        (aafPosition_t *  pPosition);

  //****************
  // SetPosition()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetPosition
        // @parm [in] Position property value
        (aafPosition_t  Position);


  //****************
  // SetComment()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetComment
        // @parm [in, string] buffer from which Comment is to be read
        (const aafCharacter * pComment);


  //****************
  // GetComment()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetComment
        (// @parm [out, string, size_is(bufSize)] buffer into which Comment is to be written
         wchar_t *  pComment,

         // @parm [in] size of *pComment buffer in bytes
         aafUInt32  bufSize);


  //****************
  // GetCommentBufLen()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetCommentBufLen
        // @parm [out] size of required buffer, in bytes
        (aafUInt32 *  pBufSize);

protected:
  OMFixedSizeProperty<aafPosition_t> _position;
  OMWideStringProperty _comment;
};

#endif // ! __ImplAAFEvent_h__


