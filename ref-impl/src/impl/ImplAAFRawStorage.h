#ifndef __ImplAAFRawStorage_h__
#define __ImplAAFRawStorage_h__
/***********************************************************************
*
*              Copyright (c) 1998-2000 Avid Technology, Inc.
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
* THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND,
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

#ifndef __ImplAAFRoot_h__
#include "ImplAAFRoot.h"
#endif

class OMRawStorage;

class ImplAAFRawStorage : public ImplAAFRoot
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFRawStorage ();

protected:
  virtual ~ImplAAFRawStorage ();

public:

  //****************
  // IsReadable()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    IsReadable
        (
		 // set to true if readable
		 aafBoolean_t *  pResult);


  //****************
  // Read()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Read
        (// @parm [out, size_is(bufSize), length_is(*pNumRead)] Buffer into which data is read
         aafMemPtr_t  buf,

         // @parm [in] Size of buf in bytes
         aafUInt32  bufSize,

         // @parm [out] Number of bytes read
         aafUInt32 *  pNumRead);


  //****************
  // IsWriteable()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    IsWriteable
        (
		 // set to true if writeable
		 aafBoolean_t *  pResult);


  //****************
  // Write()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Write
        (// @parm [in, size_is(bufSize)] Buffer from which data is written
         aafMemConstPtr_t  buf,

         // @parm [in] Number of bytes to write
         aafUInt32  bufSize,

		 // Number of bytes written
		 aafUInt32 * pNumWritten);


  //****************
  // Synchronize()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Synchronize ();


  //
  // Methods which are not exported from the toolkit
  //

  //****************
  // Initialize()
  //
  void Initialize (OMRawStorage * prs,
				   aafFileAccess_e access);

  //
  // Returns the underlying OMRawStorage
  //
  OMRawStorage * GetOMStorage();

private:
  OMRawStorage *  _rep;
  aafFileAccess_e _access;

  bool pvtIsReadable () const;
  bool pvtIsWriteable () const;
};

#endif // ! __ImplAAFRawStorage_h__
