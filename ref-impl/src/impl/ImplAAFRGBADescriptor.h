//@doc
//@class    AAFRGBADescriptor | Implementation class for AAFRGBADescriptor
#ifndef __ImplAAFRGBADescriptor_h__
#define __ImplAAFRGBADescriptor_h__


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











#ifndef __ImplAAFDigitalImageDescriptor_h__
#include "ImplAAFDigitalImageDescriptor.h"
#endif

typedef struct
{
	aafRGBAComponent_t	comps[MAX_NUM_RGBA_COMPS];
} RGBComponentArray;

class ImplAAFRGBADescriptor : public ImplAAFDigitalImageDescriptor
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFRGBADescriptor ();

protected:
  virtual ~ImplAAFRGBADescriptor ();

public:


  //****************
  // SetPixelLayoutStructure()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetPixelLayout
        (
		aafInt32	numberElements,

         // @parm [in] Array of up to 8 aafRGBAComponent_t indicating component type and size.
         aafRGBAComponent_t*  PixelLayoutArray);

   virtual AAFRESULT  STDMETHODCALLTYPE
		CountPixelLayoutElements (aafUInt32 *  pResult);

   //****************
  // GetPixelLayoutStructure()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetPixelLayout
        (// @parm [in] The number of elements in each array
         aafUInt32  numberElements,

         // @parm [out, size_is(numberElements)] Array of up to 8 aafRGBAComponent_t indicating component type and size.
         aafRGBAComponent_t*  PixelLayoutArray);

  //****************
  // SetPalette()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetPalette
        (// @parm [in] The number of bytes in the array
         aafUInt32  numberElements,

         // @parm [in, size_is(numberElements)] Array to hold the palette information
         aafUInt8*  pPalette);


  //****************
  // GetPalette()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetPalette
        (// @parm [in] The number of bytes in the array
         aafUInt32  numberBytes,

         // @parm [out, size_is(numberElements)] Array to hold the palette information
         aafUInt8 *  pPalette);


  //****************
  // GetPaletteSize()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetPaletteSize
        // @parm [out] The number of bytes in the array
        (aafUInt32 *  pNumberBytes);

  //****************
  // SetPaletteLayoute()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetPaletteLayout
        (aafInt32	numberElements,

         // @parm [in] Array of up to 8 aafRGBAComponent_t indicating component type and size.
         aafRGBAComponent_t*  PaletteLayoutArray);

   virtual AAFRESULT  STDMETHODCALLTYPE
		CountPaletteLayoutElements (aafUInt32 *  pResult);

 //****************
  // GetPaletteLayout()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetPaletteLayout
        (// @parm [in] The number of elements in each array
         aafUInt32  numberElements,

         // @parm [out, size_is(numberElements)] Array of up to 8 aafRGBAComponent_t indicating component type and size.
         aafRGBAComponent_t*  PaletteLayoutArray);

protected:
	OMFixedSizeProperty<RGBComponentArray>	_pixelLayout;
	OMVariableSizeProperty<aafUInt8>		_palette;
	OMFixedSizeProperty<RGBComponentArray>	_paletteLayout;
};

#endif // ! __ImplAAFRGBADescriptor_h__


