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

// @doc OMINTERNAL
#ifndef OMOBJECTDIRECTORY_H
#define OMOBJECTDIRECTORY_H

#include <stddef.h>

class OMStorable;

  // @class Debug only data structure for tracking objects by name.
  //   @cauthor Tim Bingham | tjb | Avid Technology, Inc.
class OMObjectDirectory {
public:

  OMObjectDirectory(void);

  ~OMObjectDirectory(void);

  //
  //
  bool lookup(const wchar_t* name, const OMStorable*& p) const;

  //
  //
  void insert(const wchar_t* name, const OMStorable* p);

  // Count of valid entries.
  //
  size_t count(void) const;

  void dump(void) const;

private:

  struct TableEntry {
    OMStorable* _object;
    wchar_t* _name;
  };
  size_t _capacity;        // Number of potential entries.
  size_t _current;         // Index of first unoccupied entry.
  TableEntry* _table;   // Dynamically allocated array.

};

#endif
