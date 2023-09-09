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

#include <pcff/BasicException.h>

#include "ThreadConfig.h"

using namespace std;
using namespace pcff;

ThreadConfig *ThreadConfig::_instance = NULL;

ThreadConfig &ThreadConfig::instance() {
    if( _instance == NULL ) {
        _instance = new ThreadConfig();
        _instance->initAttributes();
    }
    
    return *_instance;
}

    
ThreadConfig::ThreadConfig() {
    
}

ThreadConfig::~ThreadConfig() {
    
}

// TODO: switch last two parameters into setAttribute to us string contution "/" char. for long descriptions
void ThreadConfig::initAttributes() {
    setAttribute("testType",    Config::STR, Config::BOTH, "basic, contain",  "specifies the test type to be done\n" \
                                                                        "types: basic, contain, pool, derivedPool");
    setAttribute("numThreads",  Config::INT,    Config::BOTH, "5",      "number of threads");
    setAttribute("iterations",  Config::INT,    Config::BOTH, "3",      "number of iterations within each loop");
}

void ThreadConfig::printVersionHelper() {
    cout << "threadTest (GCC) 0.0.2 (Linux)" << endl;
    cout << "Copyright (C) 2004 Free Software Foundation, Inc." << endl;
    cout << "This is free software; see the source for copying conditions.  There is NO" << endl;
    cout << "warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE." << endl << endl;
}

void ThreadConfig::printUsageHelper(char *) {
   cout << " Usage: threadTest [-t <testType>] [-n <numThreads>] [-i <iterations>] [-help -?]" << endl; 
}

int ThreadConfig::getTestType() {
   string sType = m_ConfigOptions["testType"].getValue();
   
   if (sType.compare("basic") == 0) {
      _testType = BasicTest;
   }
   else if (sType.compare("contain") == 0) {
      _testType = ContainmentTest;
   }
   else if (sType.compare("pool") == 0) {
      _testType = PoolTest;
   }
   else if (sType.compare("derivedPool") == 0) {
      _testType = DerivedPoolTest;
   }
   else {
      _testType = UnknownTest;
   }
   
   return _testType;
} 

int ThreadConfig::getIterations() {
    return m_ConfigOptions["iterations"].getValueAsInt();
}

int ThreadConfig::getNumThreads() {
    return m_ConfigOptions["numThreads"].getValueAsInt();
}
