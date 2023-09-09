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

#include "TestServerPoolThread.h"

using namespace std;
using namespace pcff;

TestServerPoolThread::TestServerPoolThread (ServerPool &pool)
: ServerPoolThread( pool)
{
}

TestServerPoolThread::~TestServerPoolThread()
{
}

void TestServerPoolThread::init()
{
   // TODO: do your own stuff...
   ServerPoolThread::init();
}

void TestServerPoolThread::start()
{
   // TODO: do your own stuff...
   cout << "TestServerPoolThread::start " << endl; cout.flush();
   ServerPoolThread::start();
}

void TestServerPoolThread::startServing()
{
   // TODO: do your own stuff...
      ServerPoolThread::startServing();
}

void TestServerPoolThread::beginWork()
{
   // TODO: do your own stuff...
   ServerPoolThread::beginWork();
}

void TestServerPoolThread::waitForWork()
{
   // TODO: do your own stuff...
   ServerPoolThread::waitForWork();
}

void TestServerPoolThread::serverMain()
{
   cout << " ----START-----------(" << this << ")-----------" << endl;      
   cout.flush();
   cout << "sleeping        (" << this << ")" << endl;      
   sleep(10);
   cout << "awake and done. (" << this << ")" << endl;      
   cout << " ----END-------------(" << this << ")-----------" << endl;      
}
