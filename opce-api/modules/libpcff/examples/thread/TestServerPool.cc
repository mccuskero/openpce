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

#include <pcff/thread/ServerPoolThread.h>
#include <pcff/thread/ServerPool.h>

#include "TestServerPoolThread.h"
#include "TestServerPool.h"


using namespace std;
using namespace pcff;

TestServerPool &TestServerPool::instance()
{
   if (_instance == NULL)
   {
      _instance = new TestServerPool();
   }
   
   return (TestServerPool &)(*_instance);
}

void TestServerPool::shutdown()
{
   if (_instance != NULL)
   {
      delete _instance;
   }   
}

TestServerPool::TestServerPool()
{
}

TestServerPool::~TestServerPool()
{
   //  TODO: move to base class??
   // whatch type issues
   for (int i=0; i<_poolSize; i++)
   {
      delete _pool[i];
   }   
   delete [] _pool;
   _pool = NULL;   
}

void TestServerPool::populate()
{
   //create the pool, startup up the threads,
   // threads will add themselves to the link list.
   for (int i=0; i<_poolSize; i++)
   {
      _pool[i] = new TestServerPoolThread(*this);
      _pool[i]->init();   // adds itself to the available list 
      _pool[i]->start();
   }   
}
