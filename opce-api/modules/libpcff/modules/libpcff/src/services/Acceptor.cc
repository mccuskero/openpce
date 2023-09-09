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

#include <string>

#include <pcff/socket/Server.h>

#include <pcff/services/ServiceHandler.h>
#include <pcff/services/Acceptor.h>

using namespace std;
using namespace pcff;


Acceptor::Acceptor()
{
	initMembers();
}

void Acceptor::initMembers()
{
	_thread = NULL;
	_poolSize = 0;
	_addrFamily = 0;
	_listenHost = "";
	_listenPort = 0;
}

Acceptor::~Acceptor()
{
	if (_thread == NULL) delete _thread;
}

void Acceptor::init(int   addrFamily, 
                           int   poolSize, 
                           string listenHost,
                           int   listenPort)
{
   // TODO: setup AcceptorThread in Pool
   
   // pass on info to initialize
   Server::init(poolSize,
                addrFamily,
                listenHost,
                listenPort);
   
   // initialize thread behavior that handles listen socket
   _thread = new Thread( (THREAD_ENTRY_POINT)Acceptor::mainProc, 
                         (void*)this);
   _thread->init();
}   

void Acceptor::run()
{
	_thread->start();
}

void Acceptor::shutdownGracefully()
{
	  _thread->setFinished();
}

void Acceptor::mainProc(Acceptor *objIn) 
{
	objIn->startServer();
}

//
//
void Acceptor::startServer()
{
	Server::run();	
}

//
// derived class must override creating
// derived ServiceHandler class, and populate with
//
void Acceptor::populate()
{
	ServiceHandler *thd = NULL;
   
   //create the pool, startup up the threads,
   // threads will add themselves to the link list.
   for (int i=0; i<_poolSize; i++)
   {
      thd = new ServiceHandler(*this);
      thd->init(_addrFamily);   // adds itself to the available list 
      thd->start();
      _pool[i] = thd;
   }   
}
