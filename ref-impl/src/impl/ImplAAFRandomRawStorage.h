#ifndef __ImplAAFRandomRawStorage_h__
#define __ImplAAFRandomRawStorage_h__
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

#ifndef __ImplAAFRawStorage_h__
#include "ImplAAFRawStorage.h"
#endif

class ImplAAFRandomRawStorage : public ImplAAFRawStorage
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFRandomRawStorage ();

protected:
  virtual ~ImplAAFRandomRawStorage ();

public:

  //****************
  // ReadAt()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    ReadAt
        (
		 // position in this storage from which data is read
		 aafUInt64  position,

		 // Buffer into which data is read
		 aafMemPtr_t  buf,

		 // Size of buf in bytes
		 aafUInt32  bufSize,

		 // Number of bytes read
		 aafUInt32 *  pNumRead);


  //****************
  // GetSize()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    WriteAt
        (
		 // position in this storage into which data is written
		 aafUInt64  position,

		 // Buffer from which data is written
		 aafMemConstPtr_t  buf,

		 // Number of bytes requested to write
		 aafUInt32  bufSize,

		 // Number of bytes written
		 aafUInt32 *  pNumWritten);


  //****************
  // GetSize()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetSize
        // @parm [out] Number of bytes currently in this stream
        (aafUInt64 *  pSize);


  //****************
  // IsExtendable()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    IsExtendable
        (
		 // set to true if extendable
		 aafBoolean_t *  pResult);


  //****************
  // GetExtent()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetExtent
        (
		 // Number of bytes currently allocated for this stream
		 aafUInt64 *  pExtent);


  //****************
  // SetSize()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetExtent
        // @parm [in] Number of bytes capacity requested for this stream
        (aafUInt64  size);
};

#endif // ! __ImplAAFRandomRawStorage_h__
