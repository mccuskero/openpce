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

#ifndef UDPHEADERINFO_H_
#define UDPHEADERINFO_H_

#include <sys/types.h>

#include <string>

#include <pcff/serialize/Serializeable.h>

#include <pcsec/network/NetworkPacketInfo.h>

namespace pcsec
{

class UDPHeaderInfo : public pcff::Serializeable
{
	friend class NetworkPacketInfo;
	
public:
	UDPHeaderInfo();
	UDPHeaderInfo(const UDPHeaderInfo &in);
	virtual ~UDPHeaderInfo();
	
private:
	void initMembers();
	
public:
	void initialize(    u_int16_t sPort,
			u_int16_t dPort,
			u_int16_t len,
			u_int16_t checksum);
public:
	void      setSrcPort(u_int16_t 	sPort)      { _sPort = sPort; }
	u_int16_t getSrcPort()                      { return _sPort; }
	void      setDstPort(u_int16_t 	dPort)      { _dPort = dPort; }
	u_int16_t getDstPort()                      { return _dPort; }
	void      setUDPLen(u_int16_t 	len)        { _udpLen = len; }
	u_int16_t getUDPLen()                       { return _udpLen; }
	void      setChecksum(u_int16_t 	checksum)  { _checksum = checksum; }
	u_int16_t getChecksum()                       { return _checksum; }

public:
	virtual void  serialize(pcff::Serializer &s);
	virtual void getInfo(std::string &sInfo);

	
protected:
    u_int16_t _sPort;
    u_int16_t _dPort;
    u_int16_t _udpLen;
    u_int16_t _checksum;
};

};

#endif /*UDPHEADERINFO_H_*/
