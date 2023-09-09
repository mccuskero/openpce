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


#include <pcff/thread/Mutex.h>

using namespace std;
using namespace pcff;

Mutex::Mutex()
{
   initPrivateMembers();
}

Mutex::Mutex(const char *name)
  : ThreadComponent( name )
{
}

Mutex::~Mutex()
{
  pthread_mutex_destroy(&_mutex);
}

int Mutex::init()
{
   int ret;
   
   ret = pthread_mutex_init(&_mutex, NULL);   
   
   return ret;
}

void Mutex::initPrivateMembers()
{
}

int Mutex::lock()
{
//  cerr<< "Mutex::lock " << this << " " << endl; cerr.flush();      
  return pthread_mutex_lock(&_mutex);
}

int Mutex::unlock()
{
  
//   cerr<< "Mutex::unlock " << this << " " << endl; cerr.flush();        
  return pthread_mutex_unlock(&_mutex);
}

bool Mutex::tryLock()
{
        int retval;

        retval = pthread_mutex_trylock(&_mutex);

        if (retval)
                return false;
        else
                return true;
}
