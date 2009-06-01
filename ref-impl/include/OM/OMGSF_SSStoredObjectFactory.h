//=---------------------------------------------------------------------=
//
// $Id$ $Name$
//
// The contents of this file are subject to the AAF SDK Public Source
// License Agreement Version 2.0 (the "License"); You may not use this
// file except in compliance with the License.  The License is available
// in AAFSDKPSL.TXT, or you may obtain a copy of the License from the
// Advanced Media Workflow Association, Inc., or its successor.
//
// Software distributed under the License is distributed on an "AS IS"
// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
// the License for the specific language governing rights and limitations
// under the License.  Refer to Section 3.3 of the License for proper use
// of this Exhibit.
//
// WARNING:  Please contact the Advanced Media Workflow Association,
// Inc., for more information about any additional licenses to
// intellectual property covering the AAF Standard that may be required
// to create and distribute AAF compliant products.
// (http://www.amwa.tv/policies).
//
// Copyright Notices:
// The Original Code of this file is Copyright 1998-2009, licensor of the
// Advanced Media Workflow Association.  All rights reserved.
//
// The Initial Developer of the Original Code of this file and the
// licensor of the Advanced Media Workflow Association is
// Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

// @doc OMINTERNAL
#ifndef OMGSFxMSSSTOREDOBJECTFACTORY_H
#define OMGSFxMSSSTOREDOBJECTFACTORY_H

#ifndef OM_NO_STRUCTURED_STORAGE

#include "OMSSStoredObjectFactory.h"
#include "OMDataTypes.h"
#include <stdlib.h>

class OMRawStorage;

  // @class Factories for <c OMSSStoredObject>
  //   @cauthor Tim Bingham | tjb | Avid Technology, Inc.
class OMGSF_SSStoredObjectFactory : public OMSSStoredObjectFactory {
public:
  // @access Public members.

    // @cmember Constructor.
  OMGSF_SSStoredObjectFactory(const OMStoredObjectEncoding& encoding,
                           const OMUniqueObjectIdentification& signature,
                           const wchar_t* name,
                           const wchar_t* description);

    // @cmember Destructor.
  virtual ~OMGSF_SSStoredObjectFactory(void);

    // @cmember Initialize this <c OMGSF_SSStoredObjectFactory>.
  virtual void initialize(void);

    // @cmember Finalize this <c OMGSF_SSStoredObjectFactory>.
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

#endif // OM_NO_STRUCTURED_STORAGE

#endif // !OMGSF_SSSTOREDOBJECTFACTORY_H
