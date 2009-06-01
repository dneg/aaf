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
#ifndef OMINTEGERTYPE_H
#define OMINTEGERTYPE_H

#include "OMType.h"
#include "OMDefinition.h"
#include "OMSingleton.h"

class OMIntegerType;

// @author Tim Bingham | tjb | Avid Technology, Inc. | OMIntegerType
class OMIntegerType : public OMType {
public:

  virtual OMType::Tag tag(void) const;

  virtual ~OMIntegerType(void);

  virtual bool isFixedSize(void) const;

  virtual void reorder(OMByte* externalBytes,
                       OMUInt32 externalBytesSize) const;

  virtual OMUInt32 externalSize(const OMByte* internalBytes,
                              OMUInt32 internalBytesSize) const;

  virtual OMUInt32 externalSize(void) const;

  virtual void externalize(const OMByte* internalBytes,
                           OMUInt32 internalBytesSize,
                           OMByte* externalBytes,
                           OMUInt32 externalBytesSize,
                           OMByteOrder byteOrder) const;

  virtual OMUInt32 internalSize(const OMByte* externalBytes,
                              OMUInt32 externalBytesSize) const;

  virtual OMUInt32 internalSize(void) const;

  virtual void internalize(const OMByte* externalBytes,
                           OMUInt32 externalBytesSize,
                           OMByte* internalBytes,
                           OMUInt32 internalBytesSize,
                           OMByteOrder byteOrder) const;

  virtual OMUInt8 size(void) const = 0;

  virtual bool isSigned(void) const = 0;
  
  virtual void accept(OMTypeVisitor& visitor) const;

};

class OMBuiltinIntegerType : public OMIntegerType,
                             private OMBuiltinDefinition {
public:

  OMBuiltinIntegerType(const OMStoredObjectIdentification& identification,
                       const wchar_t* name);

  virtual ~OMBuiltinIntegerType(void);

  virtual const OMUniqueObjectIdentification& identification(void) const;

  virtual const wchar_t* name(void) const;

  virtual bool hasDescription(void) const;

  virtual const wchar_t* description(void) const;

  virtual bool isPredefined(void) const;

};

class OMInteger08Type;

// @author Tim Bingham | tjb | Avid Technology, Inc. | OMInteger08Type
class OMInteger08Type : public OMBuiltinIntegerType,
                        public OMSingleton<OMInteger08Type> {
public:

  OMInteger08Type(void);

  virtual ~OMInteger08Type(void);

  virtual OMUInt8 size(void) const;

  virtual bool isSigned(void) const;

};

class OMInteger16Type;

// @author Tim Bingham | tjb | Avid Technology, Inc. | OMInteger16Type
class OMInteger16Type : public OMBuiltinIntegerType,
                        public OMSingleton<OMInteger16Type> {
public:

  OMInteger16Type(void);

  virtual ~OMInteger16Type(void);

  virtual OMUInt8 size(void) const;

  virtual bool isSigned(void) const;

};

class OMInteger32Type;

// @author Tim Bingham | tjb | Avid Technology, Inc. | OMInteger32Type
class OMInteger32Type : public OMBuiltinIntegerType,
                        public OMSingleton<OMInteger32Type> {
public:

  OMInteger32Type(void);

  virtual ~OMInteger32Type(void);

  virtual OMUInt8 size(void) const;

  virtual bool isSigned(void) const;

};

class OMInteger64Type;

// @author Tim Bingham | tjb | Avid Technology, Inc. | OMInteger64Type
class OMInteger64Type : public OMBuiltinIntegerType,
                        public OMSingleton<OMInteger64Type> {
public:

  OMInteger64Type(void);

  virtual ~OMInteger64Type(void);

  virtual OMUInt8 size(void) const;

  virtual bool isSigned(void) const;

};

#endif
