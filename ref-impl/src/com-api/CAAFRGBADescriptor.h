//@doc
//@class    AAFRGBADescriptor | Implementation class for AAFRGBADescriptor
#ifndef __CAAFRGBADescriptor_h__
#define __CAAFRGBADescriptor_h__

//=---------------------------------------------------------------------=
//
// This file was GENERATED for the AAF SDK
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
// The Original Code of this file is Copyright 1998-2001, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=



#ifndef __AAF_h__
#include "AAF.h"
#endif









#ifndef __CAAFDigitalImageDescriptor_h__
#include "CAAFDigitalImageDescriptor.h"
#endif


class CAAFRGBADescriptor
  : public IAAFRGBADescriptor,
    public CAAFDigitalImageDescriptor
{
protected:

  //********
  //
  // Constructor/destructor
  //
  CAAFRGBADescriptor (IUnknown * pControllingUnknown, aafBool doInit = kAAFTrue);
  virtual ~CAAFRGBADescriptor ();

public:



  //***********************************************************
  //
  // SetPixelLayout()
  //
  // Sets the layout and structure of the components in a single pixel.
  // The layout array is an Array of 8 aafRGBAComponent_t.  The 'Code' field of the struct is
  // from the set:
  //   	- kAAFCompNone
  // 	- kAAFCompAlpha
  // 	- kAAFCompBlue
  // 	- kAAFCompFill
  // 	- kAAFCompGreen
  // 	- kAAFCompPalette
  // 	- kAAFCompRed
  //
  // The 'Size' field of the struct is the component size in bits.
  // 
  // If you set less than 8 entries, the remaining entries will be padded with kAAFCompNone and a size of zero.
  // If this method fails the PixelLayout property will not be changed.
  // 
  // This method will return the following codes:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  STDMETHOD (SetPixelLayout) (
    // The number of elements in each array
    /*[in]*/ aafUInt32  numberElements,

    // Array of up to 8 aafRGBAComponent_t indicating component order and size.
    /*[in]*/ aafRGBAComponent_t*  PixelLayoutArray);

  //***********************************************************
  //
  // CountPixelLayoutElements()
  //
  // This function returns the number of components in a pixel.
  // 
  // Succeeds if all of the following are true:
  // - this object has already been initialized.
  // - the pResult pointer is valid.
  // 
  // If this method fails no state is changed.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NOT_INITIALIZED
  //   - Initialize() has already been called on this object.
  //
  // AAFRESULT_NULL_PARAM
  //   - pResult is null.
  //
  STDMETHOD (CountPixelLayoutElements) (
    // Number of components 
    /*[out]*/ aafUInt32 *  pResult);

  //***********************************************************
  //
  // GetPixelLayout()
  //
  // Gets the layout and structure of the components in a single pixel.
  // The layout array is an Array of 8 aafRGBAComponent_t.  The 'Code' field of the struct is
  // from the set:
  //   	- kAAFCompNone
  // 	- kAAFCompAlpha
  // 	- kAAFCompBlue
  // 	- kAAFCompFill
  // 	- kAAFCompGreen
  // 	- kAAFCompPalette
  // 	- kAAFCompRed
  //
  // The 'Size' field of the struct is the component size in bits.
  // 
  // If you Get more entries than were set, the extra entries will be padded with kAAFCompNone and a size of zero.
  //
  // If this method fails, PixelLayoutArray will not be changed.
  // 
  // This method will return the following codes:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pNumberElements or PixelLayoutArray or PixelStructureArray is NULL.
  //
  // AAFRESULT_SMALLBUF
  //   - numberElements indicates that the array is too small to hold
  //     the data.
  //
  STDMETHOD (GetPixelLayout) (
    // The number of elements in the array
    /*[in]*/ aafUInt32  numberElements,

    // Array to hold up to 8 aafRGBAComponent_t indicating component order and size.
    /*[out, size_is(numberElements)]*/ aafRGBAComponent_t*  PixelLayoutArray);

  //***********************************************************
  //
  // SetPalette()
  //
  // Sets the palette property.  The palette must be in the form specified
  // by the palette layout and palette structure parameters.
  // 
  // Succeeds if all of the following are true:
  // - pPalette is a valid pointer
  // 
  // If this method fails, the palette property will not be
  // changed.
  // 
  // This method will return the following codes:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pPalette is NULL.
  //
  STDMETHOD (SetPalette) (
    // The number of bytes in the array
    /*[in]*/ aafUInt32  numberElements,

    // Array to hold the palette information
    /*[in, size_is(numberElements)]*/ aafUInt8*  pPalette);


  //***********************************************************
  //
  // GetPalette()
  //
  // Sets the palette property.  The palette must be in the form specified
  // by the palette layout and palette structure parameters.
  // The values are written to the array specified by pPalette,
  // which is of size numberElements.  The required size may be found
  // by calling GetPaletteSize().
  // 
  // Succeeds if all of the following are true:
  // - pPalette is a valid pointer.
  // - numberElements indicates the array is large enough to hold the
  //   data.
  // 
  // If this method fails, pPalette will not be changed.
  // 
  // This method will return the following codes:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pNumberElements is NULL.
  //
  // AAFRESULT_SMALLBUF
  //   - numberElements indicates that the array is too small to hold
  //     the data.
  //
  STDMETHOD (GetPalette) (
    // The number of bytes in the array
    /*[in]*/ aafUInt32  numberRecords,

    // Array to hold the palette information
    /*[out, size_is(numberRecords)]*/ aafUInt8*  pPalette);


  //***********************************************************
  //
  // GetPaletteSize()
  //
  // Get the number of bytes in the VideoLineMap property array.
  // 
  // Succeeds if all of the following are true:
  // - pNumberBytes is a valid pointer
  // 
  // If this method fails, *pNumberBytes will not be changed.
  // 
  // This method will return the following codes:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pNumberBytes is NULL.
  //
  STDMETHOD (GetPaletteSize) (
    // The number of bytes in the array 
    /*[out]*/ aafUInt32 *  pNumberBytes);

  //***********************************************************
  //
  // CountPaletteLayoutElements()
  //
  // This function returns the number of components in a pixel.
  // 
  // Succeeds if all of the following are true:
  // - this object has already been initialized.
  // - the pResult pointer is valid.
  // 
  // If this method fails no state is changed.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NOT_INITIALIZED
  //   - Initialize() has already been called on this object.
  //
  // AAFRESULT_NULL_PARAM
  //   - pResult is null.
  //
  STDMETHOD (CountPaletteLayoutElements) (
    // Number of components 
    /*[out]*/ aafUInt32 *  pResult);

  //***********************************************************
  //
  // SetPaletteLayout()
  //
  // Sets the layout and structure of the components in a single pixel.
  // The layout array is an Array of 8 aafRGBAComponent_t.  The 'Code' field of the struct is
  // from the set:
  //   	- kAAFCompNone
  // 	- kAAFCompAlpha
  // 	- kAAFCompBlue
  // 	- kAAFCompFill
  // 	- kAAFCompGreen
  // 	- kAAFCompPalette
  // 	- kAAFCompRed
  //
  // The 'Size' field of the struct is the component size in bits.
  // 
  // If you set less than 8 entries, the remaining entries will be padded with kAAFCompNone and a size of zero.
  // If this method fails the PaletteLayout property will not be changed.
  // 
  // This method will return the following codes:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  STDMETHOD (SetPaletteLayout) (
    // The number of elements in the array
    /*[in]*/ aafUInt32  numberElements,

    // Array of up to 8 bytes indicating component size, matching layout, and terminated by a zero byte.
    /*[in]*/ aafRGBAComponent_t*  PaletteLayoutArray);

  //***********************************************************
  //
  // GetPaletteLayout()
  //
  // // The layout array is an Array of 8 aafRGBAComponent_t.  The 'Code' field of the struct is
  // from the set:
  //   	- kAAFCompNone
  // 	- kAAFCompAlpha
  // 	- kAAFCompBlue
  // 	- kAAFCompFill
  // 	- kAAFCompGreen
  // 	- kAAFCompPalette
  // 	- kAAFCompRed
  //
  // The 'Size' field of the struct is the component size in bits.
  // 
  // If you Get more entries than were set, the extra entries will be padded with kAAFCompNone and a size of zero.
  //
  // If this method fails, PaletteLayoutArray will not be changed.
  // 
  // This method will return the following codes:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pNumberElements or PaletteLayoutArray is NULL.
  //
  // AAFRESULT_SMALLBUF
  //   - numberElements indicates that the array is too small to hold
  //     the data.
  //
  STDMETHOD (GetPaletteLayout) (
    // The number of elements in each array
    /*[in]*/ aafUInt32  numberElements,

    // Array to hold 8 aafRGBAComponent_t indicating component order and size.
    /*[out, size_is(numberElements)]*/ aafRGBAComponent_t*  PaletteLayoutArray);


protected:
  // 
  // Declare the QI that implements for the interfaces
  // for this module. This will be called by CAAFUnknown::QueryInterface().
  // 
  STDMETHOD(InternalQueryInterface)(REFIID riid, void **ppvObjOut);


public:
  //
  // This class as concrete. All AAF objects can be constructed from
  // a CLSID. This will allow subclassing all "base-classes" by
  // aggreggation.
  // 
  AAF_DECLARE_CONCRETE();
  //
  //********
};

#endif // ! __CAAFRGBADescriptor_h__


