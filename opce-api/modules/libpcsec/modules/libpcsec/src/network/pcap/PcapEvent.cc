/* 
 * File:   PcapEvent.cc
 * Author: owenm
 * 
 * Created on April 17, 2009, 7:29 PM
 */


#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <pcap.h>

#include <iostream>
#include <sstream>

#include <pcff/BasicExceptionMgr.h>
#include <pcff/DumpBuffer.h>

#include <pcsec/network/EthernetHeaderInfo.h>
#include <pcsec/network/WifiHeaderInfo.h>
#include <pcsec/network/IPHeaderInfo.h>
#include <pcsec/network/ICMPInfo.h>
#include <pcsec/network/UDPHeaderInfo.h>
#include <pcsec/network/TCPHeaderInfo.h>
#include <pcsec/network/PcapException.h>

#include <pcsec/network/PcapEvent.h>
#include <pcff/TimeStamp.h>


//  TODO: deal with BSD vs Linux build issues and struct names

using namespace std;
using namespace pcff;
using namespace pcsec;

PcapEvent::PcapEvent()
{
   initMembers();
}

PcapEvent::~PcapEvent()
{
   if (_netPacketInfo != NULL)
   {
      delete _netPacketInfo;
      _netPacketInfo = NULL;
   }

}

void PcapEvent::initMembers()
{
   _netPacketInfo = NULL;
   _dataLinkHdrOffset = 0;
   _networkHdrOffset = 0;
   _transportHdrOffset = 0;
   _dataOffset = 0;
}

void PcapEvent::initialize(const struct pcap_pkthdr &header, const u_char *packet, int &linkOffSize)
{
   _refTime_timeVal_tv_sec = header.ts.tv_sec;
   _refTime_timeVal_tv_usec = header.ts.tv_usec;

   _dataLinkHdrOffset = 0; // packet starts with ethernet information
   _networkHdrOffset = linkOffSize; // next area is IP information

   if (packet != NULL)
   {
      _packet_tv_sec = header.ts.tv_sec; // seconds
      _packet_tv_usec = header.ts.tv_usec; // microseconds
      _packetCaptureLen = header.caplen; // packet capture length
      _packetLen = header.len; // packet "real" length, it may be bigger then size allocated in alert msg

      BasicExceptionMgr::instance()->log(LogLevel::Debug, "PcapEvent::initialize: tv_sec:%d, tv_usec:%d, caplen:%d, len:%d", _packet_tv_sec, _packet_tv_usec, _packetCaptureLen, _packetLen);

      createPacketInfo(packet, header.len, header.caplen);
   }
   else
   {
      BasicExceptionMgr::instance()->log(LogLevel::Debug, "PcapEvent::initialize: Packet is NULL - tv_sec:%d, tv_usec:%d, caplen:%d, len:%d", _packet_tv_sec, _packet_tv_usec, _packetCaptureLen, _packetLen);
   }
}

void PcapEvent::createPacketInfo(const u_char *packet, u_int32_t packetLen, u_int32_t packetCaptureLen)
{
   struct SniffEthernet *ethernet = NULL; /* The ethernet header */
   struct SniffIP *ip = NULL; /* The IP header */
   struct SniffICMP *icmp = NULL; /* The IP header */
   struct SniffUDP *udp = NULL; /* The IP header */
   struct SniffTCP *tcp = NULL; /* The TCP header */
   char *payload = NULL; /* Packet payload */
   u_int sizeIPHeader = 0;
   u_int sizeTransportHeader = 0;
   u_int sizePayload = 0;

   // create network packet info, and set pacekt
   _netPacketInfo = new NetworkPacketInfo();
   _bHaveNetworkDataInfo = true;

   _netPacketInfo->setPacketCaptureTime(_packet_tv_sec, _packet_tv_usec);

   ethernet = (struct SniffEthernet*) (packet);
   retrieveEthernetHeaderInfo(ethernet, *_netPacketInfo);

   ip = (struct SniffIP*) (packet + _networkHdrOffset);

   if (ip != NULL)
   {
      // ip
      sizeIPHeader = sizeof (struct SniffIP);
      retrieveIPHeaderInfo(ip, *_netPacketInfo);

      _transportHdrOffset = _networkHdrOffset + sizeIPHeader;

      switch (_netPacketInfo->getProtocol())
      {
      case IPPROTO_ICMP:
         BasicExceptionMgr::instance()->log(LogLevel::Debug, "PcapEvent::createPacketInfo - IPPROTO_ICMP.");
         icmp = (struct SniffICMP *) (packet + _transportHdrOffset);
         sizeTransportHeader = sizeof (struct SniffICMP);
         retrieveICMPHeaderInfo(icmp, *_netPacketInfo);
         break;
      case IPPROTO_UDP:
         BasicExceptionMgr::instance()->log(LogLevel::Debug, "PcapEvent::createPacketInfo - IPPROTO_UDP.");
         udp = (struct SniffUDP*) (packet + _transportHdrOffset);
         sizeTransportHeader = sizeof (struct SniffUDP);
         retrieveUDPHeaderInfo(udp, *_netPacketInfo);
         break;
      case IPPROTO_TCP:
         BasicExceptionMgr::instance()->log(LogLevel::Debug, "PcapEvent::createPacketInfo - IPPROTO_TCP.");
         tcp = (struct SniffTCP*) (packet + _transportHdrOffset);
         sizeTransportHeader = sizeof (struct SniffTCP);

         // cout << "PcapEvent::createPacketInfo : sizeTransportHeader: " << sizeTransportHeader << " dataOffset from Header: " << tcp->th_off <<  endl;
         retrieveTCPHeaderInfo(tcp, *_netPacketInfo);
         break;
      case IPPROTO_IPV6:
         BasicExceptionMgr::instance()->log(LogLevel::Debug, "PcapEvent::createPacketInfo - IPv6 protocol not supported.");
         break;
      default:
         BasicExceptionMgr::instance()->log(LogLevel::Debug, "PcapEvent::createPacketInfo - protocol not supported %ld.", _netPacketInfo->getProtocol());
         break;
      }

      // TODO: Make sure you get the payload!!!
      // define/compute tcp payload (segment) offset
      _dataOffset = _dataLinkHdrOffset + sizeIPHeader + sizeTransportHeader;

      _netPacketInfo->setPacket((u_int8_t *) packet, packetLen, packetCaptureLen); // TODO: maybe an option in the future

      _netPacketInfo->setPacketOffsets(_dataLinkHdrOffset,
                                       _networkHdrOffset,
                                       _transportHdrOffset,
                                       _dataOffset);
   }
}

////
//	struct SniffEthernet {
//		u_char ether_dhost[ETHER_ADDR_LEN]; /* Destination host address */
//		u_char ether_shost[ETHER_ADDR_LEN]; /* Source host address */
//		u_short ether_type; /* IP? ARP? RARP? etc */
//	};
//

void PcapEvent::retrieveEthernetHeaderInfo(struct SniffEthernet *hdr,
                                           NetworkPacketInfo &netInfo)
{
   EthernetHeaderInfo etherInfo;

   etherInfo.initialize(hdr->ether_type,
                        *(hdr->ether_shost),
                        *(hdr->ether_dhost));

   netInfo.set(etherInfo);
}

////
//	struct SniffIP {
//		u_char ip_vhl;		/* version << 4 | header length >> 2 */
//		u_char ip_tos;		/* type of service */
//		u_short ip_len;		/* total length */
//		u_short ip_id;		/* identification */
//		u_short ip_off;		/* fragment offset field */
//	#define IP_RF 0x8000		/* reserved fragment flag */
//	#define IP_DF 0x4000		/* dont fragment flag */
//	#define IP_MF 0x2000		/* more fragments flag */
//	#define IP_OFFMASK 0x1fff	/* mask for fragmenting bits */
//		u_char ip_ttl;		/* time to live */
//		u_char ip_p;		/* protocol */
//		u_short ip_sum;		/* checksum */
//		struct in_addr ip_src,ip_dst; /* source and dest address */
//	};
//

void PcapEvent::retrieveIPHeaderInfo(struct SniffIP *hdr,
                                     NetworkPacketInfo &netInfo)
{
   char strSrcAddr[INET6_ADDRSTRLEN];
   char strDstAddr[INET6_ADDRSTRLEN];
   string sSrcAddr;
   string sDstAddr;
   IPHeaderInfo ipInfo;

   // TODO: make IPv6
   inet_ntop(AF_INET, (&(hdr->ip_src)), strSrcAddr, INET6_ADDRSTRLEN);
   inet_ntop(AF_INET, (&(hdr->ip_dst)), strDstAddr, INET6_ADDRSTRLEN);

   sSrcAddr = strSrcAddr;
   sDstAddr = strDstAddr;

   ipInfo.initialize(IP_MAKE_VHL(hdr->ip_v, hdr->ip_hl),
                     hdr->ip_tos,
                     hdr->ip_len,
                     hdr->ip_id,
                     hdr->ip_off,
                     hdr->ip_ttl,
                     hdr->ip_p,
                     hdr->ip_sum,
                     sSrcAddr,
                     sDstAddr);

   netInfo.set(ipInfo);
}

////
// ICMP Header
//
//struct SniffICMP
//{
// u_int8_t type;                /* message type */
//  u_int8_t code;                /* type sub-code */
//  u_int16_t checksum;
//  union
//  {
//    struct
//    {
//      u_int16_t id;
//      u_int16_t sequence;
//    } echo;                     /* echo datagram */
//    u_int32_t   gateway;        /* gateway address */
//    struct
//    {
//      u_int16_t __unused;
//      u_int16_t mtu;
//    } frag;                     /* path mtu discovery */
//  } un;
//};

void PcapEvent::retrieveICMPHeaderInfo(struct SniffICMP *hdr,
                                       NetworkPacketInfo &netInfo)
{
   ICMPInfo icmpInfo;

   icmpInfo.initialize(hdr->icmp_type,
                       hdr->icmp_code,
                       hdr->icmp_cksum,
                       1,
                       hdr->icmp_id,
                       hdr->icmp_seq);

   netInfo.set(icmpInfo);
}

////
// UDP Header
//struct SniffUDP {
//        u_short uh_sport;               // source port
//        u_short uh_dport;               // destination port
//        u_short uh_ulen;                // udp length
//        u_short uh_sum;                 // udp checksum
//};

void PcapEvent::retrieveUDPHeaderInfo(struct SniffUDP* hdr,
                                      NetworkPacketInfo &netInfo)
{
   UDPHeaderInfo udpInfo;

   udpInfo.initialize(hdr->uh_sport,
                      hdr->uh_dport,
                      hdr->uh_ulen,
                      hdr->uh_sum);

   netInfo.set(udpInfo);
}


////
// TCP header
//
//struct SniffTCP
//{
//    u_short th_sport; // source port
//    u_short th_dport; // destination port
//    tcp_seq th_seq; // sequence number
//    tcp_seq th_ack; // acknowledgement number
//    u_char th_offx2; // data offset, rsvd
//#define TH_OFF(th)      (((th)->th_offx2 & 0xf0) >> 4)
//    u_char th_flags;
//#define TH_FIN  0x01
//#define TH_SYN  0x02
//#define TH_RST  0x04
//#define TH_PUSH 0x08
//#define TH_ACK  0x10
//#define TH_URG  0x20
//#define TH_ECE  0x40
//#define TH_CWR  0x80
//#define TH_FLAGS        (TH_FIN|TH_SYN|TH_RST|TH_ACK|TH_URG|TH_ECE|TH_CWR)
//    u_short th_win; // window
//    u_short th_sum; // checksum
//    u_short th_urp; // urgent pointer
//};

void PcapEvent::retrieveTCPHeaderInfo(struct SniffTCP* hdr,
                                      NetworkPacketInfo &netInfo)
{
   TCPHeaderInfo tcpInfo;

   tcpInfo.initialize(hdr->th_sport,
                      hdr->th_dport,
                      hdr->th_seq,
                      hdr->th_ack,
                      hdr->th_off,
                      hdr->th_flags,
                      hdr->th_win,
                      hdr->th_sum,
                      hdr->th_urp);

   netInfo.set(tcpInfo);
}

void PcapEvent::serialize(Serializer &s)
{
   unsigned long packetCaptureLen = this->_packetCaptureLen;

   // event information
   s.transfer(this->_refTime_timeVal_tv_sec);
   s.transfer(this->_refTime_timeVal_tv_usec);

   // packet information
   s.transfer(this->_packet_tv_sec);
   s.transfer(this->_packet_tv_usec);
   s.transfer(this->_packetCaptureLen);
   s.transfer(this->_packetLen);
   s.transfer(this->_dataLinkHdrOffset);
   s.transfer(this->_networkHdrOffset);
   s.transfer(this->_transportHdrOffset);
   s.transfer(this->_dataOffset);
   s.transfer(this->_packet, packetCaptureLen); // TODO: serialize with packetlen, CaptureLen, save space

   s.transfer(this->_bHaveNetworkDataInfo);

   if (_bHaveNetworkDataInfo == true)
   {
      if (_netPacketInfo == NULL)
      {
         _netPacketInfo = new NetworkPacketInfo();
      }

      _netPacketInfo->serialize(s);
   }

   Event::serialize(s);
   // TODO: serializeRawEvent();
}

string PcapEvent::getEventInfo()
{
   string sTime;
   //   time_t localTime;
   ostringstream eventInfo;

   //   time(&localTime);
   TimeStamp ts(_refTime_timeVal_tv_sec, _refTime_timeVal_tv_usec);
   // TimeStamp ts(TimeStamp::SetToNow);


   //   cout << "localTime: " << localTime << endl;

   sTime = ts.get("YYYY-MM-DD HH:NN:SS");

   eventInfo << "Event Time-> ";
   eventInfo << sTime;
   eventInfo << " ";

   // get data info
   if (_bHaveNetworkDataInfo == true)
   {
      string networkDataInfo;
      _netPacketInfo->getInfo(networkDataInfo);
      eventInfo << networkDataInfo;
   }

   if (BasicExceptionMgr::instance()->getLogLevel() >= LogLevel::Info)
   {
      DumpBuffer::instance().init();
      //      DumpBuffer::instance().dump(_packetCaptureLen, (char *) _packet);
   }

   return eventInfo.str();
}

void PcapEvent::setTestData()
{

}