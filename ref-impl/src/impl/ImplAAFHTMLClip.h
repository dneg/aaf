//@doc
//@class    AAFHTMLClip | Implementation class for AAFHTMLClip
#ifndef __ImplAAFHTMLClip_h__
#define __ImplAAFHTMLClip_h__


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
 * prior written permission of Avid Technology, Inc.
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



#ifndef __ImplAAFTextClip_h__
#include "ImplAAFTextClip.h"
#endif


class ImplAAFHTMLClip : public ImplAAFTextClip
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFHTMLClip ();

protected:
  virtual ~ImplAAFHTMLClip ();

public:


  //****************
  // GetBeginAnchor()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetBeginAnchor
        (// @parm [in,string] Begin Anchor Name
         aafCharacter *  pName,

         // @parm [in] length of the buffer to hold Begin Anchor Name
         aafInt32  bufSize);

  //****************
  // GetBeginAnchorBufLen()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetBeginAnchorBufLen
        // @parm [out] Length in bytes
        (aafUInt32 *  pLen);

  //****************
  // SetBeginAnchor()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetBeginAnchor
        // @parm [in, string] Specifies the HTML tag that defines the start of the text
        (const aafCharacter *  pName);

  //****************
  // GetEndAnchor()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetEndAnchor
        (// @parm [in,string] End Anchor Name
         aafCharacter *  pName,

         // @parm [in] length of the buffer to hold End Anchor Name
         aafInt32  bufSize);

  //****************
  // GetEndAnchorBufLen()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetEndAnchorBufLen
        // @parm [out] Length in bytes
        (aafUInt32 *  pLen);

  //****************
  // SetEndAnchor()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetEndAnchor
        // @parm [in, string] Specifies the HTML tag that defines the end of the text
        (const aafCharacter *  pName);

protected:
  // Persistent properties...
  OMWideStringProperty _beginAnchor;
  OMWideStringProperty _endAnchor;
};

#endif // ! __ImplAAFHTMLClip_h__


