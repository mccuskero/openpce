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

#include <pcff/thread/Thread.h>
#include <pcff/thread/ConnectionPoolItem.h>
#include <pcff/thread/ServerPool.h>
#include <pcff/thread/ServerPoolThread.h>
#include <pcff/thread/ThreadExceptionMgr.h>

using namespace std;
using namespace pcff;

ServerPoolThread::ServerPoolThread (ServerPool &pool)
: ConnectionPoolItem( pool)
{
   _thread = NULL;
   _bExit = false;
}

ServerPoolThread::~ServerPoolThread()
{
  if (_thread)
  {
     delete _thread;
     _thread = NULL;
  }
}

void ServerPoolThread::init()
{
   _thread = new Thread ((THREAD_ENTRY_POINT)serverEntryPoint,
                        (void *)this);
   _thread->init();
   // initializes Condition, and associations to pool Mutex.
   ConnectionPoolItem::init();
}

//
// start up the thread
//
void ServerPoolThread::start()
{
   ThreadExceptionMgr::instance()->log("ServerPoolThread::start", LogLevel::Debug);

   _thread->start();
}

void ServerPoolThread::stop()
{
	_bExit = true;
}

// begin doing some server work
// The parent thread will call this to signal
// the thread to wake up.
void ServerPoolThread::startServing()
{
   setBusy();
}

//
// add yourself to the available list
// (occurs inside child: serverEntryPoint)
//
void ServerPoolThread::beginWork()
{
   ThreadExceptionMgr::instance()->log("ServerPoolThread::beginWork added to pool", LogLevel::Debug);
   _pool.add(this);
}

void ServerPoolThread::waitForWork()
{
  busy2Available();
}

void ServerPoolThread::serverMain()
{
      cout << " ----START-----------(" << this << ")-----------" << endl;
      cout.flush();
      cout << "sleeping        (" << this << ")" << endl;
      sleep(10);
      cout << "awake and done. (" << this << ")" << endl;
      cout << " ----END-------------(" << this << ")-----------" << endl;
}

void ServerPoolThread::serverEntryPoint (ServerPoolThread * thd)
{
   ThreadExceptionMgr::instance()->log("ServerPoolThread::serverEntryPoint", LogLevel::Debug);

   cout.flush();

   thd->beginWork();

   while (thd->exit() == false)
   {
      thd->serverMain();
      thd->waitForWork();
   }
}

