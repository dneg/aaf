//@doc
//@class    AAFCommentMarker | Implementation class for AAFCommentMarker
#ifndef __ImplAAFCommentMarker_h__
#define __ImplAAFCommentMarker_h__

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

#ifndef __ImplAAFEvent_h__
#include "ImplAAFEvent.h"
#endif


// Forward declaration
class ImplAAFSourceReference;


class ImplAAFCommentMarker : public ImplAAFEvent
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFCommentMarker ();

protected:
  virtual ~ImplAAFCommentMarker ();

public:


  //****************
  // GetAnnotation()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetAnnotation
        // @parm [out,retval] Annotation property value
        (ImplAAFSourceReference ** ppResult);

  //****************
  // SetAnnotation()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetAnnotation
        // @parm [in] Annotation property value
        (ImplAAFSourceReference * pAnnotation);

protected:
  // The stored object model defines the annotation to be a 
  // an optional strong reference.
  OMStrongReferenceProperty<ImplAAFSourceReference> _annotation;
};

#endif // ! __ImplAAFCommentMarker_h__


