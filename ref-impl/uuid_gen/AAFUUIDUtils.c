//=---------------------------------------------------------------------=
//
// $Id$ $Name$
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
// The Original Code of this file is Copyright 1998-2004, Licensor of the
// AAF Association.
//
//=---------------------------------------------------------------------=

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <AAFPlatform.h>

#ifdef OS_WINDOWS
#include <objbase.h>
#else
#include <sys/time.h>
#include <sys/times.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <net/if.h>
#include <netinet/in.h>
#endif
#ifdef OS_LINUX
#include <sys/sysinfo.h>
#include <uuid/uuid.h>
#endif

#ifdef OS_SOLARIS
#include <netdb.h>
#include <sys/sockio.h>
#include <sys/loadavg.h>
#include <net/if_arp.h>
#endif

#ifdef OS_MACOS
#include <CoreServices/CoreServices.h>
#endif
#ifdef OS_IRIX
#ifdef __cplusplus
extern "C" {
#endif
#include <sys/uuid.h>
#ifdef __cplusplus
}
#endif
#endif
#include "AAFMD5Utils.h"
#include "AAFTimeUtils.h"
#include "AAFUUIDUtils.h"
/*************
#ifdef HAVE_SYS_FILE_H
# include <sys/file.h>
#endif
#ifdef HAVE_SYS_IOCTL_H
# include <sys/ioctl.h>
#endif
#ifdef HAVE_SYS_SOCKET_H
# include <sys/socket.h>
#endif
#ifdef HAVE_NET_IF_H
# include <net/if.h>
#endif
#ifdef HAVE_NETINET_IN_H
# include <netinet/in.h>
#endif
*************/
# include <sys/file.h>


/***** Utilities from rpcrt4_main.c */
/*
 *  RPCRT4
 *
 * Copyright 2000 Huw D M Davies for Codeweavers
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 * 
 */

/*************************************************************************
 *           get_node_id   [RPCRT4.@]
 *
 * Creates a 6 byte node address.
 * Implemented according the DCE specification for UUID generation.
 * Code is based upon uuid library in e2fsprogs by Theodore Ts'o.
 * Copyright (C) 1996, 1997 Theodore Ts'o.
 *
 * RETURNS
 *
 *  kAAFTrue if it found the IEEE node number
 */
#if !defined OS_WINDOWS && !defined OS_IRIX && !defined OS_MACOS
#if defined OS_SOLARIS
static aafBoolean_t get_ieee_node_id(aafUUID_Node_t *node)
{
	static aafBoolean_t has_init = kAAFFalse;
	static aafBoolean_t has_ieee_id = kAAFFalse;
	struct hostent hostentBuf;
	struct hostent *phost;
	char **paddrs;
	static struct arpreq ar;
	struct sockaddr_in * psa;
	int s,i,herr;
	char name[MAXHOSTNAMELEN];
	char hbuf[512];


	if (!has_init)
	{
		has_init = kAAFTrue;
		if (gethostname(name,sizeof(name))) return kAAFFalse;


		/* get this host name */
		phost = gethostbyname_r(name, &hostentBuf, hbuf, sizeof(hbuf), &herr);
		if (phost == 0) return kAAFFalse;

		/* open a socket */
		s = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
		if (s == -1) return kAAFFalse;

		paddrs = phost->h_addr_list;
		psa    = ( struct sockaddr_in * )&( ar.arp_pa );
		memset( &ar, 0, sizeof( struct arpreq ) );
		psa->sin_family = AF_INET;
		memcpy( &( psa->sin_addr ), *paddrs, sizeof( struct in_addr ) );
		if ( ioctl( s, SIOCGARP, &ar ) == -1 )
		{
			perror("ioctl");
			close(s);
			return kAAFFalse;
		}
		close(s);
		has_ieee_id =  kAAFTrue;
	}

	if (has_ieee_id)
	{
		for (i = 0; i < 6; i++)
		{
			node->nodeID[i] = ar.arp_ha.sa_data[i] & 0xff;
		}
	}
	return kAAFTrue;
}

#elif defined OS_LINUX
static aafBoolean_t get_ieee_node_id(aafUUID_Node_t *node)
{
   static aafBoolean_t has_init = kAAFFalse;
   static aafBoolean_t has_ieee_id = kAAFFalse;
   static aafInt8 a[6];
   static int                      adjustment = 0;
#ifdef HAVE_NET_IF_H
   int             sd;
   struct ifreq    ifr, *ifrp;
   struct ifconf   ifc;
   char buf[1024];
   int             n, i;
#endif

   /* Have we already tried to get the MAC address? */
   if (!has_init)
   {
   		has_init = kAAFTrue;
#ifdef HAVE_NET_IF_H
      /* BSD 4.4 defines the size of an ifreq to be
       * max(sizeof(ifreq), sizeof(ifreq.ifr_name)+ifreq.ifr_addr.sa_len
       * However, under earlier systems, sa_len isn't present, so
       *  the size is just sizeof(struct ifreq)
       */
#ifdef HAVE_SOCKADDR_SA_LEN
#  ifndef max
#   define max(a,b) ((a) > (b) ? (a) : (b))
#  endif
#  define ifreq_size(i) max(sizeof(struct ifreq),\
sizeof((i).ifr_name)+(i).ifr_addr.sa_len)
# else
#  define ifreq_size(i) sizeof(struct ifreq)
# endif /* defined(HAVE_SOCKADDR_SA_LEN) */

      sd = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);

      if (sd < 0)
	  {
	  	return kAAFFalse;
      } 
	  else
	  {
		 memset(buf, 0, sizeof(buf));
		 ifc.ifc_len = sizeof(buf);
		 ifc.ifc_buf = buf;
		 /* get the ifconf interface */
		 if (ioctl (sd, SIOCGIFCONF, (char *)&ifc) < 0)
		 {
		    close(sd);
			return kAAFFalse;
	 	}
		else
		{
	    /* loop through the interfaces, looking for a valid one */
		    n = ifc.ifc_len;
	   		for (i = 0; i < n; i+= ifreq_size(ifr) )
			{
	   	   		ifrp = (struct ifreq *)((char *) ifc.ifc_buf+i);
	   	   		strncpy(ifr.ifr_name, ifrp->ifr_name, IFNAMSIZ);
	       		/* try to get the address for this interface */
# ifdef SIOCGIFHWADDR
	       		if (ioctl(sd, SIOCGIFHWADDR, &ifr) < 0)
		   			continue;
	       		memcpy(a, (aafUInt8 *)&ifr.ifr_hwaddr.sa_data, 6);
# else
#  ifdef SIOCGENADDR
				if(ioctl(sd, SIOCGIFFLAGS, &ifr) < 0)
				{
					perror ("SIOCGIFFLAGS Err: ");
				}
	       		if (ioctl(sd, SIOCGENADDR, &ifr) < 0)
				{
					perror ("Ethernet Addr Err: ");
		   			continue;
				}
	       		memcpy(a, (aafUInt8 *) ifr.ifr_enaddr, 6);
#  else
	       		/* XXX we don't have a way of getting the hardware address */
	       		close(sd);
	       		a[0] = 0;
	       		break;
#  endif /* SIOCGENADDR */
# endif /* SIOCGIFHWADDR */
	       /* make sure it's not blank */
	       		if (!a[0] && !a[1] && !a[2] && !a[3] && !a[4] && !a[5])
		   			continue;
				has_ieee_id = kAAFTrue;
	       		goto valid_address;
	    	}
	    	/* if we didn't find a valid address, make a random one */
	   		 /* once again, set multicast bit to avoid conflicts */
			return kAAFFalse;
	    valid_address:
	   		close(sd);
	 	}
      }
#else
      /* no networking info, so generate a random address */
	  return kAAFFalse;
#endif /* HAVE_NET_IF_H */
   }
	if (has_ieee_id)
	{
	    memcpy(node->nodeID, a, 6);
	}

	return has_ieee_id;
}
#else
#error Unknown Operating System
#endif


/*
  ** Copyright (c) 1990- 1993, 1996 Open Software Foundation, Inc.
  ** Copyright (c) 1989 by Hewlett-Packard Company, Palo Alto, Ca. &
  ** Digital Equipment Corporation, Maynard, Mass.
  ** Copyright (c) 1998 Microsoft.
  ** To anyone who acknowledges that this file is provided "AS IS"
  ** without any express or implied warranty: permission to use, copy,
  ** modify, and distribute this file for any purpose is hereby
  ** granted without fee, provided that the above copyright notices and
  ** this notice appears in all source code copies, and that none of
  ** the names of Open Software Foundation, Inc., Hewlett-Packard
  ** Company, or Digital Equipment Corporation be used in advertising
  ** or publicity pertaining to distribution of the software without
  ** specific, written prior permission.  Neither Open Software
  ** Foundation, Inc., Hewlett-Packard Company, Microsoft, nor Digital
  Equipment
  ** Corporation makes any representations about the suitability of
  ** this software for any purpose.
  */


#define LOCK
#define UNLOCK
/* format_uuid_v1 -- make a UUID from the timestamp, clockseq,
                     and node ID */
static void format_uuid_v1(aafUUID_t * uuid, aafUInt16 clock_seq, aafUUID_Time_t
timestamp, aafUUID_Node_t node)
{
	/* Construct a version 1 uuid with the information we've gathered
	 * plus a few constants. */
	uuid->time_low = (aafUInt32)(timestamp & 0xFFFFFFFF);
	uuid->time_mid = (aafUInt16)((timestamp >> 32) & 0xFFFF);
	uuid->time_hi_and_version = (aafUInt16)((timestamp >> 48) &
	   0x0FFF);
	uuid->time_hi_and_version |= (1 << 12);
	uuid->clock_seq_low = clock_seq & 0xFF;
	uuid->clock_seq_hi_and_reserved = (clock_seq & 0x3F00) >> 8;
	uuid->clock_seq_hi_and_reserved |= 0x80;
	memcpy(&uuid->node, &node, sizeof uuid->node);
}


/* true_random -- generate a crypto-quality random number.
   This sample doesn't do that. */
static aafUInt32 true_random(void)
{
	static int inited = 0;
	aafUUID_Time_t time_now;

	if (!inited)
	{
		aafGetSystemTime(&time_now);
		time_now = time_now/UIDS_PER_TICK;
		srand((aafUInt32)(((time_now >> 32) ^ time_now)&0xffffffff));
		inited = 1;
	}

	return (rand());
}


/* format_uuid_v3 -- make a UUID from a (pseudo)random 128 bit number */
static void format_uuid_v3(aafUUID_t * uuid, aafUInt8 hash[16])
{
	/* Construct a version 3 uuid with the (pseudo-)random number
	* plus a few constants. */
	
	memcpy(uuid, hash, sizeof(aafUUID_t));
	
	/* convert UUID to local byte order */
	ntohl(uuid->time_low);
	ntohs(uuid->time_mid);
	ntohs(uuid->time_hi_and_version);
	
	/* put in the variant and version bits */
	uuid->time_hi_and_version &= 0x0FFF;
	uuid->time_hi_and_version |= (3 << 12);
	uuid->clock_seq_hi_and_reserved &= 0x3F;
	uuid->clock_seq_hi_and_reserved |= 0x80;
};

#if defined OS_SOLARIS
typedef struct
{
	clock_t uptime;
	double loads[3];
	pid_t pid;
} SysInfo;
#elif defined OS_LINUX
typedef struct sysinfo SysInfo;
#else
#endif

static void getSysInfo (SysInfo *s)
{
#if defined OS_SUNOS
	struct tms *buf;
	s->uptime = times (buf);
	getloadavg(s->loads, 3);
	s->pid = getpid();
#elif defined OS_LINUX
	sysinfo(s);
#else
#endif
}

static void get_random_info(aafInt8 seed[16])
{
	MD5_CTX c;
	typedef struct
	{
		SysInfo s;
		struct timeval t;
		char hostname[257];
	} randomness;
	randomness r;
	
	MD5Init(&c);
	getSysInfo(&r.s);
	gettimeofday(&r.t, (struct timezone *)0);
	gethostname(r.hostname, 256);
	MD5Update(&c, (aafUInt8 *)&r, sizeof(randomness));
	MD5Final((aafUInt8 *)seed, &c);
}


/* 
* Function: gen_ieee_node_identifier
*   Generates a random node ID
*/
static void gen_ieee_node_identifier(aafUUID_Node_t *node)
{
	aafInt8 seed[16];
	FILE * fd;
	static aafInt32 inited = 0;
	static aafUUID_Node_t saved_node;

	if (!inited)
	{
		get_random_info(seed);
		seed[0] |= 0x80;
		memcpy(&saved_node, seed, sizeof(aafUUID_Node_t));
		inited = 1;
	}

	*node = saved_node;
}

/* aafUUIDCreateFromName -- create a UUID using a "name" from a "name space" */
extern void aafUUIDCreateFromName
(
	aafUUID_t * uuid,        /* resulting UUID */
	aafUUID_t nsid,          /* UUID to serve as context, so identical
   		                      names from different name spaces generate
   		                      different UUIDs */
	const void * name,          /* the name from which to generate a UUID */
	int namelen           /* the length of the name */
)
{
	MD5_CTX c;
	aafUInt8 hash[16];
	aafUUID_t net_nsid;      /* context UUID in network byte order */
	
	/* put name space ID in network byte order so it hashes the same
	no matter what endian machine we're on */
	net_nsid = nsid;
	htonl(net_nsid.time_low);
	htons(net_nsid.time_mid);
	htons(net_nsid.time_hi_and_version);
	
	MD5Init(&c);
	MD5Update(&c, (aafUInt8 *)&net_nsid, sizeof(aafUUID_t));
	MD5Update(&c, (aafUInt8 *)name, namelen);
	MD5Final(hash, &c);
	
	  /* the hash is in network byte order at this point */
	format_uuid_v3(uuid, hash);
}
#endif
/* aafUUIDCompare --  Compare two UUID's "lexically" and return
       -1   u1 is lexically before u2
        0   u1 is equal to u2
        1   u1 is lexically after u2

    Note:   lexical ordering is not temporal ordering!
*/
extern int aafUUIDCompare (aafUUID_t *u1, aafUUID_t *u2)
{
	int i;
	
	#define CHECK(f1, f2) if (f1 != f2) return f1 < f2 ? -1 : 1;
	CHECK(u1->time_low, u2->time_low);
	CHECK(u1->time_mid, u2->time_mid);
	CHECK(u1->time_hi_and_version, u2->time_hi_and_version);
	CHECK(u1->clock_seq_hi_and_reserved, u2->clock_seq_hi_and_reserved);
	CHECK(u1->clock_seq_low, u2->clock_seq_low)
	for (i = 0; i < 6; i++)
	{
		if (u1->node[i] < u2->node[i])
			return -1;
		if (u1->node[i] > u2->node[i])
			return 1;
	}
	return 0;
}

/* aafUUIDCreate -- generator a UUID */
extern int aafUUIDCreate (aafUUID_t * uuid)
{
#if defined OS_WINDOWS
	CoCreateGuid((GUID *)uuid);
#elif defined OS_IRIX
	uint_t retstat;
	uuid_create ((uuid_t *)uuid, &retstat);
#elif defined OS_LINUX
	uuid_t tmp;
	uuid_generate_time (tmp);
	memcpy (uuid, &tmp, sizeof(*uuid));
#elif defined OS_MACOS
	CFUUIDRef	a;
	a = CFUUIDCreate( kCFAllocatorDefault );
	CFUUIDBytes b = CFUUIDGetUUIDBytes(a);
	memcpy (uuid, &b, sizeof(*uuid));
	CFRelease(a);
#else
	aafUUID_Time_t timestamp; 
	static aafUUID_Time_t last_time;
	static aafUInt16 clockseq;
	aafUUID_Node_t node;
	static aafUUID_Node_t last_node;
	int f;

	/* get current time */
	aafGetCurrentTime (&timestamp);

	/* get ieee node ID. If can't get one generate a random one */
	if (!get_ieee_node_id(&node))
	{
		gen_ieee_node_identifier(&node);
	}
	
	/* if node ID changed (e.g., net card swap) change clockseq */
	if (memcmp(&node, &last_node, sizeof(aafUUID_Node_t)))
		clockseq = (aafUInt16)true_random();
	else if (timestamp < last_time)
		clockseq++;
	
	/* stuff fields into the UUID */
	format_uuid_v1(uuid, clockseq, timestamp, node);

	last_time = timestamp;
	last_node = node;
	
#endif
	return(1);
}

