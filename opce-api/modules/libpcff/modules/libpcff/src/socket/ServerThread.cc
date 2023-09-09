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
#include <pthread.h>
#include <iostream>
#include <sstream>
#include <assert.h>


#include <pcff/LogLevel.h>

#include <pcff/thread/Thread.h>
#include <pcff/thread/ThreadExceptionMgr.h>
#include <pcff/thread/ConnectionPoolItem.h>
#include <pcff/thread/ServerPool.h>
#include <pcff/thread/ServerPoolThread.h>
#include <pcff/thread/ThreadExceptionMgr.h>

#include <pcff/socket/SocketExceptions.h>
#include <pcff/socket/INETAddress.h>
#include <pcff/socket/Socket.h>
#include <pcff/socket/Server.h>
#include <pcff/socket/ServerThread.h>

using namespace std;
using namespace pcff;


ServerThread::ServerThread (Server &pool)
: ServerPoolThread( pool)
{
   _streamBuffer = NULL;
   _clientSocket = NULL;
   _addrFamily = BaseSocket::AF_INET_IPv4;
}

ServerThread::~ServerThread()
{
   if (_clientSocket != NULL)
   {
      delete _clientSocket;
      _clientSocket = NULL;
   }
}

void ServerThread::init(int addrFamily)
{
   _addrFamily = addrFamily; // IPv4, IPv6

   ServerPoolThread::init();
}

//
// The clientSocket is given to ServerThread
// from the Server
//
void ServerThread::set(Socket *clientSocket)
{
   assert(clientSocket != NULL);

   _clientSocket = clientSocket;
}

void ServerThread::start()
{
   ThreadExceptionMgr::instance()->log("ServerThread::start", LogLevel::Debug);

   ServerPoolThread::start();
}

void ServerThread::startServing()
{
   ThreadExceptionMgr::instance()->log("ServerThread::startServing", LogLevel::Debug);

   ServerPoolThread::startServing();
}

void ServerThread::beginWork()
{
   // TODO: do your own stuff...
   ServerPoolThread::beginWork();
}

void ServerThread::waitForWork()
{
   // TODO: do your own stuff...
   ServerPoolThread::waitForWork();
}

void ServerThread::serverMain()
{
   char  buffer[65535];
   int   numBytesRcvd = 0;
   int   numBytesSent = 0;
   bool  bExit = false;

   ThreadExceptionMgr::instance()->log("ServerThread::serverMain", LogLevel::Debug);

   try
   {
	   while (!bExit)
	   {
	      numBytesRcvd = _clientSocket->recv(buffer, 65535);

	      // TODO: process the buffer, cache it, parse it,
	      // maybe modify the numBytes received to 0, to buffer for later
	      //
	      numBytesRcvd = processStream(numBytesRcvd, buffer);

	      // The client has exited,
	      // TODO: put yourself back into the pool.
	      if (numBytesRcvd == 0)
	      {
	         bExit = true;
	         ThreadExceptionMgr::instance()->log("ServerThread::serverMain thread is done for now", LogLevel::Info);
	      }
	   }
   }
   catch(RecvException &ex)
   {
	   ThreadExceptionMgr::instance()->handle(ex);
   }
   catch(BasicException &ex)
   {
	   ThreadExceptionMgr::instance()->handle(ex);
   }
   catch(...)
   {
	   ThreadExceptionMgr::instance()->log("ServerThread::serverMain: caught unknown exception caught");
   }
}

int ServerThread::processStream(int bufSize, char *buf)
{
   stringstream sStr;
   sStr << "ServerThread::processStream bufSize: " << bufSize;
   ThreadExceptionMgr::instance()->log(sStr.str().c_str(), LogLevel::Info);

   return bufSize;
}
