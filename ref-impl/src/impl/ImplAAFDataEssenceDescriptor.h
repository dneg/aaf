//@doc
//@class    AAFDataEssenceDescriptor | Implementation class for AAFDataEssenceDescriptor
#ifndef __ImplAAFDataEssenceDescriptor_h__
#define __ImplAAFDataEssenceDescriptor_h__

//=---------------------------------------------------------------------=
//
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
// The Original Code of this file is Copyright 1998-2005, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=











#ifndef __ImplAAFFileDescriptor_h__
#include "ImplAAFFileDescriptor.h"
#endif


class ImplAAFDataEssenceDescriptor : public ImplAAFFileDescriptor
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFDataEssenceDescriptor ();


  virtual ~ImplAAFDataEssenceDescriptor ();

  virtual AAFRESULT STDMETHODCALLTYPE
	Initialize ();

public:


  //****************
  // SetDataEssenceCoding()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetDataEssenceCoding
        // @parm [in] Optional.
        (const aafUID_t & dataEssenceCoding);


  //****************
  // GetDataEssenceCoding()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetDataEssenceCoding
        // @parm [out] Optional.
        (aafUID_t *  pDataEssenceCoding);
        
protected:

	OMFixedSizeProperty<aafUID_t>		_dataEssenceCoding;

};

#endif // ! __ImplAAFDataEssenceDescriptor_h__

