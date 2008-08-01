//=---------------------------------------------------------------------=
//
// $Id$
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
// The Original Code of this file is Copyright 2008, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Metaglue Corporation.
// All rights reserved.
//
//=---------------------------------------------------------------------=

#include "framebuffer.h"

#include <stdio.h>

//! LoadXYUV() - Local function to load an image file and optionally convert convert to target buffer layout
int LoadXYUV( BufferLayout_t target, byte *video_buffer, int len, int H, int V, const char *input_filename, 
			  bool isLittleEndian/*=true*/, bool isBigEndian/*=false*/, bool isP_UYVY/*=false*/ )
{
	if( target!=Layout_UYVYM && target!=Layout_UYVYI && target!=Layout_P_UYVY && target!=Layout_UYVY ) return 0;

	FILE * ip=fopen( input_filename,"rb");
	if(!ip) return 0;

	if( target==Layout_UYVYM )
	{
		if( isP_UYVY )
		{	// remux
			size_t nwant=V*(H/2)*5;

			abuffer file_buff(nwant);
			if( !file_buff.size() )  { fclose(ip); return 0; }

			if( nwant>(size_t)len ) { fclose(ip); return 0; }

			size_t nread=fread(file_buff,(size_t)1,nwant,ip);

			if( nread!=nwant ) { fclose(ip); return 0; }

			byte* pBuff=file_buff;
			byte* pUYVYM=video_buffer;

			// unpack the LSBs
			while( pBuff<(file_buff+V*H/2) )
			{
				byte L = *pBuff++;
				pUYVYM++;
				*pUYVYM++  = (L&0xc0)<<0;
				pUYVYM++;
				*pUYVYM++  = (L&0x30)<<2;
				pUYVYM++;
				*pUYVYM++  = (L&0x0c)<<4;
				pUYVYM++;
				*pUYVYM++  = (L&0x03)<<6;
			}

			// unpack the MSBs
			pUYVYM=video_buffer;
			while( pBuff<(file_buff+nwant) )
			{
				*pUYVYM++ = *pBuff++; // u
				pUYVYM++;
				*pUYVYM++ = *pBuff++; // y
				pUYVYM++;
				*pUYVYM++ = *pBuff++; // v
				pUYVYM++;
				*pUYVYM++ = *pBuff++; // y
				pUYVYM++;
			}

			fclose( ip );
			return (int)nread;
		}
		else if( isLittleEndian )
		{	// target is UYVYM and input is UYVYI - load, then flip
			size_t nwant=H*V*4;

			if( nwant>(size_t)len ) { fclose(ip); return 0; }

			size_t nread=fread(video_buffer,1,nwant,ip);

			// flip bytes
			byte* pUYVYx=video_buffer;
			byte F;
			byte S;
			while( pUYVYx<(video_buffer+nwant) )
			{
				F = pUYVYx[0]; S = pUYVYx[1];
                *pUYVYx++ = S;   *pUYVYx++ = F;   // u
				F = pUYVYx[0]; S = pUYVYx[1];
                *pUYVYx++ = S;   *pUYVYx++ = F;   // y
				F = pUYVYx[0]; S = pUYVYx[1];
                *pUYVYx++ = S;   *pUYVYx++ = F;   // v
				F = pUYVYx[0]; S = pUYVYx[1];
                *pUYVYx++ = S;   *pUYVYx++ = F;   // y
			}

			fclose( ip );
			if( nread==nwant ) return (int)nread; else return 0;
		}
		else if( isBigEndian )
		{	// target is UYVYM and input is UYVYM - load unchanged
			size_t nwant=H*V*4;

			if( nwant>(size_t)len ) { fclose(ip); return 0; }

			size_t nread=fread(video_buffer,1,nwant,ip);

			fclose( ip );
			if( nread==nwant ) return (int)nread; else return 0;
		}
		else
		{	// input is 8 bit UYVY - widen to 10/16 bit
			size_t nwant=H*V*2;

			abuffer file_buff(nwant);

			if( (2*nwant)>(size_t)len ) { fclose(ip); return 0; }

			size_t nread=fread(file_buff,1,nwant,ip);

			if( nread!=nwant ) { fclose(ip); return 0; }

			byte* pBuff=file_buff;
			byte* pUYVYM=video_buffer;

			while( pBuff<(file_buff+nwant) )
			{
				*pUYVYM++ = *pBuff++; // u
				*pUYVYM++ = 0;
				*pUYVYM++ = *pBuff++; // y
				*pUYVYM++ = 0;
				*pUYVYM++ = *pBuff++; // v
				*pUYVYM++ = 0;
				*pUYVYM++ = *pBuff++; // y
				*pUYVYM++ = 0;
			}

			fclose( ip );
			return (int)(2*nread);
		}
	}
	else if( target==Layout_P_UYVY )
	{
		if( isP_UYVY )
		{	// target and input are both LUYVY, load unchanged
			size_t nwant=V*(H/2)*5;

			if( nwant>(size_t)len ) { fclose(ip); return 0; }

			size_t nread=fread(video_buffer,1,nwant,ip);

			fclose( ip );
			if( nread==nwant ) return (int)nread; else return 0;
		}
		else if( isLittleEndian )
		{	// contract 10/16 bit UUYYVVYY to LUYVY
			size_t nwant=V*H*4;

			abuffer file_buff(nwant);

			if( (nwant/2)>(size_t)len ) { fclose(ip); return 0; }

			size_t nread=fread(file_buff,1,nwant,ip);

			if( nread!=nwant ) { fclose(ip); return 0; }

			byte* pBuff=file_buff;
			byte* pP_UYVY=video_buffer;

			// pack the LSBs
			while( pBuff<(file_buff+nwant) )
			{
				*pP_UYVY++  =  ((pBuff[0]&0xc0)>>0)
							|  ((pBuff[2]&0xc0)>>2)
							|  ((pBuff[4]&0xc0)>>4)
							|  ((pBuff[6]&0xc0)>>6) ;
				pBuff += 8;
			}

			// copy the MSBs
			pBuff=file_buff;
			while( pBuff<(file_buff+nwant) )
			{
				pBuff++;
				*pP_UYVY++ = *pBuff++; // u
				pBuff++;
				*pP_UYVY++ = *pBuff++; // y
				pBuff++;
				*pP_UYVY++ = *pBuff++; // v
				pBuff++;
				*pP_UYVY++ = *pBuff++; // y
			}

			fclose( ip );
			return (int)(nread/2);
		}
		else if( isBigEndian )
		{
			// this code does NOT convert BigEndian
			fclose( ip );
			return 0;
		}
		else
		{	// expand 8 bit UYVY to P_UYVY
			size_t nwant=V*(H/2)*4;

			if( ((nwant*5)/2)>(size_t)len ) { fclose(ip); return 0; }

			// blank the LSBs
			byte* pP_UYVY=video_buffer;
			while( pP_UYVY<(video_buffer+V*(H/2)) )
			{
				*pP_UYVY++ = 0;		  // packed lsbs
			}

			// read the MSBs
			size_t nread=fread(video_buffer+V*(H/2),1,nwant,ip);

			if( nread!=nwant ) { fclose(ip); return 0; }

			fclose( ip );
			if( nread==nwant ) return (int)V*(H/2)*5; else return 0;
		}
	}
	else if( target==Layout_UYVYI )
	{
		if( isP_UYVY )
		{	// remux
			size_t nwant=V*(H/2)*5;

			abuffer file_buff(nwant);

			if( (nwant)>(size_t)len ) { fclose(ip); return 0; }

			size_t nread=fread(file_buff,1,nwant,ip);

			if( nread!=nwant ) { fclose(ip); return 0; }

			byte* pBuff=file_buff;
			byte* pUYVYI=video_buffer;

			// unpack the LSBs
			while( pBuff<(file_buff+V*H/2) )
			{
				byte L = *pBuff++;
				*pUYVYI++  = (L&0xc0)<<0;
				pUYVYI++;
				*pUYVYI++  = (L&0x30)<<2;
				pUYVYI++;
				*pUYVYI++  = (L&0x0c)<<4;
				pUYVYI++;
				*pUYVYI++  = (L&0x03)<<6;
				pUYVYI++;
			}

			// unpack the MSBs
			pUYVYI=video_buffer;
			while( pBuff<(file_buff+nwant) )
			{
				pUYVYI++;
				*pUYVYI++ = *pBuff++; // u
				pUYVYI++;
				*pUYVYI++ = *pBuff++; // y
				pUYVYI++;
				*pUYVYI++ = *pBuff++; // v
				pUYVYI++;
				*pUYVYI++ = *pBuff++; // y
			}

			fclose( ip );
			return (int)nread;
		}
		else if( isBigEndian )
		{	// target is UYVYI and input is UYVYM - load, then flip
			size_t nwant=H*V*4;

			if( nwant>(size_t)len ) { fclose(ip); return 0; }

			size_t nread=fread(video_buffer,1,nwant,ip);

			// flip bytes
			byte* pUYVYx=video_buffer;
			byte F;
			byte S;
			while( pUYVYx<(video_buffer+nwant) )
			{
				F = pUYVYx[0]; S = pUYVYx[1];
                *pUYVYx++ = S;   *pUYVYx++ = F;   // u
				F = pUYVYx[0]; S = pUYVYx[1];
                *pUYVYx++ = S;   *pUYVYx++ = F;   // y
				F = pUYVYx[0]; S = pUYVYx[1];
                *pUYVYx++ = S;   *pUYVYx++ = F;   // v
				F = pUYVYx[0]; S = pUYVYx[1];
                *pUYVYx++ = S;   *pUYVYx++ = F;   // y
			}

			fclose( ip );
			if( nread==nwant ) return (int)nread; else return 0;
		}
		else if( isLittleEndian )
		{	// target is UYVYI and input is UYVYI - load unchanged
			size_t nwant=H*V*4;

			if( nwant>(size_t)len ) { fclose(ip); return 0; }

			size_t nread=fread(video_buffer,1,nwant,ip);

			fclose( ip );
			if( nread==nwant ) return (int)nread; else return 0;
		}
		else
		{	// input is 8 bit UYVY - widen to 10/16 bit
			size_t nwant=H*V*2;

			abuffer file_buff(nwant);

			if( (2*nwant)>(size_t)len ) { fclose(ip); return 0; }

			size_t nread=fread(file_buff,1,nwant,ip);

			if( nread!=nwant ) { fclose(ip); return 0; }

			byte* pBuff=file_buff;
			byte* pUYVYI=video_buffer;

			while( pBuff<(file_buff+nwant) )
			{
				*pUYVYI++ = 0;
				*pUYVYI++ = *pBuff++; // u
				*pUYVYI++ = 0;
				*pUYVYI++ = *pBuff++; // y
				*pUYVYI++ = 0;
				*pUYVYI++ = *pBuff++; // v
				*pUYVYI++ = 0;
				*pUYVYI++ = *pBuff++; // y
			}

			fclose( ip );
			return (int)(2*nread);
		}
	}
	else if( target==Layout_UYVY )
	{
		if( isP_UYVY )
		{	// skip the P, load the UYVY unchanged
			size_t nwant=V*H*2;

			if( nwant>(size_t)len ) { fclose(ip); return 0; }

			// intentionally read the packed LSBs, knowing we we overwrite them in the buffer
			size_t nread=fread(video_buffer,1,V*H/2,ip);
			if( nread!=(size_t)(V*H/2) ) { fclose(ip); return 0; }

			nread=fread(video_buffer,1,nwant,ip);

			fclose( ip );
			if( nread==nwant ) return (int)nread; else return 0;
		}
		else if( isLittleEndian )
		{	// truncate 10/16 bit UYVYI to 8 bit UYVY
			size_t nwant=H*V*4;

			abuffer file_buff(nwant);

			if( (nwant/2)>(size_t)len ) { fclose(ip); return 0; }

			size_t nread=fread(file_buff,1,nwant,ip);

			if( nread!=nwant ) { fclose(ip); return 0; }

			byte* pBuff=file_buff;
			byte* pUYVY=video_buffer;

			while( pBuff<(file_buff+nwant) )
			{
				pBuff++;
				*pUYVY++ = *pBuff++; // u
				pBuff++;
				*pUYVY++ = *pBuff++; // y
				pBuff++;
				*pUYVY++ = *pBuff++; // v
				pBuff++;
				*pUYVY++ = *pBuff++; // y
			}

			fclose( ip );
			return (int)(nread/2);
		}
		else if( isBigEndian )
		{
			// this code does NOT convert BigEndian
			fclose( ip );
			return 0;
		}
		else
		{	// target and input are 8 bit UYVY, load unchanged
			size_t nwant=H*V*2;

			if( nwant>(size_t)len ) { fclose(ip); return 0; }

			size_t nread=fread(video_buffer,1,nwant,ip);

			fclose( ip );
			if( nread==nwant ) return (int)nread; else return 0;
		}
	}
	else
		// some other target
		return 0;
}

//! LoadXRGB() - Local function to load an image file and optionally convert to target buffer layout
int LoadXRGB( BufferLayout_t target, byte *video_buffer, int len, int H, int V, const char *input_filename, 
			  bool isLittleEndian/*=true*/, bool isBigEndian/*=false*/, bool hasA/*=false*/, bool isRGB/*=false*/ )
{
	FILE * ip=fopen( input_filename,"rb");
	if(!ip) return 0;

	if( target==Layout_BGRAI )
	{
		// output is 8 bytes per pixel
		if( isLittleEndian )
		{
			// input is 6 or 8 bytes per pixel
			// this code does NOT widen
			if(hasA)
			{	// input is 8 bytes per pixel: BGRA
				size_t nwant=H*V*8;

				if( nwant>(size_t)len ) { fclose(ip); return 0; }

				size_t nread=fread(video_buffer,1,nwant,ip);

				if( nread!=nwant ) { fclose(ip); return 0; }

				if( isRGB ) // is RGBA
				{
					// reverse the per pixel order
					byte* pBuff=video_buffer;

					while( pBuff<(video_buffer+nwant) )
					{
						byte A  = pBuff[7];
						byte Af = pBuff[6];
						byte R  = pBuff[5];
						byte Rf = pBuff[4];
						byte G  = pBuff[3];
						byte Gf = pBuff[2];
						byte B  = pBuff[1];
						byte Bf = pBuff[0];
						*pBuff++ = Af;
						*pBuff++ = A;
						*pBuff++ = Rf;
						*pBuff++ = R;
						*pBuff++ = Gf;
						*pBuff++ = G;
						*pBuff++ = Bf;
						*pBuff++ = B;
					}
				}

				fclose( ip );
				return (int)nread;
			}
			else 
			{	// input is 6 bytes per pixel
				size_t nwant=H*V*6;

				abuffer file_buff(nwant);

				if( ((4*nwant)/3)>(size_t)len ) { fclose(ip); return 0; }

				size_t nread=fread(file_buff,1,nwant,ip);

				if( nread!=nwant ) { fclose(ip); return 0; }

				byte* pBuff=file_buff;
				byte* pBGRA=video_buffer;

				if( !isRGB ) // is BGR
				{
					// pad
					while( pBuff<(file_buff+nwant) )
					{
						*pBGRA++ = *pBuff++;	// Bf
						*pBGRA++ = *pBuff++;	// B
						*pBGRA++ = *pBuff++;	// Gf
						*pBGRA++ = *pBuff++;	// G
						*pBGRA++ = *pBuff++;	// Rf
						*pBGRA++ = *pBuff++;	// R
						*pBGRA++ = 0;			// substitute Af
						*pBGRA++ = 0;			// substitute A
					}
				}
				else // is RGB
				{
					// pad and reverse
					while( pBuff<(file_buff+nwant) )
					{
						*pBGRA++ = pBuff[2];	// B
						*pBGRA++ = pBuff[1];	// G
						*pBGRA++ = pBuff[0];	// R
						*pBGRA++ = 0;			// substitute A
						pBuff+=3;
					}
				}

				fclose( ip );
				return (int)((4*nread)/3);
			}
		}
		else if( isBigEndian )
		{
			// this code does NOT convert BigEndian to LittleEndian
			fclose( ip );
			return 0;
		}
		else
		{
			// input is 3 or 4 bytes per pixel
			// this code does NOT widen
			if(hasA)
			{	// input is 4 bytes per pixel: BGRA
				size_t nwant=H*V*4;

				if( nwant>(size_t)len ) { fclose(ip); return 0; }

				size_t nread=fread(video_buffer,1,nwant,ip);

				if( nread!=nwant ) { fclose(ip); return 0; }

				if( isRGB ) // is RGBA
				{
					// reverse the per pixel order
					byte* pBuff=video_buffer;

					while( pBuff<(video_buffer+nwant) )
					{
						byte A = pBuff[3];
						byte R = pBuff[2];
						byte G = pBuff[1];
						byte B = pBuff[0];
						*pBuff++ = A;
						*pBuff++ = R;
						*pBuff++ = G;
						*pBuff++ = B;
					}
				}

				fclose( ip );
				return (int)nread;
			}
			else 
			{	// input is 3 bytes per pixel
				size_t nwant=H*V*3;

				abuffer file_buff(nwant);

				if( ((4*nwant)/3)>(size_t)len ) { fclose(ip); return 0; }

				size_t nread=fread(file_buff,1,nwant,ip);

				if( nread!=nwant ) { fclose(ip); return 0; }

				byte* pBuff=file_buff;
				byte* pBGRA=video_buffer;

				if( !isRGB ) // is BGR
				{
					// pad
					while( pBuff<(file_buff+nwant) )
					{
						*pBGRA++ = *pBuff++;	// B
						*pBGRA++ = *pBuff++;	// G
						*pBGRA++ = *pBuff++;	// R
						*pBGRA++ = 0;			// substitute A
					}
				}
				else // is RGB
				{
					// pad and reverse
					while( pBuff<(file_buff+nwant) )
					{
						*pBGRA++ = pBuff[2];	// B
						*pBGRA++ = pBuff[1];	// G
						*pBGRA++ = pBuff[0];	// R
						*pBGRA++ = 0;			// substitute A
						pBuff+=3;
					}
				}

				fclose( ip );
				return (int)((4*nread)/3);
			}
		}
	}
	else if( target==Layout_BGRA && !isLittleEndian && !isBigEndian )
	{
		// output is 4 bytes per pixel
		// input is 4 or 3 bytes per pixel
		// this code does NOT truncate or filter or round...
		if(hasA)
		{	// input is 4 bytes per pixel: BGRA
			size_t nwant=H*V*4;

			if( nwant>(size_t)len ) { fclose(ip); return 0; }

			size_t nread=fread(video_buffer,1,nwant,ip);

			if( nread!=nwant ) { fclose(ip); return 0; }

			if( isRGB ) // is RGBA
			{
				// reverse the per pixel order
				byte* pBuff=video_buffer;

				while( pBuff<(video_buffer+nwant) )
				{
					byte A = pBuff[3];
					byte R = pBuff[2];
					byte G = pBuff[1];
					byte B = pBuff[0];
					*pBuff++ = A;
					*pBuff++ = R;
					*pBuff++ = G;
					*pBuff++ = B;
				}
			}

			fclose( ip );
			return (int)nread;
		}
		else 
		{	// input is 3 bytes per pixel
			size_t nwant=H*V*3;

			abuffer file_buff(nwant);

			if( ((4*nwant)/3)>(size_t)len ) { fclose(ip); return 0; }

			size_t nread=fread(file_buff,1,nwant,ip);

			if( nread!=nwant ) { fclose(ip); return 0; }

			byte* pBuff=file_buff;
			byte* pBGRA=video_buffer;

			if( !isRGB ) // is BGR
			{
				// pad
				while( pBuff<(file_buff+nwant) )
				{
					*pBGRA++ = *pBuff++;	// B
					*pBGRA++ = *pBuff++;	// G
					*pBGRA++ = *pBuff++;	// R
					*pBGRA++ = 0;			// substitute A
				}
			}
			else // is RGB
			{
				// pad and reverse
				while( pBuff<(file_buff+nwant) )
				{
					*pBGRA++ = pBuff[2];	// B
					*pBGRA++ = pBuff[1];	// G
					*pBGRA++ = pBuff[0];	// R
					*pBGRA++ = 0;			// substitute A
					pBuff+=3;
				}
			}

			fclose( ip );
			return (int)((4*nread)/3);
		}
	}
	else
	{
		// unsupported target BufferLayout
		fclose( ip );
		return 0;
	}
}

//! LoadRaw() - Local function to load a complete file - presumably containing a raw bitstream
int LoadRaw( BufferLayout_t target, byte * video_buffer, int len, const char *input_filename)
{
	if( target!=Layout_Raw ) return 0;

	FILE * ip=fopen( input_filename,"rb");

	if(!ip) return false;

	fseek( ip, 0, SEEK_END );
	size_t nwant=ftell( ip );
	rewind( ip );

	if( nwant>(size_t)len )
	{
		fclose( ip );
		return false;
	}

	size_t nread=fread(video_buffer,1,nwant,ip);

	fclose( ip );

	if( nread==nwant ) return (int)nread; else return 0;
}

// Generate a video buffer containing uncompressed video representing the familiar colour bars test signal
int LoadXBars( BufferLayout_t target, byte *video_buffer, int len, int H, int V)
{
	int		i,j,b;

	/* Original
	int		UYVY_table[][5] = {
		{  52, 0x80,0xEB,0x80,0xEB },	// white
		{ 140, 0x10,0xD2,0x92,0xD2 },	// yellow
		{ 228, 0xA5,0xA9,0x10,0xA9 },	// cyan
		{ 316, 0x35,0x90,0x22,0x90 },	// green
		{ 404, 0xCA,0x6A,0xDD,0x6A },	// magenta
		{ 492, 0x5A,0x51,0xF0,0x51 },	// red
		{ 580, 0xf0,0x29,0x6d,0x29 },	// blue
		{ 668, 0x80,0x10,0x80,0x10 },	// black
		{ 720, 0x80,0xEB,0x80,0xEB },	// white
	};
	*/

	byte UYVY_table[][8] = {
		//	Cb(msb)	Cb(lsb)	Y(msb)	Y(lsb)	Cr(msb)	Cr(lsb)	Y(msb)	Y(lsb)	
		{	0x80,	0x00,	0xEB,	0x00,	0x80,	0x00,	0xEB,	0x00 }, 	// white
		{	0x10,	0x00,	0xDB,	0x00,	0x8A,	0x00,	0xDB,	0x00 }, 	// yellow
		{	0x9A,	0x00,	0xBC,	0x00,	0x10,	0x00,	0xBC,	0x00 }, 	// cyan
		{	0x2A,	0x00,	0xAD,	0x00,	0x1A,	0x00,	0xAD,	0x00 }, 	// green
		{	0xD6,	0x00,	0x4E,	0x00,	0xE6,	0x00,	0x4E,	0x00 }, 	// magenta
		{	0x66,	0x00,	0x3F,	0x00,	0xF0,	0x00,	0x3F,	0x00 }, 	// red
		{	0xF0,	0x00,	0x20,	0x00,	0x76,	0x00,	0x20,	0x00 }, 	// blue
		{	0x80,	0x00,	0x10,	0x00,	0x80,	0x00,	0x10,	0x00 }, 	// black
		{	0x80,	0x00,	0xEB,	0x00,	0x80,	0x00,	0xEB,	0x00 }, 	// white
	};

	byte CbYCrY_table[][8] = {
		//	Cb(msb)	Cb(lsb)	Y(msb)	Y(lsb)	Cr(msb)	Cr(lsb)	Y(msb)	Y(lsb)	
		{	0x80,	0x00,	0xEB,	0x00,	0x80,	0x00,	0xEB,	0x00 }, 	// white
		{	0x10,	0x00,	0xDB,	0x40,	0x8A,	0x40,	0xDB,	0x40 }, 	// yellow
		{	0x99,	0xC0,	0xBC,	0x80,	0x10,	0x00,	0xBC,	0x80 }, 	// cyan
		{	0x29,	0xC0,	0xAC,	0xC0,	0x1A,	0x40,	0xAC,	0xC0 }, 	// green
		{	0xD6,	0x40,	0x4E,	0x40,	0xE5,	0xC0,	0x4E,	0x40 }, 	// magenta
		{	0x66,	0x40,	0x3E,	0x80,	0xF0,	0x00,	0x3E,	0x80 }, 	// red
		{	0xF0,	0x00,	0x1F,	0xC0,	0x75,	0xC0,	0x1F,	0xC0 }, 	// blue
		{	0x80,	0x00,	0x10,	0x00,	0x80,	0x00,	0x10,	0x00 }, 	// black
		{	0x80,	0x00,	0xEB,	0x00,	0x80,	0x00,	0xEB,	0x00 }, 	// white
	};

	if( target==Layout_UYVYM )
	{
		if( (V*(H/2)*8)>len ) return 0;

		for (j = 0; j < V; j++)
		{
			byte* pUYVYM=video_buffer+j*H*4;

			for (i = 0; i < H; i+=2)
			{
				b=(i*9)/H;

				*pUYVYM++  = CbYCrY_table[b][0];
				*pUYVYM++  = CbYCrY_table[b][1];
				*pUYVYM++  = CbYCrY_table[b][2];
				*pUYVYM++  = CbYCrY_table[b][3];
				*pUYVYM++  = CbYCrY_table[b][4];
				*pUYVYM++  = CbYCrY_table[b][5];
				*pUYVYM++  = CbYCrY_table[b][6];
				*pUYVYM++  = CbYCrY_table[b][7];
			}
		}
		return V*(H/2)*8;
	}
	else if( target==Layout_UYVY )
	{
		if( (V*(H/2)*4)>len ) return 0;

		for (j = 0; j < V; j++)
		{
			byte* pUYVY=video_buffer+j*H*2;

			for (i = 0; i < H; i+=2)
			{
				b=(i*9)/H;

				*pUYVY++  = UYVY_table[b][0];
				*pUYVY++  = UYVY_table[b][2];
				*pUYVY++  = UYVY_table[b][4];
				*pUYVY++  = UYVY_table[b][6];
			}
		}
		return V*(H/2)*4;
	}
	else if( target==Layout_P_UYVY )
	{
		// 5 bytes for 2 pixels
		if( (V*(H/2)*5)>len ) return 0;

		// do the LSBs first
		// 1 byte for each 2 pixels
		for (j = 0; j < V; j++)
		{
			byte* pP_UYVY=video_buffer+j*(H/2);

			for (i = 0; i < H; i+=2)
			{
				b=(i*9)/H;

				*pP_UYVY++  =   ((CbYCrY_table[b][1]&0xc0)>>0)
							|  ((CbYCrY_table[b][3]&0xc0)>>2)
							|  ((CbYCrY_table[b][5]&0xc0)>>4)
							|  ((CbYCrY_table[b][7]&0xc0)>>6) ;
			}
		}

		// do the MSBs
		// just like UYVY, except starts later
		for (j = 0; j < V; j++)
		{
			byte* pP_UYVY=video_buffer+V*(H/2)+j*(H/2)*4;

			for (i = 0; i < H; i+=2)
			{
				b=(i*9)/H;

				*pP_UYVY++  = CbYCrY_table[b][0];
				*pP_UYVY++  = CbYCrY_table[b][2];
				*pP_UYVY++  = CbYCrY_table[b][4];
				*pP_UYVY++  = CbYCrY_table[b][6];
			}
		}
		return V*(H/2)*5;
	}
	else if( target==Layout_BGRA )
	{
		if( (V*H*4)>len ) return 0;

		unsigned ninth= H/9;

		byte *bp=video_buffer;

		int line;
		for( line=0; line<V; line++ )
		{
			int pixel;
			for( pixel=0; pixel< H; pixel++ )
			{
				unsigned bar=pixel/ninth;

				*bp++ = (bar&0x01)? (byte)16 : (byte)235; // b
				*bp++ = (bar&0x04)? (byte)16 : (byte)235; // g
				*bp++ = (bar&0x02)? (byte)16 : (byte)235; // r
				*bp++ = 0;								  // a
			}
		}
		return V*H*4;
	}
	else if( target==Layout_BGRAI )
	{
		if( (V*H*8)>len ) return 0;

		unsigned ninth= H/9;

		byte *bp=video_buffer;

		int line;
		for( line=0; line<V; line++ )
		{
			int pixel;
			for( pixel=0; pixel< H; pixel++ )
			{
				unsigned bar=pixel/ninth;

				*bp++ = (bar&0x01)? (byte)0 : (byte)0xc0; // lsb
				*bp++ = (bar&0x01)? (byte)16 : (byte)235; // b
				*bp++ = (bar&0x04)? (byte)0 : (byte)0xc0; // lsb
				*bp++ = (bar&0x04)? (byte)16 : (byte)235; // g
				*bp++ = (bar&0x02)? (byte)0 : (byte)0xc0; // lsb
				*bp++ = (bar&0x02)? (byte)16 : (byte)235; // r
				*bp++ = 0;								  // lsb
				*bp++ = 0;								  // a
			}
		}
		return V*H*8;
	}
	else return 0;
}
