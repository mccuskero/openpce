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

#include <pcff/thread/Mutex.h>
#include <pcff/thread/ConnectionPoolItem.h>
#include <pcff/thread/ConnectionPool.h>


using namespace std;
using namespace pcff;

ConnectionPool *ConnectionPool::_instance = NULL;   

ConnectionPool &ConnectionPool::instance() 
{
   if (_instance == NULL)
   {
      _instance = new ConnectionPool();
   }
   
   return *_instance;
}

void ConnectionPool::shutdown()
{
   
}

ConnectionPool::ConnectionPool ()
{   
   // TODO: verify gettting called
   _poolMutex = NULL;
   _available = NULL;
   _busy = NULL;
   _poolSize = 0;
   _pool = NULL;
}

ConnectionPool::~ConnectionPool()
{
   if (_poolMutex != NULL)
   {
      delete _poolMutex;
      _poolMutex = NULL;
   }
}

void ConnectionPool::init(int poolSize)
{
   _poolSize = poolSize;
   _poolMutex = new Mutex();   
   _poolMutex->init();
   
   // set up the link lisk to initial state
   _available = new ConnectionPoolItem(*this);
   _available->setState( ConnectionPoolItem::HEAD );
   _available->setNext( _available );
   _available->setPrev( _available );
   
   _busy = new ConnectionPoolItem(*this);
   _busy->setState( ConnectionPoolItem::HEAD );
   _busy->setNext( _busy );
   _busy->setPrev( _busy );
   
   _pool = (ConnectionPoolItem **)(new ConnectionPoolItem*[_poolSize]);   
}

void ConnectionPool::lock()
{
   _poolMutex->lock();
}

void ConnectionPool::unlock()
{
   _poolMutex->unlock();
}

void ConnectionPool::add(ConnectionPoolItem *item)
{
  _poolMutex->lock();

  linkAvailable(item); // get on available list

  // signal is available
  item->waitForBusy();

  _poolMutex->unlock();
}

ConnectionPoolItem *ConnectionPool::get()
{
  ConnectionPoolItem * ret = NULL;
   
  _poolMutex->lock();
  ret = _available->getNext();
  
  // if ret == current then link list
  // is empty. 
  if (ret == _available)
  {
    _poolMutex->unlock();
    return NULL;
  }

  remove(ret);
  ret->state2transitory();

  _poolMutex->unlock();
  return ret;
}

void ConnectionPool::remove(ConnectionPoolItem *item)
{
  ConnectionPoolItem *n = NULL, *p = NULL;

  n = item->getNext();
  p = item->getPrev();

  p->setNext(n);
  n->setPrev(p);

  item->setNext(NULL);
  item->setPrev(NULL);
}

void ConnectionPool::linkBusy(ConnectionPoolItem *item)
{
  ConnectionPoolItem* n = NULL;
  
  n = _busy->getNext();

  item->setNext(n);
  n->setPrev(item);

  _busy->setNext(item);
  item->setPrev(_busy);              //TODO: check for memory leaks
}

void ConnectionPool::linkAvailable(ConnectionPoolItem *item)
{
  ConnectionPoolItem *n = NULL;
  
  n = _available->getNext();

  item->setNext(n);
  n->setPrev(item);

  _available->setNext(item);
  item->setPrev(_available);

  item->setAvailable();
}
