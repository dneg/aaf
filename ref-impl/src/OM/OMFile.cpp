#include "OMFile.h"

#include "OMAssertions.h"
#include "OMStoredObject.h"
#include "OMClassFactory.h"
#include "OMObjectDirectory.h"

#include <string.h>

#define OM_CLASS_FACTORY_CAPACITY      (200)
#define OM_OBJECT_DIRECTORY_CAPACITY  (5000)

OMFile::OMFile(const char* name, const OMAccessMode mode)
: _name(0), _root(0), _classFactory(0), _objectDirectory(0),
  _mode(mode)
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
  OMFile* newFile = new OMFile(fileName, readOnlyMode);
  newFile->openRead();
  return newFile;
}

OMFile* OMFile::openModify(const char* fileName)
{
  OMFile* newFile = new OMFile(fileName, modifyMode);
  newFile->openModify();
  return newFile;
}

OMFile* OMFile::createModify(const char* fileName)
{
  OMFile* newFile = new OMFile(fileName, modifyMode);
  newFile->createModify();
  return newFile;
}

void OMFile::openRead(void)
{
  TRACE("OMFile::openRead");
  ASSERT("Valid mode", _mode == readOnlyMode);

  _root = OMStoredObject::openRead(_name);
}

void OMFile::openModify(void)
{
  TRACE("OMFile::openModify");
  ASSERT("Valid mode", _mode == modifyMode);

  _root = OMStoredObject::openModify(_name);
}

void OMFile::createModify(void)
{
  TRACE("File::createModify");
  ASSERT("Mode is modify", _mode == modifyMode);

  _root = OMStoredObject::createModify(_name);
}

void OMFile::close(void)
{
  TRACE("OMFile::close");

  _root->close();
  delete _root;
  _root = 0;
}

OMStoredObject* OMFile::root(void)
{
  TRACE("OMFile::root");

  return _root;
}

OMFile& OMFile::operator << (const OMStorable& o)
{
  TRACE("OMFile::operator <<");

  const_cast<OMStorable&>(o).setContainingObject(this);
  const_cast<OMStorable&>(o).setName("head");
  o.saveTo(*root());
  return *this;
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
