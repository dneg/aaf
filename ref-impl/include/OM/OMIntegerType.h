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
#ifndef OMINTEGERTYPE_H
#define OMINTEGERTYPE_H

#include "OMType.h"
#include "OMSingleton.h"

class OMIntegerType;

// @author Tim Bingham | tjb | Avid Technology, Inc. | OMIntegerType
class OMIntegerType : public OMType {
public:

  OMIntegerType(void);

  virtual ~OMIntegerType(void);

  virtual void reorder(OMByte* externalBytes,
                       size_t externalBytesSize) const;

  virtual size_t externalSize(OMByte* internalBytes,
                              size_t internalBytesSize) const;

  virtual void externalize(OMByte* internalBytes,
                           size_t internalBytesSize,
                           OMByte* externalBytes,
                           size_t externalBytesSize,
                           OMByteOrder byteOrder) const;

  virtual size_t internalSize(OMByte* externalBytes,
                              size_t externalBytesSize) const;

  virtual void internalize(OMByte* externalBytes,
                           size_t externalBytesSize,
                           OMByte* internalBytes,
                           size_t internalBytesSize,
                           OMByteOrder byteOrder) const;

  virtual size_t size(void) const = 0;

};

class OMInteger08Type;

// @author Tim Bingham | tjb | Avid Technology, Inc. | OMInteger08Type
class OMInteger08Type : public OMIntegerType,
                        public OMSingleton<OMInteger08Type> {
public:

  OMInteger08Type(void);

  virtual ~OMInteger08Type(void);

  virtual size_t size(void) const;

};

class OMInteger16Type;

// @author Tim Bingham | tjb | Avid Technology, Inc. | OMInteger16Type
class OMInteger16Type : public OMIntegerType,
                        public OMSingleton<OMInteger16Type> {
public:

  OMInteger16Type(void);

  virtual ~OMInteger16Type(void);

  virtual size_t size(void) const;

};

class OMInteger32Type;

// @author Tim Bingham | tjb | Avid Technology, Inc. | OMInteger32Type
class OMInteger32Type : public OMIntegerType,
                        public OMSingleton<OMInteger32Type> {
public:

  OMInteger32Type(void);

  virtual ~OMInteger32Type(void);

  virtual size_t size(void) const;

};

class OMInteger64Type;

// @author Tim Bingham | tjb | Avid Technology, Inc. | OMInteger64Type
class OMInteger64Type : public OMIntegerType,
                        public OMSingleton<OMInteger64Type> {
public:

  OMInteger64Type(void);

  virtual ~OMInteger64Type(void);

  virtual size_t size(void) const;

};

#endif
