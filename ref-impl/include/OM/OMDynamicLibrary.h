//=---------------------------------------------------------------------=
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

// @doc OMEXTERNAL
#ifndef OMDYNAMICLIBRARY_H
#define OMDYNAMICLIBRARY_H

#include <stddef.h>

  // @class Abstract base class describing dynamically loaded libraries.
  //   @cauthor Tim Bingham | tjb | Avid Technology, Inc.
class OMDynamicLibrary {
public:
  // @access Static members.

    // @cmember Find and load the given library.
  static OMDynamicLibrary* loadLibrary(const wchar_t* libraryName);

    // @cmember Unload the given library.
  static void unloadLibrary(OMDynamicLibrary* library);

  // @access Public members.

    // @cmember The address of the symbol named <p symbolName> in this
    //          <c OMDynamicLibrary>.
  virtual void* findSymbol(const char* symbolName) = 0;

protected:
  // @access Protected members.

    // @cmember Load the library described by this <c OMDynamicLibrary>.
  virtual bool load(void) = 0;

    // @cmember Unload the library described by this <c OMDynamicLibrary>.
  virtual void unload(void) = 0;

    // @cmember Destructor.
  virtual ~OMDynamicLibrary(void);

};


#endif
