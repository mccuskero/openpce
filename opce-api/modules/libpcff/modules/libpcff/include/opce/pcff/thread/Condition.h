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

#ifndef _Condition_h
#define _Condition_h

#include <pcff/thread/ThreadComponent.h>

namespace pcff
{
   
   class Mutex;
   
   class Condition : public ThreadComponent
   {
public:
      Condition();
      Condition(const char *name);
      Condition(Mutex *mutex);
      Condition(const char *name, Mutex *mutex);
      virtual ~Condition();

public:
      void init();
      void init(Mutex *mutex);

public:
      void signal();
      void wait();
      void wait(Mutex *mutex);

private:
         void initPrivateMembers();
      
private:
      pthread_cond_t   _cond;
      Mutex            *_mutex;
   };
   
};

#endif
