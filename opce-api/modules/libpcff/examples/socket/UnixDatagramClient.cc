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
#include <string.h>
#include <iostream>


#include <unistd.h>

#include <pcff/BasicException.h>
#include <pcff/BasicExceptionMgr.h>

#include <pcff/socket/SocketExceptions.h>
#include <pcff/socket/INETAddress.h>
#include <pcff/socket/SocketAddress.h>
#include <pcff/socket/Socket.h>
#include <pcff/socket/UnixDatagramSocket.h>

#include "SocketConfig.h"
#include "UnixDatagramClient.h"

using namespace std;
using namespace pcff;


//
//
UnixDatagramClient::UnixDatagramClient() 
{
}

//
//
//
UnixDatagramClient::~UnixDatagramClient() 
{
}

//
//
//
bool UnixDatagramClient::run() {
  string echoString;
  string serverPath;
  string clientPath;

  try { 
    cout << "UnixDatagramClient test starting..." << endl;

    echoString = SocketConfig::instance().getTestString();
    serverPath =   SocketConfig::instance().getServerSocketPath();
    clientPath =   SocketConfig::instance().getClientSocketPath();


    UnixDatagramSocket	clientSocket;
    char                pcRecvBuffer[65535];
    int                 iBytesTransferred;

    sprintf(pcRecvBuffer, "");

    printf("Datagram Echo Client\n\n");

    printf("Using Echo Server\n");
    clientSocket.init( serverPath.c_str() );
    clientSocket.bind( clientPath.c_str() );  // bind allows the server to get client addr in from addr in recvfrom call

    printf(" socketPath    : %s\n\n", clientSocket.socketPath().c_str());

    printf("Send Buffer    : [%s]\n", echoString.c_str() );
    printf("Receive Buffer : [%s]\n\n", pcRecvBuffer);

    printf("sending request...\n");
    iBytesTransferred = clientSocket.sendDatagram((void *)echoString.c_str(), strlen(echoString.c_str()));
    printf("sent %d bytes\n\n", iBytesTransferred);

    printf("receiving...\n");
    iBytesTransferred = clientSocket.receiveDatagram(pcRecvBuffer, iBytesTransferred);

    pcRecvBuffer[iBytesTransferred] = 0;

    printf("send Buffer    : [%s]\n", echoString.c_str());
    printf("receive Buffer : [%s]\n", pcRecvBuffer);
  }
  catch( BasicException& ex) {
    BasicExceptionMgr::instance()->handle( ex );
    return false;
  }
  catch(...) {
    BasicExceptionMgr::instance()->handle( "UnitTest::run: unknown ERROR" );
    return false; 
  }

   cout << "UnixDatagramClient test complete." << endl;

  return true;
}
