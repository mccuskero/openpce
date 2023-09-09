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
// This class is used to serialize events to store them, and to 
// send them over a communication line. 
//
// Serialize:
// When serializing a class, the class determines its size using
// the GetSizerSerializer, we create a buffer, then ToSerializer
// each native type to that buffer.
//
// DeSerialize:
// When deserializing a class, the size is written out to the first
// 4 bytes (unsigned long) of the buffer. 
//
// The first 4 bytes represents the size of the serialized object
// to read from. The buffer passed into the deSerialize member
// should match. If not this is a good indication of a versioning
// error between your class framework.
//
// By default, all object are serialzied using big-endian order.
//
// All object are serialized using native data types, of string buffers.
// "No structures/classes" should be serialzied because of padding issues
// between platforms. The type AttributeValuePair can be serialized
// using this framework.
//

#ifndef SERIALIZEABLE_H_
#define SERIALIZEABLE_H_

//
// Helper classes used by derived class
//
#include <pcff/serialize/ToSerializer.h>
#include <pcff/serialize/GetSizeSerializer.h>
#include <pcff/serialize/FromSerializer.h>
#include <pcff/serialize/MessageHeaderSerializer.h>

#include <pcff/serialize/Serializer.h>

namespace pcff
{

class Serializeable
{
public:
	Serializeable();
	virtual ~Serializeable();
	void initMembers();
// serialize:
//  returns a pointer to its own internal buffer
//  less memcpy to other buffer spaces,  
// deserialize:
// overridden to create the object from the stream passed in.
//
public:
	virtual unsigned char *serializeTo(unsigned long &bufLen);
	virtual void serializeFrom(unsigned char *buf, unsigned long bufLen);
	virtual void serializeFromGetMessageHeader(unsigned char *buf, unsigned long bufLen);
	virtual unsigned long getSerializeSize();
	
	
public:
	unsigned char getSerializationID()          { return _serializationID; }
	unsigned int  getObjVersion() 				{ return _objectVersion; }
	unsigned long getSerializedSize() 			{ return _serializedSize;}
	unsigned long getSize() 					{ return _bufferSize;}
	bool		  isValid();
	
	// derived classes must override the serialize member
protected:
	virtual void serialize(Serializer &s)       {}
	
protected:
	unsigned long _bufferSize;
	unsigned char *_buffer;
	int			   _objectVersion;
	unsigned long  _serializedSize;
	unsigned char  _serializationID;
};

};

#endif /*SERIALIZEABLE_H_*/
