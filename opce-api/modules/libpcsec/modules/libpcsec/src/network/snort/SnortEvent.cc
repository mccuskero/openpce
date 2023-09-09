/**
 * Copyright (c)  1994-2007 Owen McCusker <pcave@myeastern.com>
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
/*
 * \class SnortEvent
 * \brief This is a basic Snort Event/Alert created from a Snort network sensor
 *
 *  This SnortEvent class is linked to the AlertPkt struct delivered with snort.
 * In this struct ALL payloads are limited to 1514 bytes. Anything greater
 * that this size is cut off e.g. snapped off. In the future we may derived
 * a class that works with FLoP. The FLoP system patches the unix socket output
 * logger to send two packets per alert, one is the alert, the other is a packet,
 * associated wiht the alert. The max size is 131072 bytes, variable lenght.
 *
 */

//
// SRS: Decoding of packets is dependent of the version of UDPHdr, TCPHdr, ICMPHdr...
// If you get build going on box using installed snort, then this is mute. verify!!
//
//

#include <errno.h>
#include <netdb.h>
#include <arpa/inet.h>

#include <sstream>

#include <pcff/BasicExceptionMgr.h>

#include <pcff/proxy/SensorProxyException.h>


#include <pcsec/network/EthernetHeaderInfo.h>
#include <pcsec/network/WifiHeaderInfo.h>
#include <pcsec/network/IPHeaderInfo.h>
#include <pcsec/network/ICMPInfo.h>
#include <pcsec/network/UDPHeaderInfo.h>
#include <pcsec/network/TCPHeaderInfo.h>


#include <pcsec/network/SnortEvent.h>

using namespace std;
using namespace pcff;
using namespace pcsec;

SnortEvent::SnortEvent()
{
    initMembers();
}

SnortEvent::~SnortEvent()
{
    if (_alertMsg != NULL) delete _alertMsg;
    if (_packet != NULL) delete _packet;
    if (_netPacketInfo != NULL) delete _netPacketInfo;
    initMembers();
}

void SnortEvent::initMembers()
{
    // Event related information
    _signatureGenerator = 0;
    _signatureID = 0;
    _signatureRevision = 0;
    _classificationID = 0;
    _priority = 0;
    _eventID = 0;
    _eventReferenceID = 0;
    _refTime_timeVal_tv_sec = 0;
    _refTime_timeVal_tv_usec = 0;
    _val = 0;
    _alertMsg = NULL;

    // packet information
    _packet_tv_sec = 0; // seconds
    _packet_tv_usec = 0; // microseconds
    _packetCaptureLen = 0; // packet capture length
    _packetLen = 0; // packet "real" length, it may be bigger then size allocated in alert msg

    _dataLinkHdrOffset = 0; // datalink header offset. e.g. ethernet
    _networkHdrOffset = 0; // network header offset. e.g. ip
    _transportHdrOffset = 0; // transport header offset e.g. tcp/udp/icmp
    _dataOffset = 0; // data length, offset
    _packet = NULL;

    _rawTestDataSize = 0;

    _bHaveNetworkDataInfo = false;
    _netPacketInfo = NULL;
}

void SnortEvent::initialize()
{
}

void SnortEvent::initialize(int size, char *buf)
{
    // save a memcpy, use the existing buffer
    Alertpkt *alertPacket = (Alertpkt *) buf;
    int alertpktsize = sizeof (Alertpkt);

    if (size == alertpktsize)
    {
        initialize((Alertpkt &) * alertPacket);
    }
    else
    {
        BasicExceptionMgr::instance()->log(LogLevel::Error, "Socket read size %d, is different from Alertpkt struct size %d", size, alertpktsize);
        throw SensorProxyException(SensorProxyException::EventInitializationError,
                                   "snort Alertpkt size mis-match");
    }
}

void SnortEvent::initialize(Alertpkt &alert)
{
    Packet *snortPacket = NULL;

    if (BasicExceptionMgr::instance()->getLogLevel() >= LogLevel::Info)
    {
        BasicExceptionMgr::instance()->log(LogLevel::Info, "Event ID     : %i", alert.event.event_id);
        BasicExceptionMgr::instance()->log(LogLevel::Info, "Alert-Message: %s", alert.alertmsg);
    }

    // Event related information
    _signatureGenerator = alert.event.sig_gen;
    _signatureID = alert.event.sig_id;
    _signatureRevision = alert.event.sig_rev;
    _classificationID = alert.event.classification_id;
    _priority = alert.event.event_priority;
    _eventID = alert.event.event_id;
    _eventReferenceID = alert.event.event_reference;
    _refTime_timeVal_tv_sec = alert.event.event_reference_time.tv_sec;
    _refTime_timeVal_tv_usec = alert.event.event_reference_time.tv_usec;
    _val = alert.val;
    _alertMsg = new char[ALERTMSG_LENGTH];
    memcpy(_alertMsg, alert.alertmsg, ALERTMSG_LENGTH);

    // if a packet was captured, get packet info
    if (!(alert.val & NOPACKET_STRUCT))
    {
        // packet information
        _packet_tv_sec = alert.pkth.ts.tv_sec; // seconds
        _packet_tv_usec = alert.pkth.ts.tv_usec; // microseconds
        _packetCaptureLen = alert.pkth.caplen; // packet capture length
        _packetLen = alert.pkth.pktlen; // packet "real" length, it may be bigger then size allocated in alert msg

        _dataLinkHdrOffset = alert.dataLinkHdr; // datalink header offset. e.g. ethernet
        _networkHdrOffset = alert.networkHdr; // network header offset. e.g. ip
        _transportHdrOffset = alert.transportHdr; // transport header offset e.g. tcp/udp/icmp
        _dataOffset = alert.data; // data length, offset
        _packet = new unsigned char[_packetCaptureLen];
        memcpy(_packet, alert.pkt, _packetCaptureLen); //

        createNetworkPacketInfo((Packet *) _packet, _packetLen, _packetCaptureLen);
        // TODO: need switch to save raw data
        // memcpy(&_alertPacket, buf, size);

        // OEM: 1/1/2008 verify test box works
        //        _packet = new unsigned char[SNAPLEN];
        //        memcpy(_packet, alert.pkt, SNAPLEN);        //
    }
}

// get initial info
// Currently NetworkPacket Info only supports IP,UDP,TCP,WIFI.
// Here is where we can extract other types of info,
// such as: Ethernet frames, ARP data, ...

void SnortEvent::createNetworkPacketInfo(Packet *p, unsigned long pLen, unsigned long pCapLen)
{
    // create network packet info, and set pacekt
    _netPacketInfo = new NetworkPacketInfo();
    _bHaveNetworkDataInfo = true;

    _netPacketInfo->setPacketCaptureTime(_packet_tv_sec, _packet_tv_usec);

    // For decoding Snort packet data, and Snort Alertpkts...
    //
    // In order to retrieve packet info, offsets are needed as
    // they were passed using the Alertptk. These offsets point
    // to the relative distance from the beginning of te Packet,
    // to the actual header data.
    //
    if (p->eh)
    {
        retrieveEthernetHeaderInfo(_dataLinkHdrOffset, *_netPacketInfo);
    }

    if (p->iph != NULL && p->pkt != NULL)
    {
        // ip
        retrieveIPHeaderInfo(_networkHdrOffset, *_netPacketInfo);

        _netPacketInfo->setPacket((u_int8_t *) p, pLen, pCapLen); // TODO: maybe an option in the future

        switch (_netPacketInfo->getProtocol())
        {
        case IPPROTO_ICMP:
            retrieveICMPHeaderInfo(_transportHdrOffset, *_netPacketInfo);
            break;
        case IPPROTO_UDP:
            retrieveUDPHeaderInfo(_transportHdrOffset, *_netPacketInfo);
            break;
        case IPPROTO_TCP:
            retrieveTCPHeaderInfo(_transportHdrOffset, *_netPacketInfo);
            break;
        case IPPROTO_IPV6:
            BasicExceptionMgr::instance()->log(LogLevel::Info, "NetworkPacketInfo::initialize - IPv6 protocol not supported.");
            break;
        default:
            BasicExceptionMgr::instance()->log(LogLevel::Info, "NetworkPacketInfo::initialize - protocol not supported %ld.", _netPacketInfo->getProtocol());
            break;
        }

        // set the rest of offsets in the packet
        _netPacketInfo->setPacketOffsets(_dataLinkHdrOffset,
                                         _networkHdrOffset,
                                         _transportHdrOffset,
                                         _dataOffset);

    }
}

///////////////////////////
//
//typedef struct _EtherHdr
//{
//    u_int8_t ether_dst[6];
//    u_int8_t ether_src[6];
//    u_int16_t ether_type;
//
//}         EtherHdr;

void SnortEvent::retrieveEthernetHeaderInfo(long offset,
                                            NetworkPacketInfo &netInfo)
{
    EthernetHeaderInfo etherInfo;
    EtherHdr *hdr = (EtherHdr *) (_packet + offset);

    etherInfo.initialize(hdr->ether_type,
                         *hdr->ether_src,
                         *hdr->ether_dst);

    netInfo.set(etherInfo);
}

/////////////
//
// From Snort project
//
//typedef struct _IPHdr/
//{
//    u_int8_t ip_verhl;      /* version & header length */
//    u_int8_t ip_tos;        /* type of service */
//    u_int16_t ip_len;       /* datagram length */
//    u_int16_t ip_id;        /* identification  */
//    u_int16_t ip_off;       /* fragment offset */
//    u_int8_t ip_ttl;        /* time to live field */
//    u_int8_t ip_proto;      /* datagram protocol */
//    u_int16_t ip_csum;      /* checksum */
//    struct in_addr ip_src;  /* source IP */
//    struct in_addr ip_dst;  /* dest IP */
//} IPHdr;

void SnortEvent::retrieveIPHeaderInfo(long offset,
                                      NetworkPacketInfo &netInfo)
{
    char strSrcAddr[INET6_ADDRSTRLEN];
    char strDstAddr[INET6_ADDRSTRLEN];
    string sSrcAddr;
    string sDstAddr;
    IPHeaderInfo ipInfo;
    IPHdr *hdr = (IPHdr *) (_packet + offset);

    // TODO: make IPv6
    inet_ntop(AF_INET, (&(hdr->ip_src)), strSrcAddr, INET6_ADDRSTRLEN);
    inet_ntop(AF_INET, (&(hdr->ip_src)), strDstAddr, INET6_ADDRSTRLEN);

    sSrcAddr = strSrcAddr;
    sDstAddr = strDstAddr;

    ipInfo.initialize(hdr->ip_verhl,
                      hdr->ip_tos,
                      hdr->ip_len,
                      hdr->ip_id,
                      hdr->ip_off,
                      hdr->ip_ttl,
                      hdr->ip_proto,
                      hdr->ip_csum,
                      sSrcAddr,
                      sDstAddr);

    netInfo.set(ipInfo);
}

/*
typedef struct _ICMPHdr
{
    u_int8_t type;
    u_int8_t code;
    u_int16_t csum;
    union
    {
        u_int8_t pptr;

        struct in_addr gwaddr;

        struct idseq
        {
            u_int16_t id;
            u_int16_t seq;
        } idseq;

        int sih_void;

        struct pmtu
        {
            u_int16_t ipm_void;
            u_int16_t nextmtu;
        } pmtu;

        struct rtradv
        {
            u_int8_t num_addrs;
            u_int8_t wpa;
            u_int16_t lifetime;
        } rtradv;
    } icmp_hun;

    union
    {
        struct ts
        {
            u_int32_t otime;
            u_int32_t rtime;
            u_int32_t ttime;
        } ts;

        struct ih_ip
        {
            IPHdr *ip;
        } ip;

        struct ra_addr
        {
            u_int32_t addr;
            u_int32_t preference;
        } radv;

        u_int32_t mask;

        char    data[1];

    } icmp_dun;
}        ICMPHdr;
 */

void SnortEvent::retrieveICMPHeaderInfo(long offset,
                                        NetworkPacketInfo &netInfo)
{
    ICMPInfo icmpInfo;
    ICMPHdr *hdr = (ICMPHdr *) (_packet + offset);

    icmpInfo.initialize(hdr->type,
                        hdr->code,
                        hdr->csum,
                        1,
                        hdr->s_icmp_id,
                        hdr->s_icmp_seq);

    netInfo.set(icmpInfo);
}

//
//
//typedef struct _UDPHdr
//{
//    u_int16_t uh_sport;
//    u_int16_t uh_dport;
//    u_int16_t uh_len;
//    u_int16_t uh_chk;
//
//}       UDPHdr;

void SnortEvent::retrieveUDPHeaderInfo(long offset,
                                       NetworkPacketInfo &netInfo)
{
    UDPHeaderInfo udpInfo;
    UDPHdr *hdr = (UDPHdr *) (_packet + offset);

    udpInfo.initialize(hdr->uh_sport,
                       hdr->uh_dport,
                       hdr->uh_len,
                       hdr->uh_chk);

    netInfo.set(udpInfo);
}

//
//typedef struct _TCPHdr
//{
//    u_int16_t th_sport;     /* source port */
//    u_int16_t th_dport;     /* destination port */
//   u_int32_t th_seq;       /* sequence number */
//    u_int32_t th_ack;       /* acknowledgement number */
//    u_int8_t th_offx2;      /* offset and reserved */
//    u_int8_t th_flags;
//    u_int16_t th_win;       /* window */
//    u_int16_t th_sum;       /* checksum */
//    u_int16_t th_urp;       /* urgent pointer */
//}       TCPHdr;
//

void SnortEvent::retrieveTCPHeaderInfo(long offset,
                                       NetworkPacketInfo &netInfo)
{
    TCPHeaderInfo tcpInfo;
    TCPHdr *hdr = (TCPHdr *) (_packet + offset);

    tcpInfo.initialize(hdr->th_sport,
                       hdr->th_dport,
                       hdr->th_seq,
                       hdr->th_ack,
                       hdr->th_offx2,
                       hdr->th_flags,
                       hdr->th_win,
                       hdr->th_sum,
                       hdr->th_urp);

    netInfo.set(tcpInfo);
}

void SnortEvent::setTestData()
{
    timeval curTime;

    unsigned char *rawTestData = NULL;

    _signatureGenerator = 1;
    _signatureID = 2;
    _classificationID = 3;
    _priority = 4;
    _eventID = 5;

    gettimeofday(&curTime, NULL);
    _refTime_timeVal_tv_sec = curTime.tv_sec;
    _refTime_timeVal_tv_usec = curTime.tv_usec;

    _alertMsg = new char[ALERTMSG_LENGTH];

    strcpy(_alertMsg, "Test Alert Message");

    // set some packet data
    _packetCaptureLen = SNAPLEN;
    _packet = new unsigned char[_packetCaptureLen];
    memset(_packet, 'x', _packetCaptureLen); //

    // create network packet info, and set pacekt
    _netPacketInfo = new NetworkPacketInfo();
    _bHaveNetworkDataInfo = true;
    _netPacketInfo->setTestData();

    if (_rawTestDataSize > 0)
    {
        rawTestData = new unsigned char[_rawTestDataSize];
        memset(rawTestData, 'a', _rawTestDataSize);
        setRawEvent(rawTestData, _rawTestDataSize);
        delete rawTestData;
    }
}

void SnortEvent::serialize(Serializer &s)
{
    unsigned long msgLen = ALERTMSG_LENGTH;
    unsigned long packetLen = this->_packetCaptureLen;

    // event information
    s.transfer(this->_signatureGenerator);
    s.transfer(this->_signatureID);
    s.transfer(this->_signatureRevision);
    s.transfer(this->_classificationID);
    s.transfer(this->_priority);
    s.transfer(this->_eventID);
    s.transfer(this->_eventReferenceID);
    s.transfer(this->_refTime_timeVal_tv_sec);
    s.transfer(this->_refTime_timeVal_tv_usec);
    s.transfer(this->_val);
    s.transfer(this->_alertMsg, msgLen);

    // packet information
    s.transfer(this->_packet_tv_sec);
    s.transfer(this->_packet_tv_usec);
    s.transfer(this->_packetCaptureLen);
    s.transfer(this->_packetLen);
    s.transfer(this->_dataLinkHdrOffset);
    s.transfer(this->_networkHdrOffset);
    s.transfer(this->_transportHdrOffset);
    s.transfer(this->_dataOffset);
    s.transfer(this->_packet, packetLen); // TODO: serialize with packetlen, CaptureLen, save space

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

string SnortEvent::getEventInfo()
{
    ostringstream eventInfo;

    eventInfo << "Snort Event->" << _signatureGenerator;
    eventInfo << ":" << _signatureID;
    eventInfo << ":" << _classificationID;
    eventInfo << ":" << _priority;
    eventInfo << ":" << _eventReferenceID;
    eventInfo << ":" << _refTime_timeVal_tv_sec;
    eventInfo << ":" << _refTime_timeVal_tv_usec;
    eventInfo << ":" << _val;

    if (strlen(_alertMsg) > 0)
    {
        eventInfo << ":(" << _alertMsg << ")";
    }

    // get data info
    if (_bHaveNetworkDataInfo == true)
    {
        string networkDataInfo;
        _netPacketInfo->getInfo(networkDataInfo);
        eventInfo << ":<<";
        eventInfo << networkDataInfo;
        eventInfo << ">>";
    }

    return eventInfo.str();
}
