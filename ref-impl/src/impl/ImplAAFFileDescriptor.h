//@doc
//@class    AAFFileDescriptor | Implementation class for AAFFileDescriptor
#ifndef __ImplAAFFileDescriptor_h__
#define __ImplAAFFileDescriptor_h__


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

#ifndef __ImplAAFEssenceDescriptor_h__
#include "ImplAAFEssenceDescriptor.h"
#endif



class ImplAAFFileDescriptor : public ImplAAFEssenceDescriptor
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFFileDescriptor ();
  virtual ~ImplAAFFileDescriptor ();



//@access Public Members

  virtual AAFRESULT STDMETHODCALLTYPE
	Initialize ();


  //****************
  // SetLength()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetLength
		// @parm [in] length of the essence in samples
        (aafLength_t  length);


  //****************
  // GetLength()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetLength
		// @parm [out] returns length of the essence in samples
        (aafLength_t *  pLength);


  //****************
  // SetIsInContainer()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetIsInContainer
		// @parm [in] is this AAF or raw essence
        (aafBool  isAAF);


  //****************
  // GetIsInContainer()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetIsInContainer
		// @parm [out] is this AAF or raw medi
        (aafBool*  pIsAAF);


  //****************
  // SetSampleRate()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetSampleRate
		// @parm [in] sample rate of the essence
        (aafRational_t *  pRate);


  //****************
  // GetSampleRate()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetSampleRate
		// @parm [out] sample rate of the essence
        (aafRational_t*  pRate);


  //****************
  // SetContainerFormat()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetContainerFormat
		// @parm [in] Identifies the file format
        (aafUID_t *  pFormat);


  //****************
  // GetContainerFormat()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetContainerFormat
		// @parm [out] Identifies the file format
        (aafUID_t *  pFormat);



public:
  // Declare this class to be storable.
  //
  OMDECLARE_STORABLE(ImplAAFFileDescriptor)

public:
	// Functions internal to the toolkit
	virtual AAFRESULT STDMETHODCALLTYPE
		GetOwningMobKind (aafMobKind_t *pMobKind);

private:
	OMFixedSizeProperty<aafRational_t>	_sampleRate;
	OMFixedSizeProperty<aafLength_t>	_length;
	OMFixedSizeProperty<aafBool>        _isInContainer;
	OMFixedSizeProperty<aafUID_t>       _containerFmt;
};

#endif // ! __ImplAAFFileDescriptor_h__
