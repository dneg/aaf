#ifndef __AAFPropertyDefs_h__
#define __AAFPropertyDefs_h__
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
 *  prior written permission of Avid Technology, Inc.
 *
 * THE SOFTWARE IS PROVIDED AS-IS AND WITHOUT WARRANTY OF ANY KIND,
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

//
// GUIDs for built-in property definitions
//

// Define the AAF_PROPERTY macro to create property GUID definitions.
// Include the meta dictionary.
//
#define AAF_PROPERTY(name, id, tag, type, mandatory, uid, container) \
const aafUID_t kAAFPropID_##container##_##name = id;

#include "AAFMetaDictionary.h"

//
// Special property ids used for specifying the starting strong reference in the
// targe list for weak references.
//

// SMPTE Label: 06-0E-2B-34-01-01-01-02-0D-01-03-01-01-01-01-00
const aafUID_t kAAFPropID_Root_MetaDictionary =
{0x0D010301, 0x0101, 0x0100, {0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02}};

// SMPTE Label: 06-0E-2B-34-01-01-01-02-0D-01-03-01-01-02-01-00
const aafUID_t kAAFPropID_Root_Header =
{0x0D010301, 0x0102, 0x0100, {0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02}};


#endif // ! __AAFPropertyDefs_h__
