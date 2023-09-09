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

#ifndef _ConnectionPool_h
#define _ConnectionPool_h

namespace pcff
{

class Mutex;
class ConnectionPoolItem;

class ConnectionPool
{
public:
   static ConnectionPool &instance();
   static void shutdown();
   virtual ~ConnectionPool ();

public:
   virtual void init(int poolSize);           // 

public:
   virtual ConnectionPoolItem *get(); // get first free item
   void                       add(ConnectionPoolItem *cpi);      // add to avail list
   void                       remove(ConnectionPoolItem *cpi);   // remove from  queue

public:
   void    linkBusy(ConnectionPoolItem *item);             
   void    linkAvailable(ConnectionPoolItem *item);           
   void    lock();                  // lock pool head
   void    unlock();                // unlock pool head
   Mutex   *getMutex()      { return _poolMutex; }
   
protected:     
   ConnectionPool ();                 // 
   
protected:
   static ConnectionPool *_instance;
   ConnectionPoolItem    *_availableItem;  // head of circular list
   ConnectionPoolItem    *_busyItem;       // head of circular list
   Mutex                 *_poolMutex;      // 
   ConnectionPoolItem    *_available;
   ConnectionPoolItem    *_busy;
   int                   _poolSize;
   ConnectionPoolItem   **_pool;
};


};
#endif
