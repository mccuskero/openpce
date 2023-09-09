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

#include <iostream>
#include <string>

#include "EchoServerThread.h"
#include "EchoServer.h"

using namespace std;
using namespace pcff;

EchoServer &EchoServer::instance()
{
   if (_instance == NULL)
   {
      _instance = new EchoServer();
   }
   
   return (EchoServer &)(*_instance);
}

void EchoServer::shutdown()
{
   if (_instance != NULL)
   {
      delete _instance;
   }   
}

EchoServer::EchoServer()
{
}

void EchoServer::init(int   addrFamily, 
                           int   poolSize, 
                           string listenHost,
                           int   listenPort)
{
   // TODO: setup EchoServerThread in Pool
   
   // pass on info to initialize
   Server::init(poolSize,
                addrFamily,
                listenHost,
                listenPort);
}   

EchoServer::~EchoServer()
{
}

void EchoServer::populate()
{
   EchoServerThread *thd = NULL;
   
   //create the pool, startup up the threads,
   // threads will add themselves to the link list.
   for (int i=0; i<_poolSize; i++)
   {
      thd = new EchoServerThread(*this);
      thd->init(_addrFamily);   // adds itself to the available list 
      thd->start();
      _pool[i] = thd;
   }   
}
