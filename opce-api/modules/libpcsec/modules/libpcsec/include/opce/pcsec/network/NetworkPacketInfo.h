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

//
// This class was initially developed to support passing
// on Network packet information from a snort sensor
// (review AlertPkt in snort 2.8.0 project, alert.h)
//
//
// This class should be refactored to include other types
// of sensors in the future
//

/*
 * Protocols (RFC 1700)
#define	IPPROTO_IP		0		// dummy for IP
#ifndef _POSIX_C_SOURCE
#define	IPPROTO_HOPOPTS	0		// IP6 hop-by-hop options
#define	IPPROTO_ICMP		1		/* control message protocol
#define	IPPROTO_IGMP		2		/* group mgmt protocol
#define	IPPROTO_GGP		3		// gateway^2 (deprecated)
#define IPPROTO_IPV4		4 		// IPv4 encapsulation
#define IPPROTO_IPIP		IPPROTO_IPV4	// for compatibility
#define	IPPROTO_TCP		6		// tcp
#define	IPPROTO_UDP		17		// user datagram protocol
#define 	IPPROTO_IPV6		41		// IP6 header
8*/

#ifndef NETWORKPACKETINFO_H_
#define NETWORKPACKETINFO_H_

#include <string>
#include <sys/types.h>

#include <pcff/serialize/Serializeable.h>

namespace pcsec
{

class EthernetHeaderInfo;
class WifiHeaderInfo;
class IPHeaderInfo;
class ICMPInfo;
class UDPHeaderInfo;
class TCPHeaderInfo;

class NetworkPacketInfo : public pcff::Serializeable
{

    typedef struct _PacketOptions
    {
        u_int32_t len;
        u_int8_t code;
        u_int8_t offset; // Offset to packet pointer
    } PacketOptions;

public:
    NetworkPacketInfo();
    NetworkPacketInfo(const NetworkPacketInfo &in);
    virtual ~NetworkPacketInfo();

private:
    void initMembers();

public:
    bool initialize(int protocol, long tv_sec, long tv_usec);
    bool initialize(int protocol, long tv_sec, long tv_usec, long gmtOffset);

public:
    virtual void serialize(pcff::Serializer &s);
    virtual void getInfo(std::string &sInfo);

public:
    static void networkIPToString(struct in_addr&, std::string &sIP);

public:
    bool isSet(int mask);
    void set(EthernetHeaderInfo &info);
    void set(WifiHeaderInfo &info);
    void set(IPHeaderInfo &info);
    void set(ICMPInfo &info);
    void set(UDPHeaderInfo &info);
    void set(TCPHeaderInfo &info);

public:
    void setTestData();

public:
    void setPacketOffsets(long dataLnkOffset,
                          long networkLnkOffset,
                          long transportLnkOffset,
                          long dataOffset);

    void setPacket(u_int8_t *packet, u_int32_t pLen, u_int32_t pCapLen);

    bool setProtocol(int protocol);

    int getProtocol()
    {
        return _protocol;
    }

    void setPacketCaptureTime(long tv_sec,
                              long tv_usec);

    void setPacketCaptureTime(long tv_sec,
                              long tv_usec,
                              long gmtOffset);
public:
    u_int8_t getHeaderLen();
    u_int8_t getIPVersion();
    u_int8_t getTOS();
    u_int16_t getLen();
    u_int16_t getID();
    u_int16_t getIPFlagsFragOffset();
    u_int8_t getTTL();
    u_int8_t getProto();
    u_int16_t getChecksum();
    void getIPSrc(std::string &sAddr);
    void getIPDst(std::string &sAddr);
    u_int16_t getSrcPort();
    u_int16_t getDstPort();
    u_int8_t getTCPFlags();
    u_int8_t *getPacket();
    u_int32_t getPacketCaptureLen();
    u_int32_t getPacketRealLen();

    //
    // TODO: use proto during serialization
    //

    enum eNetworkProtocols
    {
        NotSet = 0,
        ARP,
        IP,
        ICMP,
        UDP,
        TCP
    };

    enum eProtocolBitmask
    {
        eEthernetHeaderInfo = 0x1,
        eArpHeaderInfo = 0x10,
        eWifiHeaderInfo = 0x100,
        eICMPHeaderInfo = 0x1000,
        eICMPInfo = 0x10000,
        eIPHeaderInfo = 0x100000,
        eUDPHeaderInfo = 0x1000000,
        eTCPHeaderInfo = 0x10000000
    };

    enum Constants
    {
        ProtoNotInitialized = -1,
        GMTOffsetNotSet = -1
    };

protected:
    u_int16_t _protocol;
    u_int32_t _packetDataMask; // indicates what members are used
    EthernetHeaderInfo *_ethernetHeaderInfo; // TCP/IP/Ethernet/ARP headers
    WifiHeaderInfo *_wifiHeader; // wireless LAN header
    //    EthernetARP     *_ethernetARP;
    IPHeaderInfo *_ipHeaderInfo;
    //	PacketOptions 	_ipOptions[40];
    //	u_int32_t 		_ipOptionCount;
    ICMPInfo *_icmpInfo;
    //	ICMPHeaderInfo  *_icmpHeaderInfo;
    UDPHeaderInfo *_updHeaderInfo;
    TCPHeaderInfo *_tcpHeaderInfo;
    //	PacketOptions 	tcpOptions[40];
    //	u_int32_t 		tcpOptionCount;
    u_int8_t _fragmentFlag;
    u_int16_t _fragmentOffset;
    u_int32_t _packet_tv_sec; // seconds
    u_int32_t _packet_tv_usec; // microseconds
    u_int32_t _packet_gmtOffset; // microseconds
    u_int32_t _dataLinkHdrOffset; // datalink header offset. e.g. ethernet
    u_int32_t _networkHdrOffset; // network header offset. e.g. ip
    u_int32_t _transportHdrOffset; // transport header offset e.g. tcp/udp/icmp
    u_int32_t _dataOffset; // data length, offset
    u_int32_t _packetCaptureLen;
    u_int32_t _packetLen;
    u_int8_t *_packet;
};

};

#endif /*NETWORKPACKETINFO_H_*/
