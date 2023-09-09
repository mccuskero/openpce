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
/*
 * \class SnortEvent
 * \brief This is a basic Snort Event/Alert created from a Snort network sensor
 */

#ifndef __SnortEvent_h
#define __SnortEvent_h

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <string>

#include <pcff/event/Event.h>

// TODO: make depend on snort install src dir, take out headers
#include <pcsec/network/snort/decode.h>
#include <pcsec/network/snort/snortStructs.h>

#include <pcsec/network/NetworkPacketInfo.h>

namespace pcsec
{

class SnortEvent : public pcff::Event
{
public:
    SnortEvent();
    virtual ~SnortEvent();

private:
    void initMembers();

public:
    virtual void initialize();
    virtual void initialize(int size, char *buf);
    virtual void initialize(Alertpkt &alert);

public:
    void createNetworkPacketInfo(Packet *p, unsigned long pLen, unsigned long pCapLen);
    void retrieveEthernetHeaderInfo(long offset, NetworkPacketInfo &info);
    void retrieveIPHeaderInfo(long offset, NetworkPacketInfo &info);
    void retrieveICMPHeaderInfo(long offset, NetworkPacketInfo &info);
    void retrieveUDPHeaderInfo(long offset, NetworkPacketInfo &info);
    void retrieveTCPHeaderInfo(long offset, NetworkPacketInfo &info);

    NetworkPacketInfo *getNetworkDataInfo()
    {
        return _netPacketInfo;
    }

public:

    long getSigGen()
    {
        return _signatureGenerator;
    }

    long getSigID()
    {
        return _signatureID;
    }

    long getSigRev()
    {
        return _signatureRevision;
    }

    long getClassID()
    {
        return _classificationID;
    }

    long getPriority()
    {
        return _priority;
    }

    long getEventID()
    {
        return _eventID;
    }

    long getRefEventID()
    {
        return _eventReferenceID;
    }

    long getRetTimeSec()
    {
        return _refTime_timeVal_tv_sec;
    }

    long getRetTimeUsec()
    {
        return _refTime_timeVal_tv_usec;
    }

    char *getAlertMsg()
    {
        return _alertMsg;
    }

public:
    void setTestData();

    void setRawTestDataSize(unsigned long dataSize)
    {
        _rawTestDataSize = dataSize;
    }

public:
    std::string getEventInfo();

    // serialize members
public:
    virtual void serialize(pcff::Serializer &s);

private:
    // event information
    // event times, should come after packet time, if captured
    long _signatureGenerator;
    long _signatureID;
    long _signatureRevision;
    long _classificationID;
    long _priority;
    long _eventID;
    long _eventReferenceID;
    long _refTime_timeVal_tv_sec;
    long _refTime_timeVal_tv_usec;
    char *_alertMsg; // variable..

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
    Alertpkt _snortAlert; // needs serializeation function
    bool _bHaveNetworkDataInfo;
    NetworkPacketInfo *_netPacketInfo;
};

};

#endif
