/***********************************************************************
 *
 *              Copyright (c) 1998-1999 Avid Technology, Inc.
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

// Following are the UIDs of the well-known codec definition objects in AAF, as well as well known
// flavours of codecs.
//
const aafUID_t NoCodec =
{ 0x568FB761, 0x9458, 0x11d2, { 0x80, 0x89, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F } };

const aafUID_t CodecWave = 
{ 0x820F09B1, 0xEB9B, 0x11d2, { 0x80, 0x9F, 0x00, 0x60, 0x08, 0x14, 0x3e, 0x6f } };

const aafUID_t NilCodecFlavour =
{ 0x1B31F3B1, 0x9450, 0x11d2, { 0x80, 0x89, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F } };

const aafUID_t CodecJPEG = 
{ 0x18634f8c, 0x3bab, 0x11d3, { 0xbf, 0xd6, 0x00, 0x10, 0x4b, 0xc9, 0x15, 0x6d } };
