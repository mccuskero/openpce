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
#include <pcff/socket/UnixDatagramSocket.h>

#include "SocketConfig.h"
#include "UnixDatagramServer.h"

using namespace std;
using namespace pcff;

//
//
UnixDatagramServer::UnixDatagramServer() 
{
}

//
//
//
UnixDatagramServer::~UnixDatagramServer() 
{
}

//
//
//
bool UnixDatagramServer::run() {
  string socketPath;

  try 
  {
  cout << "UnixDatagramServer test starting..." << endl;
    socketPath =    SocketConfig::instance().getServerSocketPath();


    UnixDatagramSocket	serverSocket;
    INETAddress         sourceIPAddr;
    int                 bytesTransferred, bytesRcvd, sourcePort;
    char                pBuf[65535];

    sprintf(pBuf, "");

   printf("BaseSocket::AF_INET_IPv4 %d\n", BaseSocket::AF_INET_IPv4);
   printf("BaseSocket::AF_UNIX_DOMAIN %d\n", BaseSocket::AF_UNIX_DOMAIN);

    printf("Datagram echo server\n\n");
    // TODO: check
    serverSocket.init();
    serverSocket.bind(socketPath.c_str());
    printf(" socketPath    : %s\n\n", serverSocket.socketPath().c_str());

    for (;;)
      {
        printf("waiting... :\n");
        bytesRcvd = serverSocket.receiveDatagram(pBuf, 65535);
        printf("received datagram :");
        printf(" - size    : %d bytes\n", bytesRcvd);
        printf(" - buffer  : [");
        for (int i = 0; i < bytesRcvd; i++)
	    {
            printf("%c", pBuf[i]);
	    }
        printf("]\n\n");
	
		sleep(1);
        printf("echoing datagram...\n\n");
	
        printf("echoing datagram to client (%s)...\n\n", serverSocket.getFromAddressPath());
		
        bytesTransferred = serverSocket.sendDatagram(pBuf, 
													 bytesRcvd, 
													 UnixDatagramSocket::FromServerToClient);
        printf("sent %d bytes\n\n", bytesTransferred);
		sleep(1);
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
  

  cout << "UnixDatagramServer test complete." << endl;
  return true;
}
