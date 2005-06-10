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
// The Original Code of this file is Copyright 1998-2005, Licensor of the
// AAF Association.
// 
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

#include "ImplAAFAES3PCMDescriptor.h"
#include "AAFPropertyIDs.h"



ImplAAFAES3PCMDescriptor::ImplAAFAES3PCMDescriptor()
{
}



ImplAAFAES3PCMDescriptor::~ImplAAFAES3PCMDescriptor ()
{
}



AAFRESULT STDMETHODCALLTYPE ImplAAFAES3PCMDescriptor::Initialize()
{
    if( isInitialized() )
    {
        return AAFRESULT_ALREADY_INITIALIZED;
    }


    // Call parent class' Initialize.
    AAFRESULT  ar = ImplAAFPCMDescriptor::Initialize();

    // Initialize this class required properties
    // and set the object initialized.
    if( ar == AAFRESULT_SUCCESS )
    {
        setInitialized();
    }


    return ar;
}

