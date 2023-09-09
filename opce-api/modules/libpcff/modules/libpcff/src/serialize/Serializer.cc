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

#include <iostream>

#include <pcff/serialize/Serializer.h>

using namespace std;
using namespace pcff;

int Serializer::_mode = Serializer::BigEndian;

const unsigned char Serializer::SID = 0xF5;
const unsigned char Serializer::True = 1;
const unsigned char Serializer::False = 0;

Serializer::Serializer()
{
	initMembers();
}

Serializer::~Serializer()
{
}

void Serializer::initMembers()
{
	_mode 		= Serializer::BigEndian;
	_buffer 	= NULL;
	_bOverflow 	= false;
	_bytesUsed 	= 0L;
	_size 		= 0L;
	_serializedSize = 0L;
	_objectVersion	= 0;
	_serializationID = Serializer::SID;
}

void Serializer::initialize(unsigned char *buf, unsigned long size)
{
	_buffer = buf;
	_size = size;
}

// TODO: move these to the header file
// TODO: need to understand if you need to swap...
// 		Write
//     little to big	yes
//     big to big     	no
//
//		Read
//	   big to little 	yes
//	   big to big		no

// TODO: make inline 
bool Serializer::byteSwap()
{
	bool retVal = false;
	
	if (Serializer::getMode() == BigEndian)
	{
		// if system is not BigEndian
		if (Serializer::bigEndian() != true)
		{
			retVal = true;
		}
	}
	// if mode is little endian
	else
	{
		// if system is  BigEndian
		if (Serializer::bigEndian() == true)
		{
			retVal = true;
		}
	}
	
	return retVal;
}

bool Serializer::bigEndian()
{
    long one= 1;
    return (!(*((char *)(&one))) ? true : false);
}

void Serializer::endianSwap(u_int16_t  &x)
{
	if (Serializer::byteSwap() == false) return;
	
    x = (x>>8) | 
        (x<<8);
}

void Serializer::endianSwap(u_int32_t &x)
{
	if (Serializer::byteSwap() == false) return;
	
    x = (x>>24) | 
        ((x<<8) & 0x00FF0000) |
        ((x>>8) & 0x0000FF00) |
        (x<<24);
}

void Serializer::endianSwap(float &f)
{
	if (Serializer::byteSwap() == false) return;
	u_int32_t x = f;
	
    x = (x>>24) | 
        ((x<<8) & 0x00FF0000) |
        ((x>>8) & 0x0000FF00) |
        (x<<24);
}

// __int64 for MSVC, "long long" for gcc
void Serializer::endianSwap(u_int64_t &x)
{
	if (Serializer::byteSwap() == false) return;
	
    x = (x>>56) | 
        ((x<<40) & 0x00FF000000000000ULL) |
        ((x<<24) & 0x0000FF0000000000ULL) |
        ((x<<8)  & 0x000000FF00000000ULL) |
        ((x>>8)  & 0x00000000FF000000ULL) |
        ((x>>24) & 0x0000000000FF0000ULL) |
        ((x>>40) & 0x000000000000FF00ULL) |
        (x<<56);
}

void Serializer::endianSwap(double &d)
{
	
	if (Serializer::byteSwap() == false) return;
	
	u_int64_t x = d;
    x = (x>>56) | 
        ((x<<40) & 0x00FF000000000000ULL) |
        ((x<<24) & 0x0000FF0000000000ULL) |
        ((x<<8)  & 0x000000FF00000000ULL) |
        ((x>>8)  & 0x00000000FF000000ULL) |
        ((x>>24) & 0x0000000000FF0000ULL) |
        ((x>>40) & 0x000000000000FF00ULL) |
        (x<<56);
}

u_int16_t Serializer::Read16(u_int16_t &buf) 
{
	u_int16_t val = buf;
	
	endianSwap(val);
	
	return val;
}

u_int32_t Serializer::Read32Int(u_int32_t &buf) 
{
	u_int32_t val = buf;
	
//	cout << "b val: " << buf << endl;
	endianSwap(val);
//	cout << "a val: " << buf << endl;
	
	return val;
}

float Serializer::Read32(float &buf) 
{
	u_int32_t val = buf;

	endianSwap(val);
	
	return val;
}

u_int64_t Serializer::Read64Int(u_int64_t &buf) 
{
	u_int64_t val = buf;

	endianSwap(val);

	return val;
}

double Serializer::Read64(double &buf) 
{
	u_int64_t val = buf;

	endianSwap(val);
	
	return val;
}

//
// Destructive reads are a little faster the regular reads
// but the swap the passed in bytes in the reference.
// These should not be used to check e.g. MessageHeaderSerializer 
// size before actual reading 
// The FromSerializer should be able to use this 
//
u_int16_t Serializer::Read16Destructive(u_int16_t &buf) 
{	
	endianSwap(buf);
	
	return buf;
}

u_int32_t Serializer::Read32IntDestructive(u_int32_t &buf) 
{
//	cout << "b val: " << buf << endl;
	endianSwap(buf);
//	cout << "a val: " << buf << endl;
	
	return buf;
}

float Serializer::Read32Destructive(float &buf) 
{
	endianSwap(buf);
	
	return buf;
}

u_int64_t Serializer::Read64IntDestructive(u_int64_t &buf) 
{
	endianSwap(buf);
	
	return buf;
}

double Serializer::Read64Destructive(double &buf) 
{
	endianSwap(buf);
	
	return buf;
}


void Serializer::Write16(u_int16_t val, u_int16_t &buf) 
{
	endianSwap(val);
	buf=val;
}

void Serializer::Write32Int(u_int32_t val, u_int32_t &buf) 
{
//	cout << "b val: " << val << endl;
	endianSwap(val);
//	cout << "a val: " << val << endl;
	buf=val;
}

void Serializer::Write32(float val, float &buf) 
{
	endianSwap(val);
	buf=val;
}

void Serializer::Write64Int(u_int64_t val, u_int64_t &buf) 
{
	endianSwap(val);
	buf=val;
}

void Serializer::Write64(double val, double &buf) 
{
	endianSwap(val);
	buf=val;
}
