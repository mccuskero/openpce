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

#ifndef AttackData_h_
#define AttackData_h_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// TODO: make depend on honeytrap install src dir, take out headers
#include <pcsec/network/honeytrap/honeytrapStructs.h>

#include <pcsec/network/AttackConnectionData.h>
#include <pcsec/network/AttackDownloadData.h>
#include <pcsec/network/AttackPayloadData.h>

namespace pcsec 
{

#define MAX_DOWNLOADS 5

class AttackData
{
public:
	AttackData();
	AttackData(AttackData &in);
	virtual ~AttackData();

private:
	void initMembers();
	
public:
	void initialize();
	void initialize(AttackPacket &packet);
	
public:
	u_int8_t getVirtualAttackFlag() { return _virtualAttack; }
	time_t   getStartTime()         { return _startTime; }
	time_t   getEndTime()           { return _endTime; }
	u_int8_t getOperationalMode()   { return _operationalMode; }
	AttackConnectionData &getAttackConnectionData()      { return _attackConnection; }
	AttackConnectionData &getProxyAttackConnectionData() { return _attackProxyConnection; }
	u_int16_t             getDownloadCount()             { return _downloadCount; }
	AttackDownloadData   &getDownloadData(int idx);	

private:
	void retrieveConnectionData(AttackPacket &packet);
	void retrieveProxyConnectionData(AttackPacket &packet);
	void retrieveDownloadData(AttackPacket &packet);
	
public:	
	typedef enum
	{
		none,
		ignore,
		normal,
		proxy,
		mirror
	} OperationalModes;
	
protected:
	u_int8_t        		_virtualAttack;	/* flag for marking virtual attacks */
	time_t					_startTime;	    /* time of attack start */
	time_t					_endTime;	    /* time of attack end */
	u_int8_t				_operationalMode;	/* mode of operation (none, ignore, normal, proxy, mirror) */
	AttackConnectionData 	_attackConnection;
	AttackConnectionData 	_attackProxyConnection;
	u_int16_t				_downloadCount;	/* number of downloads */
	AttackDownloadData    	_downloads[MAX_DOWNLOADS];	/* array of download structs */
};

};
#endif /*AttackData_h_*/
