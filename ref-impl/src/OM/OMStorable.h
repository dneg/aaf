/***********************************************************************
*
*              Copyright (c) 1998-2000 Avid Technology, Inc.
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
#ifndef OMSTORABLE_H
#define OMSTORABLE_H

#include "OMDataTypes.h"

#include "OMPropertySet.h"

class OMStoredObject;
class OMFile;
class OMClassFactory;

// @class Abstract base class for all objects that may be stored by
//        the Object Manager.
//
class OMStorable {
public:
  // @access Public members.

    // @cmember Constructor.
  OMStorable(void);

    // @cmember Destructor.
  virtual ~OMStorable(void);

    // @cmember The stored object identifier for the class of this
    //          <c OMStorable>.
  virtual const OMClassId& classId(void) const = 0;
  
// private:

    // @cmember Attach this <c OMStorable>.
  void attach(const OMStorable* container, const wchar_t* name);

    // @cmember Detach this <c OMStorable>.
  void detach(void);

    // @cmember Give this <c OMStorable> a name.
  void setName(const wchar_t* name);

    // @cmember Save this <c OMStorable>.
  virtual void save(void) const;

    // @cmember Close this <c OMStorable>.
  void close(void);

    // @cmember Restore an <c OMStorable> (of unknown sub-class) from
    // the stored representation <p s>.
  static OMStorable* restoreFrom(const OMStorable* container,
                                 const wchar_t* name,
                                 OMStoredObject& s);

    // @cmember Restore the contents of an <c OMStorable> (of unknown
    //          sub-class).
  virtual void restoreContents(void);

    // @cmember The <c OMFile> in which this <c OMStorable> has a
    //          persistent representation.
  virtual OMFile* file(void) const;

    // @cmember The path to this <c OMStorable> from the root of
    //          the <c OMFile> in which this <c OMStorable> resides.
  const wchar_t* pathName(void) const;

    // @cmember Find the <c OMStorable> named <p objectName> contained
    //          within this <c OMStorable>.
  OMStorable* find(const wchar_t* objectName) const;

  OMStorable* find(OMPropertyId propertyId) const;

    // @cmember Find the <c OMProperty> named <p propertyName> contained
    //          within this <c OMStorable>.
  OMProperty* findProperty(const wchar_t* propertyName) const;

  OMProperty* findProperty(OMPropertyId propertyId) const;

    // @cmember Is this <c OMStorable> the root of the object
    //          containment hierarchy.
  bool isRoot(void) const;

    // @cmember The stored representation of this <c OMStorable>.
  OMStoredObject* store(void) const;

    // @cmember Inform this <c OMStorable> where it should store its
    //          persistent representation.
  void setStore(OMStoredObject* store);

    // @cmember Is this <c OMStorable> attached to (owned by) another
    //          <c OMStorable> ?
  virtual bool attached(void) const;

    // @cmember Is this <c OMStorable> associated with an <c OMFile> ?
  virtual bool inFile(void) const;

    // @cmember Is this <c OMStorable> a persistent object ?
    //          Persistent objects are associated with a persistent
    //          store (disk file).
  virtual bool persistent(void) const;

    // @cmember Is this <c OMStorable> dirty ?
    //          A dirty object is one that has been modified since it
    //          was last saved or that has never been saved at all. 
  bool isDirty(void) const;

    // @cmember This object's property set.
  virtual OMPropertySet* propertySet(void);

    // @cmember The <c OMClassFactory> that created this object.
  OMClassFactory* classFactory(void) const;

    // @cmember Inform this <c OMStorable> of the <c OMClassFactory>
    //          that was used to create it.
  void setClassFactory(const OMClassFactory* classFactory);

  // Callbacks.

    // @cmember Inform this <c OMStorable> that it is about to be saved.
    //          The <p clientContext> passed is the one that was specified
    //          in the currently active call to <mf OMStorable::save>.
  virtual void onSave(void* clientContext) const;

    // @cmember Inform this <c OMStorable> that it has just been restored.
    //          The <p clientContext> passed is the one that was specified
    //          in the currently active call to <mf OMStorable::restore>.
  virtual void onRestore(void* clientContext) const;

private:
  // @access Private members.

  wchar_t* makePathName(void);

protected:
  // @access Protected members.

    // @cmember The name of this <c OMStorable>.
  const wchar_t* name(void) const;

  OMPropertySet _persistentProperties;

private:
  const OMStorable* _container;
  wchar_t* _name;
  wchar_t* _pathName;

  OMStoredObject* _store;
  const OMClassFactory* _classFactory;

};

#endif
