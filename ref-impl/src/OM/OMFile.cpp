#include "OMFile.h"

#include "OMAssertions.h"
#include "OMStoredObject.h"
#include "OMClassFactory.h"
#include "OMObjectDirectory.h"

#include <string.h>

#define OM_CLASS_FACTORY_CAPACITY      (200)
#define OM_OBJECT_DIRECTORY_CAPACITY  (5000)

OMFile::OMFile(const OMAccessMode mode, OMStoredObject* store)
: _root(0), _rootStoredObject(store), _classFactory(0),
  _objectDirectory(0), _mode(mode)
{
  TRACE("OMFile::OMFile");

  setName("/");
}

OMFile::OMFile(const OMAccessMode mode,
               OMStoredObject* store,
               OMStorable* root)
: _root(root), _rootStoredObject(store), _classFactory(0),
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

  delete _classFactory;
  _classFactory = 0;
  delete _objectDirectory;
  _objectDirectory = 0;
}

OMFile* OMFile::openRead(const wchar_t* fileName)
{
  TRACE("OMFile::openRead");
  PRECONDITION("Valid file name", validWideString(fileName));

  OMStoredObject* store = OMStoredObject::openRead(fileName);
  OMFile* newFile = new OMFile(readOnlyMode, store);
  return newFile;
}

OMFile* OMFile::openModify(const wchar_t* fileName)
{
  TRACE("OMFile::openModify");
  PRECONDITION("Valid file name", validWideString(fileName));

  OMStoredObject* store = OMStoredObject::openModify(fileName);
  OMFile* newFile = new OMFile(modifyMode, store);
  return newFile;
}

OMFile* OMFile::createWrite(const wchar_t* fileName, OMStorable* root)
{
  TRACE("OMFile::createWrite");
  PRECONDITION("Valid file name", validWideString(fileName));
  PRECONDITION("Valid root", root != 0);

  // Not yet implemented.
  //
  return 0;
}


OMFile* OMFile::createModify(const wchar_t* fileName, OMStorable* root)
{
  TRACE("OMFile::createModify");
  PRECONDITION("Valid file name", validWideString(fileName));
  PRECONDITION("Valid root", root != 0);

  OMStoredObject* store = OMStoredObject::createModify(fileName);
  OMFile* newFile = new OMFile(modifyMode, store, root);
  return newFile;
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

OMClassFactory* OMFile::classFactory(void)
{
  TRACE("OMFile::classFactory");

  if (_classFactory == 0) {
    _classFactory = new OMClassFactory(OM_CLASS_FACTORY_CAPACITY);
  }

  return _classFactory;
}

OMObjectDirectory* OMFile::objectDirectory(void)
{
  TRACE("OMFile::objectDirectory");

  if (_objectDirectory == 0) {
    _objectDirectory = new OMObjectDirectory(OM_OBJECT_DIRECTORY_CAPACITY);
  }
  return _objectDirectory;
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

bool OMFile::attached(void)
{
  TRACE("OMFile::attached");

  // By definition a file is attached.
  //
  return true;
}
