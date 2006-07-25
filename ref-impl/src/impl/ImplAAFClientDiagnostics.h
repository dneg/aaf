//@doc
//@class    AAFClientDiagnostics | Implementation class for AAFClientDiagnostics
#ifndef __ImplAAFClientDiagnostics_h__
#define __ImplAAFClientDiagnostics_h__


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


#include "AAF.h"
#include "OMOStream.h"



// Stream which outputs to the client-supplied diagnostic output stream.
//
class ImplAAFClientDiagnostics : public OMOStream
{
    public:

        ImplAAFClientDiagnostics( IAAFDiagnosticOutput* pOuput );
        virtual ~ImplAAFClientDiagnostics();


    protected:

        // Put a character string.
        virtual OMOStream& put(const char* string);

        // Put a new line.
        virtual OMOStream& putLine();


    protected:

        IAAFDiagnosticOutput*   _pOutput;
};



#endif // ! __ImplAAFClientDiagnostics_h__
