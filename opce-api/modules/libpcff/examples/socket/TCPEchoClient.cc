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
#include <pcff/socket/Socket.h>

#include "SocketConfig.h"
#include "TCPEchoClient.h"

using namespace std;
using namespace pcff;


//
//
TCPEchoClient::TCPEchoClient() 
{
}

//
//
//
TCPEchoClient::~TCPEchoClient() 
{
}

//
//
//
bool TCPEchoClient::run() {
  int    portNum = 0;
  string echoString;
  string hostname;
  int    addrFamily = Socket::AF_INET_IPv4;

  try 
  {
  cout << "TCPEchoClient test starting..." << endl;

    portNum =    SocketConfig::instance().getPort();
    echoString = SocketConfig::instance().getTestString();
    hostname =   SocketConfig::instance().getHost();

   if (SocketConfig::instance().getIPVersion() == SocketConfig::IPv6) 
     addrFamily = Socket::AF_INET_IPv6;

    INETAddress serverINETAddr(addrFamily);

    serverINETAddr.setHostname(hostname.c_str());

    Socket     clientSocket(addrFamily);
    char          recvBuf[65535];
    int           numBytesSent;

    sprintf(recvBuf, "");

    printf("Socket echo client\n\n");

    printf("Using echo server");
    serverINETAddr.print();
    printf(" - port    : %d\n\n", portNum);

    // TODO: check
    clientSocket.init();

    clientSocket.connect( serverINETAddr, portNum);

    printf("local socket info...");
    clientSocket.localINETAddr().print();
    printf(" - Port    : %d\n\n", clientSocket.localPort());

    printf("server socket info...");
    clientSocket.remoteINETAddr().print();
    printf(" - port    : %d\n\n", clientSocket.remotePort());

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


  cout << "TCPEchoClient test complete." << endl;

  return true;
}
