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

#ifndef Message_h_
#define Message_h_

#include <string>

#include <pcff/serialize/Serializeable.h>
#include <pcff/services/MessageProcessor.h>

namespace pcff
{

class Message : public Serializeable
{
public:
	Message(MessageProcessor &msgProcessor);
	Message(const Message &in);
	virtual ~Message();

private:
	void initMembers();

public:
	void init();

public:
	Message &operator=(Message const &in);

public:
	int   getStatus() 			 { return _msgStatus; }
	int   getMessageType()		 { return _msgType; }
	long  getMessageSize()		 { return _msgSize; }
	int   getDataType()			 { return _dataType; }
	int   getDataSubtype()	     { return _dataSubtype; }
	unsigned long  getDataSize() { return _dataSize; }
	unsigned char *getData()     { return _data; }
	int   getCompressionType()   { return _compressionType; }

public:
	void setMessageType(int type)	{ _msgType = type; }
	void setDataType(int type)		{ _dataType = type; }
	void setDataSubtype(int type)	{ _dataSubtype = type; }
	void setStatus(int msgStatus)	{ _msgStatus = msgStatus; }
	void setData(unsigned char *data, unsigned long dataSize);

public:
	void    clear();
	void 	send();

public:
	void setTestData();
	void getInfo(std::string &str);

	// serialization routines
public:
	virtual void  serialize(pcff::Serializer &s);

public:
	enum eMessageType
	{
		Test = 1,
		TestACK,
		Connect,
		ConnectACK,
		Disconnect,
		DisconnectACK,
		Data,           // Messasge Contains data
		DataACK,
		NextAvailMsgID
	};


	enum eMessageCompression
	{
		NoCompression,
		Zlib
	};

	// Data types within message
	// Maybe there is a better way to manage?
	// Create a MessageData class, ID is in it.
	enum MessageEventType
	{
		SnortEvent
	};

	// used to validate serialization
	enum eObjVersions
	{
		ObjVersion=1
	};

private:
	MessageProcessor 	&_msgProcessor;

	// attributes that persist when serialized
private:
	unsigned int _msgID;  // increments factory manages id
	int 	_msgStatus;
	int 	_msgType;
	long 	_msgSize;
	int     _compressionType;
	int 	_dataType;
	int     _dataSubtype;
	unsigned long 	_dataSize;
	unsigned char *_data;
};

};

#endif

