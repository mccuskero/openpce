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

#include "TestConfig.h"

using namespace std;
using namespace pcff;

TestConfig *TestConfig::_instance = NULL;

TestConfig &TestConfig::instance() {
    if( _instance == NULL ) {
        _instance = new TestConfig();
    }

    return *_instance;
}


TestConfig::TestConfig() {

}

TestConfig::~TestConfig() {

}

void TestConfig::initAttributes() {

}

void TestConfig::init() {
    setAttribute("testType",    Config::STR, Config::BOTH, "ex", "specifies the test type to be done\n" \
                                                                        "ex    - exception test\n" \
                                                                        "timer - timer test\n");
    setAttribute("eyesWideOpen", Config::BOOL, Config::BOTH,    "true",       "sets to true/false");
    setAttribute("number",      Config::INT,   Config::BOTH,    "10000",      "sets the default value of a number");
    setAttribute("maxLogSize",  Config::LONG,  Config::BOTH,    "350000",     "sets the max log size" );
    setAttribute("pi",          Config::FLOAT, Config::FILE,    "3.1416",     "sets the default value of pi" );
    setAttribute("streamName",  Config::STR,   Config::CMDLINE, "fred",       "set the data stream name" );
    setAttribute("pathname",    Config::STR,   Config::CMDLINE, "/tmp",       "set the default pathname" );
}

void TestConfig::printVersionHelper() {
    cout << "utilTest (GCC) 0.0.1 (Linux)" << endl;
    cout << "Copyright (C) 2004 Free Software Foundation, Inc." << endl;
    cout << "This is free software; see the source for copying conditions.  There is NO" << endl;
    cout << "warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE." << endl << endl;
}

int TestConfig::getTestType() {
   int type = Unknown;
   ConfigTuple optionTuple = m_ConfigOptions["testType"];

   string sTestType = optionTuple.getValue();

   if (sTestType.compare("ex")) {
      type = TestConfig::Exception;
   }
   else if (sTestType.compare("timer")) {
      type = TestConfig::Timer;
   }

   return type;
}

bool TestConfig::getEyesOpen() {
	ConfigTuple tuple;
	tuple	= m_ConfigOptions["eyesWideOpen"];
	return tuple.getValueAsBool();
}

int  TestConfig::getNumber() {
	ConfigTuple tuple;
	tuple	= m_ConfigOptions["eyesWideOpen"];

	return tuple.getValueAsInt();
}

long TestConfig::getMaxLogSize() {
    return m_ConfigOptions["maxLogSize"].getValueAsLong();
}

float TestConfig::getPi() {
    return m_ConfigOptions["pi"].getValueAsFloat();
}

string TestConfig::getStreamName() {
    return m_ConfigOptions["streamName"].getValue();
}

string TestConfig::getPathname() {
    return m_ConfigOptions["pathname"].getValue();
}
