//=---------------------------------------------------------------------=
//
// The contents of this file are subject to the AAF SDK Public
// Source License Agreement (the "License"); You may not use this file
// except in compliance with the License.  The License is available in
// AAFSDKPSL.TXT, or you may obtain a copy of the License from the AAF
// Association or its successor.
//
// Software distributed under the License is distributed on an "AS IS"
// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
// the License for the specific language governing rights and limitations
// under the License.
//
// The Original Code of this file is Copyright 1998-2003, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

// @doc OMEXTERNAL
// @author Tim Bingham | tjb | Avid Technology, Inc. | OMReferenceSet
#ifndef OMREFERENCESETT_H
#define OMREFERENCESETT_H

#include "OMAssertions.h"
#include "OMReferenceSetIter.h"

  // @mfunc Constructor.
template <typename UniqueIdentification, typename ReferencedObject>
OMReferenceSet<UniqueIdentification, ReferencedObject>::OMReferenceSet(void)
{
  TRACE("OMReferenceSet<UniqueIdentification, ReferencedObject>::"
                                                             "OMReferenceSet");
}

  // @mfunc Destructor.
template <typename UniqueIdentification, typename ReferencedObject>
OMReferenceSet<UniqueIdentification, ReferencedObject>::~OMReferenceSet(void)
{
  TRACE("OMReferenceSet<UniqueIdentification, ReferencedObject>::"
                                                            "~OMReferenceSet");

}

  // @mfunc The number of <p ReferencedObject>s in this <c OMReferenceSet>.
  //   @this const
template <typename UniqueIdentification, typename ReferencedObject>
size_t OMReferenceSet<UniqueIdentification,
                      ReferencedObject>::count(void) const
{
  TRACE("OMReferenceSet<UniqueIdentification, ReferencedObject>::count");

  return _set.count();
}

  // @mfunc Insert <p object> into this
  //        <c OMReferenceSet>.
  //   @tcarg class | UniqueIdentification | The type of the unique key
  //          used to identify the referenced objects.
  //   @tcarg class | ReferencedObject | The type of the referenced objects.
  //   @parm The object to insert.
template <typename UniqueIdentification, typename ReferencedObject>
void OMReferenceSet<UniqueIdentification, ReferencedObject>::insert(
                                                const ReferencedObject* object)
{
  TRACE("OMReferenceSet<UniqueIdentification, ReferencedObject>::insert");

  PRECONDITION("Valid object", object != 0);
  PRECONDITION("Object is not present", !containsValue(object));

  // Set the set to contain the new object
  //
  UniqueIdentification identification = object->identification();
  SetElement newElement(object);
  _set.insert(identification, newElement);

  POSTCONDITION("Object is present", containsValue(object));
}

  // @mfunc If it is not already present, insert <p object> into this
  //        <c OMReferenceSet> and return true,
  //        otherwise return false.
  //   @tcarg class | UniqueIdentification | The type of the unique key
  //          used to identify the referenced objects.
  //   @tcarg class | ReferencedObject | The type of the referenced objects.
  //   @parm The object to insert.
  //   @rdesc True if the object was inserted, false if it was already present.
template <typename UniqueIdentification, typename ReferencedObject>
bool OMReferenceSet<UniqueIdentification,ReferencedObject>::ensurePresent(
                                                const ReferencedObject* object)
{
  TRACE("OMReferenceSet<UniqueIdentification, "
                                           "ReferencedObject>::ensurePresent");

  PRECONDITION("Valid object", object != 0);

  // tjb - Current cost is 2 * O(lg N) this should be halved to
  //       O(lg N) by implementing an ensurePresent() on OMSet.
  bool present = containsValue(object);
  if (!present) {
    insert(object);
  }
  POSTCONDITION("Object is present", containsValue(object));
  return !present;
}

  // @mfunc Append the given <p ReferencedObject> <p object> to
  //        this <c OMReferenceSet>.
  //   @tcarg class | UniqueIdentification | The type of the unique key
  //          used to identify the referenced objects.
  //   @tcarg class | ReferencedObject | The type of the referenced objects.
  //   @parm A pointer to a <p ReferencedObject>.
template <typename UniqueIdentification, typename ReferencedObject>
void OMReferenceSet<UniqueIdentification, ReferencedObject>::appendValue(
                                                const ReferencedObject* object)
{
  TRACE("OMReferenceSet<UniqueIdentification, ReferencedObject>::appendValue");
  PRECONDITION("Valid object", object != 0);

  OBSOLETE("OMReferenceSet<UniqueIdentification, ReferencedObject>::insert");
  insert(object);
  POSTCONDITION("Object is present", containsValue(object));
}

  // @mfunc Remove the <p ReferencedObject> identified by
  //        <p identification> from this <c OMReferenceSet>.
  //   @tcarg class | UniqueIdentification | The type of the unique key
  //          used to identify the referenced objects.
  //   @tcarg class | ReferencedObject | The type of the referenced objects.
  //   @parm The unique identification of the object to be removed, the
  //         search key.
  //   @rdesc A pointer to the removed <p ReferencedObject>. If lazy
  //          loading is enabled and the referenced object was never
  //          loaded the value returned is 0.
template <typename UniqueIdentification, typename ReferencedObject>
ReferencedObject*
OMReferenceSet<UniqueIdentification, ReferencedObject>::remove(
                                    const UniqueIdentification& identification)
{
  TRACE("OMReferenceSet<UniqueIdentification, ReferencedObject>::remove");

  PRECONDITION("Object is present", contains(identification));

  SetElement* element = 0;
  bool found = _set.find(identification, &element);
  ASSERT("Object found", found);
  ReferencedObject* result = element->setValue(0);
  _set.remove(identification);

  POSTCONDITION("Object is not present", !contains(identification));

  return result;
}

  // @mfunc If it is present, remove the <p ReferencedObject> identified by
  //        <p identification> from this <c OMReferenceSet>
  //        and return true, otherwise return false.
  //   @tcarg class | UniqueIdentification | The type of the unique key
  //          used to identify the referenced objects.
  //   @tcarg class | ReferencedObject | The type of the referenced objects.
  //   @parm The object to remove.
  //   @rdesc True if the object was removed, false if it was already absent.
template <typename UniqueIdentification, typename ReferencedObject>
bool OMReferenceSet<UniqueIdentification, ReferencedObject>::ensureAbsent(
                                    const UniqueIdentification& identification)
{
  TRACE("OMReferenceSet<UniqueIdentification, "
                                            "ReferencedObject>::ensureAbsent");

  SetElement* element = 0;
  bool result = _set.find(identification, &element);
  if (result) {
    _set.remove(identification);
  }

  POSTCONDITION("Object is not present", !contains(identification));
  return result;
}

  // @mfunc Remove <p object> from this <c OMReferenceSet>.
  //   @tcarg class | UniqueIdentification | The type of the unique key
  //          used to identify the referenced objects.
  //   @tcarg class | ReferencedObject | The type of the referenced objects.
  //   @parm A pointer to a <p ReferencedObject>.
template <typename UniqueIdentification, typename ReferencedObject>
void OMReferenceSet<UniqueIdentification, ReferencedObject>::removeValue(
                                                const ReferencedObject* object)
{
  TRACE("OMReferenceSet<UniqueIdentification, ReferencedObject>::removeValue");

  PRECONDITION("Valid object", object != 0);
  PRECONDITION("Object is present", containsValue(object));

  UniqueIdentification identification = object->identification();
  remove(identification);

  POSTCONDITION("Object is not present", !containsValue(object));
}

  // @mfunc If it is present, remove <p object> from this
  //        <c OMReferenceSet> and return true, otherwise return false.
  //   @tcarg class | UniqueIdentification | The type of the unique key
  //          used to identify the referenced objects.
  //   @tcarg class | ReferencedObject | The type of the referenced objects.
  //   @parm The object to remove.
  //   @rdesc True if the object was removed, false if it was already absent.
template <typename UniqueIdentification, typename ReferencedObject>
bool OMReferenceSet<UniqueIdentification, ReferencedObject>::ensureAbsent(
                                                const ReferencedObject* object)
{
  TRACE("OMReferenceSet<UniqueIdentification, "
                                            "ReferencedObject>::ensureAbsent");

  PRECONDITION("Valid object", object != 0);

  UniqueIdentification identification = object->identification();
  bool result = ensureAbsent(identification);

  POSTCONDITION("Object is not present", !containsValue(object));
  return result;
}

  // @mfunc Does this <c OMReferenceSet> contain <p object> ?
  //   @tcarg class | UniqueIdentification | The type of the unique key
  //          used to identify the referenced objects.
  //   @tcarg class | ReferencedObject | The type of the referenced objects.
  //   @parm A pointer to a <p ReferencedObject>.
template <typename UniqueIdentification, typename ReferencedObject>
bool OMReferenceSet<UniqueIdentification, ReferencedObject>::containsValue(
                                          const ReferencedObject* object) const
{
  TRACE("OMReferenceSet<UniqueIdentification, "
                                           "ReferencedObject>::containsValue");
  PRECONDITION("Valid object", object != 0);

  bool result = _set.contains(object->identification());
  return result;
}

  // @mfunc Does this <c OMReferenceSet> contain a
  //        <p ReferencedObject> identified by <p identification>?
  //   @tcarg class | UniqueIdentification | The type of the unique key
  //          used to identify the referenced objects.
  //   @tcarg class | ReferencedObject | The type of the referenced objects.
  //   @parm The unique identification of the desired object, the search key.
  //   @rdesc True if the object is found, false otherwise.
template <typename UniqueIdentification, typename ReferencedObject>
bool OMReferenceSet<UniqueIdentification, ReferencedObject>::contains(
                              const UniqueIdentification& identification) const
{
  TRACE("OMReferenceSet<UniqueIdentification, ReferencedObject>::contains");

  return _set.contains(identification);
}

  // @mfunc The <p ReferencedObject> in this <c OMReferenceSet> identified by
  //        <p identification>.
  //   @tcarg class | UniqueIdentification | The type of the unique key
  //          used to identify the referenced objects.
  //   @tcarg class | ReferencedObject | The type of the referenced objects.
  //   @parm The unique identification of the desired object, the search key.
  //   @rdesc A pointer to the <p ReferencedObject>.
  //   @this const
template <typename UniqueIdentification, typename ReferencedObject>
ReferencedObject*
OMReferenceSet<UniqueIdentification, ReferencedObject>::value(
                              const UniqueIdentification& identification) const
{
  TRACE("OMReferenceSet<UniqueIdentification, ReferencedObject>::value");

  PRECONDITION("Object is present", contains(identification));

  SetElement* element = 0;

  _set.find(identification, &element);
  ReferencedObject* result = element->getValue();

  POSTCONDITION("Valid result", result != 0);
  return result;
}

  // @mfunc Find the <p ReferencedObject> in this
  //        <c OMReferenceSet> identified by
  //        <p identification>.  If the object is found it is returned
  //        in <p object> and the result is true. If the element is
  //        not found the result is false.
  //   @tcarg class | UniqueIdentification | The type of the unique key
  //          used to identify the referenced objects.
  //   @tcarg class | ReferencedObject | The type of the referenced objects.
  //   @parm The unique identification of the desired object, the search key.
  //   @parm A pointer to a <p ReferencedObject> by reference.
  //   @rdesc True if the object is found, false otherwise.
  //   @this const
template <typename UniqueIdentification, typename ReferencedObject>
bool OMReferenceSet<UniqueIdentification, ReferencedObject>::find(
                                    const UniqueIdentification& identification,
                                    ReferencedObject*& object) const
{
  TRACE("OMReferenceSet<UniqueIdentification, ReferencedObject>::find");

  SetElement* element = 0;

  bool result = _set.find(identification, &element);
  if (result) {
    object = element->getValue();
  }

  return result;
}

template <typename UniqueIdentification, typename ReferencedObject>
bool
OMReferenceSet<UniqueIdentification, ReferencedObject>::isValidIdentification(
                                                UniqueIdentification& id) const
{
  TRACE("OMReferenceSet<UniqueIdentification, ReferencedObject>::"
                                                      "isValidIdentification");

  bool result = false;
  OMByte* p = reinterpret_cast<OMByte*>(&id);
  for (size_t i = 0; i < sizeof(UniqueIdentification); i++) {
    if (p[i] != 0) {
      result = true;
      break;
    }
  }
  return result;
}

  // @mfunc Insert <p object> into this <c OMReferenceSet>.
  //   @tcarg class | UniqueIdentification | The type of the unique key
  //          used to identify the referenced objects.
  //   @tcarg class | ReferencedObject | The type of the referenced objects.
  //   @parm TBS
template <typename UniqueIdentification, typename ReferencedObject>
void
OMReferenceSet<UniqueIdentification, ReferencedObject>::insertObject(
                                                        const OMObject* object)
{
  TRACE("OMReferenceSet<UniqueIdentification, ReferencedObject>::"
                                                               "insertObject");

  const ReferencedObject* obj = dynamic_cast<const ReferencedObject*>(object);
  ASSERT("Object is correct type", obj != 0);
  insert(obj);
}

  // @mfunc Does this <c OMReferenceSet> contain <p object> ?
  //   @tcarg class | UniqueIdentification | The type of the unique key
  //          used to identify the referenced objects.
  //   @tcarg class | ReferencedObject | The type of the referenced objects.
  //   @parm TBS
  //   @rdesc TBS
template <typename UniqueIdentification, typename ReferencedObject>
bool
OMReferenceSet<UniqueIdentification, ReferencedObject>::containsObject(
                                                  const OMObject* object) const
{
  TRACE("OMReferenceSet<UniqueIdentification, ReferencedObject>::"
                                                             "containsObject");

  const ReferencedObject* obj = dynamic_cast<const ReferencedObject*>(object);
  ASSERT("Object is correct type", obj != 0);
  return containsValue(obj);
}

  // @mfunc Remove <p object> from this <c OMReferenceSet>.
  //   @tcarg class | UniqueIdentification | The type of the unique key
  //          used to identify the referenced objects.
  //   @tcarg class | ReferencedObject | The type of the referenced objects.
  //   @parm TBS
template <typename UniqueIdentification, typename ReferencedObject>
void
OMReferenceSet<UniqueIdentification, ReferencedObject>::removeObject(
                                                        const OMObject* object)
{
  TRACE("OMReferenceSet<UniqueIdentification, ReferencedObject>::"
                                                               "removeObject");

  const ReferencedObject* obj = dynamic_cast<const ReferencedObject*>(object);
  ASSERT("Object is correct type", obj != 0);
  removeValue(obj);
}

  // @mfunc Remove all objects from this <c OMReferenceSet>.
  //   @tcarg class | UniqueIdentification | The type of the unique key
  //          used to identify the referenced objects.
  //   @tcarg class | ReferencedObject | The type of the referenced objects.
template <typename UniqueIdentification, typename ReferencedObject>
void
OMReferenceSet<UniqueIdentification, ReferencedObject>::removeAllObjects(void)
{
  TRACE("OMReferenceSet<UniqueIdentification, ReferencedObject>::"
                                                           "removeAllObjects");

  _set.clear();
  POSTCONDITION("All objects removed", count() == 0);
}

  // @mfunc Create an <c OMReferenceContainerIterator> over this
  //        <c OMReferenceSet>.
  //   @tcarg class | UniqueIdentification | The type of the unique key
  //          used to identify the referenced objects.
  //   @tcarg class | ReferencedObject | The type of the referenced objects.
  //   @rdesc TBS
template <typename UniqueIdentification, typename ReferencedObject>
OMReferenceContainerIterator*
OMReferenceSet<UniqueIdentification, ReferencedObject>::createIterator(
                                                                    void) const
{
  TRACE("OMReferenceSet<UniqueIdentification, ReferencedObject>::"
                                                             "createIterator");

  OMReferenceSetIterator<UniqueIdentification,
                         ReferencedObject>* result =
                 new OMReferenceSetIterator<UniqueIdentification,
                                            ReferencedObject>(*this, OMBefore);
  ASSERT("Valid heap pointer", result != 0);
  return result;
}

  // @mfunc Remove the <c OMObject> identified by <p identification>
  //        from this <c OMReferenceSet>.
  //   @tcarg class | UniqueIdentification | The type of the unique key
  //          used to identify the referenced objects.
  //   @tcarg class | ReferencedObject | The type of the referenced objects.
  //   @parm TBS
  //   @rdesc TBS
template <typename UniqueIdentification, typename ReferencedObject>
OMObject*
OMReferenceSet<UniqueIdentification, ReferencedObject>::remove(
                                                          void* identification)
{
  TRACE("OMReferenceSet<UniqueIdentification, ReferencedObject>::remove");

  UniqueIdentification* id =
                       reinterpret_cast<UniqueIdentification*>(identification);
  return remove(id);
}

  // @mfunc Does this <c OMReferenceSet> contain an
  //        <c OMObject> identified by <p identification> ?
  //   @tcarg class | UniqueIdentification | The type of the unique key
  //          used to identify the referenced objects.
  //   @tcarg class | ReferencedObject | The type of the referenced objects.
  //   @parm TBS
  //   @rdesc TBS
template <typename UniqueIdentification, typename ReferencedObject>
bool
OMReferenceSet<UniqueIdentification, ReferencedObject>::contains(
                                                    void* identification) const
{
  TRACE("OMReferenceSet<UniqueIdentification, ReferencedObject>::contains");

  UniqueIdentification* id =
                       reinterpret_cast<UniqueIdentification*>(identification);
  return contains(id);
}

  // @mfunc Find the <c OMObject> in this <c OMReferenceSet>
  //        identified by <p identification>.  If the object is found
  //        it is returned in <p object> and the result is < e bool.true>.
  //        If the object is not found the result is <e bool.false>.
  //   @tcarg class | UniqueIdentification | The type of the unique key
  //          used to identify the referenced objects.
  //   @tcarg class | ReferencedObject | The type of the referenced objects.
  //   @parm TBS
  //   @parm TBS
  //   @rdesc TBS
template <typename UniqueIdentification, typename ReferencedObject>
bool
OMReferenceSet<UniqueIdentification, ReferencedObject>::findObject(
                                                       void* identification,
                                                       OMObject*& object) const
{
  TRACE("OMReferenceSet<UniqueIdentification, ReferencedObject>::findObject");

  UniqueIdentification* id =
                       reinterpret_cast<UniqueIdentification*>(identification);
  ReferencedObject* obj = dynamic_cast<ReferencedObject*>(object);
  ASSERT("Object is correct type", obj != 0);
  return find(*id, obj);
}

#endif

