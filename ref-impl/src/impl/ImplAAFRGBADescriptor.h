//@doc
//@class    AAFRGBADescriptor | Implementation class for AAFRGBADescriptor
#ifndef __ImplAAFRGBADescriptor_h__
#define __ImplAAFRGBADescriptor_h__


//=---------------------------------------------------------------------=
//
// $Id$ $Name$
//
// The contents of this file are subject to the AAF SDK Public
// Source License Agreement (the "License"); You may not use this file
// except in compliance with the License.  The License is available in
// AAFSDKPSL.TXT, or you may obtain a copy of the License from the AAF
// Association or its successor.
//
// Software distributed under the License is distributed on an "AS IS"
// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
// the License for the specific language governing rights and limitations
// under the License.
//
// The Original Code of this file is Copyright 1998-2004, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=











#ifndef __ImplAAFDigitalImageDescriptor_h__
#include "ImplAAFDigitalImageDescriptor.h"
#endif

#include "OMVariableSizeProperty.h"

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


