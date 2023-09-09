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

#include <pcsec/network/WifiHeaderInfo.h>

using namespace std;
using namespace pcff;
using namespace pcsec;

WifiHeaderInfo::WifiHeaderInfo()
{
	initMembers();
}

WifiHeaderInfo::WifiHeaderInfo(const WifiHeaderInfo &in)
{
	initMembers();

	_frameControl = in._frameControl;
	  _durationID = in._durationID;
	  memcpy(_addr1, in._addr1, WifiHeaderInfo::AddressSize);
	  memcpy(_addr2, in._addr2, WifiHeaderInfo::AddressSize);
	  memcpy(_addr3, in._addr3, WifiHeaderInfo::AddressSize);
	  memcpy(_addr4, in._addr4, WifiHeaderInfo::AddressSize);
	  _seqControl = in._seqControl;
}

WifiHeaderInfo::~WifiHeaderInfo()
{
}

void WifiHeaderInfo::initMembers()
{
	  _frameControl = 0;
	  _durationID = 0;
	  memset(_addr1, '0', WifiHeaderInfo::AddressSize);
	  memset(_addr2, '0', WifiHeaderInfo::AddressSize);
	  memset(_addr3, '0', WifiHeaderInfo::AddressSize);
	  memset(_addr4, '0', WifiHeaderInfo::AddressSize);
	  _seqControl = 0;
}

void WifiHeaderInfo::initialize(	  u_int16_t frameControl,
			u_int16_t durationID,
			u_int8_t  &addr1,
			u_int8_t  &addr2,
			u_int8_t  &addr3,
			u_int8_t  &addr4,
			u_int16_t seqControl)
{
	  _frameControl = frameControl;
	  _durationID = durationID;
	  memcpy(&_addr1, &addr1, WifiHeaderInfo::AddressSize);
	  memcpy(&_addr2, &addr2, WifiHeaderInfo::AddressSize);
	  memcpy(&_addr3, &addr3, WifiHeaderInfo::AddressSize);
	  memcpy(&_addr4, &addr4, WifiHeaderInfo::AddressSize);
	  _seqControl = seqControl;
}

void WifiHeaderInfo::serialize(pcff::Serializer &s)
{
	unsigned long addrSize = WifiHeaderInfo::AddressSize;

	s.transfer(this->_frameControl);
	s.transfer(this->_durationID);
	s.transfer(_addr1[0], addrSize);
	s.transfer(_addr2[0], addrSize);
	s.transfer(_addr3[0], addrSize);
	s.transfer(_addr4[0], addrSize);
	s.transfer(this->_seqControl);
}

void WifiHeaderInfo::getInfo(string &sInfo)
{
	ostringstream eventInfo;

	eventInfo << ":" << _frameControl;
	eventInfo << ":" << _durationID;
	eventInfo << ":(" << _addr1[0] << ":" \
				<< _addr1[1] << ":" \
				<< _addr1[2] << ":" \
				<< _addr1[3] << ":" \
				<< _addr1[4] << ":" \
				<< _addr1[5];
	eventInfo << "):(" << _addr2[0] << ":"\
				<< _addr2[1] << ":"\
				<< _addr2[2] << ":"\
				<< _addr2[3] << ":"\
				<< _addr2[4] << ":"\
				<< _addr2[5];
	eventInfo << "):(" << _addr3[0] << ":"\
				<< _addr3[1] << ":"\
				<< _addr3[2] << ":"\
				<< _addr3[3] << ":"\
				<< _addr3[4] << ":"\
				<< _addr3[5];
	eventInfo << "):(" << _addr4[0] << ":"\
				<< _addr4[1] << ":"\
				<< _addr4[2] << ":"\
				<< _addr4[3] << ":"\
				<< _addr4[4] << ":"\
				<< _addr4[5];
	eventInfo << "):" << _seqControl;

	sInfo = eventInfo.str();
}


