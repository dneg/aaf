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
// All rights reserved.
//
//=---------------------------------------------------------------------=

#include <eof.h>

// This file is necessary because CodeWarrior 6 seems to use inline tolower() and toupper() 
// but the CW5 derived libOMFANSI must have used real code in the runtime lib

extern "C"
{
int tolower(int c);
int toupper(int c);
}

unsigned char   __lower_map[];

extern  unsigned char   __lower_map[];
extern  unsigned char   __upper_map[];
#define __zero_fill(c)     ((int) (unsigned char) (c))

int tolower(int c)      { return ((c == EOF) ? EOF:((int) 
__lower_map[__zero_fill(c)])); } 
int toupper(int c)      { return ((c == EOF) ? EOF:((int) 
__upper_map[__zero_fill(c)])); } 
