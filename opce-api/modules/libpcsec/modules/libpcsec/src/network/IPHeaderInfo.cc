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

#include <string>
#include <sstream>

#include <netinet/in.h>
#include <arpa/inet.h>

#include <pcsec/network/IPHeaderInfo.h>

using namespace std;
using namespace pcff;
using namespace pcsec;

IPHeaderInfo::IPHeaderInfo()
{
	initMembers();
}

IPHeaderInfo::IPHeaderInfo(const IPHeaderInfo &in)
{
	initMembers();

    _ipVerHeaderLen = in._ipVerHeaderLen;
    _ipTOS          = in._ipTOS;
    _ipLen          = in._ipLen;
    _ipID           = in._ipID;
    _ipFlagFragOff  = in._ipFlagFragOff;
    _ipTTL          = in._ipTTL;
    _ipProto        = in._ipProto;
    _ipCksum        = in._ipCksum;
    _ipSrc          = in._ipSrc;
    _ipDst          = in._ipDst;
}

IPHeaderInfo::~IPHeaderInfo()
{
}

void IPHeaderInfo::initMembers()
{
    _ipVerHeaderLen = 0;
    _ipTOS          = 0;
    _ipLen          = 0;
    _ipID           = 0;
    _ipFlagFragOff  = 0;
    _ipTTL          = 0;
    _ipProto        = 0;
    _ipCksum        = 0;
}

void IPHeaderInfo::initialize(const u_int8_t 	ipVerhl,
		const u_int8_t 	ipTOS,
		const u_int16_t 	ipLen,
		const u_int16_t 	ipID,
		const u_int16_t 	ipOff,
		const u_int8_t 	ipTTL,
		const u_int8_t 	ipProto,
		const u_int16_t 	ipCksum,
		const string &ipSrc,
		const string &ipDst)
{
    _ipVerHeaderLen = ipVerhl;
    _ipTOS          = ipTOS;
    _ipLen          = ipLen;
    _ipID           = ipID;
    _ipFlagFragOff  = ipOff;
    _ipTTL          = ipTTL;
    _ipProto        = ipProto;
    _ipCksum        = ipCksum;
    _ipSrc.setAddressFromIPString(ipSrc);
    _ipDst.setAddressFromIPString(ipDst);
}

//
// TODO: thread safety issue!!
string IPHeaderInfo::getSrcIPAsString()
{
	string sSrc  = _ipSrc.getAddressAsStr();
	return sSrc;
}

string IPHeaderInfo::getDstIPAsString()
{
	string sDst  = _ipDst.getAddressAsStr();
	return sDst;
}

void IPHeaderInfo::serialize(pcff::Serializer &s)
{
	s.transfer(this->_ipVerHeaderLen);
	s.transfer(this->_ipTOS);
	s.transfer(this->_ipLen);
	s.transfer(this->_ipID);
	s.transfer(this->_ipFlagFragOff);
	s.transfer(this->_ipTTL);
	s.transfer(this->_ipProto);
	s.transfer(this->_ipCksum);
	_ipSrc.serialize(s);
	_ipDst.serialize(s);
}

void IPHeaderInfo::getInfo(string &sInfo)
{
	ostringstream eventInfo;

	eventInfo << "IP: (";
    eventInfo << "vhdrlen " << hex << (int)_ipVerHeaderLen << ", ";
    eventInfo << "tos " << hex << (int)_ipTOS << ", ";
    eventInfo << "ttl " << hex << (int)_ipTTL << ", ";
    eventInfo << "id " << hex << ntohs(_ipID) << ", ";
    eventInfo << "offset " << hex << ntohs(_ipFlagFragOff) << ", ";
    eventInfo << "proto " << hex << (int)_ipProto << ", ";
    eventInfo << "length " << ntohs(_ipLen) << ", ";
    eventInfo << "checksum " << hex << ntohs(_ipCksum) << ")";
	eventInfo << ": src(" << getSrcIPAsString();
	eventInfo << "): dst(" << getDstIPAsString() << ")";

	sInfo = eventInfo.str();
}
