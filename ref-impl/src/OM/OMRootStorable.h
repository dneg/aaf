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
#ifndef OMROOTSTORABLE_H
#define OMROOTSTORABLE_H

#include "OMStorable.h"
#include "OMStrongRefProperty.h"
#include "OMDictionary.h"

// @class Concrete sub-class of the abstract OMStorable for use as the
//        root object in a file that may be managed by the Object Manager.
//
class OMRootStorable : public OMStorable {
public:

  OMRootStorable(void);

  OMRootStorable(OMStorable* clientRoot,
                 OMDictionary* dictionary);

  ~OMRootStorable(void);

  // OMStorable overrides

  virtual const OMClassId& classId(void) const;

    // @cmember Save this <c OMRootStorable>.
  virtual void save(void) const;

    // @cmember Restore the contents of an <c OMRootStorable>.
  virtual void restoreContents(void);

  // OMRootStorable functions

  OMStorable* clientRoot(void) const;

  OMDictionary* dictionary(void) const;

  static const OMClassId _rootClassId;

private:

  OMStrongReferenceProperty<OMStorable> _clientRoot;
  OMStrongReferenceProperty<OMDictionary> _dictionary;

};

#endif
