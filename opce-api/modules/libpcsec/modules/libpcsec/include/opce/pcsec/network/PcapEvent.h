/** \class PcapEvent
 * \brief
 * \author Owen McCusker
 * \version current version
 * \date  04/17/2009
 *
 * \bug list of potential bugs (one per line)
 * \warning list of warnings (one per line)
 * \todo add here any comment about possible improvements
 *
 */

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


#ifndef _PCAPEVENT_H
#define	_PCAPEVENT_H

#include <pcap.h>
#include <string>

#include <pcff/event/Event.h>

#include <pcsec/network/pcap/pcap_structs.h>

#include <pcsec/network/NetworkPacketInfo.h>

namespace pcsec
{

class PcapEvent : public pcff::Event
{
public:
    PcapEvent();
    PcapEvent(const PcapEvent& orig);
    virtual ~PcapEvent();

private:
    void initMembers();

public:

    enum eDataLinkLayerConstants
    {
        EthernetHeaderSize = 14,
        SlipHeaderSize = 4,
        PPPHeaderSize = 16,
        RawHeaderSize = 0,
        LoopHeaderSize = 4
    };

public:
    void initialize(const struct pcap_pkthdr &header, const u_char *packet, int &linkOffSize);
    void createPacketInfo(const u_char *packet, u_int32_t packetLen, u_int32_t packetCaptureLen);

private:
    void retrieveEthernetHeaderInfo(struct SniffEthernet *hdr, NetworkPacketInfo &netInfo);
    void retrieveIPHeaderInfo(struct SniffIP *hdr, NetworkPacketInfo &netInfo);
    void retrieveICMPHeaderInfo(struct SniffICMP *hdr, NetworkPacketInfo &netInfo);
    void retrieveUDPHeaderInfo(struct SniffUDP* hdr, NetworkPacketInfo &netInfo);
    void retrieveTCPHeaderInfo(struct SniffTCP* hdr, NetworkPacketInfo &netInfo);

public:
    std::string getEventInfo();
    virtual void setTestData();

// serialize members
public:
	virtual void  serialize(pcff::Serializer &s);

protected:
    long _refTime_timeVal_tv_sec;
    long _refTime_timeVal_tv_usec;

    // packet information, offset are into _packet,
    // _val bitmap tells us if packet data captured
    long _val; // which fields are valid. (NULL is also valid)
    long _packet_tv_sec; // seconds
    long _packet_tv_usec; // microseconds
    long _packetCaptureLen; // packet capture length
    long _packetLen; // packet "real" length
    long _dataLinkHdrOffset; // datalink header offset. e.g. ethernet
    long _networkHdrOffset; // network header offset. e.g. ip
    long _transportHdrOffset; // transport header offset e.g. tcp/udp/icmp
    long _dataOffset; // data offset
    unsigned char *_packet; // TODO: make variable length ???
    unsigned long _rawTestDataSize;
    bool _bHaveNetworkDataInfo;
    NetworkPacketInfo *_netPacketInfo;
};

};

#endif	/* _PCAPEVENT_H */

