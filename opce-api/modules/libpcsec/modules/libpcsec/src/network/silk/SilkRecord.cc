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
 * \class SilkRecord
 * \brief This is a basic Silk Record created by the silk toolset
 *
 *  This SilkRecord class is linked to the rwRec struct delivered with silk Tools.
 *
 */

#include <iostream>
#include <sstream>

#include <pcff/BasicExceptionMgr.h>
#include <pcff/TimeStamp.h>
#include <pcff/serialize/Serializer.h>

#include <pcsec/network/EthernetHeaderInfo.h>
#include <pcsec/network/WifiHeaderInfo.h>
#include <pcsec/network/IPHeaderInfo.h>
#include <pcsec/network/ICMPInfo.h>
#include <pcsec/network/UDPHeaderInfo.h>
#include <pcsec/network/TCPHeaderInfo.h>

#include <pcsec/network/SilkRecord.h>

using namespace std;
using namespace pcff;
using namespace pcsec;

SilkRecord::SilkRecord()
{
	initMembers();
}

SilkRecord::~SilkRecord()
{
	initMembers();
}

void SilkRecord::initMembers()
{
	_recIsIPv6 = 0;
	memset(_srcIP, '0', INETAddress::MaxAddressLength);	      // source IP
	memset(_dstIP, '0', INETAddress::MaxAddressLength);	      // destination IP
	memset(_nextHopIP, '0', INETAddress::MaxAddressLength);	  // router next hop
	_srcPort			= 0;        // soure port
	_dstPort			= 0;        // destination port
	_icmpType			= 0;   //
	_icmpCode			= 0;   //
	_protocol			= 0;       //
	_numPkts			= 0;        // number of packets in flow
	_numBytes			= 0;       // number of total bytes in flow
	_flags				= 0;          // OR of all flags in (Netflow flags)
	_initialFlags		= 0;   // TCP flags for inital packet
	_restFlags			= 0;      // TCP on non-initial packet
	_tcpState			= 0;       // TCP state machine
	_application		= 0;
	_elapsedTime		= 0;    // flow elapsed time
	_elapsedTimeSec		= 0;    // flow elapsed time
	_startTime			= 0;      // flow start time
	_endTime			= 0;        // flow end time
	_sensorID		= 0;       // sensor ID e.g. router ID
	_input			= 0;          // router incoming SNMP interface
	_output			= 0;         // router outgoing SNMP interface
	_type			= 0;           // flow type
}

void SilkRecord::initialize()
{
}

//
// Silk returns network byte order for IP addresses.
// This must be return to host order before managing
// them in the INETAddress class.
//
// TODO: verify that swapping is not machine dependent
//
void SilkRecord::swapBytesFromSilkAddress(u_int32_t &x)
{
    x = (x>>24) |
        ((x<<8) & 0x00FF0000) |
        ((x>>8) & 0x0000FF00) |
        (x<<24);
}

void SilkRecord::initialize(rwRec &rwrec)
{
	char buffer[128];
    uint32_t ip_flags = 0;
    skipaddr_t ip;
    in_addr_t my_addr;
    uint32_t my_addr2 = 0;

	if (BasicExceptionMgr::instance()->getLogLevel() >= LogLevel::Info)
	{
		BasicExceptionMgr::instance()->log(LogLevel::Debug, "SilkRecord::initialize - Flow Record: ");
	}

	_recIsIPv6 = rwRecIsIPv6(&rwrec);

	// IPs are stored in network byte order
	rwRecMemGetSIP(&rwrec,  _srcIP);
	if (Serializer::bigEndian() == false)
	{
		swapBytesFromSilkAddress((u_int32_t&)_srcIP);
	}

	rwRecMemGetDIP(&rwrec,  _dstIP);

	if (Serializer::bigEndian() == false)
	{
		swapBytesFromSilkAddress((u_int32_t&)_dstIP);
	}

	rwRecMemGetNhIP(&rwrec, _nextHopIP);

	if (Serializer::bigEndian() == false)
	{
		swapBytesFromSilkAddress((u_int32_t&)_nextHopIP);
	}

    if (rwRecGetProto(&rwrec) == 1)
    {
    	_icmpCode = rwRecGetIcmpCode(&rwrec);
    	_icmpType = rwRecGetIcmpType(&rwrec);
    }
    else
    {
    	_srcPort = (unsigned int)rwRecGetSPort(&rwrec);
    	_dstPort = (unsigned int)rwRecGetDPort(&rwrec);
    }

    _protocol 			= (unsigned int)rwRecGetProto(&rwrec);
    _numPkts  			= rwRecGetPkts(&rwrec);
    _numBytes 			= rwRecGetBytes(&rwrec);
    _flags 				= rwRecGetFlags(&rwrec);
    _initialFlags 		= rwRecGetInitFlags(&rwrec);
    _restFlags 			= rwRecGetRestFlags(&rwrec);
    _tcpState 			= rwRecGetTcpState(&rwrec);
    _application 		= rwRecGetApplication(&rwrec);
    _elapsedTimeSec 	= rwRecGetElapsedSeconds(&rwrec);
    _elapsedTime 		= rwRecGetElapsed(&rwrec);
    _startTime			= rwRecGetStartTime(&rwrec);
    _endTime			= rwRecGetEndTime(&rwrec);
    _sensorID			= (unsigned int)rwRecGetSensor(&rwrec);
    _input				= (unsigned int)rwRecGetInput(&rwrec);
    _output				= (unsigned int)rwRecGetOutput(&rwrec);
    _type				= rwRecGetFlowType(&rwrec);
}

void SilkRecord::setTestData()
{
	struct timeval currtime;
	long recvTime_sec = 0;

	gettimeofday(&currtime, NULL);
	recvTime_sec  = currtime.tv_sec;


	INETAddress srcIPAddr;
	INETAddress dstIPAddr;
	INETAddress nextHopIPAddr;

	srcIPAddr.setAddressFromIPString(INETAddress::createInetAddrIPv4() );
	dstIPAddr.setAddressFromIPString(INETAddress::createInetAddrIPv4() );
	nextHopIPAddr.setAddressFromIPString(INETAddress::createInetAddrIPv4() );

	_recIsIPv6 = 0;
	memcpy(_srcIP,     srcIPAddr.getInAddr(), INETAddress::MaxAddressLength);	      // source IP
	memcpy(_dstIP,     dstIPAddr.getInAddr(), INETAddress::MaxAddressLength);	      // destination IP
	memcpy(_nextHopIP, nextHopIPAddr.getInAddr(), INETAddress::MaxAddressLength);	  // router next hop
	_srcPort			= 4;        // soure port
	_dstPort			= 5;        // destination port
	_icmpType			= 6;   //
	_icmpCode			= 7;   //
	_protocol			= 8;       //
	_numPkts			= 9;        // number of packets in flow
	_numBytes			= 0;       // number of total bytes in flow
	_flags				= 1;          // OR of all flags in (Netflow flags)
	_initialFlags		= 2;   // TCP flags for inital packet
	_restFlags			= 3;      // TCP on non-initial packet
	_tcpState			= 4;       // TCP state machine
	_application		= 5;
	_startTime			= currtime.tv_sec;      // flow start time
	_endTime			= currtime.tv_sec+1000;        // flow end time
	_elapsedTime		= 1000;    // flow elapsed time
	_elapsedTimeSec		= 1000;    // flow elapsed time
	_sensorID		= 1;       // sensor ID
	_input			= 1;          // router incoming SNMP interface
	_output			= 2;         // router outgoing SNMP interface
	_type			= 3;           // flow type
}

void SilkRecord::serialize(Serializer &s)
{
	unsigned long addrSize = EthernetHeaderInfo::AddressSize;

	s.transfer(this->_recIsIPv6);
	s.transfer(this->_srcIP[0], addrSize);
	s.transfer(this->_dstIP[0], addrSize);
	s.transfer(this->_nextHopIP[0], addrSize);

	s.transfer(this->_srcPort);
	s.transfer(this->_icmpType);
	s.transfer(this->_icmpCode);
	s.transfer(this->_protocol);
	s.transfer(this->_numPkts);
	s.transfer(this->_numBytes);
	s.transfer(this->_flags);
	s.transfer(this->_initialFlags);
	s.transfer(this->_restFlags);
	s.transfer(this->_tcpState);
	s.transfer(this->_application);
	s.transfer(this->_startTime);
	s.transfer(this->_endTime);
	s.transfer(this->_elapsedTime);
	s.transfer(this->_elapsedTimeSec);
	s.transfer(this->_sensorID);
	s.transfer(this->_input);
	s.transfer(this->_output);
	s.transfer(this->_type);

	Event::serialize(s);
}


string SilkRecord::getSrcIPAsString()
{
	INETAddress inetAddr( (_recIsIPv6 == 1) ? AF_INET6 : AF_INET);

	inetAddr.setAddress(_srcIP);

	return inetAddr.getAddressAsStr();
}

string SilkRecord::getDstIPAsString()
{
	INETAddress inetAddr( (_recIsIPv6 == 1) ? AF_INET6 : AF_INET);
	inetAddr.setAddress(_dstIP);

	return inetAddr.getAddressAsStr();
}

string SilkRecord::getNextHopIPAsString()
{
	INETAddress inetAddr( (_recIsIPv6 == 1) ? AF_INET6 : AF_INET);
	inetAddr.setAddress(_nextHopIP);

	return inetAddr.getAddressAsStr();
}


string SilkRecord::getEventInfo()
{
	ostringstream eventInfo;

	INETAddress srcIP( (_recIsIPv6 == 1) ? AF_INET6 : AF_INET);
	INETAddress dstIP( (_recIsIPv6 == 1) ? AF_INET6 : AF_INET);
	INETAddress nhIP(  (_recIsIPv6 == 1) ? AF_INET6 : AF_INET);

	srcIP.setAddress(_srcIP);
	dstIP.setAddress(_dstIP);
	nhIP.setAddress(_nextHopIP);

	TimeStamp startTime(  _startTime );
	TimeStamp endTime(  _endTime );

	eventInfo <<        "silkRecord:->("       << srcIP.getAddressAsStr() << "):(" \
            			                       << dstIP.getAddressAsStr()    << "):(" \
	                                           << nhIP.getAddressAsStr()     << "):<" \
	                                           << _srcPort			      << ">:<" \
	                                       	   << _dstPort 			      << ">:" \
	                                       	   << (int)_icmpType 			  << ":" \
	                                       	<< 	(int)_icmpCode			      << ":{" \
	                                       	<< _protocol			      << "}:[" \
	                                       	<< _numPkts			          << "]:[" \
	                                       	<< _numBytes			      << "]:" \
	                                       	<< (int)_flags			          << ":" \
	                                       	<< (int)_initialFlags			  << ":" \
	                                       	<< (int)_restFlags			      << ":" \
	                                       	<< (int)_tcpState			      << ":" \
	                                       	<< _application			      << ":" \
	                                       	<< _elapsedTime			      << ":" \
	                                       	<< _elapsedTimeSec			  << ":|" \
	                                       	<< startTime.get("YYYY-MM-DD HH:NN:SS")			      << "|:|" \
	                                       	<< endTime.get("YYYY-MM-DD HH:NN:SS")			      << "|:" \
	                                       	<< _sensorID			      << ":" \
	                                       	<< _input			      << ":" \
	                                       	<< _output			      << ":" \
	                                       	<< _type			      << ":";


	return eventInfo.str();
}
