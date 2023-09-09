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
#include <string>
#include <sstream>

#include <pcsec/network/ICMPInfo.h>

using namespace std;
using namespace pcff;
using namespace pcsec;

ICMPInfo::ICMPInfo()
{
	initMembers();
}

ICMPInfo::ICMPInfo(const ICMPInfo &in)
{
	initMembers();

	_type 		= in._type;
	_code 		= in._code;
	_checksum 	= in._checksum;
	_ext 		= in._ext;
	_extID 		= in._extID;
	_extSequenceNum = in._extSequenceNum;
}

ICMPInfo::~ICMPInfo()
{
}

void ICMPInfo::initMembers()
{
	_type 			= 0;
	_code 			= 0;
	_checksum 		= 0;
	_ext 			= 0;
	_extID 			= 0;
	_extSequenceNum = 0;
}

void ICMPInfo::initialize(u_int8_t  type,
	u_int16_t code,
	u_int16_t checksum,
	u_int8_t  ext,
	u_int16_t extID,
	u_int16_t extSequenceNum)
{
	_type = type;
	_code = code;
	_checksum = checksum;
	_ext = ext;
	_extID = extID;
	_extSequenceNum = extSequenceNum;
}

void ICMPInfo::serialize(pcff::Serializer &s)
{
	s.transfer(this->_type);
	s.transfer(this->_code);
	s.transfer(this->_checksum);
	s.transfer(this->_ext);
	s.transfer(this->_extID);
	s.transfer(this->_extSequenceNum);
}

void ICMPInfo::getInfo(string &sInfo)
{
	ostringstream eventInfo;

    eventInfo << "ICMP: (";
	eventInfo << hex << "type " << (int)_type << ", ";
	eventInfo << hex << "code " << ntohs(_code)  << ", ";
	eventInfo << hex << "checksum " << ntohs(_checksum)  << ", ";
	eventInfo << hex << "ext " << (int)_ext  << ", ";
	eventInfo << hex << "extID " << ntohs(_extID)  << ", ";
	eventInfo << hex << "extseq " << ntohs(_extSequenceNum)  << ")";

	sInfo = eventInfo.str();
}

