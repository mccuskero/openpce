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

//
// This class is closely modeled from the Nepenthes and Honeytrap
// projects.
//

#ifndef AttackConnectionData_h_
#define AttackConnectionData_h_

#include <pcsec/network/AttackPayloadData.h>

namespace pcsec
{

class AttackConnectionData
{	
	friend class AttackData;
	
public:
	AttackConnectionData();
	virtual ~AttackConnectionData();
	
private:
	void initMembers();
	
public:
	u_int32_t 	       getLocalAddr() 	 { return _localAddr; }
	u_int32_t 	       getRemoteAddr()   { return _remoteAddr; }
	u_int16_t 	       getLocalPort() 	 { return _localPort; }
	u_int16_t 	       getRemotePort()   { return _remotePort; }
	u_int16_t 	       getProtocol()     { return _protocol; }
	AttackPayloadData &getPayloadData()  { return _payload; }
	
protected:
	u_int32_t			_localAddr;	 /* local ip address */
	u_int32_t			_remoteAddr; /* remote ip address */
	u_int16_t			_localPort;	 /* local (tcp/udp) port */
	u_int16_t			_remotePort; /* remote (tcp/udp) port */
	u_int8_t			_protocol;	 /* IP protocol id (tcp/udp) */
	AttackPayloadData   _payload;   /* */
};

};
#endif /*AttackConnectionData_h_ */
