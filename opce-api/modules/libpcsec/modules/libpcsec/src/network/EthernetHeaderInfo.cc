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


#include <sstream>

#include <pcsec/network/EthernetHeaderInfo.h>

using namespace std;
using namespace pcff;
using namespace pcsec;

EthernetHeaderInfo::EthernetHeaderInfo()
{
	initMembers();
}

EthernetHeaderInfo::EthernetHeaderInfo(const EthernetHeaderInfo &in)
{
	initMembers();

	_etherType = in._etherType;
	memcpy(_etherSrcAddr, in._etherSrcAddr, EthernetHeaderInfo::AddressSize);
	memcpy(_etherDstAddr, in._etherDstAddr, EthernetHeaderInfo::AddressSize);
}

EthernetHeaderInfo::~EthernetHeaderInfo()
{
}

void EthernetHeaderInfo::initMembers()
{
	_etherType = 0;
	memset(_etherSrcAddr, '0', EthernetHeaderInfo::AddressSize);
	memset(_etherDstAddr, '0', EthernetHeaderInfo::AddressSize);
}

void EthernetHeaderInfo::initialize(u_int16_t etherType,
		  u_int8_t  &srcAddr,
		  u_int8_t  &dstAddr)
{
	_etherType = etherType;
	memcpy(&_etherSrcAddr, &srcAddr, EthernetHeaderInfo::AddressSize);
	memcpy(&_etherDstAddr, &dstAddr, EthernetHeaderInfo::AddressSize);
}

void EthernetHeaderInfo::serialize(pcff::Serializer &s)
{
	unsigned long addrSize = EthernetHeaderInfo::AddressSize;

	s.transfer(this->_etherType);
	s.transfer(_etherSrcAddr[0], addrSize);
	s.transfer(_etherDstAddr[0], addrSize);
}


void EthernetHeaderInfo::getInfo(string &sInfo)
{
	ostringstream eventInfo;

	eventInfo << "EH: " << "type (" << _etherType << ")";
	eventInfo	<< ": src(" \
                << hex << (int)_etherSrcAddr[0] << ":"\
				<< hex << (int)_etherSrcAddr[1] << ":"\
				<< hex << (int)_etherSrcAddr[2] << ":"\
				<< hex << (int)_etherSrcAddr[3] << ":"\
				<< hex << (int)_etherSrcAddr[4] << ":"\
				<< hex << (int)_etherSrcAddr[5];
	eventInfo << "): dst(" \
                << hex << (int)_etherDstAddr[0] << ":"\
				<< hex << (int)_etherDstAddr[1] << ":"\
				<< hex << (int)_etherDstAddr[2] << ":"\
				<< hex << (int)_etherDstAddr[3] << ":"\
				<< hex << (int)_etherDstAddr[4] << ":"\
				<< hex << (int)_etherDstAddr[5] << ")";
	sInfo = eventInfo.str();
}


