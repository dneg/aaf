/***********************************************************************
*
*              Copyright (c) 1998-2000 Avid Technology, Inc.
*
* Permission to use, copy and modify this software and accompanying
* documentation, and to distribute and sublicense application software
* incorporating this software for any purpose is hereby granted,
* provided that (i) the above copyright notice and this permission
* notice appear in all copies of the software and related documentation,
* and (ii) the name Avid Technology, Inc. may not be used in any
* advertising or publicity relating to the software without the specific,
* prior written permission of Avid Technology, Inc.
*
* THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND,
* EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY
* WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
* IN NO EVENT SHALL AVID TECHNOLOGY, INC. BE LIABLE FOR ANY DIRECT,
* SPECIAL, INCIDENTAL, PUNITIVE, INDIRECT, ECONOMIC, CONSEQUENTIAL OR
* OTHER DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER ARISING OUT OF
* OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE AND
* ACCOMPANYING DOCUMENTATION, INCLUDING, WITHOUT LIMITATION, DAMAGES
* RESULTING FROM LOSS OF USE, DATA OR PROFITS, AND WHETHER OR NOT
* ADVISED OF THE POSSIBILITY OF DAMAGE, REGARDLESS OF THE THEORY OF
* LIABILITY.
*
************************************************************************/

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
//
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

     // @cmember Is <p signature> a valid signature for an <c OMFile> ?
  static bool validSignature(const OMFileSignature& signature);

  // @access Public members.

    // @cmember Constructor. Create an <c OMFile> object representing
    //          an existing external file.
  OMFile(const wchar_t* fileName,
         void* clientOnRestoreContext,
         const OMAccessMode mode,
         OMStoredObject* store,
         const OMClassFactory* factory,
         OMDictionary* dictionary,
         const OMLoadMode loadMode);

    // @cmember Constructor. Create an <c OMFile> object representing
    //          a new external file.
  OMFile(const wchar_t* fileName,
         void* clientOnRestoreContext,
         OMFileSignature signature,
         const OMAccessMode mode,
         OMStoredObject* store,
         const OMClassFactory* factory,
         OMDictionary* dictionary,
         OMRootStorable* root);

    // @cmember Destructor.
  ~OMFile(void);

    // @cmember Save all changes made to the contents of this
    //          <c OMFile>. It is not possible to save
    //          read-only or transient files.
  void saveFile(void* clientOnSaveContext = 0);

    // @cmember Save the entire contents of this <c OMFile> as well as
    //          any unsaved changes in the new file <p fileName>. The file
    //          must not already exist. <mf OMFile::saveAs> may be called
    //          for files opened in modify mode and for files opened in
    //          read-only and transient modes.
  void saveAs(const wchar_t* fileName) const;

    // @cmember Discard all changes made to this <c OMFile> since the
    //          last <mf OMFile::save> or <mf OMFile::open>.
  void revert(void);

    // @cmember Restore the client root <c OMStorable> object from
    //          this <c OMFile>.
  OMStorable* restore(void);

    // @cmember Close this <c OMFile>, any unsaved changes are discarded.
  void close(void);

    // @cmember Retrieve the client root <c OMStorable> from this <c OMFile>.
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

    // @cmember Is this file recognized by the Object Manager ?
  bool isOMFile(void) const;

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

    // @cmember Write the signature to the given file.
  static void writeSignature(const wchar_t* fileName,
                             const OMFileSignature& signature);

    // @cmember Read the signature from the given file.
  static void readSignature(const wchar_t* fileName,
                            OMFileSignature& signature);

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
};

#endif
