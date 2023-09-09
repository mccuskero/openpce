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

#include <pcff/BasicExceptionMgr.h>
#include <pcff/proxy/SensorProxyException.h>

#include <pcsec/network/EthernetHeaderInfo.h>
#include <pcsec/network/WifiHeaderInfo.h>
#include <pcsec/network/IPHeaderInfo.h>
#include <pcsec/network/ICMPInfo.h>
#include <pcsec/network/UDPHeaderInfo.h>
#include <pcsec/network/TCPHeaderInfo.h>

#include <pcsec/network/honeytrap/honeytrapStructs.h>
#include <pcsec/network/AttackData.h>

#include <pcsec/network/HoneytrapEvent.h>

using namespace std;
using namespace pcff;
using namespace pcsec;

HoneytrapEvent::HoneytrapEvent()
{
	initMembers();
}

HoneytrapEvent::~HoneytrapEvent()
{
	if (_attackData != NULL)    delete _attackData;
	if (_netPacketInfo != NULL) delete _netPacketInfo;
	initMembers();
}

void HoneytrapEvent::initMembers()
{
	_bHaveAttackData = false;
	_attackData = NULL;
	_bHaveNetworkDataInfo = false;
	_netPacketInfo = NULL;
}

void HoneytrapEvent::initialize()
{
}

void HoneytrapEvent::initialize(int size, char *buf)
{
	// save a memcpy, use the existing buffer
	AttackPacket *attackPacket = (AttackPacket *)buf;
	int attackpktsize=sizeof(AttackPacket);

	if ( size == attackpktsize )
	{
		initialize((AttackPacket &)*attackPacket);
	}
	else
	{
		BasicExceptionMgr::instance()->log(LogLevel::Error, "Socket read size %d, is different from AttackPacket struct size %d", size, attackpktsize);
		throw SensorProxyException(SensorProxyException::EventInitializationError,
				"honeytrap AttackPacket size mis-match");
	}
}

void HoneytrapEvent::initialize(AttackPacket &attack)
{
	_attackData = new AttackData();
	_bHaveAttackData  = true;

	if (BasicExceptionMgr::instance()->getLogLevel() >= LogLevel::Info)
	{
		BasicExceptionMgr::instance()->log(LogLevel::Info, "Attack Info Recieved ID:");
		BasicExceptionMgr::instance()->log(LogLevel::Info, "Alert-Message:          ");
	}


	createAttackData(attack);
   // createNetworkPacketInfo((Packet *)_packet, _packetLen, _packetCaptureLen);
}


void HoneytrapEvent::createAttackData(AttackPacket &attack)
{
	try
	{
		_attackData->initialize(attack);
	}
	catch(...)
	{
		BasicExceptionMgr::instance()->log(LogLevel::Error, "Error occured in HoneytrapEvent::createAttackData");
	}
}

// get initial info
// Currently NetworkPacket Info only supports IP,UDP,TCP,WIFI.
// Here is where we can extract other types of info,
// such as: Ethernet frames, ARP data, ...
//void HoneytrapEvent::createNetworkPacketInfo(Packet *p, unsigned long pLen, unsigned long pCapLen)
//{
	// create network packet info, and set pacekt
//	_netPacketInfo = new NetworkPacketInfo();
//	_bHaveNetworkDataInfo = true;
//
//	_netPacketInfo->setPacketCaptureTime(_packet_tv_sec, _packet_tv_usec);
//
//}
void HoneytrapEvent::serialize(Serializer &s)
{
	s.transfer(this->_bHaveNetworkDataInfo);

	if (_bHaveNetworkDataInfo == true)
	{
		if (_netPacketInfo == NULL)
		{
			_netPacketInfo = new NetworkPacketInfo();
		}

		_netPacketInfo->serialize(s);
	}

	Event::serialize(s);
	// TODO: serializeRawEvent();
}

string HoneytrapEvent::getEventInfo()
{
	ostringstream eventInfo;

	eventInfo << endl << "info:      " << "test" << endl;

	return eventInfo.str();
}

void HoneytrapEvent::setTestData()
{

}

