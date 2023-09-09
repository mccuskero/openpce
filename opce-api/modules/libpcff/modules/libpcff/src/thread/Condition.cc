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
#include <assert.h>
#include <iostream>

#include <pcff/thread/Mutex.h>
#include <pcff/thread/Condition.h>

using namespace std;
using namespace pcff;

Condition::Condition()
{
   initPrivateMembers();
}

Condition::Condition(const char *name) 
: ThreadComponent(name)
{
   initPrivateMembers();   
}

Condition::Condition(Mutex *mutex) 
{   
   initPrivateMembers();
   _mutex = mutex;
}

Condition::Condition(const char *name, Mutex *mutex) 
: ThreadComponent(name)
{   
   initPrivateMembers();
   _mutex = mutex;
}

Condition::~Condition()
{
   pthread_cond_destroy (&_cond);   
}

void Condition::init()
{
   int ret = pthread_cond_init (&_cond, NULL);
   assert (ret==0);
}

void Condition::init(Mutex *mutex)
{
   assert(mutex != NULL);
   _mutex = mutex;
   this->init();
}


void Condition::initPrivateMembers()
{
   _mutex = NULL;
}

void Condition::signal()
{
   int ret = pthread_cond_signal (&_cond);
   assert (ret==0);
}

void Condition::wait()
{
   if (_mutex != NULL)
   {
      this->wait(_mutex);      
   }
}

void Condition::wait(Mutex *mutex)
{
   pthread_mutex_t* pthdMutex;
   
   assert(mutex != NULL);

   pthdMutex = mutex->getPThreadMutex();
   
   int ret = pthread_cond_wait(&_cond, pthdMutex ); 

   assert(ret==0);
}
