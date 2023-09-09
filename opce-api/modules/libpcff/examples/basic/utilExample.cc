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
#include <stdlib.h>
#include <sstream>
#include <iostream>

#include <pcff/BasicExceptionMgr.h>
#include <pcff/Timer.h>
#include <pcff/LogLevel.h>
#include <pcff/DumpBuffer.h>
#include <pcff/BasicMessage.h>
#include <pcff/filesystem/Path.h>
#include <pcff/filesystem/PathException.h>
#include <pcff/socket/INETAddress.h>
#include <pcff/socket/CIDRAddress.h>


#include "SerializeTest.h"
#include "TestException.h"
#include "TestConfig.h"

using namespace std;
using namespace pcff;

static int MAX_I = 1000;
static int MAX_J = 1000;

static bool pathTest();
static bool timerExample();
static void serializationExampleINETAddress();
static void serializationExample();
static void serializationBasicMessageExample();
static void randomINETAddressTest();
static void cidrAddressTest();

int main(int argc, char** argv)
{
    LogLevel *myLog = NULL;

    BasicExceptionMgr::instance();

    try
    {
        myLog = new LogLevel(Log::logToFile | Log::logToCrt, 1000);
        myLog->init("test.log");
        myLog->logMessage("Opening log");
        BasicExceptionMgr::instance()->setLog(myLog);

        TestConfig::instance().init();

        if (TestConfig::instance().showUsageString(argc, argv) == true)
        {
            exit(0);
        }

        if (TestConfig::instance().showVersionString(argc, argv) == true)
        {
            exit(0);
        }

        if (TestConfig::instance().readConfigFile(argc, argv) == false)
        {
            TestConfig::instance().readConfigFile("test.conf");
        }

        TestConfig::instance().parseCommandLine(argc, argv);
        TestConfig::instance().showInitialValues();

        pathTest();

        timerExample();

        cidrAddressTest();

        randomINETAddressTest();

        serializationExample();

        serializationExampleINETAddress();

        serializationBasicMessageExample();

        throw TestException(TestException::Test1, "This is a test");
    }
    catch (TestException& ex)
    {
        BasicExceptionMgr::instance()->handle(ex);
    }
    catch (BasicException& ex)
    {
        BasicExceptionMgr::instance()->handle(ex);
    }
    catch (...)
    {
        BasicExceptionMgr::instance()->handle("Unknown exception");
    }
}

bool pathTest()
{

    string pathname = TestConfig::instance().getPathname();

    BasicExceptionMgr::instance()->log("Starting filesystem framework test");
    BasicExceptionMgr::instance()->log(pathname.c_str());

    try
    {
        Path testPath;
        testPath.initialize(pathname);
        testPath.print();
    }
    catch (PathException &ex)
    {
        BasicExceptionMgr::instance()->handle(ex);
    }
    catch (BasicException& ex)
    {
        BasicExceptionMgr::instance()->handle(ex);
    }
    catch (...)
    {
        BasicExceptionMgr::instance()->handle("pathTest: Unknown exception");
    }

    BasicExceptionMgr::instance()->log("Ending filesystem framework test");
}

bool timerExample()
{

    try
    {
        char buf[100];
        int h = 0;
        Timer testTimer;

        testTimer.start();

        // take up some time
        for (int i = 0; i < MAX_I; i++)
            for (int j = 0; j < MAX_J; j++)
                memset(buf, 0, sizeof (buf));

        testTimer.stop();
        cout << "Elapsed time is: ";
        cout << testTimer.getElapsedTimeAsString();
        cout << endl;

        testTimer.start();

        // take up some time
        for (int i = 0; i < MAX_I; i++)
            for (int j = 0; j < MAX_J; j++)
                h++;

        testTimer.stop();
        cout << "Elapsed time is: ";
        cout << testTimer.getElapsedTimeAsString();
        cout << endl;

        testTimer.start();

        // take up some time
        sleep(1);

        testTimer.stop();
        cout << "Elapsed time is: ";
        cout << testTimer.getElapsedTimeAsString();
        cout << endl;
    }
    catch (BasicException& ex)
    {
        BasicExceptionMgr::instance()->handle(ex);
        return false;
    }
    catch (...)
    {
        BasicExceptionMgr::instance()->handle("Units_UnitTest::run: unknown ERROR");
        return false;
    }

    return true;
}

void cidrAddressTest()
{
    string sSubnet     = "192.168.10.255";
    string sIP        = "192.168.10.10";
    string sIP2        = "192.169.10.10";
    int    cidrBits   = 24;

    CIDRAddress subnet;

    BasicExceptionMgr::instance()->log("Starting CIDRAddress test");

    subnet.initialize(sSubnet, cidrBits);


    if ( subnet.isIPAddrInSubnet(sIP) == true )
    {
        BasicExceptionMgr::instance()->log(LogLevel::NotInitialized, "IP Address %s is in subnet %s/%d", sIP.c_str(), subnet.getAddressAsStr(), subnet.getCIDRValue() );
    }
    else
    {
        BasicExceptionMgr::instance()->log(LogLevel::NotInitialized, "IP Address %s is NOT in subnet %s/%d", sIP.c_str(), subnet.getAddressAsStr(), subnet.getCIDRValue() );
    }


        if ( subnet.isIPAddrInSubnet(sIP2) == true )
    {
        BasicExceptionMgr::instance()->log(LogLevel::NotInitialized, "IP Address %s is in subnet %s/%d", sIP2.c_str(), subnet.getAddressAsStr(), subnet.getCIDRValue() );
    }
    else
    {
        BasicExceptionMgr::instance()->log(LogLevel::NotInitialized, "IP Address %s is NOT in subnet %s/%d", sIP2.c_str(), subnet.getAddressAsStr(), subnet.getCIDRValue() );
    }

    BasicExceptionMgr::instance()->log("Ending CIDRAddress test");
}

void randomINETAddressTest()
{
    string inetAddr;
    BasicExceptionMgr::instance()->log("Starting random INETAddress test");

    for (int i = 0; i < 30; i++)
    {
        inetAddr = INETAddress::createInetAddrIPv4();
        cout << inetAddr << endl;
    }

}

void serializationExampleINETAddress()
{
    // TODO: make SerializationTest contain another class, and serialize inner class
    try
    {
        string infoStr;
        ostringstream ss;

        unsigned long bufLen = 0;
        unsigned long serialSize = 0;
        unsigned char *buf = NULL; // Serialize base class deletes its own buffer
        INETAddress test;
        INETAddress test2;

        BasicExceptionMgr::instance()->log("Starting Serialization Test - SerializationTest INETAddress");

        test.setTestData();

        BasicExceptionMgr::instance()->log("test info:");
        test.getInfo(infoStr);
        BasicExceptionMgr::instance()->log(infoStr.c_str());

        serialSize = test.getSerializeSize();
        ss << "serializedSize: (" << serialSize << ")";
        BasicExceptionMgr::instance()->log(ss.str().c_str());

        // serialize the object to a buffer
        buf = test.serializeTo(bufLen);

        // dump the stream to the console
        DumpBuffer::instance().init();
        DumpBuffer::instance().dump(bufLen, (char *) buf);

        ss << "returned serializedSize: (" << bufLen << ")";
        BasicExceptionMgr::instance()->log(ss.str().c_str());

        // initialize the object from the buffer
        test2.serializeFrom(buf, bufLen);

        BasicExceptionMgr::instance()->log("test2 info:");
        test2.getInfo(infoStr);
        BasicExceptionMgr::instance()->log(infoStr.c_str());

        BasicExceptionMgr::instance()->log("Ending Serialization Test");
    }
    catch (BasicException& ex)
    {
        BasicExceptionMgr::instance()->handle(ex);
    }
    catch (...)
    {
        BasicExceptionMgr::instance()->handle("Units_UnitTest::run: unknown ERROR");
    }

}

void serializationExample()
{
    // TODO: make SerializationTest contain another class, and serialize inner class
    try
    {
        string infoStr;
        ostringstream ss;

        unsigned long bufLen = 0;
        unsigned long serialSize = 0;
        unsigned char *buf = NULL; // Serialize base class deletes its own buffer
        SerializeTest test;
        SerializeTest test2;

        BasicExceptionMgr::instance()->log("Starting Serialization Test - SerializationTest");

        test.setTestData();

        BasicExceptionMgr::instance()->log("test info:");
        test.getInfo(infoStr);
        BasicExceptionMgr::instance()->log(infoStr.c_str());

        serialSize = test.getSerializeSize();
        ss << "serializedSize: (" << serialSize << ")";
        BasicExceptionMgr::instance()->log(ss.str().c_str());

        // serialize the object to a buffer
        buf = test.serializeTo(bufLen);

        // dump the stream to the console
        DumpBuffer::instance().init();
        DumpBuffer::instance().dump(bufLen, (char *) buf);

        ss << "returned serializedSize: (" << bufLen << ")";
        BasicExceptionMgr::instance()->log(ss.str().c_str());

        // initialize the object from the buffer
        test2.serializeFrom(buf, bufLen);

        BasicExceptionMgr::instance()->log("test2 info:");
        test2.getInfo(infoStr);
        BasicExceptionMgr::instance()->log(infoStr.c_str());

        BasicExceptionMgr::instance()->log("Ending Serialization Test");
    }
    catch (BasicException& ex)
    {
        BasicExceptionMgr::instance()->handle(ex);
    }
    catch (...)
    {
        BasicExceptionMgr::instance()->handle("Units_UnitTest::run: unknown ERROR");
    }
}

void serializationBasicMessageExample()
{
    try
    {
        ostringstream ss;
        string infoStr;
        unsigned long bufLen = 0;
        unsigned long serialSize = 0;
        unsigned char *buf = NULL; // Serialize base class deletes its own buffer
        BasicMessage msg;
        BasicMessage msg2;

        BasicExceptionMgr::instance()->log("Starting Serization Test - Basic Message");

        msg.init();
        msg.setTestData();

        BasicExceptionMgr::instance()->log("msg info:");
        msg.getInfo(infoStr);
        BasicExceptionMgr::instance()->log(infoStr.c_str());

        serialSize = msg.getSerializeSize();
        ss << "serializedSize: (" << serialSize << ")";
        BasicExceptionMgr::instance()->log(ss.str().c_str());


        buf = msg.serializeTo(bufLen);

        DumpBuffer::instance().init();
        DumpBuffer::instance().dump(bufLen, (char *) buf);

        ss << "serializedSize: (" << bufLen << ")";
        BasicExceptionMgr::instance()->log(ss.str().c_str());

        msg2.serializeFrom(buf, bufLen);

        BasicExceptionMgr::instance()->log("msg2 info:");
        msg2.getInfo(infoStr);
        BasicExceptionMgr::instance()->log(infoStr.c_str());

        BasicExceptionMgr::instance()->log("Ending Serization Test");
    }
    catch (BasicException& ex)
    {
        BasicExceptionMgr::instance()->handle(ex);
    }
    catch (...)
    {
        BasicExceptionMgr::instance()->handle("Units_UnitTest::run: unknown ERROR");
    }
}
