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

#include "OMObject.h"
#include "OMDataTypes.h"

#include "OMPropertySet.h"

class OMStoredObject;
class OMFile;
class OMClassFactory;
class OMClassDefinition;

  // @class Abstract base class for all objects that may be stored by
  //        the Object Manager.
  //   @cauthor Tim Bingham | tjb | Avid Technology, Inc.
class OMStorable : public OMObject {
public:
  // @access Public members.

    // @cmember Constructor.
  OMStorable(void);

    // @cmember Destructor.
  virtual ~OMStorable(void);

    // @cmember The stored object identifier for the class of this
    //          <c OMStorable>.
  virtual const OMClassId& classId(void) const = 0;

    // @cmember Set the <c OMClassDefinition> defining this <c OMStorable>.
  virtual void setDefinition(const OMClassDefinition* definition);

    // @cmember The <c OMClassDefinition> defining this <c OMStorable>.
  virtual const OMClassDefinition* definition(void) const;

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
  virtual void close(void);

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

  // Copying.

    // @cmember Create a shallow copy of this <c OMStorable>.
    //          In a shallow copy, contained objects (strong object
    //          references) and streams are not copied.
  OMStorable* shallowCopy(void) const;

    // @cmember Create a deep copy of this <c OMStorable>, attach the
    //          copy to <p destination>.
    //          In a deep copy, contained objects (strong object
    //          references) and streams are copied.
    //          This function copies the entire object tree rooted at this
    //          <c OMStorable>. The root object is treated differently than
    //          the contained objects in that only the strong references
    //          and streams are copied. Clients may choose to create
    //          <p destination> using <mf OMStorable::shallowCopy>.
    //          All strong reference properties of this <c OMStorable> must
    //          be present in the property set of <p destination>. The values
    //          of the strong reference properties of <p destination> must
    //          be void and are replaced by those of this <c OMStorable>.
    //          Any properties of <p destination> not also in this
    //          <c OMStorable> are left unchanged.
  void deepCopyTo(OMStorable* destination, void* clientContext) const;

  // Callbacks.

    // @cmember Inform this <c OMStorable> that it is about to be saved.
    //          The <p clientContext> passed is the one that was specified
    //          in the currently active call to <mf OMStorable::save>.
  virtual void onSave(void* clientContext) const;

    // @cmember Inform this <c OMStorable> that it has just been restored.
    //          The <p clientContext> passed is the one that was specified
    //          in the currently active call to <mf OMStorable::restore>.
  virtual void onRestore(void* clientContext) const;

    // @cmember Inform this <c OMStorable> that it has just been copied by
    //          <mf OMStorable::deepCopyTo>. The <p clientContext> passed is
    //          the one that was specified in the currently active call
    //          to <mf OMStorable::deepCopyTo>. When <mf OMStorable::onCopy>
    //          is called only the shallow portion of the deep copy to be
    //          performed by <mf OMStorable::deepCopyTo> has been completed.
    //          That is, this <c OMStorable> contains copies of all of the
    //          properties of the souce <c OMStorable> except for strong
    //          references (contained objects). Those properties wll be
    //          copied after <mf OMStorable::onCopy> returns.
  virtual void onCopy(void* clientContext) const;

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
  const OMClassDefinition* _definition;

    // OMStorable can't be assigned - declare but don't define
  OMStorable& operator = (const OMStorable& rhs);

    // OMStorable can't be copied - declare but don't define
  OMStorable(const OMStorable& rhs);

};

#endif
