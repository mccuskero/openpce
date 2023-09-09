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

#ifndef SERIALIZETEST_H_
#define SERIALIZETEST_H_

#include <pcff/serialize/Serializeable.h>

class SerializeTest : public pcff::Serializeable
{
public:
	SerializeTest();
	virtual ~SerializeTest();
	
private:
	void initMembers();
	
public:
	void setTestData();
	void getInfo(std::string &str);
	
public:
	virtual void  serialize(pcff::Serializer &s);

private:
	bool			_testBool;
	char			_testChar;
	unsigned char	_testUChar;
	int				_testInt;
	unsigned int	_testUInt;
	long			_testLong;
	unsigned long	_testULong;
	float			_testFloat;
	long long       	_testLongLong;
	unsigned long long  _testULongLong;
	double			_testDouble;
	std::string		_testStr;
	char			*_testCharBuf;
	char			_testCharBuf2[6];
	u_int8_t		_testCharBuf3[6];
	unsigned long   _testDataBufLen;
	unsigned char   *_testDataBuf;
	// AttributeValuePair
};

#endif /*SERIALIZETEST_H_*/
