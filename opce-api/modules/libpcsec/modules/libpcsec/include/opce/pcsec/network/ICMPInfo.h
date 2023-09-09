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

#ifndef ICMPINFO_H_
#define ICMPINFO_H_

#include <sys/types.h>
#include <netinet/in.h> // AF_INET and AF_INET6 address families.

#include <string>

#include <pcff/serialize/Serializeable.h>

#include <pcsec/network/NetworkPacketInfo.h>

namespace pcsec
{

class ICMPInfo : public pcff::Serializeable
{
	friend class NetworkPacketInfo;
	
public:
	ICMPInfo();
	ICMPInfo(const ICMPInfo &in);
	virtual ~ICMPInfo();

private:
	void initMembers();
	
public:
	void initialize(u_int8_t  type,
		u_int16_t code,
		u_int16_t checksum,
		u_int8_t  ext,     
		u_int16_t extid,
		u_int16_t extSequenceNum);
	
public:
	void     setType(u_int8_t 	type)               { _type = type; }
	u_int8_t getType()                            	{ return _type; }
	void     setCode(u_int16_t 	code)               { _code = code; }
	u_int16_t getCode()                            	{ return _code; }
	void     setChecksum(u_int16_t 	checksum)       { _checksum = checksum; }
	u_int16_t getChecksum()                         { return _checksum; }
	void     setExtension(u_int8_t 	ext)            { _ext = ext; }
	u_int8_t getExtension()                         { return _ext; }
	void     setExtensionID(u_int16_t extID)        { _extID = extID; }
	u_int16_t getExtensionID()                      { return _extID; }
	void     setExtSequenceNum(u_int16_t extSequenceNum) { _extSequenceNum = extSequenceNum; }
	u_int16_t getExtSequenceNum()                        { return _extSequenceNum; }
	
public:
	virtual void serialize(pcff::Serializer &s);
	virtual void getInfo(std::string &sInfo);

protected:
	u_int8_t  _type;
	u_int16_t _code;
	u_int16_t _checksum;
	u_int8_t  _ext;       
	u_int16_t _extID;
	u_int16_t _extSequenceNum;
};

};

#endif /*ICMPINFO_H_*/
