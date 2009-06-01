#ifndef __jpegesdata_h__
#define __jpegesdata_h__

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

/* Include the definition for IAAFEssenceStream */
#include "AAFPlugin.h"

/* this is not a core library module, so it doesn't define JPEG_INTERNALS */
#include "jinclude.h"
#include "jpeglib.h"

/* Standard data source and destination managers: stdio streams. */
/* Caller is responsible for opening the file before and closing after. */
EXTERN(void) jpeg_essencestream_dest JPP((j_compress_ptr cinfo, aafInt32 resID, aafInt32 width, aafInt32 height, IAAFEssenceStream * outstream));
EXTERN(void) jpeg_essencestream_src JPP((j_decompress_ptr cinfo, IAAFEssenceStream * instream, aafUInt32 samplesize));
EXTERN(void) jpeg_essencestream_srcCleanup JPP((j_decompress_ptr cinfo));

#endif // #ifndef __jpegesdata_h__
