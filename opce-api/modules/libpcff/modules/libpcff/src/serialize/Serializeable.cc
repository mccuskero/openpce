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

#include <string.h>
#include <pcff/BasicExceptionMgr.h>
#include <pcff/serialize/Serializeable.h>

using namespace pcff;

Serializeable::Serializeable()
{
	initMembers();
}

Serializeable::~Serializeable()
{
	if (_buffer != NULL)
	{
		delete [] _buffer;
		_buffer = NULL;
	}
	initMembers();
}

void Serializeable::initMembers()
{
	_buffer = NULL;
	_bufferSize  = 0;
	_objectVersion = 0;
	_serializedSize = 0;
	_serializationID = 0;
}

unsigned long Serializeable::getSerializeSize()
{
	unsigned long size = 0;

	// get the initialize obj size to set
	GetSizeSerializer sSize;
	sSize.initialize();
	serialize(sSize);
	size = sSize.getSize();

	_bufferSize = size;

	return size;
}

unsigned char *Serializeable::serializeTo(unsigned long &bufLen)
{
	_bufferSize = getSerializeSize();

	_buffer = new unsigned char[_bufferSize];

	memset(_buffer, '0', _bufferSize);

	// serialize obj to buffer
	ToSerializer toSerializer;
	toSerializer.initialize(_buffer, _bufferSize);

	serialize(toSerializer);

	bufLen = _bufferSize;

	return _buffer;
}

void  Serializeable::serializeFrom(unsigned char *buf, unsigned long bufLen)
{
	  FromSerializer fromSerializer;

	  fromSerializer.initialize((unsigned char*)buf, bufLen);
	  serialize(fromSerializer);

	  _serializationID = fromSerializer.getSerializationID();
	  _serializedSize  = fromSerializer.getSerializedSize();
	  _objectVersion   = fromSerializer.getObjVersion();
	  _bufferSize	= bufLen;

	  BasicExceptionMgr::instance()->log(LogLevel::Fine,
			  "Serializeable::serializeFrom sID: %c, serializeSize %ld, objVersion %d, bufLen %ld",
			  _serializationID,
			  _serializedSize,
			  _objectVersion,
			  _bufferSize);
}

//
// used to read initial buffer to extract serialized size.
// The FromSerializer will error, if the size of the stream
// does not match the serialized size.
//
void  Serializeable::serializeFromGetMessageHeader(unsigned char *buf, unsigned long bufLen)
{
	  MessageHeaderSerializer msgHeaderSerializer;

	  msgHeaderSerializer.initialize((unsigned char*)buf, bufLen);
	  serialize(msgHeaderSerializer);

	  _serializationID = msgHeaderSerializer.getSerializationID();
	  _serializedSize  = msgHeaderSerializer.getSerializedSize();
	  _objectVersion   = msgHeaderSerializer.getObjVersion();
	  _bufferSize	= bufLen;

	  BasicExceptionMgr::instance()->log(LogLevel::Finer,
			  "Serializeable::serializeFromGetMessageHeader sID: %c, serializeSize %ld, objVersion %d, bufLen %ld",
			  _serializationID,
			  _serializedSize,
			  _objectVersion,
			  _bufferSize);
}

bool Serializeable::isValid()
{
	bool retVal = false;

	if (_serializationID == Serializer::SID)
	{
		retVal = true;
	}

	return retVal;
}
