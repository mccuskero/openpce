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


#include <pcff/BasicException.h>
#include <pcff/Task.h>

#include "UDPEchoServer.h"
#include "UDPEchoClient.h"
#include "UnixDatagramServer.h"
#include "UnixDatagramClient.h"
#include "UnixServer.h"
#include "UnixClient.h"
#include "TCPEchoServer.h"
#include "TCPEchoClient.h"
#include "SocketConfig.h"
#include "SocketTestDirector.h"

using namespace pcff;

//
//
//
bool SocketTestDirector::run(int argc, char** argv) 
{

  Task* task = getTask( argc, argv );

  if ( task != NULL ) 
  {
    task->run();
  }
  else 
  {
    throw BasicException( BasicException::ERR,
			  BasicException::UnitTestNotFound,
			  "SocketTestDirector::run Unknown task");
  }

  delete task;

  return true;
}

//
//
//
Task* SocketTestDirector::getTask(int argc, char** argv) 
{
  Task* task = NULL;

  if ( SocketConfig::instance().getTestType() == SocketConfig::TCPEchoClient )
  {
    task = new TCPEchoClient();
  }
  else if ( SocketConfig::instance().getTestType() == SocketConfig::TCPEchoServer )
  {
    task = new TCPEchoServer();
  }
  else if ( SocketConfig::instance().getTestType() == SocketConfig::UDPEchoClient )
  {
    task = new UDPEchoClient();
  }
  else if ( SocketConfig::instance().getTestType() == SocketConfig::UDPEchoServer )
  {
    task = new UDPEchoServer();
  }
  else if ( SocketConfig::instance().getTestType() == SocketConfig::UnixDatagramEchoClient )
  {
    task = new UnixDatagramClient();
  }
  else if ( SocketConfig::instance().getTestType() == SocketConfig::UnixDatagramEchoServer )
  {
    task = new UnixDatagramServer();
  }
  else if ( SocketConfig::instance().getTestType() == SocketConfig::UnixEchoClient )
  {
    task = new UnixClient();
  }
  else if ( SocketConfig::instance().getTestType() == SocketConfig::UnixEchoServer )
  {
    task = new UnixServer();
  }
  else
  {
    throw BasicException( BasicException::ERR,
		   	  BasicException::UnitTestNotFound,
			  "task is undefined"  );
  }

  return task;
}
