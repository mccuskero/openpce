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
// BasicMessage class can hold binary serialized or ascii data, that has been
// that contains xml. Need to look into uuencoding, or mime encoding of 
// buffer data.
//


#ifndef BasicMessage_h_
#define BasicMessage_h_

#include <string>

#include <pcff/serialize/Serializeable.h>

namespace pcff
{

class BasicMessage : public pcff::Serializeable
{
public:
	BasicMessage();
	virtual ~BasicMessage();

private:
	void initMembers();
	
public:
	virtual void init();
	virtual void init(int msgType, long msgSize, int dataType, long dataSize);
	
public:
	void setTestData();
	
public:
	int   getStatus() 			{ return _msgStatus; }
	int   getMessageType()		{ return _msgType; }
	unsigned long  getMessageSize()		{ return _msgSize; }
	int   getDataType()			{ return _dataType; }
	unsigned long  getDataSize()			{ return _dataSize; }
	void  *getData()            { return _dataBuf;  }
	
public:
	void getInfo(std::string &str);

public:
	void setDataBuffer(Serializeable &msgData);
	void setDataBuffer(unsigned char *msgData, unsigned long msgDataLen);

public:
	virtual void  serialize(Serializer &s);
	
public:
	typedef enum
	{
		Test,
		TestACK,
		Connect,
		ConnectACK,
		Disconnect,
		DisconnectACK,
		RegisterSensor,
		RegisterSensorACK,
		SensorData,
		SensorDataACK
	} eMessageTypes;
	
	typedef enum
	{
		Unknown,
		Created,
		CreationPending,
		CreationError,
	} eCreationTypes;

private:
	int 			_msgStatus;  // u_int32 (check out snortStructs.h)
	int 			_msgType;
	unsigned long 	_msgSize;
	unsigned long 	_dataSize;
	int     		_dataType;
	unsigned char  	*_dataBuf;
};
	
};

#endif /*BASICMESSAGE_H_*/
