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
// Portions created by British Broadcasting Corporation are
// Copyright 2005, British Broadcasting Corporation.  All rights reserved.
//
//=---------------------------------------------------------------------=

/* Include the prototype for jpeg_essencestream_dest */
#include "jpegesdata.h"


/* this is not a core library module, so it doesn't define JPEG_INTERNALS */
#include "jerror.h"


/* Expanded data destination object for stdio output */

typedef struct {
  struct jpeg_destination_mgr pub; /* public fields */

	IAAFEssenceStream *outfile; /* target stream */
  JOCTET *buffer;					/* start of buffer */
  JOCTET *tmp_buf;					/* tmp buffer allocated once */
  aafInt32 resolutionID;			/* Avid special metadata */
  aafUInt32 OUTPUT_BUF_SIZE;		/* max size of compressed image */
  aafUInt32 last_jpeg_size;			/* for Avid MJPEG lat image size metadata */
} my_destination_mgr;

typedef my_destination_mgr * my_dest_ptr;

/*
 * Initialize destination --- called by jpeg_start_compress
 * before any data is actually written.
 */

METHODDEF(void)
init_destination (j_compress_ptr cinfo)
{
  my_dest_ptr dest = (my_dest_ptr) cinfo->dest;

  /* Allocate the output buffer --- it will be released when done with image */
  dest->buffer = (JOCTET *)
      (*cinfo->mem->alloc_small) ((j_common_ptr) cinfo, JPOOL_IMAGE,
				  dest->OUTPUT_BUF_SIZE * SIZEOF(JOCTET));

  /* temporary buffer to rearrange jpeg header */
  dest->tmp_buf = (JOCTET *)
      (*cinfo->mem->alloc_small) ((j_common_ptr) cinfo, JPOOL_IMAGE,
				  dest->OUTPUT_BUF_SIZE * SIZEOF(JOCTET));

  dest->pub.next_output_byte = dest->buffer;
  dest->pub.free_in_buffer = dest->OUTPUT_BUF_SIZE;
}


/*
 * Empty the output buffer --- called whenever buffer fills up.
 *
 * In typical applications, this should write the entire output buffer
 * (ignoring the current state of next_output_byte & free_in_buffer),
 * reset the pointer & count to the start of the buffer, and return TRUE
 * indicating that the buffer has been dumped.
 *
 * In applications that need to be able to suspend compression due to output
 * overrun, a FALSE return indicates that the buffer cannot be emptied now.
 * In this situation, the compressor will return to its caller (possibly with
 * an indication that it has not accepted all the supplied scanlines).  The
 * application should resume compression after it has made more room in the
 * output buffer.  Note that there are substantial restrictions on the use of
 * suspension --- see the documentation.
 *
 * When suspending, the compressor will back up to a convenient restart point
 * (typically the start of the current MCU). next_output_byte & free_in_buffer
 * indicate where the restart point will be if the current call returns FALSE.
 * Data beyond this point will be regenerated after resumption, so do not
 * write it out when emptying the buffer externally.
 */

METHODDEF(boolean)
empty_output_buffer (j_compress_ptr cinfo)
{
  my_dest_ptr dest = (my_dest_ptr) cinfo->dest;
  aafUInt32 bytesWritten;

  HRESULT hr = (dest->outfile)->Write(dest->OUTPUT_BUF_SIZE, (aafDataBuffer_t)dest->buffer, &bytesWritten);
	if (FAILED(hr))
    ERREXIT(cinfo, JERR_FILE_WRITE);

  dest->pub.next_output_byte = dest->buffer;
  dest->pub.free_in_buffer = dest->OUTPUT_BUF_SIZE;

  return TRUE;
}

// Big-endian integer storage routines
static void storeUInt16_BE(aafUInt8 *p, aafUInt16 value)
{
    p[0] = (aafUInt8)((value & 0x0000ff00) >> 8);
    p[1] = (aafUInt8)((value & 0x000000ff) >> 0);
}

static void storeUInt32_BE(aafUInt8 *p, aafUInt32 value)
{
    p[0] = (aafUInt8)((value & 0xff000000) >> 24);
    p[1] = (aafUInt8)((value & 0x00ff0000) >> 16);
    p[2] = (aafUInt8)((value & 0x0000ff00) >> 8);
    p[3] = (aafUInt8)((value & 0x000000ff) >> 0);
}

// Rearrange JPEG markers into the fashion needed by Avid:
//   APP0 (always 16 bytes incl. 2 byte segment length)
//   COM  (always 61 bytes)
//   DRI
//   DQT (Y table and UV table together in one segment)
//   DHT (always default libjpeg tables but all in one segment)
//   SOF0
//   SOS
//
static int rearrange_jpeg(JOCTET *p_in, aafInt32 resID, int size, int last_size, JOCTET *p_out)
{
	int		i, end_of_header = size;
	JOCTET	*p = p_out;

	// integrity check of JPEG picture
	if ( ! (p_in[0] == 0xFF && p_in[1] == 0xD8) ) {
		printf("No SOI marker\n");
		return 0;
	}
	if ( ! (p_in[size-2] == 0xFF && p_in[size-1] == 0xD9) ) {
		printf("No EOI marker\n");
		return 0;
	}

	// Find SOS marker and use it effectively as end-of-header
	for (i = 0; i < (size - 1); i++) {
		if (p_in[i] == 0xFF && p_in[i+1] == 0xDA) {
			end_of_header = i;
			break;
		}
	}
	if (end_of_header == size) {
		printf("No SOS marker\n");
		return 0;
	}

	// Start with SOI marker
	*p++ = 0xFF;
	*p++ = 0xD8;
	
	// Create Avid APP0 marker
	*p++ = 0xFF;
	*p++ = 0xE0;
	*p++ = 0x00;
	*p++ = 0x10;
	unsigned char app0_buf[14] = "AVI1";
	aafUInt8 *pos_app_data = p + 6;				// use pointer to update data later
	memcpy(p, app0_buf, sizeof(app0_buf));
	p += sizeof(app0_buf);

	// Create Avid COM marker
	*p++ = 0xFF;
	*p++ = 0xFE;
	*p++ = 0x00;
	*p++ = 0x3D;
	unsigned char com_buf[59] = "AVID\x11";
	com_buf[11] = static_cast<unsigned char>(resID);						// ResolutionID 0x4C=2:1, 0x4E=15:1
	com_buf[12] = 0x02;							// Always 2
	storeUInt32_BE(&com_buf[7], last_size);		// store last size
	memcpy(p, com_buf, sizeof(com_buf));
	p += sizeof(com_buf);

	// Insert DRI (appears necessary for Avid)
	*p++ = 0xFF;
	*p++ = 0xDD;
	*p++ = 0x00;
	*p++ = 0x04;
	*p++ = 0x00;
	*p++ = 0x00;

	// Copy DQT tables all as single segment
	*p++ = 0xFF;
	*p++ = 0xDB;
	aafUInt8 *pos_dqt_len = p;		// use this to update length later
	*p++ = 0x00;
	*p++ = 0x00;
	// Find DQT markers, copy contents to destination
	aafUInt16 dqt_len = 2;
	for (i = 0; i < end_of_header - 1; i++) {
		// search for DQT marker
		if (p_in[i] == 0xFF && p_in[i+1] == 0xDB) {
			// length is 16bit big-endian and includes storage of length (2 bytes)
			aafUInt16 length = (p_in[i+2] << 8) + p_in[i+3];
			length -= 2;			// compute length of table contents

			// copy contents of this table
			memcpy(p, &p_in[i+4], length);
			p += length;
			dqt_len += length;
		}
	}
	// update DQT segment length
	storeUInt16_BE(pos_dqt_len, dqt_len);

	// Copy DHT tables all as single segment
	// TODO: test whether Avid can handle different order of tables
	*p++ = 0xFF;
	*p++ = 0xC4;
	aafUInt8 *pos_dht_len = p;		// use this to update length later
	*p++ = 0x00;
	*p++ = 0x00;
	// Find DHT markers, copy contents to destination
	aafUInt16 dht_len = 2;
	aafUInt8 dht_table[2][2][512];
	int		dht_length[2][2];
	for (i = 0; i < end_of_header - 1; i++) {
		// search for DHT marker
		if (p_in[i] == 0xFF && p_in[i+1] == 0xC4) {
			// length is 16bit big-endian and includes storage of length (2 bytes)
			aafUInt16 length = (p_in[i+2] << 8) + p_in[i+3];
			length -= 2;			// compute length of table contents

			// Table class Tc and Table destination idenitifier Th follow length
			int Tc = p_in[i+4] >> 4;
			int Th = p_in[i+4] & 0x0f;

			// copy contents of this table
			memcpy(&dht_table[Tc][Th], &p_in[i+4], length);
			dht_length[Tc][Th] = length;
		}
	}
	// Copy out ordered DHT segment in Avid order:
	//	Tc=0 Th=0
	//	Tc=0 Th=1
	//	Tc=1 Th=0
	//	Tc=1 Th=1
	int Tc, Th;
	for (Tc = 0; Tc < 2; Tc++)
		for (Th = 0; Th < 2; Th++) {
			memcpy(p, &dht_table[Tc][Th], dht_length[Tc][Th]);
			p += dht_length[Tc][Th];
			dht_len += dht_length[Tc][Th];
		}

	// update DHT segment length
	storeUInt16_BE(pos_dht_len, dht_len);

	// Copy first SOF0 segment found
	*p++ = 0xFF;
	*p++ = 0xC0;
	for (i = 0; i < end_of_header - 1; i++) {
		// search for SOF0 marker
		if (p_in[i] == 0xFF && p_in[i+1] == 0xC0) {
			// length is 16bit big-endian and includes storage of length (2 bytes)
			aafUInt16 length = (p_in[i+2] << 8) + p_in[i+3];
			memcpy(p, &p_in[i+2], length);
			p += length;
			break;
		}
	}

	// Copy SOS and entropy coded segment through to EOI marker
	memcpy(p, &p_in[end_of_header], size - end_of_header);
	p += size - end_of_header;

	// update Avid-special size metadata now that we know the new size
	int new_size = p - p_out;
	storeUInt32_BE(pos_app_data, new_size);
	storeUInt32_BE(pos_app_data+4, new_size);

	return new_size;
}

/*
 * Terminate destination --- called by jpeg_finish_compress
 * after all data has been written.  Usually needs to flush buffer.
 *
 * NB: *not* called by jpeg_abort or jpeg_destroy; surrounding
 * application must deal with any cleanup that should happen even
 * for error exit.
 */

METHODDEF(void)
term_destination (j_compress_ptr cinfo)
{
	HRESULT hr = S_OK;
  my_dest_ptr dest = (my_dest_ptr) cinfo->dest;
	size_t datacount = dest->OUTPUT_BUF_SIZE - dest->pub.free_in_buffer;
	aafUInt32 new_size;
  aafUInt32 bytesWritten;

	/* Rearrange JPEG header for systems with very fussy JPEG read support */
	new_size = rearrange_jpeg(dest->buffer, dest->resolutionID,
  							datacount, dest->last_jpeg_size, dest->tmp_buf);
	if (new_size < 1)
		ERREXIT(cinfo, JERR_FILE_WRITE);
	dest->last_jpeg_size = new_size;
	
  /* Write any data remaining in the buffer */
  if (datacount > 0) {
		hr = (dest->outfile)->Write(new_size, (aafDataBuffer_t)dest->tmp_buf, &bytesWritten);
	  if (FAILED(hr))
      ERREXIT(cinfo, JERR_FILE_WRITE);
  }

	hr = (dest->outfile)->FlushCache();
	if (FAILED(hr))
    ERREXIT(cinfo, JERR_FILE_WRITE);
}


/*
 * Prepare for output to a stdio stream.
 * The caller must have already opened the stream, and is responsible
 * for closing it after finishing compression.
 */

GLOBAL(void)
jpeg_essencestream_dest(j_compress_ptr cinfo, aafInt32 resID, aafInt32 width, aafInt32 height,
						IAAFEssenceStream * outfile)
{
  my_dest_ptr dest;

  /* The destination object is made permanent so that multiple JPEG images
   * can be written to the same file without re-executing jpeg_stdio_dest.
   * This makes it dangerous to use this manager and a different destination
   * manager serially with the same JPEG object, because their private object
   * sizes may be different.  Caveat programmer.
   */
  if (cinfo->dest == NULL) {	/* first time for this JPEG object? */
    cinfo->dest = (struct jpeg_destination_mgr *)
      (*cinfo->mem->alloc_small) ((j_common_ptr) cinfo, JPOOL_PERMANENT,
				  SIZEOF(my_destination_mgr));
  }

  dest = (my_dest_ptr) cinfo->dest;
  dest->pub.init_destination = init_destination;
  dest->pub.empty_output_buffer = empty_output_buffer;
  dest->pub.term_destination = term_destination;
  dest->outfile = outfile;
  dest->resolutionID = resID;
  dest->last_jpeg_size = 0;			// for Avid MJPEG compressed size metadata
  // Buffer must be large enough for worst-case compressed image size
  dest->OUTPUT_BUF_SIZE = width * height * 4 + 4096;
}
