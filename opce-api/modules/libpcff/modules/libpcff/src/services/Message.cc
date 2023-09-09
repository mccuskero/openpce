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


#include <string>
#include <sstream>

#include <pcff/BasicExceptionMgr.h>

#include <pcff/services/MessageProcessor.h>
#include <pcff/services/ServicesException.h>
#include <pcff/services/Message.h>

using namespace std;
using namespace pcff;

Message::Message(MessageProcessor &msgProcessor)
	: _msgProcessor(msgProcessor)
{
	initMembers();
}

//
// TODO: maybe issue with ServiceHandler and copy constructor
// maybe a message processor may not exist
//
Message::Message(const Message &in)
: _msgProcessor(in._msgProcessor)
{
	*this = in;
/*
	_msgID 		= in._msgID;  // increments factory manages id
	_msgStatus	= in._msgStatus;
	_msgType	= in._msgType;
	_msgSize	= in._msgSize;
	_compressionType	= in._compressionType;
	_dataType	= in._dataType;
	_dataSubtype = in._dataSubtype;
	_dataSize	= in._dataSize;

	if (in._dataSize > 0)
	{
		memcpy(_data, in._data, _dataSize);
	}
	else
	{
		_data = NULL;
	}
	*/
}

Message::~Message()
{
	if (_data != NULL)
	{
		delete [] _data;
	}

	initMembers();
}

void Message::initMembers()
{
	_msgID = 0;
	_msgType = 0;
	_msgSize = 0;
	_compressionType = 0;
	_dataType = 0;
	_dataSubtype = 0;
	_dataSize = 0;
	_data = NULL;
}

void Message::init()
{
	// TODO: debug for now, must change
	_msgType = Message::Connect;
}

void Message::clear()
{
	BasicExceptionMgr::instance()->log(LogLevel::Debug, "Message::clear");
	if (_data != NULL)
	{
		delete [] _data;
	}

	initMembers();
}

//
// TODO: local referece returned here. Need to fixup/validate
//
Message &Message::operator=(Message const &in)
{
	_msgProcessor = in._msgProcessor;
	_msgID 		= in._msgID;  // increments factory manages id
	_msgStatus	= in._msgStatus;
	_msgType	= in._msgType;
	_msgSize	= in._msgSize;
	_compressionType	= in._compressionType;
	_dataType	= in._dataType;
	_dataSubtype = in._dataSubtype;
	_dataSize	= in._dataSize;

	if (in._dataSize > 0)
	{
		memcpy(_data, in._data, _dataSize);
	}
	else
	{
		_data = NULL;
	}

	return *this;

//	static Message out(in);
//	return out;
}



void Message::send()
{
	BasicExceptionMgr::instance()->log(LogLevel::Debug, "Message::send");
	_msgProcessor.send(*this);
}

void Message::setTestData()
{
	_msgType = Message::Test;
	_msgSize = 0;
	_dataType = 0;
	_compressionType = Message::NoCompression;
	_dataSize = 0;
	_dataSubtype = 0;
	_data = NULL;
}

void Message::setData(unsigned char *data, unsigned long dataSize)
{
	if (data == NULL || dataSize == 0)
	{
		throw ServicesException(ServicesException::InvalidData, "data passed in is invalid during setData call");
	}
	else
	{
		_dataSize = dataSize;
		_data = new unsigned char[_dataSize];
		memset(_data, '0', _dataSize);
		memcpy(_data, data, _dataSize);
	}
}

void Message::serialize(Serializer &s)
{
	s.transfer(this->_msgStatus);
	s.transfer(this->_msgType);
	s.transfer(this->_msgSize);
	s.transfer(this->_dataType);
	s.transfer(this->_dataSubtype);
	s.transfer(this->_dataSize);
	s.transfer(this->_compressionType);
	s.transfer(this->_data, _dataSize);
}

void Message::getInfo(string &str)
{
	ostringstream ss;

	ss << endl << \
	"msgStatus       :  " << _msgStatus << endl << \
	"msgType         :  " << _msgType << endl << \
    "msgSize         :  " << _msgSize << endl << \
    "dataType        : " << _dataType << endl << \
    "dataSubtype     : " << _dataSubtype << endl << \
    "dataSize        : " << _dataSize << endl << \
	"_compressionType: " << _compressionType << endl;

	str =  ss.str();
}


