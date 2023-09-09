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

#include <pcff/serialize/GetSizeSerializer.h>

using namespace pcff;

void GetSizeSerializer::initialize()
{
	transferSerializedID(_serializationID);	
	transferObjVersion(_objectVersion);	
	transferObjSize(_serializedSize);
}

void GetSizeSerializer::transfer(bool &val)
{
	_size++;  // TODO: save as unsigned char
}

void GetSizeSerializer::transfer(char &val)
{
	_size++;  
}

void GetSizeSerializer::transfer(unsigned char &val)
{
	_size++;  
}

void GetSizeSerializer::transfer(u_int16_t &val)
{
	_size+=2;  
}

void GetSizeSerializer::transfer(int &val)
{
	_size+=sizeof(int);  
}

void GetSizeSerializer::transfer(unsigned int &val)
{
	_size+=sizeof(unsigned int);  
}

void GetSizeSerializer::transfer(long &val)
{
	_size+=4;
}

void GetSizeSerializer::transfer(unsigned long &val)
{
	_size+=4;  
}

void GetSizeSerializer::transfer(float &val)
{
	_size+=4;  
}

void GetSizeSerializer::transfer(long long &val)
{
	_size+=8;  
}

void GetSizeSerializer::transfer(unsigned long long &val)
{
	_size+=8;  
}

void GetSizeSerializer::transfer(double &val)
{
	_size+=8;  
}

void GetSizeSerializer::transfer(std::string &val)
{
	unsigned long size = val.length();
	_size+=size; 
	_size+=4;     // for string size written to buffer
}

void GetSizeSerializer::transfer(u_int8_t &buf, unsigned long &size)
{
	_size+=size;
	_size+=4;     // for string size written to buffer
}

void GetSizeSerializer::transfer(char &buf, unsigned long &size)
{
	_size+=size;
	_size+=4;     // for string size written to buffer
}

void GetSizeSerializer::transfer(char *&buf, unsigned long &size)
{
	_size+=size;
	_size+=4;     // for string size written to buffer
}

void GetSizeSerializer::transfer(unsigned char *&buf, unsigned long &size)
{
	_size+=size;
	_size+=4;     // for string size written to buffer
}

void GetSizeSerializer::transfer(AttributeValuePair &val)
{
	// TODO: need to implement
}

void GetSizeSerializer::transferSerializedID(unsigned char &sID)
{
	_size+=1; // unsigned char: serialization ID (0xF5)
}

void GetSizeSerializer::transferObjVersion(int &objVersion)
{
	_size+=sizeof(int);  
}

void GetSizeSerializer::transferObjSize(unsigned long &objSize)
{
	_size+=4; // unsigned long for the obj size
}
