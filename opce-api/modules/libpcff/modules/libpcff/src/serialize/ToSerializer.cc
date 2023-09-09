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



#include <pcff/serialize/SerializeException.h>
#include <pcff/serialize/ToSerializer.h>

using namespace pcff;

ToSerializer::ToSerializer()
{
	_size 		= 0L;
}

ToSerializer::~ToSerializer()
{
}

void ToSerializer::initialize(unsigned char *buf, unsigned long size)
{
	Serializer::initialize(buf, size);
	_serializedSize = size;
	transferSerializedID(_serializationID);	
	transferObjVersion(_objectVersion);	
	transferObjSize(_serializedSize);
}

void ToSerializer::transfer(bool &val)
{
	unsigned char boolVal = 0;
	
    if( _bOverflow == false)
    {
		if(_bytesUsed+1>_size)
		{
			_bOverflow=true; 
			throw SerializeException(SerializeException::BufferOverflow, "ToSerializer::transfer bool");
		}
		
		boolVal = (val == true) ? Serializer::True : Serializer::False; 

		*_buffer = boolVal;
		
		_buffer+=1; 
		_bytesUsed+=1;	
    }
    else
    {
       	throw SerializeException(SerializeException::BufferOverflow, "ToSerializer::transfer bool");
    }
}

void ToSerializer::transfer(char &val)
{
    if( _bOverflow == false)
    {
        if(_bytesUsed+1>_size)
        {
        	_bOverflow=true; 
        	throw SerializeException(SerializeException::BufferOverflow, "ToSerializer::transfer char");
        }
        
        *_buffer = (unsigned char)val;

        _buffer+=1; 
        _bytesUsed+=1;	
    }
    else
    {
       	throw SerializeException(SerializeException::BufferOverflow, "ToSerializer::transfer char");
    }	
}

void ToSerializer::transfer(unsigned char &val)
{
    if( _bOverflow == false)
    {
        if(_bytesUsed+1>_size)
        {
        	_bOverflow=true; 
        	throw SerializeException(SerializeException::BufferOverflow, "ToSerializer::transfer unsigned char");
        }
        
        *_buffer = val;

        _buffer+=1; 
        _bytesUsed+=1;	
    }
    else
    {
       	throw SerializeException(SerializeException::BufferOverflow, "ToSerializer::transfer unsigned char");
    }	
}

void ToSerializer::transfer(u_int16_t &val)
{
    if( _bOverflow == false)
    {
        if(_bytesUsed+2>_size)
        {
        	_bOverflow=true; 
        	throw SerializeException(SerializeException::BufferOverflow, "ToSerializer::transfer int");
        }
        
        Serializer::Write16(val, (u_int16_t &)*_buffer);
        _buffer+=2; 
        _bytesUsed+=2;	
    }
    else
    {
       	throw SerializeException(SerializeException::BufferOverflow, "ToSerializer::transfer int");
    }	
}

void ToSerializer::transfer(int &val)
{
	int transSize = sizeof(int);
	
    if( _bOverflow == false)
    {
        if(_bytesUsed+transSize>_size)
        {
        	_bOverflow=true; 
        	throw SerializeException(SerializeException::BufferOverflow, "ToSerializer::transfer int");
        }
        
        if (transSize == 2)
        {
        	Serializer::Write16(val, (u_int16_t &)*_buffer);        	
        }
        else if (transSize == 4)
        {
        	Serializer::Write32Int(val, (u_int32_t &)*_buffer);
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
       	throw SerializeException(SerializeException::BufferOverflow, "ToSerializer::transfer int");
    }	
}

void ToSerializer::transfer(unsigned int &val)
{
	int transSize = sizeof(unsigned int);

    if( _bOverflow == false)
    {
        if(_bytesUsed+transSize>_size)
        {
        	_bOverflow=true; 
        	throw SerializeException(SerializeException::BufferOverflow, "ToSerializer::transfer int");
        }
        
        if (transSize == 2)
        {
        	Serializer::Write16(val, (u_int16_t &)*_buffer);        	
        }
        else if (transSize == 4)
        {
        	Serializer::Write32Int(val, (u_int32_t &)*_buffer);
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
       	throw SerializeException(SerializeException::BufferOverflow, "ToSerializer::transfer int");
    }
}

void ToSerializer::transfer(long &val)
{
    if( _bOverflow == false)
    {
        if(_bytesUsed+4>_size)
        {
        	_bOverflow=true; 
        	throw SerializeException(SerializeException::BufferOverflow, "ToSerializer::transfer long");
        }
        
        Serializer::Write32Int(val, (u_int32_t &)*_buffer);
        _buffer+=4; 
        _bytesUsed+=4;	
    }
    else
    {
       	throw SerializeException(SerializeException::BufferOverflow, "ToSerializer::transfer long");
    }
}

void ToSerializer::transfer(unsigned long &val)
{
    if( _bOverflow == false)
    {
        if(_bytesUsed+4>_size)
        {
        	_bOverflow=true; 
        	throw SerializeException(SerializeException::BufferOverflow, "ToSerializer::transfer unsigned long");
        }
        
        Serializer::Write32Int(val, (u_int32_t &)*_buffer);
        _buffer+=4; 
        _bytesUsed+=4;	
    }
    else
    {
       	throw SerializeException(SerializeException::BufferOverflow, "ToSerializer::transfer unsigned long");
    }
}

void ToSerializer::transfer(float &val)
{
    if( _bOverflow == false)
    {
        if(_bytesUsed+4>_size)
        {
        	_bOverflow=true; 
        	throw SerializeException(SerializeException::BufferOverflow, "ToSerializer::transfer float");
        }
        
        Serializer::Write32(val, (float &)*_buffer);
        _buffer+=4; 
        _bytesUsed+=4;	
    }
    else
    {
       	throw SerializeException(SerializeException::BufferOverflow, "ToSerializer::transfer float");
    }
}

void ToSerializer::transfer(long long &val)
{
    if( _bOverflow == false)
    {
        if(_bytesUsed+8>_size)
        {
        	_bOverflow=true; 
        	throw SerializeException(SerializeException::BufferOverflow, "ToSerializer::transfer long long");
        }
        
        Serializer::Write64Int(val, (u_int64_t &)*_buffer);
        _buffer+=8; 
        _bytesUsed+=8;	
    }
    else
    {
       	throw SerializeException(SerializeException::BufferOverflow, "ToSerializer::transfer long long");
    }
}

void ToSerializer::transfer(unsigned long long &val)
{
    if( _bOverflow == false)
    {
        if(_bytesUsed+8>_size)
        {
        	_bOverflow=true; 
        	throw SerializeException(SerializeException::BufferOverflow, "ToSerializer::transfer unsigned long long");
        }
        
        Serializer::Write64Int(val, (u_int64_t &)*_buffer);
        _buffer+=8; 
        _bytesUsed+=8;	
    }
    else
    {
       	throw SerializeException(SerializeException::BufferOverflow, "ToSerializer::transfer unsigned long long");
    }
}

void ToSerializer::transfer(double &val)
{
    if( _bOverflow == false)
    {
        if(_bytesUsed+8>_size)
        {
        	_bOverflow=true; 
        	throw SerializeException(SerializeException::BufferOverflow, "ToSerializer::transfer double");
        }
        
        Serializer::Write64(val, (double &)*_buffer);
        _buffer+=8; 
        _bytesUsed+=8;	
    }
    else
    {
       	throw SerializeException(SerializeException::BufferOverflow, "ToSerializer::transfer double");
    }
}

void ToSerializer::transfer(std::string &val)
{
	unsigned long strSize = (unsigned long)val.length();
		
	if( _bOverflow == false)
    {
		writeCharBufferSize(strSize);

		if (strSize > 0) 
		{			
			if(_bytesUsed+strSize>_size)
	        {
	        	_bOverflow=true; 
	        	throw SerializeException(SerializeException::BufferOverflow, "ToSerializer::transfer string");
	        }
	        
	        memcpy(_buffer, val.c_str(), strSize);
	        
	        _buffer+=strSize; 
	        _bytesUsed+=strSize;	
		}
    }
    else
    {
       	throw SerializeException(SerializeException::BufferOverflow, "ToSerializer::transfer string");
   	}
}

void ToSerializer::transfer(u_int8_t &buf, unsigned long &size)
{
    if( _bOverflow == false)
    {
    	writeCharBufferSize(size);

		if (size > 0) 
		{			
	        if(_bytesUsed+size>_size)
	        {
	        	_bOverflow=true; 
	        	throw SerializeException(SerializeException::BufferOverflow, "ToSerializer::transfer u_int8_t&");
	        }
	        
	        memcpy(_buffer, &buf, size);
	        
	        _buffer+=size; 
	        _bytesUsed+=size;	
		}
    }
    else
    {
       	throw SerializeException(SerializeException::BufferOverflow, "ToSerializer::transfer u_int8_t&");
    }	
}

void ToSerializer::transfer(char &buf, unsigned long &size)
{
    if( _bOverflow == false)
    {
    	writeCharBufferSize(size);

		if (size > 0) 
		{			
	        if(_bytesUsed+size>_size)
	        {
	        	_bOverflow=true; 
	        	throw SerializeException(SerializeException::BufferOverflow, "ToSerializer::transfer char&");
	        }
	        
	        memcpy(_buffer, &buf, size);
	        
	        _buffer+=size; 
	        _bytesUsed+=size;	
		}
    }
    else
    {
       	throw SerializeException(SerializeException::BufferOverflow, "ToSerializer::transfer char&");
    }	
}

void ToSerializer::transfer(char *&buf, unsigned long &size)
{
    if( _bOverflow == false)
    {
    	writeCharBufferSize(size);

		if (size > 0) 
		{			
	        if(_bytesUsed+size>_size)
	        {
	        	_bOverflow=true; 
	        	throw SerializeException(SerializeException::BufferOverflow, "ToSerializer::transfer char*");
	        }
	        
	        memcpy(_buffer, buf, size);
	        
	        _buffer+=size; 
	        _bytesUsed+=size;	
		}
    }
    else
    {
       	throw SerializeException(SerializeException::BufferOverflow, "ToSerializer::transfer char*");
    }	
}

void ToSerializer::transfer(unsigned char *&buf, unsigned long &size)
{
    if( _bOverflow == false)
    {
    	writeCharBufferSize(size);

		if (size > 0) 
		{			
	        if(_bytesUsed+size>_size)
	        {
	        	_bOverflow=true; 
	        	throw SerializeException(SerializeException::BufferOverflow, "ToSerializer::transfer unsgined char*");
	        }
	        
	        memcpy(_buffer, buf, size);
	        
	        _buffer+=size; 
	        _bytesUsed+=size;	
		}
    }
    else
    {
       	throw SerializeException(SerializeException::BufferOverflow, "ToSerializer::transfer unsigned char*");
    }	
}

void ToSerializer::transfer(AttributeValuePair &val)
{
	// TODO: need to implement
}

//
// read in initial data
//
void ToSerializer::transferSerializedID(unsigned char &sID)
{
	transfer(sID);	
}

void ToSerializer::transferObjVersion(int &objVersion)
{
	transfer(objVersion);	
}

void ToSerializer::transferObjSize(unsigned long &objSize)
{
	transfer(objSize);
}

//
// helper function to store char bufs
//
void ToSerializer::writeCharBufferSize(unsigned long &strSize)
{
	transfer(strSize);
}

