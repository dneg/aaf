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
#ifndef OMFILE_H
#define OMFILE_H

#include "OMStorable.h"
#include "OMDataTypes.h"

#include <stddef.h>

class OMClassFactory;
class OMObjectDirectory;
class OMPropertyTable;
class OMStoredObject;
class OMDictionary;
class OMRootStorable;
class OMRawStorage;

  // @class Files supported by the Object Manager.
  //   @base public | <c OMStorable>
  //   @cauthor Tim Bingham | tjb | Avid Technology, Inc.
class OMFile : public OMStorable {
public:

    // @cmember,menum File access modes.
  enum OMAccessMode {
    readOnlyMode,  // @@emem The file may be read but may not be written.
    writeOnlyMode, // @@emem The file may be written but may not be read.
    modifyMode     // @@emem The file may be read and/or written.
  };

    // @cmember,menum Lazy loading modes (degrees of indolence).
  enum OMLoadMode {
    eagerLoad,   // @@emem Objects are loaded when the root object is loaded.
    lazyLoad     // @@emem Objects are loaded on demand.
  };

    // @cmember,menum Supported file encodings.
  enum OMFileEncoding {
    MSSBinaryEncoding,  // @@emem Microsoft Structured Storage (binary).
    KLVBinaryEncoding,  // @@emem SMPTE KLV (binary).
    XMLTextEncoding     // @@emem XML (text).
  };

  // @access Static members.

    // @cmember Open an existing <c OMFile> for read-only access, the
    //          <c OMFile> is named <p fileName>, use the <c OMClassFactory>
    //          <p factory> to create the objects. The file must already
    //          exist.
  static OMFile* openExistingRead(const wchar_t* fileName,
                                  const OMClassFactory* factory,
                                  void* clientOnRestoreContext,
                                  const OMLoadMode loadMode,
                                  OMDictionary* dictionary = 0);

    // @cmember Open an existing <c OMFile> for modify access, the
    //          <c OMFile> is named <p fileName>, use the <c OMClassFactory>
    //          <p factory> to create the objects. The file must already
    //          exist.
  static OMFile* openExistingModify(const wchar_t* fileName,
                                    const OMClassFactory* factory,
                                    void* clientOnRestoreContext,
                                    const OMLoadMode loadMode,
                                    OMDictionary* dictionary = 0);

    // @cmember Open a new <c OMFile> for modify access, the
    //          <c OMFile> is named <p fileName>, use the <c OMClassFactory>
    //          <p factory> to create the objects. The file must not already
    //          exist. The byte ordering on the newly created file is given
    //          by <p byteOrder>. The client root <c OMStorable> in the newly
    //          created file is given by <p clientRoot>.
  static OMFile* openNewModify(const wchar_t* fileName,
                               const OMClassFactory* factory,
                               void* clientOnRestoreContext,
                               const OMByteOrder byteOrder,
                               OMStorable* clientRoot,
                               const OMFileSignature& signature,
                               OMDictionary* dictionary = 0);

    // @cmember Is the given <c OMRawStorage> compatible with the given file
    //          access mode and signature ? Can a file of the encoding
    //          specified by <p signature> be created successfully on
    //          <p rawStorage> and then accessed successfully in the mode
    //          specified by <p accessMode> ?
  static bool compatibleRawStorage(const OMRawStorage* rawStorage,
                                   const OMAccessMode accessMode,
                                   const OMFileSignature& signature);

    // @cmember Open an existing <c OMFile> for read-only access.
    // @devnote Will superceed openExistingRead() above.
  static OMFile* openExistingRead(OMRawStorage* rawStorage,
                                  const OMClassFactory* factory,
                                  void* clientOnRestoreContext,
                                  const OMLoadMode loadMode,
                                  OMDictionary* dictionary = 0);

    // @cmember Open an existing <c OMFile> for modify access.
    // @devnote Will superceed openExistingModify() above.
  static OMFile* openExistingModify(OMRawStorage* rawStorage,
                                    const OMClassFactory* factory,
                                    void* clientOnRestoreContext,
                                    const OMLoadMode loadMode,
                                    OMDictionary* dictionary = 0);

    // @cmember Open a new <c OMFile> for write access.
  static OMFile* openNewWrite(OMRawStorage* rawStorage,
                              const OMClassFactory* factory,
                              void* clientOnRestoreContext,
                              const OMByteOrder byteOrder,
                              OMStorable* clientRoot,
                              const OMFileSignature& signature,
                              OMDictionary* dictionary = 0);

    // @cmember Open a new <c OMFile> for modify access.
    // @devnote Will superceed openNewModify() above.
  static OMFile* openNewModify(OMRawStorage* rawStorage,
                               const OMClassFactory* factory,
                               void* clientOnRestoreContext,
                               const OMByteOrder byteOrder,
                               OMStorable* clientRoot,
                               const OMFileSignature& signature,
                               OMDictionary* dictionary = 0);

     // @cmember Is <p signature> a valid signature for an <c OMFile> ?
  static bool validSignature(const OMFileSignature& signature);


     // @cmember Is the file named <p fileName> a recognized file ?
     //          If so, the result is true, and the signature is returned
     //          in <p signature> and the encoding in <p encoding>.
  static bool isRecognized(const wchar_t* fileName,
                           OMFileSignature& signature,
                           OMFileEncoding& encoding);

     // @cmember Does <p rawStorage> contain a recognized file ?
     //          If so, the result is true, and the signature is returned
     //          in <p signature> and the encoding in <p encoding>.
  static bool isRecognized(OMRawStorage* rawStorage,
                           OMFileSignature& signature,
                           OMFileEncoding& encoding);

     // @cmember Is <p signature> recognized ?
     //          If so, the result is true, and the encoding in <p encoding>.
  static bool isRecognized(const OMFileSignature& signature,
                           OMFileEncoding& encoding);

  // @access Public members.

    // @cmember Destructor.
  ~OMFile(void);

    // @cmember Save all changes made to the contents of this
    //          <c OMFile>. It is not possible to save
    //          read-only or transient files.
    //   @precondition <f isOpen()>
  void saveFile(void* clientOnSaveContext = 0);

    // @cmember Save the entire contents of this <c OMFile> as well as
    //          any unsaved changes in the new file <p destFile>. <p destFile>
    //          must be open, writeable and not yet contain any objects.
    //          <mf OMFile::saveAsFile> may be called
    //          for files opened in modify mode and for files opened in
    //          read-only and transient modes.
  void saveAsFile(OMFile* destFile) const;

  void saveCopyAs(OMFile* destFile) const;

    // @cmember Discard all changes made to this <c OMFile> since the
    //          last <mf OMFile::save> or <mf OMFile::open>.
  void revert(void);

    // @cmember Restore the client root <c OMStorable> object from
    //          this <c OMFile>.
    //   @precondition <f isOpen()>
  OMStorable* restore(void);

    // @cmember Open this <c OMFile>.
    //   @precondition <f !isOpen()>
    //   @precondition <f !isClosed()>
    //   @postcondition <f isOpen()>
  void open(void);

    // @cmember Close this <c OMFile>, any unsaved changes are discarded.
    //   @precondition <f isOpen()>
    //   @postcondition <f !isOpen()>
    //   @postcondition <f isClosed()>
  void close(void);

    // @cmember Is this <c OMFile> open ?
  bool isOpen(void) const;

    // @cmember Is this <c OMFile> closed ? Note that <f isClosed()> is not the
    //          same as !<f isOpen()> since before open() is called,
    //          <f isClosed()> is false. That is, <f isClosed()> means
    //          "was once open and is now closed".
  bool isClosed(void) const;

    // @cmember Retrieve the client root <c OMStorable> from this <c OMFile>.
  OMStorable* clientRoot(void);

    // @cmember Retrieve the root <c OMStorable> from this <c OMFile>.
  OMStorable* root(void);

  OMDictionary* dictionary(void) const;

    // @cmember Retrieve the <c OMPropertyTable> from this <c OMFile>.
  OMPropertyTable* referencedProperties(void);

    // @cmember Retrieve the <c OMObjectDirectory> from this <c OMFile>.
  OMObjectDirectory* objectDirectory(void);

    // @cmember The byte order of this <c OMFile>.
  OMByteOrder byteOrder(void) const;

    // @cmember The loading mode (eager or lazy) of this <c OMFile>.
  OMLoadMode loadMode(void) const;

    // @cmember The access mode of this <c OMFile>.
  OMAccessMode accessMode(void) const;

    // @cmember Is it possible to read from this <c OMFile> ?
  bool isReadable(void) const;

    // @cmember Is it possible to write to this <c OMFile> ?
  bool isWritable(void) const;

    // @cmember Is this file recognized by the Object Manager ?
  bool isOMFile(void) const;

    // @cmember The name of this <c OMFile>.
    //   @devnote Soon to be obsolete.
  const wchar_t* fileName(void) const;

    // @cmember The signature of this <c OMFile>.
  OMFileSignature signature(void) const;

    // @cmember The encoding of this <c OMFile>.
  OMFileEncoding encoding(void) const;

    // @cmember The raw storage on which this <c OMFile> is stored.
  OMRawStorage* rawStorage(void) const;

    // @cmember Find the property instance in this <c OMFile>
    //          named by <p propertyPathName>.
  virtual OMProperty* findPropertyPath(const wchar_t* propertyPathName) const;

  OMPropertyId* path(const wchar_t* propertyPathName) const;

  OMProperty* findProperty(const OMPropertyId* path) const;

  // OMStorable overrides.
  //
  virtual const OMClassId& classId(void) const;

  virtual OMFile* file(void) const;

  virtual bool inFile(void) const;

  virtual bool persistent(void) const;

  void* clientOnSaveContext(void);

  void* clientOnRestoreContext(void);

private:
  // @access Private members.

    // @cmember Constructor. Create an <c OMFile> object representing
    //          an existing named external file.
  OMFile(const wchar_t* fileName,
         void* clientOnRestoreContext,
         OMFileSignature signature,
         const OMAccessMode mode,
         OMStoredObject* store,
         const OMClassFactory* factory,
         OMDictionary* dictionary,
         const OMLoadMode loadMode);

    // @cmember Constructor. Create an <c OMFile> object representing
    //          a new named external file.
  OMFile(const wchar_t* fileName,
         void* clientOnRestoreContext,
         OMFileSignature signature,
         const OMAccessMode mode,
         OMStoredObject* store,
         const OMClassFactory* factory,
         OMDictionary* dictionary,
         OMRootStorable* root);

    // @cmember Constructor. Create an <c OMFile> object representing
    //          an existing external file on the given <c OMRawStorage>.
    // @devnote Will superceed OMFile::OMFile (for existing files) above.
  OMFile(OMRawStorage* rawStorage,
         void* clientOnRestoreContext,
         const OMAccessMode mode,
         const OMClassFactory* factory,
         OMDictionary* dictionary,
         const OMLoadMode loadMode);

    // @cmember Constructor. Create an <c OMFile> object representing
    //          a new external file on the given <c OMRawStorage>.
    // @devnote Will superceed OMFile::OMFile (for new files) above.
  OMFile(OMRawStorage* rawStorage,
         void* clientOnRestoreContext,
         OMFileSignature signature,
         const OMAccessMode mode,
         const OMClassFactory* factory,
         OMDictionary* dictionary,
         OMRootStorable* root,
         const OMByteOrder byteOrder);

    // @cmember Read the signature from the given file.
  static void readSignature(const wchar_t* fileName,
                            OMFileSignature& signature);

    // @cmember Read the signature from the given raw storage.
  static void readSignature(OMRawStorage* rawStorage,
                            OMFileSignature& signature);

  static OMFileEncoding encodingOf(const OMFileSignature& signature);

  void openRead(void);

  void openModify(void);

  void createModify(void);

  void createWrite(void);

  OMRootStorable* restoreRoot(void);

  OMRootStorable* _root;
  OMStoredObject* _rootStore;

  OMDictionary* _dictionary;
  OMObjectDirectory* _objectDirectory;
  OMPropertyTable* _referencedProperties;

  enum OMAccessMode _mode;
  enum OMLoadMode _loadMode;
  wchar_t* _fileName;
  OMFileSignature _signature;

  void* _clientOnSaveContext;
  void* _clientOnRestoreContext;

  enum OMFileEncoding _encoding;
  OMRawStorage* _rawStorage;

  bool _isOpen;
  bool _isClosed;
  bool _isNew;
  OMByteOrder _byteOrder;

};

#endif
