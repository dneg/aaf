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
// The Original Code of this file is Copyright 1998-2002, Licensor of the
// AAF Association.
// 
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

// @doc OMINTERNAL
#ifndef OMXMLSTOREDOBJECTFACTORY_H
#define OMXMLSTOREDOBJECTFACTORY_H

#include "OMStoredObjectFactory.h"
#include "OMDataTypes.h"
#include <stdlib.h>

class OMRawStorage;

  // @class Factories for <c OMXMLStoredObject>
  //   @cauthor Tim Bingham | tjb | Avid Technology, Inc.
class OMXMLStoredObjectFactory : public OMStoredObjectFactory {
public:
  // @access Public members.

    // @cmember Constructor.
  OMXMLStoredObjectFactory(const OMStoredObjectEncoding& encoding,
                           const wchar_t* name,
                           const wchar_t* description);

    // @cmember Destructor.
  virtual ~OMXMLStoredObjectFactory(void);

    // @cmember Initialize this <c OMXMLStoredObjectFactory>.
  virtual void initialize(void);

    // @cmember Finalize this <c OMXMLStoredObjectFactory>.
  virtual void finalize(void);

    // @cmember Open the root <c OMXMLStoredObject> in the raw storage
    //          <p rawStorage> for reading only.
  virtual OMStoredObject* openRead(OMRawStorage* rawStorage);

    // @cmember Open the root <c OMXMLStoredObject> in the raw storage
    //          <p rawStorage> for modification.
  virtual OMStoredObject* openModify(OMRawStorage* rawStorage);

    // @cmember Create a new root <c OMXMLStoredObject> in the raw storage
    //          <p rawStorage>. The byte order of the newly created root
    //          is given by <p byteOrder>.
  virtual OMStoredObject* createWrite(OMRawStorage* rawStorage,
                                      const OMByteOrder byteOrder);

    // @cmember Create a new root <c OMXMLStoredObject> in the raw storage
    //          <p rawStorage>. The byte order of the newly created root
    //          is given by <p byteOrder>.
  virtual OMStoredObject* createModify(OMRawStorage* rawStorage,
                                       const OMByteOrder byteOrder);

    // @cmember Open the root <c OMXMLStoredObject> in the disk file
    //          <p fileName> for reading only.
  virtual OMStoredObject* openRead(const wchar_t* fileName);

    // @cmember Open the root <c OMXMLStoredObject> in the disk file
    //          <p fileName> for modification.
  virtual OMStoredObject* openModify(const wchar_t* fileName);

    // @cmember Create a new root <c OMXMLStoredObject> in the disk file
    //          <p fileName>. The byte order of the newly created root
    //          is given by <p byteOrder>.
  virtual OMStoredObject* createModify(const wchar_t* fileName,
                                       const OMByteOrder byteOrder);

    // @cmember Create a new root <c OMXMLStoredObject> in the disk file
    //          <p fileName>. The byte order of the newly created root
    //          is given by <p byteOrder>.
  virtual OMStoredObject* createWrite(const wchar_t* fileName,
                                      const OMByteOrder byteOrder);

    // @cmember Is the file named <p fileName> a recognized file ?
    //          If so, the result is true.
  virtual bool isRecognized(const wchar_t* fileName);

    // @cmember Does <p rawStorage> contain a recognized file ?
    //          If so, the result is true.
  virtual bool isRecognized(OMRawStorage* rawStorage);

    // @cmember Can a file be created successfully on the given
    //          <c OMRawStorage> and accessed successfully in the mode
    //          specified by <p accessMode> ?
  virtual bool compatibleRawStorage(const OMRawStorage* rawStorage,
                                    const OMFile::OMAccessMode accessMode);

    // @cmember Can a file be created successfully as a named file and
    //          accessed successfully in the mode specified by <p accessMode> ?
  virtual bool compatibleNamedFile(const OMFile::OMAccessMode accessMode);

    // @cmember Perform any necessary actions when the file
    //          contained in <p fileName> is closed.
  virtual void close(const wchar_t* fileName);

    // @cmember Perform any necessary actions when the file
    //          contained in <p rawStorage> is closed.
  virtual void close(OMRawStorage* rawStorage);

private:
  // @access Private members.

    // @cmember Read the signature from the given raw storage.
  static void readSignature(OMRawStorage* rawStorage,
                            char* signature,
                            size_t signatureSize);

};

#endif
