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
 * \class SilkRecord
 * \brief This is a class holds all the information in a rwrec. SilkRecords can
 * be serialized and passed over streams.
 */

#ifndef __SilkRecord_h
#define __SilkRecord_h

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <string>

extern "C"
{
#include <silk/silk.h>
#include <silk/rwpack.h>
#include <silk/dynlib.h>
}

#include <pcff/socket/INETAddress.h>
#include <pcff/event/Event.h>

namespace pcsec
{

class SilkRecord : public pcff::Event
{
public:
	SilkRecord();
	virtual ~SilkRecord();

private:
	void initMembers();

public:
	virtual void initialize();
	virtual void initialize(rwRec &rwrec);

public:
	unsigned int getRecIsIPv6()     { return _recIsIPv6; }
	const void  *getSrcIP()			{ return (void *)_srcIP; }
	std::string  getSrcIPAsString();
	const void  *getDstIP()			{ return (void *)_dstIP; }
	std::string  getDstIPAsString();
	const void  *getNextHopIP()    	{ return (void *)_nextHopIP; }
	std::string  getNextHopIPAsString();
	unsigned int getSrcPort()		{ return _srcPort; }
	unsigned int getDstPort()		{ return _dstPort; }
	unsigned char getIcmpType()	 	{ return _icmpType; }
	unsigned char getIcmpCode()	 	{ return _icmpCode; }
	unsigned int  getProtocol()		{ return _protocol; }
	unsigned int  getNumPackets()	{ return _numPkts; }
	unsigned int  getNumBytes()	    { return _numBytes; }
	unsigned char getFlags()		{ return _flags; }
	unsigned char getInitialFlags() { return _initialFlags; }
	unsigned char getRestFlags()    { return _restFlags; }
	unsigned char getTcpState()    	{ return _tcpState; }
	unsigned int  getApplication()  { return _application; }
	long long getStartTime()			{ return _startTime; }
	long long getEndTime()			{ return _endTime; }
	long getElapsedTimeSec()	{ return _elapsedTimeSec; }
	long getElapsedTime()		{ return _elapsedTime; }
	long getSensorID()		{ return _sensorID; }
	long getInput()			{ return _input; }
	long getOutput()		{ return _output; }
	long getClass()			{ return _class; }
	long getType()			{ return _type; }

public:
	void setTestData();

public:
     virtual std::string getEventInfo();

// serialize members
public:
	virtual void  serialize(pcff::Serializer &s);

private:
	void swapBytesFromSilkAddress(u_int32_t &x);

private:
//	long 	_ipFlags;
	unsigned int    _recIsIPv6;      // recored is IPv6
	unsigned char	_srcIP[pcff::INETAddress::MaxAddressLength];          // source IP
	unsigned char 	_dstIP[pcff::INETAddress::MaxAddressLength];          // destination IP
	unsigned char	_nextHopIP[pcff::INETAddress::MaxAddressLength];      // router next hop
	unsigned int	_srcPort;        // soure port
	unsigned int  	_dstPort;        // destination port
	unsigned char	_icmpType;   //
	unsigned char	_icmpCode;   //
	unsigned int	_protocol;       //
	unsigned int	_numPkts;        // number of packets in flow
	unsigned int	_numBytes;       // number of total bytes in flow
	unsigned char   _flags;          // OR of all flags in (Netflow flags)
	unsigned char   _initialFlags;   // TCP flags for inital packet
	unsigned char   _restFlags;      // TCP on non-initial packet
	unsigned char    _tcpState;       // TCP state machine
	unsigned int	 _application;
	long    _elapsedTimeSec;    // flow elapsed time
	long    _elapsedTime;    // flow elapsed time
	long long    _startTime;      // flow start time
	long long    _endTime;        // flow end time
	long    _sensorID;       // network flow sensor ID in ipfix record
	long    _input;          // router incoming SNMP interface
	long    _output;         // router outgoing SNMP interface
	long    _class;          // flow class
	long    _type;           // flow type
};

};

#endif
