//@doc
//@class    AAFEssenceFormat | Implementation class for AAFEssenceFormat
#ifndef __ImplAAFEssenceFormat_h__
#define __ImplAAFEssenceFormat_h__


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
#ifndef __ImplAAFRoot_h__
#include "ImplAAFRoot.h"
#endif

typedef struct
{
	aafUID_t		parmName;
	aafDataBuffer_t	parmValue;
	aafUInt32		valueSize;
	aafUInt32		allocSize;
} oneParm_t;

class ImplAAFEssenceFormat : public ImplAAFRoot
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFEssenceFormat ();

protected:
  virtual ~ImplAAFEssenceFormat ();

public:

  //****************
  // AddFormatSpecifier()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    AddFormatSpecifier
        (// @parm [in] From aaddefuids.h
         aafUID_t  essenceFormatCode,

         // @parm [in] Size of preallocated buffer
         aafInt32  valueSize,

         // @parm [in, size_is(valueSize)] Value data
         aafDataBuffer_t  value);
			//@comm The value data is passed in as a void * through the "value"
			// argument.  The size of the value must be passed through the
			// valueSize argument.	


/****/
  //****************
  // GetFormatSpecifier()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetFormatSpecifier
        (// @parm [in] From aaddefuids.h
         aafUID_t  essenceFormatCode,

         // @parm [in] Size of preallocated buffer
         aafInt32  valueSize,

         // @parm [out, size_is(valueSize),length_is(*bytesRead)] Preallocated buffer to hold value
         aafDataBuffer_t  value,

         // @parm [out] Number of actual bytes read
         aafInt32*  bytesRead);
			//@comm The actual number of bytes read is returned in
			// bytesRead.  If the buffer is not big enough to return the entire
			// value, an error is returned.
/****/
  //****************
  // NumFormatSpecifiers()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    NumFormatSpecifiers
        // @parm [out] The number of specifiers present.
        (aafInt32*  numSpecifiers);
/****/
  //****************
  // GetIndexedFormatSpecifier()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetIndexedFormatSpecifier
        (// @parm [in] 0-based index
         aafInt32  index,

         // @parm [out] From aaddefuids.h
         aafUID_t*  essenceFormatCode,

         // @parm [in] Size of preallocated buffer
         aafInt32  valueSize,

         // @parm [out, size_is(valueSize),length_is(*bytesRead)] Preallocated buffer to hold value
         aafDataBuffer_t  value,

         // @parm [out] Number of actual bytes read
         aafInt32*  bytesRead);


private:
		oneParm_t	*_elements;
		aafUInt32	_elemUsed;
		aafUInt32	_elemAllocated;

		oneParm_t	*Lookup(aafUID_t);
};

#endif // ! __ImplAAFEssenceFormat_h__


