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

// @doc OMINTERNAL
#ifndef OMGSFxMSSSTOREDOBJECTFACTORY_H
#define OMGSFxMSSSTOREDOBJECTFACTORY_H

#include "OMMSSStoredObjectFactory.h"
#include "OMDataTypes.h"
#include <stdlib.h>

class OMRawStorage;

  // @class Factories for <c OMMSSStoredObject>
  //   @cauthor Tim Bingham | tjb | Avid Technology, Inc.
class OMGSFxMSSStoredObjectFactory : public OMMSSStoredObjectFactory {
public:
  // @access Public members.

    // @cmember Constructor.
  OMGSFxMSSStoredObjectFactory(const OMStoredObjectEncoding& encoding,
                           const OMUniqueObjectIdentification& signature,
                           const wchar_t* name,
                           const wchar_t* description);

    // @cmember Destructor.
  virtual ~OMGSFxMSSStoredObjectFactory(void);

    // @cmember Initialize this <c OMGSFxMSSStoredObjectFactory>.
  virtual void initialize(void);

    // @cmember Finalize this <c OMGSFxMSSStoredObjectFactory>.
  virtual void finalize(void);

    // @cmember Can a file be created successfully on the given
    //          <c OMRawStorage> and accessed successfully in the mode
    //          specified by <p accessMode> ?
  virtual bool compatibleRawStorage(const OMRawStorage* rawStorage,
                                    const OMFile::OMAccessMode accessMode);

    // @cmember Can a file be created successfully as a named file and
    //          accessed successfully in the mode specified by <p accessMode> ?
  virtual bool compatibleNamedFile(const OMFile::OMAccessMode accessMode);

protected:
  virtual OMStoredObject* openFile (OMRawStorage* rawStorage,
                                     const OMFile::OMAccessMode mode);

  virtual OMStoredObject* openFile (const wchar_t* fileName,
                                     const OMFile::OMAccessMode mode);

  virtual OMStoredObject* createFile (OMRawStorage* rawStorage,
                             const OMByteOrder byteOrder,
							 const OMUniqueObjectIdentification& signature);

  virtual OMStoredObject* createFile (const wchar_t* fileName,
                             const OMByteOrder byteOrder,
							 const OMUniqueObjectIdentification& signature);
};

#endif
