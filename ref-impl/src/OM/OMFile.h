#ifndef OMFILE_H
#define OMFILE_H

#include "OMStorable.h"
#include "OMStoredObject.h"
#include "OMTypes.h"

#include <stddef.h>

class OMClassFactory;
class OMObjectDirectory;

class OMFile : public OMStorable {
public:

  OMFile(const OMAccessMode mode,
         OMStoredObject* store,
         const OMClassFactory* factory);
  OMFile(const OMAccessMode mode,
         OMStoredObject* store,
         const OMClassFactory* factory,
         OMStorable* root);
  ~OMFile(void);

  static OMFile* openRead(const wchar_t* fileName,
                          const OMClassFactory* factory);
  static OMFile* openModify(const wchar_t* fileName,
                            const OMClassFactory* factory);
  static OMFile* createWrite(const wchar_t* fileName,
                             const OMClassFactory* factory,
                             OMStorable* root);
  static OMFile* createModify(const wchar_t* fileName,
                              const OMClassFactory* factory,
                              OMStorable* root);

  void save(void);

  void revert(void);

  OMStorable* restore(void);

  void close(void);

  OMStorable* root(void);

  OMStoredObject* rootStoredObject(void);

  const OMClassFactory* classFactory(void) const;
  OMObjectDirectory* objectDirectory(void);
  
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
