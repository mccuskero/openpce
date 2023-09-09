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

#include <stdio.h>
#include <string>
#include <iostream>


#include <unistd.h>

#include <pcff/BasicException.h>
#include <pcff/BasicExceptionMgr.h>

#include <pcff/socket/SocketExceptions.h>
#include <pcff/socket/INETAddress.h>
#include <pcff/socket/SocketAddress.h>
#include <pcff/socket/UnixSocket.h>

#include "SocketConfig.h"
#include "UnixClient.h"

using namespace std;
using namespace pcff;


//
//
UnixClient::UnixClient() 
{
}

//
//
//
UnixClient::~UnixClient() 
{
}

//
//
//
bool UnixClient::run() {
  string echoString;
  string socketPath;

  try 
  {
  cout << "UnixClient test starting..." << endl;

    socketPath =   SocketConfig::instance().getServerSocketPath();
    echoString = SocketConfig::instance().getTestString();

    UnixSocket     clientSocket;
    char          recvBuf[65535];
    int           numBytesSent;

    sprintf(recvBuf, "");

    printf("UnixSocket echo client\n\n");

    printf("Using echo server");

    clientSocket.init();
    clientSocket.connect( socketPath.c_str() );
    printf(" socketPath    : %s\n\n", clientSocket.socketPath().c_str());

    printf("send buffer    : [%s]\n", echoString.c_str());

    numBytesSent = clientSocket.send((void *)echoString.c_str(), strlen(echoString.c_str()));
    printf("sent %d bytes \n\n", numBytesSent);

    numBytesSent = clientSocket.recv(recvBuf, 65535);
    printf("received %d bytes\n", numBytesSent);
    recvBuf[numBytesSent] = 0;

    printf("send buffer    : [%s]\n", echoString.c_str());
    printf("receive buffer : [%s]\n", recvBuf);
  }
  catch( BasicException& ex) {
    BasicExceptionMgr::instance()->handle( ex );
    return false;
  }
  catch(...) {
    BasicExceptionMgr::instance()->handle( "UnitTest::run: unknown ERROR" );
    return false; 
  }


  cout << "UnixClient test complete." << endl;

  return true;
}
