/* 
 * File:   pcap_structus.h
 * Author: owenm
 *
 * Created on April 20, 2009, 8:23 AM
 */

#ifndef _PCAP_STRUCTUS_H
#define	_PCAP_STRUCTUS_H

// #include <sys/appleapiopts.h>
#include <sys/types.h>          /* XXX temporary hack to get u_ types */
#include <netinet/in.h>
#include <netinet/in_systm.h>

#ifdef	__cplusplus
extern "C"
{
#endif

// Ethernet header
#define ETHER_ADDR_LEN 6

struct SniffEthernet
{
   u_char ether_dhost[ETHER_ADDR_LEN]; // destination host address
   u_char ether_shost[ETHER_ADDR_LEN]; // source host address
   u_short ether_type; // IP? ARP? RARP? etc
};

// IP header

struct SniffIP
{
#ifdef _IP_VHL
   u_char ip_vhl; /* version << 4 | header length >> 2 */
#else
#if BYTE_ORDER == LITTLE_ENDIAN
   u_int ip_hl : 4, /* header length */
           ip_v : 4; /* version */
#endif
#if BYTE_ORDER == BIG_ENDIAN
   u_int ip_v : 4, /* version */
           ip_hl : 4; /* header length */
#endif
#endif /* not _IP_VHL */
   u_char ip_tos; /* type of service */
   u_short ip_len; /* total length */
   u_short ip_id; /* identification */
   u_short ip_off; /* fragment offset field */
#define IP_RF 0x8000                    /* reserved fragment flag */
#define IP_DF 0x4000                    /* dont fragment flag */
#define IP_MF 0x2000                    /* more fragments flag */
#define IP_OFFMASK 0x1fff               /* mask for fragmenting bits */
   u_char ip_ttl; /* time to live */
   u_char ip_p; /* protocol */
   u_short ip_sum; /* checksum */
   struct in_addr ip_src, ip_dst; /* source and dest address */

};

#define IP_HL(ip)               (((ip)->ip_vhl) & 0x0f)
#define IP_V(ip)                (((ip)->ip_vhl) >> 4)
#define IP_MAKE_VHL(v, hl)      ((v) << 4 | (hl))

#ifdef _IP_VHL
#define IP_MAKE_VHL(v, hl)      ((v) << 4 | (hl))
#define IP_VHL_HL(vhl)          ((vhl) & 0x0f)
#define IP_VHL_V(vhl)           ((vhl) >> 4)
#define IP_VHL_BORING           0x45
#endif



// ICMP Header

struct Sniff_ICMP_ra_addr
{
   u_int32_t ira_addr;
   u_int32_t ira_preference;
};

struct SniffICMP
{
   u_char icmp_type; /* type of message, see below */
   u_char icmp_code; /* type sub code */
   u_short icmp_cksum; /* ones complement cksum of struct */

#if _LINUX

   union
   {

      struct
      {
         u_int16_t id;
         u_int16_t sequence;
      } echo; /* echo datagram */
      u_int32_t gateway; /* gateway address */

      struct
      {
         u_int16_t __unused;
         u_int16_t mtu;
      } frag; /* path mtu discovery */
   } un;
#endif

   //#if _BSD

   union
   {
      u_char ih_pptr; /* ICMP_PARAMPROB */
      struct in_addr ih_gwaddr; /* ICMP_REDIRECT */

      struct ih_idseq
      {
         n_short icd_id;
         n_short icd_seq;
      } ih_idseq;
      int ih_void;

      /* ICMP_UNREACH_NEEDFRAG -- Path MTU Discovery (RFC1191) */
      struct ih_pmtu
      {
         n_short ipm_void;
         n_short ipm_nextmtu;
      } ih_pmtu;

      struct ih_rtradv
      {
         u_char irt_num_addrs;
         u_char irt_wpa;
         u_int16_t irt_lifetime;
      } ih_rtradv;
   } icmp_hun;
#define icmp_pptr       icmp_hun.ih_pptr
#define icmp_gwaddr     icmp_hun.ih_gwaddr
#define icmp_id         icmp_hun.ih_idseq.icd_id
#define icmp_seq        icmp_hun.ih_idseq.icd_seq
#define icmp_void       icmp_hun.ih_void
#define icmp_pmvoid     icmp_hun.ih_pmtu.ipm_void
#define icmp_nextmtu    icmp_hun.ih_pmtu.ipm_nextmtu
#define icmp_num_addrs  icmp_hun.ih_rtradv.irt_num_addrs
#define icmp_wpa        icmp_hun.ih_rtradv.irt_wpa
#define icmp_lifetime   icmp_hun.ih_rtradv.irt_lifetime

   union
   {

      struct id_ts
      {
         n_time its_otime;
         n_time its_rtime;
         n_time its_ttime;
      } id_ts;

      struct id_ip
      {
         struct SniffIP idi_ip;
         /* options and then 64 bits of data */
      } id_ip;
      struct Sniff_ICMP_ra_addr id_radv;
      u_int32_t id_mask;
      char id_data[1];
   } icmp_dun;
#define icmp_otime      icmp_dun.id_ts.its_otime
#define icmp_rtime      icmp_dun.id_ts.its_rtime
#define icmp_ttime      icmp_dun.id_ts.its_ttime
#define icmp_ip         icmp_dun.id_ip.idi_ip
#define icmp_radv       icmp_dun.id_radv
#define icmp_mask       icmp_dun.id_mask
#define icmp_data       icmp_dun.id_data

   //#endif  // BSD

};

// UDP Header

struct SniffUDP
{
   u_short uh_sport; // source port
   u_short uh_dport; // destination port
   u_short uh_ulen; // udp length
   u_short uh_sum; // udp checksum
};

// TCP header
typedef u_int tcp_seq;

struct SniffTCP
{
   u_short th_sport; // source port
   u_short th_dport; // destination port
   tcp_seq th_seq; // sequence number
   tcp_seq th_ack; // acknowledgement number
#if BYTE_ORDER == LITTLE_ENDIAN  // unused and data offset
   u_int th_x2 : 4, th_off : 4;
#endif
#if BYTE_ORDER == BIG_ENDIAN
   u_int th_off : 4, th_x2 : 4;
#endif
   //   u_char th_offx2; // data offset, rsvd
   //#define TH_OFF(th)      (((th)->th_offx2 & 0xf0) >> 4)
   u_char th_flags;
#define TH_FIN  0x01
#define TH_SYN  0x02
#define TH_RST  0x04
#define TH_PUSH 0x08
#define TH_ACK  0x10
#define TH_URG  0x20
#define TH_ECE  0x40
#define TH_CWR  0x80
#define TH_FLAGS        (TH_FIN|TH_SYN|TH_RST|TH_ACK|TH_URG|TH_ECE|TH_CWR)
   u_short th_win; // window
   u_short th_sum; // checksum
   u_short th_urp; // urgent pointer
};

#ifdef	__cplusplus
}
#endif

#endif	/* _PCAP_STRUCTUS_H */

