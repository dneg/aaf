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
#ifndef OMOSTREAM_H
#define OMOSTREAM_H

#if defined(OM_ENABLE_DEBUG)

#include "OMDataTypes.h"

  // @class Simple, platform independent, text output stream
  //        for diagnostic (debug only) use by the Object Manager.
  //   @cauthor Tim Bingham | tjb | Avid Technology, Inc.
class OMOStream {
public:
  // @access Public members.

    // @cmember Insert a character string.
  OMOStream& operator << (const char* string);

    // @cmember Insert an OMUInt32 in decimal.
  OMOStream& operator << (OMUInt32 i);

    // @cmember Insert a new line.
  OMOStream& endLine(void);

    // @cmember Insert (call) a manipulator.
  OMOStream& operator << (OMOStream& (*manipulator)(OMOStream&));

protected:
  // @access Protected members.

    // @cmember Put a character string.
  OMOStream& put(const char* string);

    // @cmember Put an OMUInt32 in decimal.
  OMOStream& put(OMUInt32 i);

    // @cmember Put a new line.
  OMOStream& putLine(void);

};

 // @func <c OMOStream> end of line manipulator.
 //   @parm The <c OMOStream> in which to inset the new line.
 //   @rdesc The modified <c OMOStream>.
OMOStream& endl(OMOStream& s);

 // @globalv Global <c OMOStream> for Object Manager logging.
 //          Debug use only.
extern OMOStream omlog;

#endif
#endif
