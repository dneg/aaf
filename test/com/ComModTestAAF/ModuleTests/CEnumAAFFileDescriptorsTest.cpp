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
//

#include <iostream>
#include <assert.h>
using namespace std;


#include "AAF.h"
#include "AAFResult.h"

#include "ModuleTest.h"

// Required function prototype.
extern "C" HRESULT CEnumAAFFileDescriptors_test(
    testMode_t mode,
    aafUID_t fileKind,
    testRawStorageType_t rawStorageType,
    aafProductIdentification_t productID);

// The public entry for this module test,
//

HRESULT CEnumAAFFileDescriptors_test(
    testMode_t,
    aafUID_t,
    testRawStorageType_t,
    aafProductIdentification_t)
{
    HRESULT                 ar = AAFRESULT_NOT_IMPLEMENTED;
    return ar;
}
