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


#include <sstream>
#include <string>

#include <pcff/BasicMessage.h>

using namespace std;
using namespace pcff;

BasicMessage::BasicMessage()
{
	initMembers();
}

BasicMessage::~BasicMessage()
{
}

void BasicMessage::initMembers()
{
	_msgType = 0; 
	_msgSize = 0;
	_dataType = 0;
	_dataSize = 0;
//	_data = NULL;
}

void BasicMessage::init()
{
	// TODO: debug for now, must change
	_msgType = BasicMessage::Connect;
}

void BasicMessage::init(int msgType, long msgSize, int dataType, long dataSize)
{
	
}

void BasicMessage::setTestData()
{
	_msgType = BasicMessage::Test; 
	_msgSize = 1;
	_dataType = 2;
	_dataSize = 3;
//	_data = NULL;
}

void BasicMessage::serialize(Serializer &s)
{
	s.transfer(this->_msgType);
	s.transfer(this->_msgSize);
	s.transfer(this->_dataType);
	s.transfer(this->_dataSize);
//	s.transfer(this->_data, _dataSize);	
}

void BasicMessage::getInfo(string &str)
{
	ostringstream ss;
	
	ss << endl << \
	"msgType:  " << _msgType << endl << \
    "msgSize:  " << _msgSize << endl << \
    "dataType: " << _dataType << endl << \
    "dataSize: " << _dataSize << endl;
	
	str =  ss.str();
}

