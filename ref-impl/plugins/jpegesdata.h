#ifndef __jpegesdata_h__
#define __jpegesdata_h__

/* Include the definition for IAAFEssenceStream */
#include "AAFPlugin.h"

/* this is not a core library module, so it doesn't define JPEG_INTERNALS */
#include "jinclude.h"
#include "jpeglib.h"

/* Standard data source and destination managers: stdio streams. */
/* Caller is responsible for opening the file before and closing after. */
EXTERN(void) jpeg_essencestream_dest JPP((j_compress_ptr cinfo, IAAFEssenceStream * outstream));
EXTERN(void) jpeg_essencestream_src JPP((j_decompress_ptr cinfo, IAAFEssenceStream * instream));


#endif // #ifndef __jpegesdata_h__