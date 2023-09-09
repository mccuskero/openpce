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
#include <pcff/DumpBuffer.h>
#include <pcff/serialize/SerializeException.h>
#include <pcff/serialize/MessageHeaderSerializer.h>

using namespace std;
using namespace pcff;

MessageHeaderSerializer::MessageHeaderSerializer()
{	
	_size 		= 0L;
}

MessageHeaderSerializer::~MessageHeaderSerializer()
{
}

void MessageHeaderSerializer::initialize(unsigned char *buf, unsigned long size)
{
	unsigned char sID = 0;
	
	Serializer::initialize(buf, size);
	
	transferSerializedID(sID);
	
	if (sID == Serializer::SID)
	{
		_serializationID = sID;

		transferObjVersion(_objectVersion);
		
		transferObjSize(_serializedSize);
		
		BasicExceptionMgr::instance()->log(LogLevel::Info, 
					  "MessageHeaderSerializer::initialize: serializeSize %ld, bufferSize %ld", 
					  _serializedSize,
					  size);	
	}
	else
	{
		BasicExceptionMgr::instance()->log(LogLevel::Info, 
					  "MessageHeaderSerializer::initialize: InvalidID: SID %x, should be 0xF5", 
					  sID);	

		DumpBuffer::instance().init();
		DumpBuffer::instance().dump(size, (char *)buf);			
	}
}

void MessageHeaderSerializer::transfer(bool &val)
{
	unsigned char boolVal = 0;
	
    if( _bOverflow == false)
    {
		if(_bytesUsed+1>_size)
		{
			_bOverflow=true; 
			throw SerializeException(SerializeException::BufferOverflow, "MessageHeaderSerializer::transfer bool");
		}
		
		boolVal = *_buffer;
		
		val = (boolVal == Serializer::True) ? true : false; 
		 
		_buffer+=1; 
		_bytesUsed+=1;	
    }
    else
    {
       	throw SerializeException(SerializeException::BufferOverflow, "MessageHeaderSerializer::transfer bool");
    }
}

void MessageHeaderSerializer::transfer(char &val)
{
    if( _bOverflow == false)
    {
        if(_bytesUsed+1>_size)
        {
        	_bOverflow=true; 
        	throw SerializeException(SerializeException::BufferOverflow, "MessageHeaderSerializer::transfer char");
        }
        
        val = (char )*_buffer;

        _buffer+=1; 
        _bytesUsed+=1;	
    }
    else
    {
       	throw SerializeException(SerializeException::BufferOverflow, "MessageHeaderSerializer::transfer char");
    }
}

void MessageHeaderSerializer::transfer(unsigned char &val)
{
    if( _bOverflow == false)
    {
        if(_bytesUsed+1>_size)
        {
        	_bOverflow=true; 
        	throw SerializeException(SerializeException::BufferOverflow, "MessageHeaderSerializer::transfer unsigned char");
        }
        
        val = *_buffer;

        _buffer+=1; 
        _bytesUsed+=1;	
    }
    else
    {
       	throw SerializeException(SerializeException::BufferOverflow, "MessageHeaderSerializer::transfer unsigned char");
    }
}

void MessageHeaderSerializer::transfer(u_int16_t &val)
{
    if( _bOverflow == false)
    {
        if(_bytesUsed+2>_size)
        {
        	_bOverflow=true; 
        	throw SerializeException(SerializeException::BufferOverflow, "MessageHeaderSerializer::transfer int");
        }
        
        val = (int)Serializer::Read16((u_int16_t &)*_buffer);
        _buffer+=2; 
        _bytesUsed+=2;	
    }
    else
    {
       	throw SerializeException(SerializeException::BufferOverflow, "MessageHeaderSerializer::transfer int");
    }
}

void MessageHeaderSerializer::transfer(int &val)
{
	int transSize = sizeof(int);
	
    if( _bOverflow == false)
    {
        if(_bytesUsed+transSize>_size)
        {
        	_bOverflow=true; 
        	throw SerializeException(SerializeException::BufferOverflow, "MessageHeaderSerializer::transfer int");
        }
        
        if (transSize == 2)
        {
            val = (int)Serializer::Read16((u_int16_t &)*_buffer);
        }
        else if (transSize == 4)
        {
            val = (int)Serializer::Read32Int((u_int32_t &)*_buffer);
        }
        else
        {
           	throw SerializeException(SerializeException::InvalidIntSize, "ToSerializer::transfer int");        	
        }

        _buffer+=transSize; 
        _bytesUsed+=transSize;	
    }
    else
    {
       	throw SerializeException(SerializeException::BufferOverflow, "MessageHeaderSerializer::transfer int");
    }
}

void MessageHeaderSerializer::transfer(unsigned int &val)
{
	int transSize = sizeof(unsigned int);

	if( _bOverflow == false)
    {
        if(_bytesUsed+transSize>_size)
        {
        	_bOverflow=true; 
        	throw SerializeException(SerializeException::BufferOverflow, "MessageHeaderSerializer::transfer unsigned int");
        }
        
        if (transSize == 2)
        {
            val = (unsigned int)Serializer::Read16((u_int16_t &)*_buffer);
        }
        else if (transSize == 4)
        {
            val = (unsigned int)Serializer::Read32Int((u_int32_t &)*_buffer);
        }
        else
        {
           	throw SerializeException(SerializeException::InvalidIntSize, "ToSerializer::transfer int");        	
        }
        
        _buffer+=transSize; 
        _bytesUsed+=transSize;	
    }
    else
    {
       	throw SerializeException(SerializeException::BufferOverflow, "MessageHeaderSerializer::transfer unsigned int");
    }
}

void MessageHeaderSerializer::transfer(long &val)
{
    if( _bOverflow == false)
    {
        if(_bytesUsed+4>_size)
        {
        	_bOverflow=true; 
        	throw SerializeException(SerializeException::BufferOverflow, "MessageHeaderSerializer::transfer long");
        }
        
        val = (long)Serializer::Read32Int((u_int32_t &)*_buffer);
        _buffer+=4; 
        _bytesUsed+=4;	
    }
    else
    {
       	throw SerializeException(SerializeException::BufferOverflow, "MessageHeaderSerializer::transfer long");
    }
}

void MessageHeaderSerializer::transfer(unsigned long &val)
{
    if( _bOverflow == false)
    {
        if(_bytesUsed+4>_size)
        {
        	_bOverflow=true; 
        	throw SerializeException(SerializeException::BufferOverflow, "MessageHeaderSerializer::transfer unsigned long");
        }
        
        val = (unsigned long)Serializer::Read32Int((u_int32_t &)*_buffer);
        _buffer+=4; 
        _bytesUsed+=4;	
    }
    else
    {
       	throw SerializeException(SerializeException::BufferOverflow, "MessageHeaderSerializer::transfer unsigned long");
    }
}

void MessageHeaderSerializer::transfer(float &val)
{
    if( _bOverflow == false)
    {
        if(_bytesUsed+4>_size)
        {
        	_bOverflow=true; 
        	throw SerializeException(SerializeException::BufferOverflow, "MessageHeaderSerializer::transfer float");
        }
        
        val = Serializer::Read32((float &)*_buffer);
        _buffer+=4; 
        _bytesUsed+=4;	
    }
    else
    {
       	throw SerializeException(SerializeException::BufferOverflow, "MessageHeaderSerializer::transfer float");
    }
}

void MessageHeaderSerializer::transfer(long long &val)
{
    if( _bOverflow == false)
    {
        if(_bytesUsed+8>_size)
        {
        	_bOverflow=true; 
           	throw SerializeException(SerializeException::BufferOverflow, "MessageHeaderSerializer::transfer double");
        }
        
        val = (long long )Serializer::Read64Int((u_int64_t &)*_buffer);
        _buffer+=8; 
        _bytesUsed+=8;	
    }
    else
    {
       	throw SerializeException(SerializeException::BufferOverflow, "MessageHeaderSerializer::transfer double");
    }
}

void MessageHeaderSerializer::transfer(unsigned long long &val)
{
    if( _bOverflow == false)
    {
        if(_bytesUsed+8>_size)
        {
        	_bOverflow=true; 
           	throw SerializeException(SerializeException::BufferOverflow, "MessageHeaderSerializer::transfer double");
        }
        
        val = (unsigned long long )Serializer::Read64Int((u_int64_t &)*_buffer);
        _buffer+=8; 
        _bytesUsed+=8;	
    }
    else
    {
       	throw SerializeException(SerializeException::BufferOverflow, "MessageHeaderSerializer::transfer double");
    }
}

void MessageHeaderSerializer::transfer(double &val)
{
}

void MessageHeaderSerializer::transfer(std::string &val)
{
}

void MessageHeaderSerializer::transfer(u_int8_t &buf, unsigned long &size)
{
}

void MessageHeaderSerializer::transfer(char &buf, unsigned long &size)
{
}

void MessageHeaderSerializer::transfer(char *&buf, unsigned long &size)
{
}

void MessageHeaderSerializer::transfer(unsigned char *&buf, unsigned long &size)
{
}


void MessageHeaderSerializer::transfer(AttributeValuePair &val)
{
	// TODO: need to implement
}

//
// read in initial data
//
void MessageHeaderSerializer::transferSerializedID(unsigned char &sID)
{
	transfer(sID);	
}

void MessageHeaderSerializer::transferObjVersion(int &objVersion)
{
	transfer(objVersion);	
}

void MessageHeaderSerializer::transferObjSize(unsigned long &objSize)
{
	transfer(objSize);
}
