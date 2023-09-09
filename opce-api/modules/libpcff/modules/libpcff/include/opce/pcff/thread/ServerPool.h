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

#ifndef _ServerPool_h
#define _ServerPool_h

#include <pcff/thread/ConnectionPoolItem.h>
#include <pcff/thread/ConnectionPool.h>
#include <pcff/thread/ServerPoolThread.h>

namespace pcff
{
  
class ServerPoolThread;
class Thread;
   
class ServerPool : public ConnectionPool
{
public:
   static ServerPool &instance();
   static void        shutdown();
   virtual ~ServerPool();

   
public:
   virtual ConnectionPoolItem *get(); // get first free item
   void waitTillStart();
   void waitTillEnd();
   
protected:   
   ServerPool();

public:
  virtual void init(int poolSize);         // initializes ServerPoolThreads
  virtual void populate();                 // allows derived class to populated 
                                           // with derived ConnectionPoolItems
public:
  void lock();           
  void unlock();         
  void lock_once();
  void unlock_completely();
  
public:
   enum GetBlockMode 
  {
     NoBlock = 1,
     WaitForAvailableItem
  };
  
protected:
     int _getBlockMode;
};

};

#endif
