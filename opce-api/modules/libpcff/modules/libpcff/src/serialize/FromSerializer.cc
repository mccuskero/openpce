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
#include <pcff/serialize/SerializeException.h>
#include <pcff/serialize/FromSerializer.h>

using namespace std;
using namespace pcff;

FromSerializer::FromSerializer()
{	
	_size 		= 0L;
}

FromSerializer::~FromSerializer()
{
}

void FromSerializer::initialize(unsigned char *buf, unsigned long size)
{
	unsigned char sID = 0;
	unsigned long serializedSize = 0L;
	
	Serializer::initialize(buf, size);
	
	transferSerializedID(sID);
	
	if (sID == Serializer::SID)
	{
		_serializationID = sID;
	}
	else
	{
		throw SerializeException(SerializeException::InvalidID, 
					"FromSerializer::initialize");
	}
	
	transferObjVersion(_objectVersion);
	
	transferObjSize(serializedSize);
	
	// then either streams broke apart, or were combined.
	if (size == serializedSize)
	{		
		_serializedSize = serializedSize;
	}
	else
	{
		BasicExceptionMgr::instance()->log(LogLevel::Error, 
					  "FromSerializer::initialize: serializeSize %ld, bufferSize %ld", 
					  serializedSize,
					  size);
		throw SerializeException(SerializeException::InvalidObjSize, 
							"FromSerializer::initialize");
	}
}

void FromSerializer::transfer(bool &val)
{
	unsigned char boolVal = 0;
	
    if( _bOverflow == false)
    {
		if(_bytesUsed+1>_size)
		{
			_bOverflow=true; 
			throw SerializeException(SerializeException::BufferOverflow, "FromSerializer::transfer bool");
		}
		
		boolVal = *_buffer;
		
		val = (boolVal == Serializer::True) ? true : false; 
		 
		_buffer+=1; 
		_bytesUsed+=1;	
    }
    else
    {
       	throw SerializeException(SerializeException::BufferOverflow, "FromSerializer::transfer bool");
    }
}

void FromSerializer::transfer(char &val)
{
    if( _bOverflow == false)
    {
        if(_bytesUsed+1>_size)
        {
        	_bOverflow=true; 
        	throw SerializeException(SerializeException::BufferOverflow, "FromSerializer::transfer char");
        }
        
        val = (char )*_buffer;

        _buffer+=1; 
        _bytesUsed+=1;	
    }
    else
    {
       	throw SerializeException(SerializeException::BufferOverflow, "FromSerializer::transfer char");
    }
}

void FromSerializer::transfer(unsigned char &val)
{
    if( _bOverflow == false)
    {
        if(_bytesUsed+1>_size)
        {
        	_bOverflow=true; 
        	throw SerializeException(SerializeException::BufferOverflow, "FromSerializer::transfer unsigned char");
        }
        
        val = *_buffer;

        _buffer+=1; 
        _bytesUsed+=1;	
    }
    else
    {
       	throw SerializeException(SerializeException::BufferOverflow, "FromSerializer::transfer unsigned char");
    }
}

void FromSerializer::transfer(u_int16_t &val)
{
    if( _bOverflow == false)
    {
        if(_bytesUsed+2>_size)
        {
        	_bOverflow=true; 
        	throw SerializeException(SerializeException::BufferOverflow, "FromSerializer::transfer int");
        }
        
        val = (int)Serializer::Read16Destructive((u_int16_t &)*_buffer);
        _buffer+=2; 
        _bytesUsed+=2;	
    }
    else
    {
       	throw SerializeException(SerializeException::BufferOverflow, "FromSerializer::transfer int");
    }
}

void FromSerializer::transfer(int &val)
{
	int transSize = sizeof(int);
	
    if( _bOverflow == false)
    {
        if(_bytesUsed+transSize>_size)
        {
        	_bOverflow=true; 
        	throw SerializeException(SerializeException::BufferOverflow, "FromSerializer::transfer int");
        }
        
        if (transSize == 2)
        {
            val = (int)Serializer::Read16Destructive((u_int16_t &)*_buffer);        	
        }
        else if (transSize == 4)
        {
            val = (int)Serializer::Read32IntDestructive((u_int32_t &)*_buffer);	
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
       	throw SerializeException(SerializeException::BufferOverflow, "FromSerializer::transfer int");
    }
}

void FromSerializer::transfer(unsigned int &val)
{
	int transSize = sizeof(unsigned int);

	if( _bOverflow == false)
    {
        if(_bytesUsed+transSize>_size)
        {
        	_bOverflow=true; 
        	throw SerializeException(SerializeException::BufferOverflow, "FromSerializer::transfer unsigned int");
        }
        
        if (transSize == 2)
        {
            val = (unsigned int)Serializer::Read16Destructive((u_int16_t &)*_buffer);        	
        }
        else if (transSize == 4)
        {
            val = (unsigned int)Serializer::Read32IntDestructive((u_int32_t &)*_buffer);	
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
       	throw SerializeException(SerializeException::BufferOverflow, "FromSerializer::transfer unsigned int");
    }
}

void FromSerializer::transfer(long &val)
{
	int transSize = sizeof(long);

	if( _bOverflow == false)
    {
        if(_bytesUsed+4>_size)
        {
        	_bOverflow=true; 
        	throw SerializeException(SerializeException::BufferOverflow, "FromSerializer::transfer long");
        }
        
        val = (long)Serializer::Read32IntDestructive((u_int32_t &)*_buffer);
        _buffer+=4; 
        _bytesUsed+=4;	
    }
    else
    {
       	throw SerializeException(SerializeException::BufferOverflow, "FromSerializer::transfer long");
    }
}

void FromSerializer::transfer(unsigned long &val)
{
    if( _bOverflow == false)
    {
        if(_bytesUsed+4>_size)
        {
        	_bOverflow=true; 
        	throw SerializeException(SerializeException::BufferOverflow, "FromSerializer::transfer unsigned long");
        }
        
        val = (unsigned long)Serializer::Read32IntDestructive((u_int32_t &)*_buffer);
        _buffer+=4; 
        _bytesUsed+=4;	
    }
    else
    {
       	throw SerializeException(SerializeException::BufferOverflow, "FromSerializer::transfer unsigned long");
    }
}

void FromSerializer::transfer(float &val)
{
    if( _bOverflow == false)
    {
        if(_bytesUsed+4>_size)
        {
        	_bOverflow=true; 
        	throw SerializeException(SerializeException::BufferOverflow, "FromSerializer::transfer float");
        }
        
        val = Serializer::Read32Destructive((float &)*_buffer);
        _buffer+=4; 
        _bytesUsed+=4;	
    }
    else
    {
       	throw SerializeException(SerializeException::BufferOverflow, "FromSerializer::transfer float");
    }
}

void FromSerializer::transfer(long long &val)
{
    if( _bOverflow == false)
    {
        if(_bytesUsed+8>_size)
        {
        	_bOverflow=true; 
           	throw SerializeException(SerializeException::BufferOverflow, "FromSerializer::transfer double");
        }
        
        val = (long long )Serializer::Read64IntDestructive((u_int64_t &)*_buffer);
        _buffer+=8; 
        _bytesUsed+=8;	
    }
    else
    {
       	throw SerializeException(SerializeException::BufferOverflow, "FromSerializer::transfer double");
    }
}

void FromSerializer::transfer(unsigned long long &val)
{
    if( _bOverflow == false)
    {
        if(_bytesUsed+8>_size)
        {
        	_bOverflow=true; 
           	throw SerializeException(SerializeException::BufferOverflow, "FromSerializer::transfer double");
        }
        
        val = (unsigned long long )Serializer::Read64IntDestructive((u_int64_t &)*_buffer);
        _buffer+=8; 
        _bytesUsed+=8;	
    }
    else
    {
       	throw SerializeException(SerializeException::BufferOverflow, "FromSerializer::transfer double");
    }
}

void FromSerializer::transfer(double &val)
{
    if( _bOverflow == false)
    {
        if(_bytesUsed+8>_size)
        {
        	_bOverflow=true; 
           	throw SerializeException(SerializeException::BufferOverflow, "FromSerializer::transfer double");
        }
        
        val = (double )Serializer::Read64Destructive((double &)*_buffer);
        _buffer+=8; 
        _bytesUsed+=8;	
    }
    else
    {
       	throw SerializeException(SerializeException::BufferOverflow, "FromSerializer::transfer double");
    }
}

void FromSerializer::transfer(std::string &val)
{
	unsigned long strSize = 0;
	
    if( _bOverflow == false)
    {
    	strSize = readCharBufferSize();

    	if (strSize > 0)
    	{
        	if(_bytesUsed+strSize>_size)
            {
            	_bOverflow=true; 
            	throw SerializeException(SerializeException::BufferOverflow, "FromSerializer::transfer string");
            }
            
            val = (char *)_buffer;
            
            _buffer+=strSize; 
            _bytesUsed+=strSize;	    		
    	}
    }
    else
    {
       	throw SerializeException(SerializeException::BufferOverflow, "FromSerializer::transfer string");
    }
}

void FromSerializer::transfer(u_int8_t &buf, unsigned long &size)
{
	unsigned long strSize = 0;

    if( _bOverflow == false)
    {
    	strSize = readCharBufferSize();

    	if (strSize > 0)
    	{
        	size = strSize;
        	
            if(_bytesUsed+strSize>_size)
            {
            	_bOverflow=true; 
            	throw SerializeException(SerializeException::BufferOverflow, "FromSerializer::transfer u_int8_t&");
            }
            
            memcpy(&buf, _buffer, strSize);
            
            _buffer+=strSize; 
            _bytesUsed+=strSize;	
    	}
    }
    else
    {
       	throw SerializeException(SerializeException::BufferOverflow, "FromSerializer::transfer u_int8_t&");
    }
}

void FromSerializer::transfer(char &buf, unsigned long &size)
{
	unsigned long strSize = 0;

    if( _bOverflow == false)
    {
    	strSize = readCharBufferSize();

    	if (strSize > 0)
    	{
        	size = strSize;
        	
            if(_bytesUsed+strSize>_size)
            {
            	_bOverflow=true; 
            	throw SerializeException(SerializeException::BufferOverflow, "FromSerializer::transfer char&");
            }
            
            memcpy(&buf, _buffer, strSize);
            
            _buffer+=strSize; 
            _bytesUsed+=strSize;	
    	}
    }
    else
    {
       	throw SerializeException(SerializeException::BufferOverflow, "FromSerializer::transfer char&");
    }
}

void FromSerializer::transfer(char *&buf, unsigned long &size)
{
	unsigned long strSize = 0;

    if( _bOverflow == false)
    {
    	strSize = readCharBufferSize();

    	if (strSize > 0)
    	{
        	size = strSize;
        	buf = new char[strSize+1]; // add in space for /0
        	memset(buf, '\0', strSize+1);
        	
            if(_bytesUsed+strSize>_size)
            {
            	_bOverflow=true; 
            	throw SerializeException(SerializeException::BufferOverflow, "FromSerializer::transfer char*");
            }
            
            memcpy(buf, _buffer, strSize);
            
            _buffer+=strSize; 
            _bytesUsed+=strSize;	
    	}
    }
    else
    {
       	throw SerializeException(SerializeException::BufferOverflow, "FromSerializer::transfer char*");
    }
}

void FromSerializer::transfer(unsigned char *&buf, unsigned long &size)
{
	unsigned long strSize = 0;

    if( _bOverflow == false)
    {
    	strSize = readCharBufferSize();

    	if (strSize > 0)
    	{
        	size = strSize;
        	buf = new unsigned char[strSize+1]; // add in space for /0
        	memset(buf, '\0', strSize+1);
        	
            if(_bytesUsed+strSize>_size)
            {
            	_bOverflow=true; 
            	throw SerializeException(SerializeException::BufferOverflow, "FromSerializer::transfer unsigned char*");
            }
            
            memcpy(buf, _buffer, strSize);
            
            _buffer+=strSize; 
            _bytesUsed+=strSize;	
    	}
    }
    else
    {
       	throw SerializeException(SerializeException::BufferOverflow, "FromSerializer::transfer unsigned char*");
    }
}


void FromSerializer::transfer(AttributeValuePair &val)
{
	// TODO: need to implement
}

//
// read in initial data
//
void FromSerializer::transferSerializedID(unsigned char &sID)
{
	transfer(sID);	
}

void FromSerializer::transferObjVersion(int &objVersion)
{
	transfer(objVersion);	
}

void FromSerializer::transferObjSize(unsigned long &objSize)
{
	transfer(objSize);
}

//
// helper function to store char bufs
//
unsigned long FromSerializer::readCharBufferSize()
{
	unsigned long retVal = 0L;
	
	transfer(retVal);
	
	return retVal;
}
