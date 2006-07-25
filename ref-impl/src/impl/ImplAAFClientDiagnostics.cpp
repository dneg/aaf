
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


#include "ImplAAFClientDiagnostics.h"

#include "OMAssertions.h"
#include <stddef.h>
#include <string.h>

#include "OMUtilities.h"




ImplAAFClientDiagnostics::ImplAAFClientDiagnostics(
    IAAFDiagnosticOutput* pOutput ) :

    _pOutput( NULL )
{
    ASSERTU( pOutput != NULL );

    pOutput->AddRef();
    _pOutput = pOutput;
}



ImplAAFClientDiagnostics::~ImplAAFClientDiagnostics()
{
    _pOutput->Release();
    _pOutput = NULL;
}



OMOStream& ImplAAFClientDiagnostics::put(
    const char* pString )
{
    ASSERTU( pString != NULL );


    // Convert the passed-in string to a wide character string
    const size_t stringLength = strlen( pString );
    wchar_t*  pWideString = new wchar_t[ stringLength + 1 ];
    ASSERTU( pWideString != NULL );
    convertStringToWideString( pWideString, pString, stringLength + 1 );

    // Put the converted string to the client-supplied stream
    _pOutput->PutString( pWideString );

    delete[] pWideString;
    pWideString = NULL;


    return *this;
}



OMOStream& ImplAAFClientDiagnostics::putLine()
{
    const wchar_t  newLine[] = L"\n";
    _pOutput->PutString( newLine );


    return *this;
}
