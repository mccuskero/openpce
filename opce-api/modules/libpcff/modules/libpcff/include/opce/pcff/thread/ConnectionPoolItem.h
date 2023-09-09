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

#ifndef _ConnectionPoolItem_h
#define _ConnectionPoolItem_h

#include <pcff/thread/ConnectionPool.h>

namespace pcff
{
   class Condition;
   
class ConnectionPoolItem
{
public:
  ConnectionPoolItem (ConnectionPool &myPool);  // start life as free
  virtual ~ConnectionPoolItem ();

public:
  virtual void init();
  virtual void start();
  
public:
  void                setState(int state)   { _state = state; }
  void                setPrev(ConnectionPoolItem *item ) { _prev = item; }
  ConnectionPoolItem *getPrev()                         { return _prev; }
  void                setNext( ConnectionPoolItem *item) { _next = item; }
  ConnectionPoolItem *getNext()                         { return _next; }
  
public:
  void                        setAvailable()        { _state = ConnectionPoolItem::AVAILABLE; }
  void                        setBusy ();         
  void                        busy2Available();        
  void                        waitForBusy();      
  void                        state2transitory()    { _state = ConnectionPoolItem::TRANSITORY; }

public:
  void lock()              { _pool.lock();      }
  void unlock()            { _pool.unlock();    }
    
public:
typedef enum
  {
     NONE,
     INITIALIZED,
     HEAD,
     AVAILABLE,
     TRANSITORY,
     BUSY
  } POOL_ITEM_STATE;

protected:
  ConnectionPool     &_pool;
     
private:
  int                _state;            // 
  Condition          *_condition;       // each pool item has its own condition (TODO: verify)
  ConnectionPoolItem *_next;
  ConnectionPoolItem *_prev;
};

};
#endif
