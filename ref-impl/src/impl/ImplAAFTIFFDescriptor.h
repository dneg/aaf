//@doc
//@class    AAFTIFFDescriptor | Implementation class for AAFTIFFDescriptor
#ifndef __ImplAAFTIFFDescriptor_h__
#define __ImplAAFTIFFDescriptor_h__


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

#ifndef __AAFTypes_h__
#include "AAFTypes.h"
#endif


/******************************************\
*                                          *
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




#ifndef __ImplAAFFileDescriptor_h__
#include "ImplAAFFileDescriptor.h"
#endif


class ImplAAFTIFFDescriptor : public ImplAAFFileDescriptor
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFTIFFDescriptor ();

protected:
  virtual ~ImplAAFTIFFDescriptor ();

public:

  //****************
  // SetIsUniform()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetIsUniform
        // @parm [in] Sets the IsUniform flag value
        (aafBool  IsUniform);


  //****************
  // GetIsUniform()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetIsUniform
        // @parm [out] Gets the IsUniform flag value
        (aafBool *  pIsUniform);

  //****************
  // SetIsContiguous()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetIsContiguous
        // @parm [in] is data stores in contiguous bytes 
        (aafBool  IsContiguous);


  //****************
  // GetIsContiguous()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetIsContiguous
        // @parm [out] is this data stored in contiguous bytes
        (aafBool *  pIsContiguous);

  //****************
  // SetLeadingLines()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetLeadingLines
        // @parm [in] Number of leading lines to be thrown away Optional
        (aafInt32  LeadingLines);

  //****************
  // GetLeadingLines()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetLeadingLines
        // @parm [out] Address to store the number of leading lines
        (aafInt32 *  pLeadingLines);

  //****************
  // SetTrailingLines()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetTrailingLines
        // @parm [in] Number of trailing lines to be thrown away Optional.
        (aafInt32  TrailingLines);

  //****************
  // GetTrailingLines()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetTrailingLines
        // @parm [out] Address to store the number of trailing lines.
        (aafInt32 *  pTrailingLines);

  //****************
  // SetJPEGTableID()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetJPEGTableID
        // @parm [in] Registered JPEG table code or JT_NULL.
        (aafJPEGTableID_t  JPEGTableID);

  //****************
  // GetJPEGTableID()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetJPEGTableID
        // @parm [out] Address to store the nJPEG table code
        (aafJPEGTableID_t *  pJPEGTableID);

  //****************
  // GetSummary()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetSummary
        (// @parm [in] Size of preallocated buffer
         aafUInt32  size,

         // @parm [out, size_is(size)] Preallocated buffer to hold the TIFF IFD file information
         aafDataValue_t  pSummary);

  //****************
  // GetSummaryBufferSize()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetSummaryBufferSize
        // @parm [out] required buffer size
        (aafUInt32 *  pSize);

  //****************
  // SetSummary()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetSummary
        (// @parm [in] Size of preallocated buffer
         aafUInt32  size,

         // @parm [in, size_is(size)] buffer containing value
         aafDataValue_t  pSummary);

private:
	OMFixedSizeProperty<aafBool>			_isUniform;
	OMFixedSizeProperty<aafBool>			_isContiguous;
	OMFixedSizeProperty<aafInt32>			_leadingLines;
	OMFixedSizeProperty<aafInt32>			_trailingLines;
	OMFixedSizeProperty<aafJPEGTableID_t>	_jpegTableID;
	OMVariableSizeProperty<aafUInt8>		_summary;

};

#endif // ! __ImplAAFTIFFDescriptor_h__


