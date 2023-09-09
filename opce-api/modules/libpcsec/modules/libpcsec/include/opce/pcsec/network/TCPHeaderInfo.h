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

#ifndef TCPHEADERINFO_H_
#define TCPHEADERINFO_H_

#include <sys/types.h>

#include <string>

#include <pcff/serialize/Serializeable.h>

#include <pcsec/network/NetworkPacketInfo.h>

namespace pcsec
{

class TCPHeaderInfo : public pcff::Serializeable
{
	friend class NetworkPacketInfo;
	
public:
	TCPHeaderInfo();
	TCPHeaderInfo(const TCPHeaderInfo &in);
	virtual ~TCPHeaderInfo();
	
private:
	void initMembers();
	
public:
	void initialize(u_int16_t sPort,     
	    u_int16_t dPort,  
	    u_int32_t sequence,  
	    u_int32_t ackNum,
	    u_int8_t  offsetReserved, 
	    u_int8_t  flags,
	    u_int16_t window,     
	    u_int16_t checksum,     
	    u_int16_t urgentPtr);
	
public:
	void      setSrcPort(u_int16_t 	sPort)      { _sPort = sPort; }
	u_int16_t getSrcPort()                      { return _sPort; }
	void      setDstPort(u_int16_t 	dPort)      { _dPort = dPort; }
	u_int16_t getDstPort()                      { return _dPort; }
	void      setSequence(u_int32_t sequence)  { _sequence = sequence; }
	u_int32_t getSequence()                    { return _sequence; }
	void      setAckNum(u_int32_t ackNum)      { _ackNum = ackNum; }
	u_int32_t getAckNum()                      { return _ackNum; }
	void      setOffsetReserved(u_int8_t offsetReserved)  { _offsetReserved = offsetReserved; }
	u_int8_t  getOffsetReserved()                          { return _offsetReserved; }
	void      setFlags(u_int8_t flags)         { _flags = flags; }
	u_int8_t  getFlags()                       { return _flags; }
	void      setWindow(u_int16_t 	window)      { _window = window; }
	u_int16_t getWindow()                      { return _window; }
	void      setChecksum(u_int16_t checksum)    { _checksum = checksum; }
	u_int16_t getChecksum()                    { return _checksum; }
	void      setUrgentPtr(u_int16_t urgentPtr)    { _urgentPtr = urgentPtr; }
	u_int16_t getUrgentPtr()                    { return _urgentPtr; }

public:
	virtual void  serialize(pcff::Serializer &s);
	virtual void getInfo(std::string &sInfo);
	
protected:
    u_int16_t _sPort;     
    u_int16_t _dPort;     
    u_int32_t _sequence;  
    u_int32_t _ackNum;    
    u_int8_t  _offsetReserved; 
    u_int8_t  _flags;
    u_int16_t _window;       
    u_int16_t _checksum;     
    u_int16_t _urgentPtr; 
};

};

#endif /*TCPHEADERINFO_H_*/
