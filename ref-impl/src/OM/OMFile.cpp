// @doc OMINTERNAL
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

  // @mfunc Open an existing <c OMFile> for read-only access, the
  //        <c OMFile> is named <p fileName>, use the <c OMClassFactory>
  //        <p factory> to create the objects. The file must already
  //        exist.
  //   @parm The name of the file to open.
  //   @parm The factory to use for creating objects.
  //   @rdesc The newly opened <c OMFile>.
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

  // @mfunc Open an existing <c OMFile> for modify access, the
  //        <c OMFile> is named <p fileName>, use the <c OMClassFactory>
  //        <p factory> to create the objects. The file must already
  //        exist.
  //   @parm The name of the file to open.
  //   @parm The factory to use for creating objects.
  //   @rdesc The newly opened <c OMFile>.
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

  // @mfunc Create a new <c OMFile> for write-only access, the
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

  // @mfunc Create a new <c OMFile> for modify access, the
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

  // @mfunc Create a transient <c OMFile> for modify access, the
  //        <c OMFile> is not named, use the <c OMClassFactory>
  //        <p factory> to create the objects.
  //        The byte ordering on the newly created file is given
  //        by <p byteOrder>. The root <c OMStorable> in the newly
  //        created file is given by <p root>.
  //   @parm The factory to use for creating objects.
  //   @parm The byte order to use for the newly created file.
  //   @parm The root <c OMStorable> in the newly created file.
  //   @rdesc The newly created <c OMFile>.
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

  // @mfunc Save all changes made to the contents of this
  //        <c OMFile>. It is not possible to <mf OMFile::save>
  //        read-only or transient files.
void OMFile::save(void)
{
  TRACE("OMFile::save");

  if (_mode == modifyMode) {
    _root->save();
  }
}

  // @mfunc Discard all changes made to this <c OMFile> since the
  //        last <mf OMFile::save> or <mf OMFile::open>.
void OMFile::revert(void)
{
  TRACE("OMFile::revert");
}

  // @mfunc Restore the root <c OMStorable> object from this <c OMFile>.
  //   @rdesc The newly restored roo <c OMStorable>.
OMStorable* OMFile::restore(void)
{
  TRACE("OMFile::restore");

  _root = OMStorable::restoreFrom(this, "head", *rootStoredObject());
  return root();
}

  // @mfunc Close this <c OMFile>, any unsaved changes are discarded.
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

  // @mfunc Retrieve the <c OMClassFactory> from this <c OMFile>.
  //   @rdesc The <c OMClassfactory> used to create objects in this file.
  //   @this const
const OMClassFactory* OMFile::classFactory(void) const
{
  TRACE("OMFile::classFactory");

  return _classFactory;
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
  //   @this const
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
