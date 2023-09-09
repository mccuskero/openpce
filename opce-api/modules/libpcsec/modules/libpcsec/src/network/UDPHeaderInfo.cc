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

#include <netinet/in.h>

#include <sstream>
#include <string>

#include <pcsec/network/UDPHeaderInfo.h>

using namespace std;
using namespace pcff;
using namespace pcsec;

UDPHeaderInfo::UDPHeaderInfo()
{
	initMembers();
}

UDPHeaderInfo::UDPHeaderInfo(const UDPHeaderInfo &in)
{
	initMembers();

    _sPort = in._sPort;
    _dPort = in._dPort;
    _udpLen = in._udpLen;
    _checksum = in._checksum;
}


UDPHeaderInfo::~UDPHeaderInfo()
{
}

void UDPHeaderInfo::initMembers()
{
    _sPort = 0;
    _dPort = 0;
    _udpLen = 0;
    _checksum = 0;
}

void UDPHeaderInfo::initialize( u_int16_t sPort,
		u_int16_t dPort,
		u_int16_t len,
		u_int16_t checksum)
{
    _sPort = sPort;
    _dPort = dPort;
    _udpLen = len;
    _checksum = checksum;
}

void UDPHeaderInfo::serialize(pcff::Serializer &s)
{
	s.transfer(this->_sPort);
	s.transfer(this->_dPort);
	s.transfer(this->_udpLen);
	s.transfer(this->_checksum);
}

void UDPHeaderInfo::getInfo(string &sInfo)
{
	ostringstream eventInfo;

    eventInfo  << "UDP: (";
	eventInfo << "sport " << ntohs(_sPort) << " ,";
	eventInfo << "dport " << ntohs(_dPort) << " ,";
	eventInfo << "len " << ntohs(_udpLen) << " ,";
	eventInfo << "checksum " << ntohs(_checksum) << ")";

	sInfo = eventInfo.str();
}
