#include "OMFile.h"

#include "OMAssertions.h"
#include "OMStoredObject.h"
#include "OMClassFactory.h"
#include "OMObjectDirectory.h"

#include <string.h>

OMFile::OMFile(const char* name)
: _name(name), _root(0)
{
  TRACE("OMFile::OMFile");

  setName("/");
}

OMFile::~OMFile(void)
{
  TRACE("OMFile::~OMFile");
}

OMFile* OMFile::open(const char* fileName)
{
  OMFile* newFile = new OMFile(fileName);
  newFile->open();
  return newFile;
}

OMFile* OMFile::create(const char* fileName)
{
  OMFile* newFile = new OMFile(fileName);
  newFile->create();
  return newFile;
}

void OMFile::create(void)
{
  TRACE("File::create");

  _root = OMStoredObject::create(_name);
}

void OMFile::open(void)
{
  TRACE("OMFile::open");

  _root = OMStoredObject::open(_name);
}

void OMFile::close(void)
{
  TRACE("OMFile::close");

  _root->close();
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

OMClassFactory* OMFile::_classFactory = 0;

OMClassFactory* OMFile::classFactory(void)
{
  TRACE("OMFile::classFactory");

  if (_classFactory == 0) {
    _classFactory = new OMClassFactory(100);
  }

  return _classFactory;
}

OMObjectDirectory* OMFile::_objectDirectory = 0;

OMObjectDirectory* OMFile::objectDirectory(void)
{
  TRACE("OMFile::objectDirectory");

  if (_objectDirectory == 0) {
    _objectDirectory = new OMObjectDirectory(100);
  }
  return _objectDirectory;
}

int OMFile::classId(void) const
{
  TRACE("OMFile::classId");

  return 63;
}

OMFile* OMFile::file(void) const
{
  TRACE("OMFile::file");

  return const_cast<OMFile*>(this);
}
