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

#include <assert.h>
#include <iostream>

#include <pcff/thread/Mutex.h>
#include <pcff/thread/Condition.h>
#include <pcff/thread/ConnectionPool.h>
#include <pcff/thread/ConnectionPoolItem.h>
#include <pcff/thread/ThreadExceptionMgr.h>

using namespace std;
using namespace pcff;

ConnectionPoolItem::ConnectionPoolItem (ConnectionPool &myPool) 
: _pool(myPool)
{
   _state = ConnectionPoolItem::NONE;
   _condition = NULL;
}

ConnectionPoolItem::~ConnectionPoolItem()
{
   assert (_state!=ConnectionPoolItem::HEAD);
   
   if (_state != ConnectionPoolItem::NONE ||
       _state != ConnectionPoolItem::INITIALIZED )
   {
         _pool.remove(this);
   }
  
   if (_condition != NULL)
   {
      delete _condition;     
   }
}

void ConnectionPoolItem::init()
{
   _condition = new Condition();
   _condition->init(_pool.getMutex());
   _state = ConnectionPoolItem::INITIALIZED;
}

void ConnectionPoolItem::start()
{
   ThreadExceptionMgr::instance()->log("ConnectionPoolItem::start", LogLevel::Debug);
}

void ConnectionPoolItem::setBusy()
{
   // this should be an unlinked unit in transitory state
   assert(_state == ConnectionPoolItem::TRANSITORY);

   _pool.lock();
   _state = ConnectionPoolItem::BUSY;

   _pool.linkBusy(this);

   // signal condition to wake up item
   ThreadExceptionMgr::instance()->log("ConnectionPoolItem::setBusy condition signal", LogLevel::Debug);
   _condition->signal();
   
   _pool.unlock();
}

void ConnectionPoolItem::busy2Available()
{
   assert(_state == ConnectionPoolItem::BUSY);

   _pool.lock();
   _pool.remove(this);

   _state = ConnectionPoolItem::AVAILABLE;

   _pool.linkAvailable(this);

   waitForBusy();

   _pool.unlock();
}

void ConnectionPoolItem::waitForBusy()
{
   ThreadExceptionMgr::instance()->log("ConnectionPoolItem::waitForBusy", LogLevel::Debug);

   assert(_state == ConnectionPoolItem::AVAILABLE);
   
   while (_state == ConnectionPoolItem::AVAILABLE)
   {
      _condition->wait();  // uses condiition and its mutex to signal
   }
}
