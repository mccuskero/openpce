/**
 * Copyright (c) 1994-2007 Owen McCusker <pcave@myeastern.com>
 * All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 * All rights reserved.
 */

#ifndef _snortStructs_h_
#define _snortStructs_h_

#include <sys/types.h>
#include <sys/time.h>

// temporary: output-plugins/spo_alert_unixsock.h
// TODO: need to use headers from snort install, instead
// of taking them from the project

// used in universial log parsing
#define SNORT_MAGIC     0xa1b2c3d4
#define ALERT_MAGIC     0xDEAD4137  // part of header
#define LOG_MAGIC       0xDEAD1080  //

// problem with IRIX machines, where SNAPLEN is 1510 (decode.h)
#define SNAPLEN         1514
#define ALERTMSG_LENGTH 256

#ifndef __APPLE__ 
struct timeval32
{
    u_int32_t tv_sec;      /* seconds */
    u_int32_t tv_usec;     /* microseconds */
};
#endif

struct pcap_pkthdr32
{
    struct timeval32 ts;   /* packet timestamp */
    u_int32_t caplen;      /* packet capture length */
    u_int32_t pktlen;      /* packet "real" length */
};

typedef struct _Event
{
    u_int32_t sig_gen;
    u_int32_t sig_id;
    u_int32_t sig_rev;         
    u_int32_t classification_id;  
    u_int32_t event_priority;    
    u_int32_t event_id;        
    u_int32_t event_reference; 
    struct timeval event_reference_time;
} Event;

// this struct is for the alert socket code....
typedef struct _Alertpkt
{
    u_int8_t alertmsg[ALERTMSG_LENGTH]; // variable.. */
    struct pcap_pkthdr32 pkth;
    u_int32_t dataLinkHdr;       // datalink header offset. e.g. ethernet
    u_int32_t networkHdr;       // network header offset. e.g. ip 
    u_int32_t transportHdr;     // transport header offset e.g. tcp/udp/icmp
    u_int32_t data;
    u_int32_t val;  // which fields are valid. (NULL is also valid)
    // Packet struct --> was null 
#define NOPACKET_STRUCT 0x1
    // no transport headers in packet 
#define NO_TRANSHDR    0x2
    u_int8_t pkt[SNAPLEN];
    Event event;
} Alertpkt;


#endif
