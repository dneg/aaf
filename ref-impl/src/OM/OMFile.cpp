/***********************************************************************
*
*              Copyright (c) 1998-1999 Avid Technology, Inc.
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
               const OMAccessMode mode,
               OMStoredObject* store,
               const OMClassFactory* factory,
               const OMLoadMode loadMode)
: _root(0), _rootStoredObject(store),
  _objectDirectory(0), _referencedProperties(0), _mode(mode),
  _loadMode(loadMode), _fileName(0)
{
  TRACE("OMFile::OMFile");

  PRECONDITION("Valid file name", validWideString(fileName));
  _fileName = saveWideString(fileName);
  setClassFactory(factory);
  readSignature(_fileName);
  setName("/");
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
               OMFileSignature signature,
               const OMAccessMode mode,
               OMStoredObject* store,
               const OMClassFactory* factory,
               OMStorable* root)
: _root(root), _rootStoredObject(store),
  _objectDirectory(0), _referencedProperties(0), _mode(mode),
  _loadMode(lazyLoad), _fileName(0), _signature(signature)
{
  TRACE("OMFile::OMFile");

  PRECONDITION("Valid file name", validWideString(fileName));
  _fileName = saveWideString(fileName);
  setClassFactory(factory);
  setName("<file>");
  _root->attach(this, "/");
  _root->setStore(rootStoredObject());
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
                                 const OMLoadMode loadMode)
{
  TRACE("OMFile::openExistingRead");
  PRECONDITION("Valid file name", validWideString(fileName));
  PRECONDITION("Valid class factory", factory != 0);

  OMStoredObject* store = OMStoredObject::openRead(fileName);
  OMFile* newFile = new OMFile(fileName,
                               readOnlyMode,
                               store,
                               factory,
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
                                   const OMLoadMode loadMode)
{
  TRACE("OMFile::openExistingModify");
  PRECONDITION("Valid file name", validWideString(fileName));
  PRECONDITION("Valid class factory", factory != 0);

  OMStoredObject* store = OMStoredObject::openModify(fileName);
  OMFile* newFile = new OMFile(fileName,
                               modifyMode,
                               store,
                               factory,
                               loadMode);
  ASSERT("Valid heap pointer", newFile != 0);
  POSTCONDITION("Object Manager file", newFile->isOMFile());
  return newFile;
}

  // @mfunc Open a new <c OMFile> for modify access, the
  //        <c OMFile> is named <p fileName>, use the <c OMClassFactory>
  //        <p factory> to create the objects. The file must not already
  //        exist. The byte ordering on the newly created file is given
  //        by <p byteOrder>. The root <c OMStorable> in the newly
  //        created file is given by <p root>.
  //   @parm The name of the file to create.
  //   @parm The factory to use for creating objects.
  //   @parm The byte order to use for the newly created file.
  //   @parm The root <c OMStorable> in the newly created file.
  //   @rdesc The newly created <c OMFile>.
OMFile* OMFile::openNewModify(const wchar_t* fileName,
                              const OMClassFactory* factory,
                              const OMByteOrder byteOrder,
                              OMStorable* root,
                              const OMFileSignature& signature)
{
  TRACE("OMFile::openNewModify");
  PRECONDITION("Valid file name", validWideString(fileName));
  PRECONDITION("Valid class factory", factory != 0);
  PRECONDITION("Valid byte order",
                    ((byteOrder == littleEndian) || (byteOrder == bigEndian)));
  PRECONDITION("Valid root", root != 0);
  PRECONDITION("Valid signature", validSignature(signature));

  OMStoredObject* store = OMStoredObject::createModify(fileName, byteOrder);
  OMFile* newFile = new OMFile(fileName,
                               signature,
                               modifyMode,
                               store,
                               factory,
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
  //        <c OMFile>. It is not possible to <mf OMFile::save>
  //        read-only or transient files.
  //   @parm Client context for callbacks.
void OMFile::save(void* clientContext)
{
  TRACE("OMFile::save");

  if (_mode == modifyMode) {
    _root->onSave(clientContext);
    _root->save(clientContext);
    _rootStoredObject->save(referencedProperties());
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

  // @mfunc Restore the root <c OMStorable> object from this <c OMFile>.
  //   @rdesc The newly restored roo <c OMStorable>.
OMStorable* OMFile::restore(void)
{
  TRACE("OMFile::restore");

  _rootStoredObject->restore(_referencedProperties);
  _root = OMStorable::restoreFrom(this, "/", *rootStoredObject());
  return root();
}

  // @mfunc Close this <c OMFile>, any unsaved changes are discarded.
void OMFile::close(void)
{
  TRACE("OMFile::close");

  _root->close();
  if (_mode == modifyMode) {
    writeSignature(_fileName);
  }
  _root->detach();
}

  // @mfunc Retrieve the root <c OMStorable> from this <c OMFile>.
  //   @rdesc The root <c OMStorable>.
OMStorable* OMFile::root(void)
{
  TRACE("OMFile::root");

  return _root;
}

  // @mfunc Retrieve the root <c OMStoredObject> from this <c OMFile>.
  //   @rdesc The root <c OMStoredObject>.
OMStoredObject* OMFile::rootStoredObject(void)
{
  TRACE("OMFile::rootStoredObject");

  return _rootStoredObject;
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

  ASSERT("Valid root", _rootStoredObject != 0);
  return _rootStoredObject->byteOrder();
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

  // @mfunc Find the property instance in this <c OMFile>
  //        named by <p propertyPathName>.
  //   @parm The pathname to the desired property.
  //   @rdesc The property instance.
  //   @this const
OMProperty* OMFile::findPropertyPath(const char* propertyPathName) const
{
  TRACE("OMFile::findPropertyPath");
  PRECONDITION("Valid property path name", validString(propertyPathName));
  PRECONDITION("Path name is absolute", propertyPathName[0] == '/');
  PRECONDITION("Valid root", _root != 0);

  char* path = new char[strlen(propertyPathName) + 1];
  ASSERT("Valid heap pointer", path != 0);
  strcpy(path, propertyPathName);
  
  char* element = path;
  element++; // skip first '/'

  const OMStorable* storable = _root;
  OMProperty* result = 0;

  char* end = strchr(element, '/');
  
  while (end != 0) {
    *end = 0;
    storable = storable->find(element);
    ASSERT("Valid storable pointer", storable != 0);
    element = ++end;
    end = strchr(element, '/');
  }

  if ((element != 0) && (strlen(element) > 0)) {
    result = storable->findProperty(element);
  } else {
    result = 0;
  }

  delete [] path;
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

  // @mfunc Write the signature to the given file.
  //   @parm The file name.
void OMFile::writeSignature(const wchar_t* fileName)
{
  TRACE("OMFile::writeSignature");
  OMFileSignature sig = _signature;

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
void OMFile::readSignature(const wchar_t* fileName)
{
  TRACE("OMFile::readSignature");
  OMFileSignature sig;

  char cFileName[256];
  size_t status = wcstombs(cFileName, fileName, 256);
  ASSERT("Convert succeeded", status != (size_t)-1);

  FILE* f = fopen(cFileName, "rb");
  ASSERT("File exists", f != 0);
  status = fseek(f, 8, SEEK_SET);
  ASSERT("Seek succeeded", status == 0);
  status = fread(&sig, sizeof(sig), 1, f);
  ASSERT("Read succeeded", status == 1);

  fclose(f);

  if (hostByteOrder() != littleEndian) {
    OMStoredObject::reorderUniqueObjectIdentification(sig);
  }

  _signature = sig;

}
