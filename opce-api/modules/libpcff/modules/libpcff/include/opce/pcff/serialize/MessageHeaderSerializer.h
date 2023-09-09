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

#ifndef MESSAGEHEADERSERIALIZER_H_
#define MESSAGEHEADERSERIALIZER_H_

#include <string>

#include <pcff/AttributeValuePair.h>
#include <pcff/serialize/Serializer.h>

namespace pcff
{

class MessageHeaderSerializer : public Serializer
{
public:
	MessageHeaderSerializer();
	virtual ~MessageHeaderSerializer();

public:
	void initialize(unsigned char* buf, long unsigned buflen);
	
public:
	virtual void transferSerializedID(unsigned char &sID);
	virtual void transferObjVersion(int &objVersion);
	virtual void transferObjSize(unsigned long &objSize);

public:
	virtual void transfer(bool &val);
	virtual void transfer(char &val);
	virtual void transfer(unsigned char &val);
	virtual void transfer(u_int16_t &val);
	virtual void transfer(int &val);
	virtual void transfer(unsigned int &val);
	virtual void transfer(long &val);
	virtual void transfer(unsigned long &val);
	virtual void transfer(float &val);
	virtual void transfer(long long &val);
	virtual void transfer(unsigned long long &val);
	virtual void transfer(double &val);
	virtual void transfer(std::string &val);
	virtual void transfer(u_int8_t &buf, unsigned long &size);
	virtual void transfer(char &buf, unsigned long &size);
	virtual void transfer(char *&buf, unsigned long &size);
	virtual void transfer(unsigned char *&buf, unsigned long &size);
	virtual void transfer(AttributeValuePair &val);
};

};

#endif /*MESSAGEHEADERSERIALIZER_H_*/
