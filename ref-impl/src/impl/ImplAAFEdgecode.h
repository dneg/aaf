//@doc
//@class    AAFEdgecode | Implementation class for AAFEdgecode
#ifndef __ImplAAFEdgecode_h__
#define __ImplAAFEdgecode_h__


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

/***********************************************\
*	Stub only.   Implementation not yet added	*
\***********************************************/


#ifndef __ImplAAFSegment_h__
#include "ImplAAFSegment.h"
#endif

class ImplAAFEdgecode : public ImplAAFSegment
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFEdgecode ();

protected:
  virtual ~ImplAAFEdgecode ();

public:


  //****************
  // Create()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Create
        (// @parm [in] Length Property Value
         aafLength_t    length  ,

         // @parm [in] Edgecode Value
         aafEdgecode_t  edgecode);

  //****************
  // GetEdgecode()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetEdgecode
        // @parm [out] Edgecode
        (aafEdgecode_t *  edgecode);



public:
  // Declare this class to be storable.
  //
  OMDECLARE_STORABLE(ImplAAFEdgecode)

private:
	OMFixedSizeProperty<aafPosition_t>			_start;
	OMFixedSizeProperty<aafFilmType_t>			_filmType;
	OMFixedSizeProperty<aafEdgeType_t>			_edgeType;
	OMVariableSizeProperty<aafUInt8>			_header;

};

#endif // ! __ImplAAFEdgecode_h__


