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
               const OMClassFactory* factory,
               const OMLoadMode loadMode)
: _root(0), _rootStoredObject(store), _classFactory(factory),
  _objectDirectory(0), _mode(mode), _loadMode(loadMode)
{
  TRACE("OMFile::OMFile");

  setName("/");
}

OMFile::OMFile(const OMAccessMode mode,
               OMStoredObject* store,
               const OMClassFactory* factory,
               OMStorable* root)
: _root(root), _rootStoredObject(store), _classFactory(factory),
  _objectDirectory(0), _mode(mode), _loadMode(lazyLoad)
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
  OMFile* newFile = new OMFile(readOnlyMode, store, factory, loadMode);
  ASSERT("Valid heap pointer", newFile != 0);
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
  OMFile* newFile = new OMFile(modifyMode, store, factory, loadMode);
  ASSERT("Valid heap pointer", newFile != 0);
  return newFile;
}

  // @mfunc Open a new <c OMFile> for write-only access, the
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
OMFile* OMFile::openNewWrite(const wchar_t* fileName,
                            const OMClassFactory* factory,
                            const OMByteOrder byteOrder,
                            OMStorable* root)
{
  TRACE("OMFile::openNewWrite");
  PRECONDITION("Valid file name", validWideString(fileName));
  PRECONDITION("Valid class factory", factory != 0);
  PRECONDITION("Valid byte order",
                    ((byteOrder == littleEndian) || (byteOrder == bigEndian)));
  PRECONDITION("Valid root", root != 0);

  // Not yet implemented.
  //
  return 0;
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
                              OMStorable* root)
{
  TRACE("OMFile::openNewModify");
  PRECONDITION("Valid file name", validWideString(fileName));
  PRECONDITION("Valid class factory", factory != 0);
  PRECONDITION("Valid byte order",
                    ((byteOrder == littleEndian) || (byteOrder == bigEndian)));
  PRECONDITION("Valid root", root != 0);

  OMStoredObject* store = OMStoredObject::createModify(fileName, byteOrder);
  OMFile* newFile = new OMFile(modifyMode, store, factory, root);
  ASSERT("Valid heap pointer", newFile != 0);
  return newFile;
}

  // @mfunc Open a new transient <c OMFile> for modify access, the
  //        <c OMFile> is not named, use the <c OMClassFactory>
  //        <p factory> to create the objects.
  //        The byte ordering on the newly created file is given
  //        by <p byteOrder>. The root <c OMStorable> in the newly
  //        created file is given by <p root>.
  //   @parm The factory to use for creating objects.
  //   @parm The byte order to use for the newly created file.
  //   @parm The root <c OMStorable> in the newly created file.
  //   @rdesc The newly created <c OMFile>.
OMFile* OMFile::openNewTransient(const OMClassFactory* factory,
                                 const OMByteOrder byteOrder,
                                 OMStorable* root)
{
  TRACE("OMFile::openNewTransient");
  PRECONDITION("Valid class factory", factory != 0);
  PRECONDITION("Valid byte order",
                    ((byteOrder == littleEndian) || (byteOrder == bigEndian)));
  PRECONDITION("Valid root", root != 0);

  // Not yet implemented.
  //
  ASSERT("Not yet implemented", false);
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

  // @mfunc Save the entire contents of this <c OMFile> as well as
  //        any unsaved changes in the new file <p fileName>. The file
  //        must not already exist. <mf OMFile::saveAs> may be called
  //        for files opened in modify mode and for files opened in
  //        read-only and transient modes.
  //   @parm The name of the file to open.
  //
  //   @this const
void OMFile::saveAs(const wchar_t* fileName) const
{
  TRACE("OMFile::saveAs");
  PRECONDITION("Valid file name", validWideString(fileName));

  ASSERT("Not yet implemented", false);
}

  // @mfunc Discard all changes made to this <c OMFile> since the
  //        last <mf OMFile::save> or <mf OMFile::open>.
void OMFile::revert(void)
{
  TRACE("OMFile::revert");

  ASSERT("Not yet implemented", false);
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
  //   @rdesc The <c OMClassFactory> used to create objects in this file.
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
