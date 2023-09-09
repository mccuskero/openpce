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

#include <pcff/thread/ThreadDirector.h>
#include <pcff/thread/ServerPoolThread.h>
#include <pcff/thread/ServerPool.h>

#include <pcff/socket/Socket.h>
#include <pcff/socket/ProxyServerThread.h>
#include <pcff/socket/ProxyServer.h>

using namespace std;
using namespace pcff;


ProxyServer &ProxyServer::instance()
{
   if (_instance == NULL)
   {
      _instance = new ProxyServer();
      ThreadDirector::instance().initialize();
   }

   
   return (ProxyServer &)(*_instance);
}

void ProxyServer::shutdown()
{
   if (_instance != NULL)
   {
      delete _instance;
   }   

   ThreadDirector::instance().shutdown();
}

ProxyServer::ProxyServer()
: Server()
{
   _forwardHost = "";
   _forwardPort = 0;
}

ProxyServer::~ProxyServer()
{
   for (int i=0; i<_poolSize; i++)
   {
      delete _pool[i];
   }   
   delete [] _pool;
   _pool = NULL;   
}

void ProxyServer::init(int poolSize, 
                       int addrFamily, 
                       string listenHost,
                       int listenPort,
                       string forwardHost,
                       int  forwardPort)
{   
   _forwardHost = forwardHost;
   _forwardPort = forwardPort;
   
   Server::init(poolSize, addrFamily, listenHost, listenPort );
}

void ProxyServer::populate()
{
   ProxyServerThread *thd = NULL;
   //create the pool, startup up the threads,
   // threads will add themselves to the link list.
   for (int i=0; i<_poolSize; i++)
   {
      thd = new ProxyServerThread(*this);
      thd->init(_addrFamily, _forwardHost, _forwardPort);   // adds itself to the available list 
      thd->start();
      _pool[i] = thd;
   }   
}
