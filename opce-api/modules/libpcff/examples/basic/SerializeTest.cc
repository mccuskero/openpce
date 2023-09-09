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
#include <iostream>

#include "SerializeTest.h"

using namespace std;
using namespace pcff;

SerializeTest::SerializeTest()
{
	initMembers();
}

SerializeTest::~SerializeTest()
{
}

void SerializeTest::initMembers()
{
	_testBool 	= false;
	_testChar 	= 0;
	_testUChar 	= 0;
	_testInt 	= 0;
	_testUInt	= 0;
	_testLong	= 0;
	_testULong	= 0;
	_testFloat	= 0.0;
	_testLongLong = 0LL;
	_testULongLong = 0ULL;
	_testDouble	= 0.0;
	_testStr	= "";
	_testCharBuf = NULL;
	_testDataBuf = NULL;
}

void SerializeTest::setTestData()
{
	_testBool 	= true;
	_testChar 	= 0xF5;
	_testUChar 	= 0xF6;
	_testInt 	= 1;
	_testUInt	= 2;
	_testLong	= 3;
	_testULong	= 4;
	_testFloat	= 3.14;
	_testLongLong = 5LL;
	_testULongLong = 6ULL;
	_testDouble	= 6.022;
	_testStr	= "Hello String";
	_testCharBuf = strdup("Hello char buf");
	_testCharBuf2[0] = 'a';
	_testCharBuf2[1] = 'a';
	_testCharBuf2[2] = 'a';
	_testCharBuf2[3] = 'a';
	_testCharBuf2[4] = 'a';
	_testCharBuf2[5] = 0;
	_testCharBuf3[0] = 'b';
	_testCharBuf3[1] = 'b';
	_testCharBuf3[2] = 'b';
	_testCharBuf3[3] = 'b';
	_testCharBuf3[4] = 'b';
	_testCharBuf3[5] = 0;
	_testDataBufLen = 15;  // this buffer happend to be a char * with NULL, need +1 for 15
	_testDataBuf = (unsigned char *)strdup("Hello data buf");
}

void SerializeTest::serialize(Serializer &s)
{
	unsigned long strLen = 0;
	
	s.transfer(this->_testBool);
	s.transfer(this->_testChar);
	s.transfer(this->_testUChar);
	s.transfer(this->_testInt);
	s.transfer(this->_testUInt);
	s.transfer(this->_testLong);
	s.transfer(this->_testULong);
	s.transfer(this->_testFloat);
	s.transfer(this->_testLongLong);
	s.transfer(this->_testULongLong);
	s.transfer(this->_testDouble);
	s.transfer(this->_testStr);
	if (_testCharBuf == NULL)
	{
		strLen = 0;
	}
	else
	{
		strLen = strlen(_testCharBuf);
	}
	s.transfer(this->_testCharBuf, strLen);
    strLen = 6;
	s.transfer(this->_testCharBuf2[0], strLen);
	s.transfer(this->_testCharBuf3[0], strLen);
	s.transfer(this->_testDataBufLen);
	s.transfer(this->_testDataBuf, _testDataBufLen);
}

void SerializeTest::getInfo(string &str)
{
	ostringstream ss;
	
	ss << endl << \
	"_testBool 		 : " << _testBool << endl << \
	"_testChar		 : " << _testChar << endl << \
	"_testUChar		 : " << _testUChar << endl << \
	"_testInt		 : " << _testInt << endl << \
	"_testUInt		 : "	<< _testUInt << endl << \
	"_testLong		 : "	<< _testLong << endl << \
	"_testULong		 : "	<< _testULong << endl << \
	"_testFloat		 : "	<< _testFloat << endl << \
	"_testLongLong	 : "	<< _testLongLong << endl << \
	"_testULongLong	 : "	<< _testULongLong << endl << \
	"_testDouble	 : "	<< _testDouble << endl << \
	"_testStr		 : " << _testStr << endl << \
	"_testCharBuf	 : " << _testCharBuf << endl <<\
	"_testCharBuf2	 : " << _testCharBuf2 << endl <<\
	"_testCharBuf3	 : " << _testCharBuf3 << endl <<\
	"_testDataBufLen : " << _testDataBufLen << endl <<\
	"_testDataBuf 	 : " << _testDataBuf << endl;

	str =  ss.str();
}
