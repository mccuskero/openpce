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

#include "TestEvent.h"

using namespace std;
using namespace pcff;

TestEvent::TestEvent()
{
	initMembers();
}

TestEvent::~TestEvent()
{
}

void TestEvent::initMembers()
{
	_signatureGenerator = 0;
	_signatureID = 0;
	_classificationID = 0;
	_priority = 0;
	_eventID = 0;
	
	_rawTestDataSize = 0;
}
	
void TestEvent::initialize()
{
	
}
	
void TestEvent::setTestData()
{
	unsigned char *rawTestData = NULL;
	
	_signatureGenerator = 1;
	_signatureID = 2;
	_classificationID = 3;
	_priority = 4;
	_eventID = 5;	

	if (_rawTestDataSize > 0)
	{
		rawTestData = new unsigned char[_rawTestDataSize];
		memset(rawTestData, 'a', _rawTestDataSize);
		setRawEvent(rawTestData, _rawTestDataSize);
		delete rawTestData;
	}
}

void TestEvent::serialize(Serializer &s)
{
	s.transfer(this->_signatureGenerator);
	s.transfer(this->_signatureID);
	s.transfer(this->_classificationID);
	s.transfer(this->_priority);
	s.transfer(this->_eventID);
	Event::serialize(s);
}

const char *TestEvent::getEventInfo()
{
	ostringstream eventInfo;
	
	eventInfo << endl << "_signatureGenerator: " << _signatureGenerator << endl;
	eventInfo << "_signatureID:        " << _signatureID << endl;
	eventInfo << "_classificationID:   " << _classificationID << endl;
	eventInfo << "_priority:           " << _priority << endl;
	eventInfo << "_eventID:            " << _eventID << endl;

	return eventInfo.str().c_str();
}

