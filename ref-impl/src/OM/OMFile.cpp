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
#include "OMFile.h"

#include "OMAssertions.h"
#include "OMStoredObject.h"
#include "OMClassFactory.h"
#include "OMObjectDirectory.h"
#include "OMPropertyTable.h"
#include "OMUtilities.h"
#include "OMDictionary.h"
#include "OMRootStorable.h"
#include "OMRawStorage.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define OM_OBJECT_DIRECTORY_CAPACITY  (5000)

  // @mfunc Constructor. Create an <c OMFile> object representing
  //        an existing external file.
  //   @parm The name of this <c OMFile>.
  //   @parm The access mode of this <c OMFile>.
  //   @parm The <c OMStoredObject> containing the root
  //         <c OMStorable> object.
  //   @parm The <c OMClassFactory> to use to restore objects from
  //         this <c OMFile>.
  //   @parm The <e OMFile::OMLoadMode> for this <c OMFile>.
OMFile::OMFile(const wchar_t* fileName,
               void* clientOnRestoreContext,
               const OMAccessMode mode,
               OMStoredObject* store,
               const OMClassFactory* factory,
               OMDictionary* dictionary,
               const OMLoadMode loadMode)
: _root(0),
  _rootStore(store),
  _dictionary(dictionary),
  _objectDirectory(0),
  _referencedProperties(0),
  _mode(mode),
  _loadMode(loadMode),
  _fileName(0),
  _clientOnSaveContext(0),
  _clientOnRestoreContext(clientOnRestoreContext),
  _encoding(MSSBinaryEncoding),
  _rawStorage(0)
{
  TRACE("OMFile::OMFile");

  PRECONDITION("Valid file name", validWideString(fileName));
  PRECONDITION("Valid dictionary", _dictionary != 0);
  _fileName = saveWideString(fileName);
  setClassFactory(factory);
  readSignature(_fileName, _signature);
  setName(L"/");
}

  // @mfunc Constructor. Create an <c OMFile> object representing
  //        a new external file.
  //   @parm The name of this <c OMFile>.
  //   @parm The signature of this <c OMFile>.
  //   @parm The access mode of this <c OMFile>.
  //   @parm The <c OMStoredObject> in which to store the root
  //         <c OMStorable> object.
  //   @parm The <c OMClassFactory> to use to restore objects from
  //         this <c OMFile>.
  //   @parm The root <c OMStorable> object to save in this file.
OMFile::OMFile(const wchar_t* fileName,
               void* clientOnRestoreContext,
               OMFileSignature signature,
               const OMAccessMode mode,
               OMStoredObject* store,
               const OMClassFactory* factory,
               OMDictionary* dictionary,
               OMRootStorable* root)
: _root(root),
  _rootStore(store),
  _dictionary(dictionary),
  _objectDirectory(0),
  _referencedProperties(0),
  _mode(mode),
  _loadMode(lazyLoad),
  _fileName(0),
  _signature(signature),
  _clientOnSaveContext(0),
  _clientOnRestoreContext(clientOnRestoreContext),
  _encoding(MSSBinaryEncoding),
  _rawStorage(0)
{
  TRACE("OMFile::OMFile");

  PRECONDITION("Valid file name", validWideString(fileName));
  PRECONDITION("Valid dictionary", _dictionary != 0);
  _fileName = saveWideString(fileName);
  setClassFactory(factory);
  setName(L"<file>");
  _root->attach(this, L"/");
  _root->setStore(_rootStore);
}

  // @mfunc Constructor. Create an <c OMFile> object representing
  //        an existing external file.
OMFile::OMFile(OMRawStorage* rawStorage,
               const OMFileEncoding encoding,
               void* clientOnRestoreContext,
               const OMAccessMode mode,
               OMStoredObject* store,
               const OMClassFactory* factory,
               OMDictionary* dictionary,
               const OMLoadMode loadMode)
: _root(0),
  _rootStore(store),
  _dictionary(dictionary),
  _objectDirectory(0),
  _referencedProperties(0),
  _mode(mode),
  _loadMode(loadMode),
  _fileName(0),
//_signature(), // tjb
  _clientOnSaveContext(0),
  _clientOnRestoreContext(clientOnRestoreContext),
  _encoding(encoding),
  _rawStorage(rawStorage)
{
  TRACE("OMFile::OMFile");

  PRECONDITION("Valid raw storage", _rawStorage != 0);
  PRECONDITION("Consistent access modes",
                     IMPLIES(((mode == modifyMode) || (mode == writeOnlyMode)),
                     rawStorage->isWritable()));
  PRECONDITION("Valid dictionary", _dictionary != 0);

  setClassFactory(factory);
  readSignature(_rawStorage, _signature);
  setName(L"/");
}

  // @mfunc Constructor. Create an <c OMFile> object representing
  //          a new external file.
OMFile::OMFile(OMRawStorage* rawStorage,
               const OMFileEncoding encoding,
               void* clientOnRestoreContext,
               OMFileSignature signature,
               const OMAccessMode mode,
               OMStoredObject* store,
               const OMClassFactory* factory,
               OMDictionary* dictionary,
               OMRootStorable* root)
: _root(root),
  _rootStore(store),
  _dictionary(dictionary),
  _objectDirectory(0),
  _referencedProperties(0),
  _mode(mode),
  _loadMode(lazyLoad),
  _fileName(0),
  _signature(signature),
  _clientOnSaveContext(0),
  _clientOnRestoreContext(clientOnRestoreContext),
  _encoding(encoding),
  _rawStorage(rawStorage)
{
  TRACE("OMFile::OMFile");

  PRECONDITION("Valid dictionary", _dictionary != 0);
  PRECONDITION("Valid raw storage", rawStorage != 0);
  PRECONDITION("Consistent access modes",
                     IMPLIES(((mode == modifyMode) || (mode == writeOnlyMode)),
                     rawStorage->isWritable()));

  setClassFactory(factory);
  setName(L"<file>");
  _root->attach(this, L"/");
  _root->setStore(_rootStore);
}

  // @mfunc Destructor.
OMFile::~OMFile(void)
{
  TRACE("OMFile::~OMFile");

  delete _objectDirectory;
  _objectDirectory = 0;
  delete _referencedProperties;
  _referencedProperties = 0;
  delete _fileName;
  _fileName = 0;
}

  // @mfunc Open an existing <c OMFile> for read-only access, the
  //        <c OMFile> is named <p fileName>, use the <c OMClassFactory>
  //        <p factory> to create the objects. The file must already
  //        exist.
  //   @parm The name of the file to open.
  //   @parm The factory to use for creating objects.
  //   @parm Specifies the use of lazy or eager loading.
  //   @rdesc The newly opened <c OMFile>.
OMFile* OMFile::openExistingRead(const wchar_t* fileName,
                                 const OMClassFactory* factory,
                                 void* clientOnRestoreContext,
                                 const OMLoadMode loadMode,
                                 OMDictionary* dictionary)
{
  TRACE("OMFile::openExistingRead");

  PRECONDITION("Valid file name", validWideString(fileName));
  PRECONDITION("Valid class factory", factory != 0);
  PRECONDITION("Valid dictionary", dictionary != 0);

  OMStoredObject* store = OMStoredObject::openRead(fileName);
  OMFile* newFile = new OMFile(fileName,
                               clientOnRestoreContext,
                               readOnlyMode,
                               store,
                               factory,
                               dictionary,
                               loadMode);
  ASSERT("Valid heap pointer", newFile != 0);
  POSTCONDITION("Object Manager file", newFile->isOMFile());
  return newFile;
}

  // @mfunc Open an existing <c OMFile> for modify access, the
  //        <c OMFile> is named <p fileName>, use the <c OMClassFactory>
  //        <p factory> to create the objects. The file must already
  //        exist.
  //   @parm The name of the file to open.
  //   @parm The factory to use for creating objects.
  //   @parm Specifies the use of lazy or eager loading.
  //   @rdesc The newly opened <c OMFile>.
OMFile* OMFile::openExistingModify(const wchar_t* fileName,
                                   const OMClassFactory* factory,
                                   void* clientOnRestoreContext,
                                   const OMLoadMode loadMode,
                                   OMDictionary* dictionary)
{
  TRACE("OMFile::openExistingModify");

  PRECONDITION("Valid file name", validWideString(fileName));
  PRECONDITION("Valid class factory", factory != 0);
  PRECONDITION("Valid dictionary", dictionary != 0);

  OMStoredObject* store = OMStoredObject::openModify(fileName);
  OMFile* newFile = new OMFile(fileName,
                               clientOnRestoreContext,
                               modifyMode,
                               store,
                               factory,
                               dictionary,
                               loadMode);
  ASSERT("Valid heap pointer", newFile != 0);
  POSTCONDITION("Object Manager file", newFile->isOMFile());
  return newFile;
}

  // @mfunc Open a new <c OMFile> for modify access, the
  //        <c OMFile> is named <p fileName>, use the <c OMClassFactory>
  //        <p factory> to create the objects. The file must not already
  //        exist. The byte ordering on the newly created file is given
  //        by <p byteOrder>. The client root <c OMStorable> in the newly
  //        created file is given by <p clientRoot>.
  //   @parm The name of the file to create.
  //   @parm The factory to use for creating objects.
  //   @parm The byte order to use for the newly created file.
  //   @parm The client root <c OMStorable> in the newly created file.
  //   @rdesc The newly created <c OMFile>.
OMFile* OMFile::openNewModify(const wchar_t* fileName,
                              const OMClassFactory* factory,
                              void* clientOnRestoreContext,
                              const OMByteOrder byteOrder,
                              OMStorable* clientRoot,
                              const OMFileSignature& signature,
                              OMDictionary* dictionary)
{
  TRACE("OMFile::openNewModify");

  PRECONDITION("Valid file name", validWideString(fileName));
  PRECONDITION("Valid class factory", factory != 0);
  PRECONDITION("Valid byte order",
                    ((byteOrder == littleEndian) || (byteOrder == bigEndian)));
  PRECONDITION("Valid client root", clientRoot != 0);
  PRECONDITION("Valid signature", validSignature(signature));
  PRECONDITION("Valid dictionary ", dictionary != 0);

  OMStoredObject* store = OMStoredObject::createModify(fileName, byteOrder);
  OMRootStorable* root = new OMRootStorable(clientRoot, dictionary);
  ASSERT("Valid heap pointer", root != 0);

  OMFile* newFile = new OMFile(fileName,
                               clientOnRestoreContext,
                               signature,
                               modifyMode,
                               store,
                               factory,
                               dictionary,
                               root);
  ASSERT("Valid heap pointer", newFile != 0);
  return newFile;
}

OMFile* OMFile::openExistingRead(OMRawStorage* rawStorage,
                                 const OMClassFactory* factory,
                                 void* clientOnRestoreContext,
                                 const OMLoadMode loadMode,
                                 OMDictionary* dictionary)
{
  TRACE("OMFile::openExistingRead");

  PRECONDITION("Valid raw storage", rawStorage != 0);
  PRECONDITION("Valid class factory", factory != 0);
  PRECONDITION("Valid dictionary", dictionary != 0);

  OMStoredObject* store = 0;
  OMFileSignature s;
  readSignature(rawStorage, s);
  OMFileEncoding encoding = encodingOf(s);
  ASSERT("Valid encoding", (encoding == MSSBinaryEncoding) ||
                           (encoding == KLVBinaryEncoding) ||
                           (encoding == XMLTextEncoding));

  // TBS

  ASSERT("Valid store", store != 0);

  OMFile* newFile = new OMFile(rawStorage,
                               encoding,
                               clientOnRestoreContext,
                               readOnlyMode,
                               store,
                               factory,
                               dictionary,
                               loadMode);
  ASSERT("Valid heap pointer", newFile != 0);
  POSTCONDITION("Object Manager file", newFile->isOMFile());
  return newFile;
}

OMFile* OMFile::openExistingModify(OMRawStorage* rawStorage,
                                   const OMClassFactory* factory,
                                   void* clientOnRestoreContext,
                                   const OMLoadMode loadMode,
                                   OMDictionary* dictionary)
{
  TRACE("OMFile::openExistingModify");

  PRECONDITION("Valid raw storage", rawStorage != 0);
  PRECONDITION("Valid class factory", factory != 0);
  PRECONDITION("Valid dictionary", dictionary != 0);

  OMStoredObject* store = 0;
  OMFileSignature s;
  readSignature(rawStorage, s);
  OMFileEncoding encoding = encodingOf(s);
  ASSERT("Valid encoding", (encoding == MSSBinaryEncoding) ||
                           (encoding == KLVBinaryEncoding) ||
                           (encoding == XMLTextEncoding));

  // TBS

  ASSERT("Valid store", store != 0);

  OMFile* newFile = new OMFile(rawStorage,
                               encoding,
                               clientOnRestoreContext,
                               modifyMode,
                               store,
                               factory,
                               dictionary,
                               loadMode);
  ASSERT("Valid heap pointer", newFile != 0);
  POSTCONDITION("Object Manager file", newFile->isOMFile());
  return newFile;
}

OMFile* OMFile::openNewModify(OMRawStorage* rawStorage,
                              const OMClassFactory* factory,
                              void* clientOnRestoreContext,
                              const OMByteOrder byteOrder,
                              OMStorable* clientRoot,
                              const OMFileSignature& signature,
                              OMDictionary* dictionary)
{
  TRACE("OMFile::openNewModify");

  PRECONDITION("Valid raw storage", rawStorage != 0);
  PRECONDITION("Valid class factory", factory != 0);
  PRECONDITION("Valid byte order",
                    ((byteOrder == littleEndian) || (byteOrder == bigEndian)));
  PRECONDITION("Valid client root", clientRoot != 0);
  PRECONDITION("Valid signature", validSignature(signature));
  PRECONDITION("Valid dictionary ", dictionary != 0);

  OMFileEncoding encoding = encodingOf(signature);
  ASSERT("Valid encoding", (encoding == MSSBinaryEncoding) ||
                           (encoding == KLVBinaryEncoding) ||
                           (encoding == XMLTextEncoding));
  OMStoredObject* store = 0;

  // TBS

  ASSERT("Valid store", store != 0);

  OMRootStorable* root = new OMRootStorable(clientRoot, dictionary);
  ASSERT("Valid heap pointer", root != 0);

  OMFile* newFile = new OMFile(rawStorage,
                               encoding,
                               clientOnRestoreContext,
                               signature,
                               modifyMode,
                               store,
                               factory,
                               dictionary,
                               root);
  ASSERT("Valid heap pointer", newFile != 0);
  return newFile;
}

   // @mfunc Is <p signature> a valid signature for an <c OMFile> ?
   //   @parm The signature to check.
   //   @rdesc True if <p signature> is a valid signature for an
   //          <c OMFile>, false otherwise.
bool OMFile::validSignature(const OMFileSignature& signature)
{
  TRACE("OMFile::validSignature");

  bool result;

  if (signature.Data3 == 0x4D4F) { // "OM"
    result = true;
  } else {
    result = false;
  }
  return result;
}

  // @mfunc Save all changes made to the contents of this
  //        <c OMFile>. It is not possible to save
  //        read-only or transient files.
  //   @parm Client context for callbacks.
void OMFile::saveFile(void* clientOnSaveContext)
{
  TRACE("OMFile::saveFile");

  _clientOnSaveContext = clientOnSaveContext;

  if (_mode == modifyMode) {
    _root->save();
    _rootStore->save(referencedProperties());
  }
}

  // @mfunc Save the entire contents of this <c OMFile> as well as
  //        any unsaved changes in the new file <p fileName>. The file
  //        must not already exist. <mf OMFile::saveAs> may be called
  //        for files opened in modify mode and for files opened in
  //        read-only and transient modes.
  //   @parm const wchar_t* | fileName | The name of the file to open.
  //   @this const
void OMFile::saveAs(const wchar_t* ANAME(fileName)) const
{
  TRACE("OMFile::saveAs");

  PRECONDITION("Valid file name", validWideString(fileName));

  ASSERT("Unimplemented code not reached", false);
}

  // @mfunc Discard all changes made to this <c OMFile> since the
  //        last <mf OMFile::save> or <mf OMFile::open>.
void OMFile::revert(void)
{
  TRACE("OMFile::revert");

  ASSERT("Unimplemented code not reached", false);
}

  // @mfunc Restore the client root <c OMStorable> object from this <c OMFile>.
  //   @rdesc The newly restored root <c OMStorable>.
OMStorable* OMFile::restore(void)
{
  TRACE("OMFile::restore");

  _rootStore->restore(_referencedProperties);

  OMClassId id;
  _rootStore->restore(id);
  ASSERT("Valid root stored object", id == OMRootStorable::_rootClassId);

  _root = new OMRootStorable();
  _root->attach(this, L"/");
  _root->setStore(_rootStore);
  _root->setClassFactory(_dictionary);

  _root->restoreContents();

  OMDictionary *metaDictionary = _root->dictionary();
  ASSERT("Consistent dictionaries", metaDictionary == _dictionary);
  _root->setClassFactory(classFactory());

  return root();
}

  // @mfunc Close this <c OMFile>, any unsaved changes are discarded.
void OMFile::close(void)
{
  TRACE("OMFile::close");

  _root->close();
  if (_mode == modifyMode) {
    writeSignature(_fileName, _signature);
  }
  _root->detach();
  delete _root;
  _root = 0;
}

  // @mfunc Retrieve the client root <c OMStorable> from this <c OMFile>.
  //   @rdesc The root <c OMStorable>.
OMStorable* OMFile::root(void)
{
  TRACE("OMFile::root");

  OMStorable* result;
  result = _root->clientRoot();
  return result;
}

OMDictionary* OMFile::dictionary(void) const
{
  TRACE("OMFile::dictionary");

  return _dictionary;
}

  // @mfunc Retrieve the <c OMPropertyTable> from this <c OMFile>.
  //   @rdesc The table of referenced properties.
OMPropertyTable* OMFile::referencedProperties(void)
{
  TRACE("OMFile::referencedProperties");

  if (_referencedProperties == 0) {
    _referencedProperties = new OMPropertyTable();
    ASSERT("Valid heap pointer", _referencedProperties != 0);
  }
  return _referencedProperties;
}

  // @mfunc Retrieve the <c OMObjectDirectory> from this <c OMFile>.
  //   @rdesc The <c OMObjectDirectory> associated with this file.
OMObjectDirectory* OMFile::objectDirectory(void)
{
  TRACE("OMFile::objectDirectory");

  if (_objectDirectory == 0) {
    _objectDirectory = new OMObjectDirectory(OM_OBJECT_DIRECTORY_CAPACITY);
    ASSERT("Valid heap pointer", _objectDirectory != 0);
  }
  return _objectDirectory;
}

  // @mfunc The byte order of this <c OMFile>.
  //   @rdesc The byte order of this <c OMFile>.
  //   @this const
OMByteOrder OMFile::byteOrder(void) const
{
  TRACE("OMFile::byteOrder");

  ASSERT("Valid root", _rootStore != 0);
  return _rootStore->byteOrder();
}

  // @mfunc The loading mode (eager or lazy) of this <c OMFile>.
  //   @rdesc The loading mode (eager or lazy) of this <c OMFile>.
  //   @this const
OMFile::OMLoadMode OMFile::loadMode(void) const
{
  TRACE("OMFile::loadMode");

  return _loadMode;
}

  // @mfunc Is this file recognized by the Object Manager ?
  //   @rdesc True if this file is recognized by the Object Manager,
  //          false otherwise.
  //   @this const
bool OMFile::isOMFile(void) const
{
  TRACE("OMFile::isOMFile");

  bool result;

  if (validSignature(_signature)) {
    result = true;
  } else {
    result = false;
  }
  return result;
}

  // @mfunc The signature of this <c OMFile>.
  //   @rdesc The signature of this <c OMFile>.
OMFileSignature OMFile::signature(void) const
{
  return _signature;
}

  // @mfunc The encoding of this <c OMFile>.
  //   @rdesc The encoding of the <c OMFile>.
OMFile::OMFileEncoding OMFile::encoding(void) const
{
  TRACE("OMFile::encoding");

  return _encoding;
}

  // @mfunc The raw storage on which this <c OMFile> is stored.
  //   @rdesc The raw storage on which the raw bytes of
  //          this < c OMFile> reside.
OMRawStorage* OMFile::rawStorage(void) const
{
  TRACE("OMFile::rawStorage");

  return _rawStorage;
}

  // @mfunc Find the property instance in this <c OMFile>
  //        named by <p propertyPathName>.
  //   @parm The pathname to the desired property.
  //   @rdesc The property instance.
  //   @this const
OMProperty* OMFile::findPropertyPath(const wchar_t* propertyPathName) const
{
  TRACE("OMFile::findPropertyPath");

  PRECONDITION("Valid property path name", validWideString(propertyPathName));
  PRECONDITION("Path name is absolute", propertyPathName[0] == L'/');
  PRECONDITION("Valid root", _root != 0);

  wchar_t* path = saveWideString(propertyPathName);

  wchar_t* element = path;
  element++; // skip first '/'

  const OMStorable* storable = _root;
  OMProperty* result = 0;

  wchar_t* end = findWideCharacter(element, L'/');

  while (end != 0) {
    *end = 0;
    storable = storable->find(element);
    ASSERT("Valid storable pointer", storable != 0);
    element = ++end;
    end = findWideCharacter(element, L'/');
  }

  if ((element != 0) && (lengthOfWideString(element) > 0)) {
    result = storable->findProperty(element);
  } else {
    result = 0;
  }

  delete [] path;
  return result;
}

OMPropertyId* OMFile::path(const wchar_t* propertyPathName) const
{
  TRACE("OMFile::path");

  wchar_t delimiter = L'/';
  PRECONDITION("Valid property path name", validWideString(propertyPathName));
  PRECONDITION("Path name is absolute", propertyPathName[0] == delimiter);
  PRECONDITION("Valid root", _root != 0);

  // Allocate result
  //
  size_t count = countWideCharacter(propertyPathName, delimiter);
  OMPropertyId* result = new OMPropertyId[count + 1];
  ASSERT("Valid heap pointer", result != 0);

  // Parse path name
  //
  wchar_t* path = saveWideString(propertyPathName);
  wchar_t* element = path;
  element++; // skip first '/'
  const OMStorable* storable = _root;
  OMProperty* property = 0;

  size_t index = 0;
  wchar_t* end = findWideCharacter(element, delimiter);

  while (end != 0) {
    *end = 0;
    property = storable->findProperty(element);
    result[index] = property->propertyId();
    index = index + 1;
    storable = storable->find(element);
    ASSERT("Valid storable pointer", storable != 0);
    element = ++end;
    end = findWideCharacter(element, delimiter);
  }

  if ((element != 0) && (lengthOfWideString(element) > 0)) {
    property = storable->findProperty(element);
    result[index] = property->propertyId();
    index = index + 1;
  }

  result[index] = 0;

  delete [] path;
  return result;
}

OMProperty* OMFile::findProperty(const OMPropertyId* path) const
{
  TRACE("OMFile::findProperty");

  PRECONDITION("Valid root", _root != 0);

  size_t count = lengthOfPropertyPath(path);
  const OMStorable* storable = _root;

  for (size_t index = 0; index < count - 1; index++) {
    OMPropertyId pid = path[index];
    storable = storable->find(pid);
    ASSERT("Valid storable pointer", storable != 0);
  }

  OMProperty* result = storable->findProperty(path[count - 1]);

  return result;
}

const OMClassId& OMFile::classId(void) const
{
  TRACE("OMFile::classId");

  return nullOMClassId;
}

OMFile* OMFile::file(void) const
{
  TRACE("OMFile::file");

  return const_cast<OMFile*>(this);
}

bool OMFile::inFile(void) const
{
  TRACE("OMFile::inFile");
  return true;
}

bool OMFile::persistent(void) const
{
  TRACE("OMFile::persistent");

  // Transient files NYI so by definition a file is persistent.
  //
  return true;
}

void* OMFile::clientOnSaveContext(void)
{
  return _clientOnSaveContext;
}

void* OMFile::clientOnRestoreContext(void)
{
  return _clientOnRestoreContext;
}

  // @mfunc Write the signature to the given file.
  //   @parm The file name.
  //   @parm The signature.
void OMFile::writeSignature(const wchar_t* fileName,
                            const OMFileSignature& signature)
{
  TRACE("OMFile::writeSignature");

  PRECONDITION("Valid file name", validWideString(fileName));

  OMFileSignature sig = signature;

  // There's no ANSI function to open a file with a wchar_t* name.
  // for now convert the name. In future add 
  // FILE* fopen(const wchar_t* fileName, const wchar_t* mode);
  //
  char cFileName[256];
  size_t status = wcstombs(cFileName, fileName, 256);
  ASSERT("Convert succeeded", status != (size_t)-1);

  if (hostByteOrder() != littleEndian) {
    OMStoredObject::reorderUniqueObjectIdentification(sig);
  }

  FILE* f = fopen(cFileName, "rb+");
  ASSERT("File exists", f != 0);
  status = fseek(f, 8, SEEK_SET);
  ASSERT("Seek succeeded", status == 0);
  status = fwrite(&sig, sizeof(sig), 1, f);
  ASSERT("Write succeeded", status == 1);

  fclose(f);
}

  // @mfunc Read the signature from the given file.
  //   @parm The file name.
  //   @parm The signature.
void OMFile::readSignature(const wchar_t* fileName,
                           OMFileSignature& signature)
{
  TRACE("OMFile::readSignature");

  PRECONDITION("Valid file name", validWideString(fileName));

  char cFileName[256];
  size_t status = wcstombs(cFileName, fileName, 256);
  ASSERT("Convert succeeded", status != (size_t)-1);

  FILE* f = fopen(cFileName, "rb");
  ASSERT("File exists", f != 0);
  status = fseek(f, 8, SEEK_SET);
  ASSERT("Seek succeeded", status == 0);
  status = fread(&signature, sizeof(signature), 1, f);
  ASSERT("Read succeeded", status == 1);

  fclose(f);

  if (hostByteOrder() != littleEndian) {
    OMStoredObject::reorderUniqueObjectIdentification(signature);
  }
}

  // @mfunc Write the signature to the given raw storage.
  //   @parm The raw storage.
  //   @parm The signature.
void OMFile::writeSignature(OMRawStorage* rawStorage,
                            const OMFileSignature& signature)
{
  TRACE("OMFile::writeSignature");

  OMFileSignature sig = signature;
  if (hostByteOrder() != littleEndian) {
    OMStoredObject::reorderUniqueObjectIdentification(sig);
  }

  OMUInt32 count;
  rawStorage->setPosition(8);
  rawStorage->write(reinterpret_cast<const OMByte*>(&sig), sizeof(sig), count);
  ASSERT("All bytes written", count == sizeof(sig));
}

  // @mfunc Read the signature from the given raw storage.
  //   @parm The raw storage.
  //   @parm The signature.
void OMFile::readSignature(OMRawStorage* rawStorage,
                           OMFileSignature& signature)
{
  TRACE("OMFile::readSignature");

  PRECONDITION("Valid raw storage", rawStorage != 0);

  OMUInt32 count;
  rawStorage->setPosition(8);
  rawStorage->read(reinterpret_cast<OMByte*>(&signature),
                   sizeof(signature),
                   count);
  ASSERT("All bytes read", count == sizeof(signature));
}

OMFile::OMFileEncoding OMFile::encodingOf(const OMFileSignature& signature)
{
  TRACE("OMFile::encodingOf");
  
  OMFileEncoding result;
  const char* p = reinterpret_cast<const char *>(&signature);
  char tag = p[3];
  switch (tag) {
  case 'B': // structured storage (binary)
    result = MSSBinaryEncoding;
    break;
  case 'K': // SMPTE KLV (binary)
    result = KLVBinaryEncoding;
    break;
  case 'X': // XML (text)
    result = XMLTextEncoding;
    break;
  default:
    ASSERT("Recognized encoding", false);
    break;
  }
  return result;
}
