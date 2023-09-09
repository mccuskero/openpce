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
#include <iostream>

#include <pcff/TestException.h>
#include <pcff/BasicExceptionMgr.h>

#include <pcff/socket/SocketExceptions.h>
#include <pcff/socket/INETAddress.h>
#include <pcff/socket/SocketAddress.h>
#include <pcff/socket/DatagramSocket.h>

#include "SocketConfig.h"
#include "UDPEchoServer.h"

using namespace std;
using namespace pcff;

//
//
UDPEchoServer::UDPEchoServer() 
{
}

//
//
//
UDPEchoServer::~UDPEchoServer() 
{
}

//
//
//
bool UDPEchoServer::run() {
  int portNum = 0;
  int family = DatagramSocket::AF_INET_IPv4;

  try 
  {
  cout << "UDPEchoServer test starting..." << endl;
    portNum =    SocketConfig::instance().getPort();

    if (SocketConfig::instance().getIPVersion() == SocketConfig::IPv6) 
       family = DatagramSocket::AF_INET_IPv6;

    DatagramSocket      serverSocket(family);
    INETAddress         sourceIPAddr;
    int                 bytesTransferred, sourcePort;
    char                pBuf[65535];

    sprintf(pBuf, "");

    printf("Datagram echo server\n\n");
    // TODO: check
    serverSocket.init();
    serverSocket.bind( portNum );
    serverSocket.localINETAddr().print();
    printf(" port    : %d\n\n", serverSocket.localPort());

    for (;;)
      {
        printf("waiting... :\n");
        bytesTransferred = serverSocket.receiveDatagram(pBuf, 
							65535, 
							sourceIPAddr, 
							sourcePort);
        printf("received datagram from :");
        sourceIPAddr.print();
        printf(" - port    : %d\n", sourcePort);
        printf(" - size    : %d bytes\n", bytesTransferred);
        printf(" - buffer  : [");
        for (int i = 0; i < bytesTransferred; i++)
	  {
            printf("%c", pBuf[i]);
	  }
        printf("]\n\n");
	

        printf("echoing datagram...\n\n");
	
        bytesTransferred = serverSocket.sendDatagram(pBuf, 
						     bytesTransferred, 
						     sourceIPAddr, 
						     sourcePort);
        printf("sent %d bytes\n\n", bytesTransferred);
      }      
    }
  catch(TestException& ex)
    {
      BasicExceptionMgr::instance()->handle( ex );
    }
  catch(BasicException& ex)
    {
      BasicExceptionMgr::instance()->handle( ex );
    }
  catch(...)
    {
      BasicExceptionMgr::instance()->handle( "Unknown exception" );
    }
  

  cout << "UDPEchoServer test complete." << endl;
  return true;
}
