#include "OMFile.h"

#include "OMAssertions.h"
#include "OMStoredObject.h"
#include "OMClassFactory.h"
#include "OMObjectDirectory.h"

#include <string.h>

#define OM_CLASS_FACTORY_CAPACITY      (200)
#define OM_OBJECT_DIRECTORY_CAPACITY  (5000)

OMFile::OMFile(const char* name, const OMAccessMode mode)
: _name(0), _root(0), _rootStoredObject(0), _classFactory(0),
  _objectDirectory(0), _mode(mode)
{
  TRACE("OMFile::OMFile");

  _name = new char[strlen(name) + 1];
  strcpy(_name, name);

  setName("/");
}

OMFile::~OMFile(void)
{
  TRACE("OMFile::~OMFile");

  delete _name;
  _name = 0;
  delete _classFactory;
  _classFactory = 0;
  delete _objectDirectory;
  _objectDirectory = 0;
}

OMFile* OMFile::openRead(const char* fileName)
{
  TRACE("OMFile::openRead");
  PRECONDITION("Valid file name", validString(fileName));

  OMFile* newFile = new OMFile(fileName, readOnlyMode);
  newFile->openRead();
  return newFile;
}

OMFile* OMFile::openModify(const char* fileName)
{
  TRACE("OMFile::openModify");
  PRECONDITION("Valid file name", validString(fileName));

  OMFile* newFile = new OMFile(fileName, modifyMode);
  newFile->openModify();
  return newFile;
}

OMFile* OMFile::createWrite(const char* fileName, const OMStorable* root)
{
  TRACE("OMFile::createWrite");
  PRECONDITION("Valid file name", validString(fileName));
  PRECONDITION("Valid root", root != 0);

  // Not yet implemented.
  //
  return 0;
}


OMFile* OMFile::createModify(const char* fileName, const OMStorable* root)
{
  TRACE("OMFile::createModify");
  PRECONDITION("Valid file name", validString(fileName));
  PRECONDITION("Valid root", root != 0);

  OMFile* newFile = new OMFile(fileName, modifyMode);
  newFile->createModify(root);
  return newFile;
}

void OMFile::openRead(void)
{
  TRACE("OMFile::openRead");
  ASSERT("Valid mode", _mode == readOnlyMode);

  _rootStoredObject = OMStoredObject::openRead(_name);
}

void OMFile::openModify(void)
{
  TRACE("OMFile::openModify");
  ASSERT("Valid mode", _mode == modifyMode);

  _rootStoredObject = OMStoredObject::openModify(_name);
}

void OMFile::createModify(const OMStorable* root)
{
  TRACE("File::createModify");
  PRECONDITION("Valid root object", root != 0);
  ASSERT("Mode is modify", _mode == modifyMode);

  _root = const_cast<OMStorable*>(root);
  _rootStoredObject = OMStoredObject::createModify(_name);
}

void OMFile::save(void)
{
  TRACE("OMFile::save");

  if (_mode == modifyMode) {
    _root->setContainingObject(this);
    _root->setName("head");
    _root->saveTo(*rootStoredObject());
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

  _rootStoredObject->close();
  delete _rootStoredObject;
  _rootStoredObject = 0;
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
