//=---------------------------------------------------------------------=
//
// $Id$ $Name$
//
// The contents of this file are subject to the AAF SDK Public Source
// License Agreement Version 2.0 (the "License"); You may not use this
// file except in compliance with the License.  The License is available
// in AAFSDKPSL.TXT, or you may obtain a copy of the License from the
// Advanced Media Workflow Association, Inc., or its successor.
//
// Software distributed under the License is distributed on an "AS IS"
// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
// the License for the specific language governing rights and limitations
// under the License.  Refer to Section 3.3 of the License for proper use
// of this Exhibit.
//
// WARNING:  Please contact the Advanced Media Workflow Association,
// Inc., for more information about any additional licenses to
// intellectual property covering the AAF Standard that may be required
// to create and distribute AAF compliant products.
// (http://www.amwa.tv/policies).
//
// Copyright Notices:
// The Original Code of this file is Copyright 1998-2009, licensor of the
// Advanced Media Workflow Association.  All rights reserved.
//
// The Initial Developer of the Original Code of this file and the
// licensor of the Advanced Media Workflow Association is
// Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

// @doc OMEXTERNAL
#ifndef OMDATASET_H
#define OMDATASET_H

#include "OMProperty.h"
#include "OMDataContainer.h"

#include "OMDataTypes.h"

  // @class Abstract base class for persistent properties representing
  //        sets of data items supported by the Object Manager.
  //   @base public | <c OMProperty>
  //   @base public | <c OMDataContainer>
  //   @cauthor Tim Bingham | tjb | Avid Technology, Inc.
class OMDataSet : public OMProperty,
                  public OMDataContainer {
public:
  // @access Public members.

     // @cmember Constructor.
  OMDataSet(const OMPropertyId propertyId,
            const OMStoredForm storedForm,
            const wchar_t* name);

   // @cmember Destructor.
  virtual ~OMDataSet(void);

    // @cmember Save this <c OMDataSet>.
  virtual void save(void) const;

    // @cmember Restore this <c OMDataSet>, the external (persisted)
    //          size of the <c OMDataSet> is <p externalSize>.
  virtual void restore(OMPropertySize externalSize);

    // @cmember The number of items in this this <c OMDataSet>.
  virtual OMUInt32 count(void) const = 0;

  virtual void insert(void* value) = 0;

  virtual OMUInt32 elementSize(void) const = 0;

private:

    // OMDataSet can't be assigned - declare but don't define
  OMDataSet& operator = (const OMDataSet& rhs);

    // OMDataSet can't be copied - declare but don't define
  OMDataSet(const OMDataSet& rhs);

};

#endif
