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

#include <pcff/LogLevel.h>

#include <pcff/thread/Thread.h>
#include <pcff/thread/ThreadExceptionMgr.h>
#include <pcff/thread/ConnectionPoolItem.h>
#include <pcff/thread/ServerPool.h>
#include <pcff/thread/ServerPoolThread.h>
#include <pcff/thread/ThreadExceptionMgr.h>

#include <pcff/socket/INETAddress.h>
#include <pcff/socket/Socket.h>
#include <pcff/socket/ProxyServer.h>
#include <pcff/socket/ProxyServerThread.h>

using namespace std;
using namespace pcff;


ProxyServerThread::ProxyServerThread (ProxyServer &pool)
: ServerThread( pool)
{
   _forwardSocket = NULL;
   _forwardHost = "";
   _forwardPort = 0;
}

ProxyServerThread::~ProxyServerThread()
{
   if (_forwardSocket != NULL)
   {
      delete _forwardSocket;
      _forwardSocket = NULL;
   }
}

void ProxyServerThread::init(int addrFamily, string forwardHost, int forwardPort)
{
   _forwardHost = forwardHost;
   _forwardPort = forwardPort;
   
   ServerThread::init(addrFamily);
}

void ProxyServerThread::start()
{
   ThreadExceptionMgr::instance()->log("ProxyServerThread::start", LogLevel::Debug);   
   ServerThread::start();
}

void ProxyServerThread::startServing()
{
   stringstream sStr;
   
   INETAddress forwardINETAddr(_addrFamily);
   forwardINETAddr.setHostname(_forwardHost.c_str());
   
   sStr << "ProxyServerThread::startServing connect to forwarding host from (" <<\
      _clientSocket->remoteINETAddr().getAddressAsStr() <<\
      ")";
   
   ThreadExceptionMgr::instance()->log(sStr.str().c_str(), LogLevel::Debug);   

   _forwardSocket = new Socket(_addrFamily);
   _forwardSocket->init();
   _forwardSocket->connect(forwardINETAddr, _forwardPort);
   
   // pay it forward...
   ServerThread::startServing();
}

void ProxyServerThread::beginWork()
{
   // TODO: do your own stuff...
   ServerThread::beginWork();
}

void ProxyServerThread::waitForWork()
{
   // TODO: do your own stuff...
   ServerThread::waitForWork();
}

void ProxyServerThread::serverMain()
{
   char  buffer[65535];
   int   numBytesRcvd = 0;
   int   numBytesSent = 0;
   bool  bExit = false;
   BaseSocket *readSocket = NULL;
   BaseSocket *writeSocket = NULL;
      
   ThreadExceptionMgr::instance()->log("ProxyServerThread::serverMain", LogLevel::Debug);   

   while (!bExit)
   {
      // block here waiting for socket activity
      Socket::proxySelect(_clientSocket, 
                          _forwardSocket,
                          &readSocket,
                          &writeSocket);
      
      numBytesRcvd = readSocket->recv(buffer, 65535, 0);
      
      // TODO: process the buffer, cache it, parse it, 
      // maybe modify the numBytes received to 0, to buffer for later
      //
      numBytesRcvd = processStream(numBytesRcvd, buffer);
      
      numBytesSent = writeSocket->send(buffer, numBytesRcvd, 0);      
      
      if (numBytesRcvd <= 0 || numBytesSent < 0)
      {
         bExit = true;
         ThreadExceptionMgr::instance()->log("ProxyServerThread::serverMain thread is done for now", LogLevel::Debug);   
      }      
   }
}

int ProxyServerThread::processStream(int bufSize, char *buf)
{
   stringstream sStr;
   sStr << "ProxyServerThread::processStream bufSize: " << bufSize;
   ThreadExceptionMgr::instance()->log(sStr.str().c_str(), LogLevel::Debug);   
   
   return bufSize;
}
