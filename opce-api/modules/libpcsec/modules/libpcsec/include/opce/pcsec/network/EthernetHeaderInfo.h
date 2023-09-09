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


#ifndef ETHERNETHEADERINFO_H_
#define ETHERNETHEADERINFO_H_

#include <sys/types.h>
#include <netinet/in.h> // AF_INET and AF_INET6 address families.

#include <string>

#include <pcff/serialize/Serializeable.h>
#include <pcff/socket/INETAddress.h>

#include <pcsec/network/NetworkPacketInfo.h>

namespace pcsec
{

class EthernetHeaderInfo
{
	friend class NetworkPacketInfo;
	
public:
	EthernetHeaderInfo();
	EthernetHeaderInfo(const EthernetHeaderInfo &in);
	virtual ~EthernetHeaderInfo();
	
public:
	enum Constants
	{
		AddressSize=pcff::INETAddress::MaxAddressLength
	};
		
private:
	void initMembers();
	
public:
	void initialize(u_int16_t _etherType,
	  u_int8_t  &_srcAddr,
	  u_int8_t  &_dstAddr);
	
public:
	void      setEtherType(u_int16_t etherType)      { _etherType = etherType; }
	u_int16_t getEtherType()                         { return _etherType; }
	void     setSrcAddr(u_int8_t 	&etherSrcAddr)   { memcpy(&_etherSrcAddr, &etherSrcAddr, AddressSize); }
	u_int8_t &getSrcAddr()                           { return *_etherSrcAddr; }
	void     setDstAddr(u_int8_t 	&etherDstAddr)   { memcpy(&_etherDstAddr, &etherDstAddr, AddressSize); }
	u_int8_t &getDstAddr()                           { return *_etherDstAddr; }
	
public:
	virtual void serialize(pcff::Serializer &s);
	virtual void getInfo(std::string &sInfo);

protected:
    u_int16_t 	_etherType;
    u_int8_t 	_etherSrcAddr[AddressSize];
    u_int8_t 	_etherDstAddr[AddressSize];
};

};

#endif /*ETHERNETHEADERINFO_H_*/
