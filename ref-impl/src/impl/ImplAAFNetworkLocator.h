//@doc
//@class    AAFNetworkLocator | Implementation class for AAFNetworkLocator
#ifndef __ImplAAFNetworkLocator_h__
#define __ImplAAFNetworkLocator_h__

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





#ifndef __ImplAAFLocator_h__
#include "ImplAAFLocator.h"
#endif

#include "OMWideStringProperty.h"



class ImplAAFNetworkLocator : public ImplAAFLocator
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFNetworkLocator ();
  virtual ~ImplAAFNetworkLocator ();


  virtual AAFRESULT STDMETHODCALLTYPE
	Initialize ();


  // Override from AAFLocator
  virtual AAFRESULT STDMETHODCALLTYPE
    GetPath (/*[out, size_is(bufSize), string]*/ aafCharacter *  pPathBuf,
	  /*[in]*/ aafUInt32  bufSize);



  // Override from AAFLocator
  virtual AAFRESULT STDMETHODCALLTYPE
    GetPathBufLen (/*[out]*/ aafUInt32 *  pLen);



  // Override from AAFLocator
  virtual AAFRESULT STDMETHODCALLTYPE
    SetPath (/*[in, string]*/ const aafCharacter *  pPathBuf);

private:
  OMWideStringProperty                             _path;
};

#endif // ! __ImplAAFNetworkLocator_h__

