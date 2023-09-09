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

#include <pcff/BasicException.h>
#include <pcff/BasicExceptionMgr.h>

#include <pcff/socket/SocketExceptions.h>
#include <pcff/socket/INETAddress.h>
#include <pcff/socket/SocketAddress.h>
#include <pcff/socket/UnixSocket.h>

#include "SocketConfig.h"
#include "UnixServer.h"

using namespace std;
using namespace pcff;


//
//
UnixServer::UnixServer() 
{
}

//
//
//
UnixServer::~UnixServer() 
{
}

//
//
//
bool UnixServer::run() {
  string socketPath;

  try 
  {
    cout << "UnixServer test starting..." << endl;

    socketPath =   SocketConfig::instance().getServerSocketPath();

    UnixSocket               servSock;
    UnixSocket              *pClientSock;
    int                  numBytesSent;
    char                 pcBuffer[65535];
    
    sprintf(pcBuffer, "");
    printf("Unix Domain Server\n");
    // TODO: check
    servSock.init();
    servSock.bind( socketPath.c_str() );
    servSock.listen();

    for (;;)
    {
      printf("Waiting for Connection...\n");
      
      pClientSock = servSock.accept();
      
      printf("recv'd client connection...\n\n");
            
      numBytesSent = pClientSock->recv(pcBuffer, 65535);
      printf("received %d bytes\n", numBytesSent);
      printf("buffer : [");
      for (int i = 0; i < numBytesSent; i++)
      {
	     printf("%c", pcBuffer[i]);
      }
      printf("]\n\n");
      
      numBytesSent = pClientSock->send(pcBuffer, numBytesSent);
      printf("sent %d bytes\n\n", numBytesSent);
      
      delete pClientSock;
    }
  }
  catch(BasicException& ex)
  {
    BasicExceptionMgr::instance()->handle( ex );
  }
  catch(...)
  {
    BasicExceptionMgr::instance()->handle( "Unknown exception" );
  }
  
  cout << "UnixServer test complete." << endl;
  return true;
}
