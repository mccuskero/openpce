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

#include <pcff/LogLevel.h>

#include <pcff/thread/ThreadExceptionMgr.h>
#include <pcff/thread/ServerPoolThread.h>
#include <pcff/thread/ServerPool.h>

#include <pcff/socket/Socket.h>
#include <pcff/socket/SocketExceptions.h>
#include <pcff/socket/ServerThread.h>
#include <pcff/socket/Server.h>

using namespace std;
using namespace pcff;


Server &Server::instance()
{
   if (_instance == NULL)
   {
      _instance = new Server();
   }

   return (Server &)(*_instance);
}

void Server::shutdown()
{
   if (_instance != NULL)
   {
      delete _instance;
   }

}

Server::Server()
{
   _listenHost = "";
   _listenPort = 0;
   _addrFamily = BaseSocket::AF_INET_IPv4;
   _listenSocket = NULL;
}

Server::~Server()
{
   if (_listenSocket != NULL)
   {
      delete _listenSocket;
      _listenSocket = NULL;
   }

   for (int i=0; i<_poolSize; i++)
   {
	   ((ServerPoolThread *)_pool[i])->stop();    // sets flag to exit
	   ((ServerPoolThread *)_pool[i])->setBusy(); // signals condition

      delete _pool[i];
      _pool[i] = NULL;
   }

   delete [] _pool;
   _pool = NULL;
}

void Server::init(     int poolSize,
                       int addrFamily,
                       string listenHost,
                       int listenPort)
{
   _addrFamily  = addrFamily;
   _listenHost = listenHost;
   _listenPort = listenPort;

   // TODO: check addrFamily
   _listenSocket = new Socket();
   _listenSocket->init();
   _listenSocket->bind( _listenPort );
   _listenSocket->listen();
   ThreadExceptionMgr::instance()->log("Server listen socket initialized");

   ServerPool::init(poolSize);
}

void Server::populate()
{
   ServerThread *thd = NULL;
   //create the pool, startup up the threads,
   // threads will add themselves to the link list.
   for (int i=0; i<_poolSize; i++)
   {
      thd = new ServerThread(*this);
      thd->init(_addrFamily);   // adds itself to the available list
      thd->start();
      _pool[i] = thd;
   }
}

void Server::run()
{
   bool              bExit = false;
   Socket            *pClientSock = NULL;
   ServerThread      *thd = NULL;

   try
   {
      while (bExit != true)
      {
         ThreadExceptionMgr::instance()->log("Server::run waiting for Connection", LogLevel::Debug);
         pClientSock = _listenSocket->accept();
         ThreadExceptionMgr::instance()->log("Server accepted socket connection");
         // TODO: print out address when client acccepted
         //         pClientSock->remoteINETAddr().print();
         thd = (ServerThread *)Server::get();
         thd->set(pClientSock);
         thd->startServing();
      }
   }
   catch(SocketException &ex)
   {
      ThreadExceptionMgr::instance()->handle(ex);
      delete pClientSock;
   }
   catch(BasicException &ex)
   {
      ThreadExceptionMgr::instance()->handle(ex);
      delete pClientSock;
   }
}
