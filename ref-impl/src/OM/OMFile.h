// @doc OMINTERNAL
#ifndef OMFILE_H
#define OMFILE_H

#include "OMStorable.h"
#include "OMStoredObject.h"
#include "OMTypes.h"

#include <stddef.h>

class OMClassFactory;
class OMObjectDirectory;

// @class Files supported by the Object Manager.
//
class OMFile : public OMStorable {
public:
  // @access Static members.

    // @cmember Open an existing <c OMFile> for read-only access, the
    //          <c OMFile> is named <p fileName>, use the <c OMClassFactory>
    //          <p factory> to create the objects. The file must already
    //          exist.
  static OMFile* openRead(const wchar_t* fileName,
                          const OMClassFactory* factory);

    // @cmember Open an existing <c OMFile> for modify access, the
    //          <c OMFile> is named <p fileName>, use the <c OMClassFactory>
    //          <p factory> to create the objects. The file must already
    //          exist.
  static OMFile* openModify(const wchar_t* fileName,
                            const OMClassFactory* factory);

    // @cmember Create a new <c OMFile> for write-only access, the
    //          <c OMFile> is named <p fileName>, use the <c OMClassFactory>
    //          <p factory> to create the objects. The file must not already
    //          exist. The byte ordering on the newly created file is given
    //          by <p byteOrder>. The root <c OMStorable> in the newly
    //          created file is given by <p root>.
  static OMFile* createWrite(const wchar_t* fileName,
                             const OMClassFactory* factory,
                             const OMByteOrder byteOrder,
                             OMStorable* root);

    // @cmember Create a new <c OMFile> for modify access, the
    //          <c OMFile> is named <p fileName>, use the <c OMClassFactory>
    //          <p factory> to create the objects. The file must not already
    //          exist. The byte ordering on the newly created file is given
    //          by <p byteOrder>. The root <c OMStorable> in the newly
    //          created file is given by <p root>.
  static OMFile* createModify(const wchar_t* fileName,
                              const OMClassFactory* factory,
                              const OMByteOrder byteOrder,
                              OMStorable* root);

    // @cmember Create a transient <c OMFile> for modify access, the
    //          <c OMFile> is not named, use the <c OMClassFactory>
    //          <p factory> to create the objects.
    //          The byte ordering on the newly created file is given
    //          by <p byteOrder>. The root <c OMStorable> in the newly
    //          created file is given by <p root>.
  static OMFile* createTransient(const OMClassFactory* factory,
                                 const OMByteOrder byteOrder,
                                 OMStorable* root);

  // @access Public members.

    // @cmember Constructor.
  OMFile(const OMAccessMode mode,
         OMStoredObject* store,
         const OMClassFactory* factory);

    // @cmember Constructor.
  OMFile(const OMAccessMode mode,
         OMStoredObject* store,
         const OMClassFactory* factory,
         OMStorable* root);

    // @cmember Destructor.
  ~OMFile(void);

    // @cmember Save all changes made to the contents of this
    //          <c OMFile>. It is not possible to <mf OMFile::save>
    //          read-only or transient files.
  void save(void);

    // @cmember Discard all changes made to this <c OMFile> since the
    //          last <mf OMFile::save> or <mf OMFile::open>.
  void revert(void);

    // @cmember Restore the root <c OMStorable> object from this <c OMFile>.
  OMStorable* restore(void);

    // @cmember Close this <c OMFile>, any unsaved changes are discarded.
  void close(void);

    // @cmember Retrieve the root <c OMStorable> from this <c OMFile>.
  OMStorable* root(void);

    // @cmember Retrieve the root <c OMStoredObject> from this <c OMFile>.
  OMStoredObject* rootStoredObject(void);

    // @cmember Retrieve the <c OMClassFactory> from this <c OMFile>.
    //   @this const
  const OMClassFactory* classFactory(void) const;

    // @cmember Retrieve the <c OMObjectDirectory> from this <c OMFile>.
  OMObjectDirectory* objectDirectory(void);

    // @cmember The byte order of this <c OMFile>.
    //   @this const
  OMByteOrder byteOrder(void) const;

  // OMStorable overrides.
  //
  virtual const OMClassId& classId(void) const;

  virtual OMFile* file(void) const;

  virtual bool inFile(void) const;

  virtual bool persistent(void) const;

private:

  OMStorable* _root;
  OMStoredObject* _rootStoredObject;
  
  const OMClassFactory* _classFactory;
  OMObjectDirectory* _objectDirectory;

  enum OMAccessMode _mode;

};

#endif
