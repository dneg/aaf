#ifndef __jpegesdata_h__
#define __jpegesdata_h__

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
// The Original Code of this file is Copyright 1998-2001, Licensor of the
// AAF Association.
// 
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
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
EXTERN(void) jpeg_essencestream_dest JPP((j_compress_ptr cinfo, IAAFEssenceStream * outstream));
EXTERN(void) jpeg_essencestream_src JPP((j_decompress_ptr cinfo, IAAFEssenceStream * instream, aafUInt32 samplesize));
EXTERN(void) jpeg_essencestream_srcCleanup JPP((j_decompress_ptr cinfo));

#endif // #ifndef __jpegesdata_h__
