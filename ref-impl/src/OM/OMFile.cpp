#include "OMFile.h"

#include "OMAssertions.h"
#include "OMStoredObject.h"
#include "OMClassFactory.h"
#include "OMObjectDirectory.h"

#include <string.h>

#define OM_OBJECT_DIRECTORY_CAPACITY  (5000)

OMFile::OMFile(const OMAccessMode mode,
               OMStoredObject* store,
               const OMClassFactory* factory)
: _root(0), _rootStoredObject(store), _classFactory(factory),
  _objectDirectory(0), _mode(mode)
{
  TRACE("OMFile::OMFile");

  setName("/");
}

OMFile::OMFile(const OMAccessMode mode,
               OMStoredObject* store,
               const OMClassFactory* factory,
               OMStorable* root)
: _root(root), _rootStoredObject(store), _classFactory(factory),
  _objectDirectory(0), _mode(mode)
{
  TRACE("OMFile::OMFile");

  setName("/");
  _root->setContainingObject(this);
  _root->setName("head");
  _root->setStore(rootStoredObject());
}

OMFile::~OMFile(void)
{
  TRACE("OMFile::~OMFile");

  _classFactory = 0;
  delete _objectDirectory;
  _objectDirectory = 0;
}

OMFile* OMFile::openRead(const wchar_t* fileName,
                         const OMClassFactory* factory)
{
  TRACE("OMFile::openRead");
  PRECONDITION("Valid file name", validWideString(fileName));
  PRECONDITION("Valid class factory", factory != 0);

  OMStoredObject* store = OMStoredObject::openRead(fileName);
  OMFile* newFile = new OMFile(readOnlyMode, store, factory);
  ASSERT("Valid heap pointer", newFile != 0);
  return newFile;
}

OMFile* OMFile::openModify(const wchar_t* fileName,
                           const OMClassFactory* factory)
{
  TRACE("OMFile::openModify");
  PRECONDITION("Valid file name", validWideString(fileName));
  PRECONDITION("Valid class factory", factory != 0);

  OMStoredObject* store = OMStoredObject::openModify(fileName);
  OMFile* newFile = new OMFile(modifyMode, store, factory);
  ASSERT("Valid heap pointer", newFile != 0);
  return newFile;
}

OMFile* OMFile::createWrite(const wchar_t* fileName,
                            const OMClassFactory* factory,
                            const OMByteOrder byteOrder,
                            OMStorable* root)
{
  TRACE("OMFile::createWrite");
  PRECONDITION("Valid file name", validWideString(fileName));
  PRECONDITION("Valid class factory", factory != 0);
  PRECONDITION("Valid root", root != 0);

  // Not yet implemented.
  //
  return 0;
}


OMFile* OMFile::createModify(const wchar_t* fileName,
                             const OMClassFactory* factory,
                             const OMByteOrder byteOrder,
                             OMStorable* root)
{
  TRACE("OMFile::createModify");
  PRECONDITION("Valid file name", validWideString(fileName));
  PRECONDITION("Valid class factory", factory != 0);
  PRECONDITION("Valid root", root != 0);

  OMStoredObject* store = OMStoredObject::createModify(fileName, byteOrder);
  OMFile* newFile = new OMFile(modifyMode, store, factory, root);
  ASSERT("Valid heap pointer", newFile != 0);
  return newFile;
}

OMFile* OMFile::createTransient(const OMClassFactory* factory,
                                const OMByteOrder byteOrder,
                                OMStorable* root)
{
  TRACE("OMFile::createTransient");
  PRECONDITION("Valid class factory", factory != 0);
  PRECONDITION("Valid root", root != 0);

  // Not yet implemented.
  //
  return 0;
}

void OMFile::save(void)
{
  TRACE("OMFile::save");

  if (_mode == modifyMode) {
    _root->save();
  }
}

void OMFile::revert(void)
{
  TRACE("OMFile::revert");
}

OMStorable* OMFile::restore(void)
{
  TRACE("OMFile::restore");

  _root = OMStorable::restoreFrom(this, "head", *rootStoredObject());
  return root();
}

void OMFile::close(void)
{
  TRACE("OMFile::close");

  _root->close();
#if 0
  _rootStoredObject->close();
  delete _rootStoredObject;
  _rootStoredObject = 0;
#endif
}

OMStorable* OMFile::root(void)
{
  TRACE("OMFile::root");

  return _root;
}

OMStoredObject* OMFile::rootStoredObject(void)
{
  TRACE("OMFile::rootStoredObject");

  return _rootStoredObject;
}

const OMClassFactory* OMFile::classFactory(void) const
{
  TRACE("OMFile::classFactory");

  return _classFactory;
}

OMObjectDirectory* OMFile::objectDirectory(void)
{
  TRACE("OMFile::objectDirectory");

  if (_objectDirectory == 0) {
    _objectDirectory = new OMObjectDirectory(OM_OBJECT_DIRECTORY_CAPACITY);
    ASSERT("Valid heap pointer", _objectDirectory != 0);
  }
  return _objectDirectory;
}

OMByteOrder OMFile::byteOrder(void) const
{
  ASSERT("Valid root", _rootStoredObject != 0);
  return _rootStoredObject->byteOrder();
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
