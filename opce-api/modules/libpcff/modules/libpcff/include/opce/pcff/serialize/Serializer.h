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
// The goal of the serialization framework is to ensure that there is "one" member
// function in the derived class that manages, size, reads, and writes.
// This can reduce errors in reads/writes and determining size in the class
// for serialization.
//
////////////////////////////////////////
// Serialization Buffer Format:
//
// The pcff serialization ID (0xF5)
// is stored in the 1st byte.
//
// The object version (2 bytes) is stored
// after the pcff serialization ID
// (0xF5). Value is set to 0 by default.
//
// The object size, (4 bytes),  is stored
// after the object version, used for verification
//
// when storing string, store len, as long,
// before the string itself /0 is a legit value
// in string
//
// AttributeValue Pairs are stored as:
//	name,  (unsigned long size, char buf)
// 	type,  (int)
// 	value  (unsigned long size, char buf)
//
/////////////////////
// char bufs and strings management:
//
// when transfering strings, and char bufs, the len,
// should be pre-pended to the string, string are allowed,
// buffers may have /0 in them
//
// TODO:
// implement long long, unsigned long long, (windows __Int64)

#ifndef SERIALIZER_H_
#define SERIALIZER_H_

#include <sys/types.h>
#include <sys/time.h>
#include <string>

#include <pcff/AttributeValuePair.h>

namespace pcff
{

class Serializer
{
public:
	Serializer();
	virtual ~Serializer();

private:
	void initMembers();

public:
	void initialize(unsigned char *buf, unsigned long size);

public:
	unsigned char getSerializationID()             { return _serializationID; }
	unsigned int  setObjVersion(int objVersion)    { _objectVersion = objVersion; }
	unsigned int  getObjVersion() 		       { return _objectVersion; }
	unsigned long getSize()                        { return _size; }
	unsigned long getSerializedSize()              { return _serializedSize; }

public:
	typedef enum
	{
		BigEndian,
		LittleEndian
	} eEndianTypes;

	// derived serializer classes must override
public:
	virtual void transferSerializedID(unsigned char &sID)	= 0;
	virtual void transferObjVersion(int &objVersion)	= 0;
	virtual void transferObjSize(unsigned long &objSize)	= 0;

	// derived serializer classes must override
public:
	virtual void transfer(bool &val)          	        = 0;
	virtual void transfer(char &val)        		= 0;
	virtual void transfer(unsigned char &val) 		= 0;
	virtual void transfer(u_int16_t &val)      		= 0;
	virtual void transfer(int &val)                         = 0;
	virtual void transfer(unsigned int &val)                = 0;
	virtual void transfer(long &val) 		        = 0;
	virtual void transfer(unsigned long &val) 		= 0;
	virtual void transfer(float &val)                       = 0;
	virtual void transfer(long long &val)                  	= 0;
	virtual void transfer(unsigned long long &val)     	= 0;
	virtual void transfer(double &val)        		= 0;
	virtual void transfer(std::string &val)   		= 0;
	virtual void transfer(u_int8_t &buf, unsigned long &size)  = 0;
	virtual void transfer(char &buf, unsigned long &size)  = 0;
	virtual void transfer(char *&buf, unsigned long &size) = 0;
	virtual void transfer(unsigned char *&buf, unsigned long &size) = 0;
	virtual void transfer(AttributeValuePair &val)			= 0;

public:
	virtual u_int16_t Read16(u_int16_t &buf);
	virtual float 	  Read32(float &buf);
	virtual u_int32_t Read32Int(u_int32_t &buf);
	virtual double    Read64(double &buf);
	virtual u_int64_t Read64Int(u_int64_t &buf);

	//
	// Destructive reads are a little faster the regular reads
	// but the swap the passed in bytes in the reference.
	// These should not be used to check e.g. MessageHeaderSerializer
	// size before actual reading
	// The FromSerializer should be able to use this
	//
	virtual u_int16_t Read16Destructive(u_int16_t &buf);
	virtual float 	  Read32Destructive(float &buf);
	virtual u_int32_t Read32IntDestructive(u_int32_t &buf);
	virtual double    Read64Destructive(double &buf);
	virtual u_int64_t Read64IntDestructive(u_int64_t &buf);

	virtual void Write16(u_int16_t val, u_int16_t &buf);
	virtual void Write32(float val, float &buf);
	virtual void Write32Int(u_int32_t val, u_int32_t &buf);
	virtual void Write64(double    val, double &buf);
	virtual void Write64Int(u_int64_t val, u_int64_t &buf);

public:
	static void endianSwap(u_int16_t &x);
	static void endianSwap(u_int32_t &x);
	static void endianSwap(float &x);
	static void endianSwap(u_int64_t &x);
	static void endianSwap(double &x);

	//
	// static helper functions:
	//
public:
	static int  getMode()             { return _mode; }
	static void setBigEndianMode()    { _mode = Serializer::BigEndian; }
	static void setLittleEndianMode() { _mode = Serializer::LittleEndian; }
	static bool byteSwap();    // depending on mode will swap
	static bool bigEndian();   // check bigendianness at runtime

protected:
	unsigned char   _serializationID;
	int				_objectVersion;
	unsigned char 	*_buffer;
	bool 			_bOverflow;
	unsigned long 	_size;
	unsigned long 	_bytesUsed;
	unsigned long   _serializedSize; // read in from buffer, 1st 4 bytes

private:
	static int _mode;

// unsinged char enumerations
public:
	static const unsigned char SID;  //serialization ID
	static const unsigned char True;
	static const unsigned char False;
};

};

#endif /*SERIALIZER_H_*/
