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

#include <pcsec/network/TCPHeaderInfo.h>

using namespace std;
using namespace pcff;
using namespace pcsec;


TCPHeaderInfo::TCPHeaderInfo()
{
	initMembers();
}


TCPHeaderInfo::TCPHeaderInfo(const TCPHeaderInfo &in)
{
	initMembers();

	_sPort				= in._sPort;
    _dPort				= in._dPort;
    _sequence			= in._sequence;
    _ackNum				= in._ackNum;
    _offsetReserved		= in._offsetReserved;
    _flags				= in._flags;
    _window				= in._window;
    _checksum			= in._checksum;
    _urgentPtr			= in._urgentPtr;
}

TCPHeaderInfo::~TCPHeaderInfo()
{
}

void TCPHeaderInfo::initMembers()
{
	_sPort				= 0;
    _dPort				= 0;
    _sequence			= 0;
    _ackNum				= 0;
    _offsetReserved		= 0;
    _flags				= 0;
    _window				= 0;
    _checksum			= 0;
    _urgentPtr			= 0;
}

void TCPHeaderInfo::initialize(u_int16_t sPort,
    u_int16_t dPort,
    u_int32_t sequence,
    u_int32_t ackNum,
    u_int8_t  offsetReserved,
    u_int8_t  flags,
    u_int16_t window,
    u_int16_t checksum,
    u_int16_t urgentPtr)
{
	_sPort				= sPort;
    _dPort				= dPort;
    _sequence			= sequence;
    _ackNum				= ackNum;
    _offsetReserved		= offsetReserved;
    _flags				= flags;
    _window				= window;
    _checksum			= checksum;
    _urgentPtr			= urgentPtr;
}

void TCPHeaderInfo::serialize(pcff::Serializer &s)
{
	s.transfer(this->_sPort);
	s.transfer(this->_dPort);
	s.transfer(this->_sequence);
	s.transfer(this->_ackNum);
	s.transfer(this->_offsetReserved);
	s.transfer(this->_flags);
	s.transfer(this->_window);
	s.transfer(this->_checksum);
	s.transfer(this->_urgentPtr);
}

void TCPHeaderInfo::getInfo(string &sInfo)
{
	ostringstream eventInfo;

    eventInfo  << "TCP: (";
	eventInfo << "sport " << ntohs(_sPort) << ", ";
	eventInfo << "dport " << ntohs(_dPort) << " , ";
	eventInfo << "seq " << ntohl(_sequence) << " , ";
	eventInfo << "acknum " << ntohl(_ackNum) << " , ";
	eventInfo << "offset " << hex << (int)_offsetReserved << " , ";
	eventInfo << "flags " << hex << (int)_flags << ", ";
	eventInfo << "win " << ntohs(_window) << ", ";
	eventInfo << "checksum " << hex << ntohs(_checksum) << ", ";
	eventInfo << "urgptr " << hex << ntohs(_urgentPtr) << ")";

	sInfo = eventInfo.str();
}
