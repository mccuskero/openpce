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

#ifndef HONEYTRAPEVENT_H_
#define HONEYTRAPEVENT_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <pcff/event/Event.h>

#include <pcsec/network/honeytrap/honeytrapStructs.h>

#include <pcsec/network/AttackData.h>
#include <pcsec/network/NetworkPacketInfo.h>

namespace pcsec
{

class HoneytrapEvent : public pcff::Event
{
public:
	HoneytrapEvent();
	virtual ~HoneytrapEvent();

private:
	void initMembers();

public:
	void initialize();
	void initialize(int size, char *buf);
	void initialize(AttackPacket &attack);

public:
	void 		createAttackData(AttackPacket &attack);
	AttackData &getAttackData() { return *_attackData; }

public:
	void serialize(pcff::Serializer &s);

public:
	std::string getEventInfo();
	void              setTestData();

private:
	bool               _bHaveAttackData;
	AttackData 			*_attackData;
	bool 			   _bHaveNetworkDataInfo;
	NetworkPacketInfo  *_netPacketInfo;
};

};

#endif /*HONEYTRAPEVENT_H_*/
