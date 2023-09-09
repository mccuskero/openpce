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
// TODO: need to convert the _ipSrc, _ipDst to make IPv6 ready
//

#ifndef IPHEADERINFO_H_
#define IPHEADERINFO_H_

#include <sys/types.h>
#include <netinet/in.h> // AF_INET and AF_INET6 address families.

#include <string>

#include <pcff/serialize/Serializeable.h>
#include <pcff/socket/INETAddress.h>

#include <pcsec/network/NetworkPacketInfo.h>

namespace pcsec
{

class IPHeaderInfo : public pcff::Serializeable
{
	friend class NetworkPacketInfo;

public:
	IPHeaderInfo();
	IPHeaderInfo(const IPHeaderInfo &in);
	virtual ~IPHeaderInfo();

private:
	void initMembers();

public:
	void initialize(    const u_int8_t 	ipVerhl,
			const u_int8_t 	ipTOS,
			const u_int16_t 	ipLen,
			const u_int16_t 	ipID,
			const u_int16_t 	ipOff,
			const u_int8_t 	ipTTL,
			const u_int8_t 	ipProto,
			const u_int16_t 	ipCksum,
			const std::string &ipSrc,
			const std::string &ipDst);
public:
	void     setVersionHeaderLen(u_int8_t 	ipVerHeaderLen) { _ipVerHeaderLen = ipVerHeaderLen; }
	u_int8_t getVersionHeaderLen()                          { return _ipVerHeaderLen; }
	void     setTOS(u_int8_t 	ipTOS)                      { _ipTOS = ipTOS; }
	u_int8_t getTOS()                            			{ return _ipTOS; }
	void     setLen(u_int16_t 	ipLen)          			{ _ipLen = ipLen; }
	u_int16_t getLen()                            			{ return _ipLen; }
	void     setID(u_int16_t 	ipID)          				{ _ipID = ipID; }
	u_int16_t getID()                            			{ return _ipID; }
	void     setOffset(u_int16_t 	ipOff)          		{ _ipFlagFragOff = ipOff; }
	u_int16_t getOffset()                            		{ return _ipFlagFragOff; }
	void     setTTL(u_int8_t 	ipTTL)          			{ _ipTTL = ipTTL; }
	u_int8_t getTTL()                            			{ return _ipTTL; }
	void     setProtocol(u_int8_t 	ipProto)          		{ _ipProto = ipProto; }
	u_int8_t getProtocol()                            		{ return _ipProto; }
	void     setChecksum(u_int16_t 	ipCksum)          		{ _ipCksum = ipCksum; }
	u_int16_t getChecksum()                            		{ return _ipCksum; }
	void            setIpSrc(pcff::INETAddress &ipSrc)         { _ipSrc = ipSrc; }
	pcff::INETAddress &getIpSrc()                              { return _ipSrc; }
	void            setIpDst(pcff::INETAddress &ipDst)         { _ipDst = ipDst; }
	pcff::INETAddress &getIpDst()                              { return _ipDst; }

public:
	virtual void serialize(pcff::Serializer &s);
	virtual void getInfo(std::string &sInfo);

	std::string getSrcIPAsString();
	std::string getDstIPAsString();

protected:
    u_int8_t 	_ipVerHeaderLen;      // version & header length
    u_int8_t 	_ipTOS;        // type of service
    u_int16_t 	_ipLen;       // datagram length
    u_int16_t 	_ipID;        // identification
    u_int16_t 	_ipFlagFragOff;       // fragment offset and flags, 1st 3 bits
    u_int8_t 	_ipTTL;        // time to live field
    u_int8_t 	_ipProto;      // datagram protocol
    u_int16_t 	_ipCksum;      // checksum
    pcff::INETAddress _ipSrc;  // source IP
    pcff::INETAddress _ipDst;  // dest IP
};

};

#endif /*IPHEADERINFO_H_*/
