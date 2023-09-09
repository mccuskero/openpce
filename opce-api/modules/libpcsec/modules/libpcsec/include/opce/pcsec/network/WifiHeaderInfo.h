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


#ifndef WIFIHEADERINFO_H_
#define WIFIHEADERINFO_H_

#include <sys/types.h>
#include <netinet/in.h> // AF_INET and AF_INET6 address families.
#include <string.h>

#include <string>

#include <pcff/serialize/Serializeable.h>

#include <pcsec/network/NetworkPacketInfo.h>

namespace pcsec
{

class WifiHeaderInfo : public pcff::Serializeable
{
	friend class NetworkPacketInfo;
	
public:
	WifiHeaderInfo();
	WifiHeaderInfo(const WifiHeaderInfo &in);
	virtual ~WifiHeaderInfo();

public:
	enum Contants
	{
		AddressSize=6
	};
		
private:
	void initMembers();
	
public:
	void initialize(	  u_int16_t _frameControl,
	  u_int16_t _durationID,
	  u_int8_t  &_addr1,
	  u_int8_t  &_addr2,
	  u_int8_t  &_addr3,
	  u_int8_t  &_addr4,
	  u_int16_t _seqControl);
	
public:
	void     setFrameControl(u_int16_t 	frameControl)  { _frameControl = frameControl; }
	u_int16_t getFrameControl()                         { return _frameControl; }
	void     setDurationID(u_int16_t 	durationID)   { _durationID = durationID; }
	u_int16_t getDurationID()                          { return _durationID; }
	void     setAddr1(u_int8_t 	&addr)              { memcpy(&_addr1, &addr, AddressSize); }
	u_int8_t &getAddr1()                            { return *_addr1; }
	void     setAddr2(u_int8_t 	&addr)              { memcpy(&_addr2, &addr, AddressSize); }
	u_int8_t &getAddr2()                            { return *_addr2; }
	void     setAddr3(u_int8_t 	&addr)              { memcpy(&_addr3, &addr, AddressSize); }
	u_int8_t &getAddr3()                            { return *_addr3; }
	void     setAddr4(u_int8_t 	&addr)              { memcpy(&_addr4, &addr, AddressSize); }
	u_int8_t &getAddr4()                            { return *_addr4; }
	void     setSeqControl(u_int16_t seqControl)    { _seqControl = seqControl; }
	u_int16_t getSeqControl()                       { return _seqControl; }
	
public:
	virtual void serialize(pcff::Serializer &s);
	virtual void getInfo(std::string &sInfo);

protected:
	  u_int16_t _frameControl;
	  u_int16_t _durationID;
	  u_int8_t  _addr1[6];
	  u_int8_t  _addr2[6];
	  u_int8_t  _addr3[6];
	  u_int8_t  _addr4[6];
	  u_int16_t _seqControl;
};

};
#endif /*WIFIHEADERINFO_H_*/
