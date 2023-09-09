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

//
// Description:
//   All derived classes are started in the serverEntryPoint
// static function. The derived classes at a minimum should override
// the following members: init, serverMain.
//
// If beginWork or init is overridden, ServerPoolThread::init and/or
// ServerPoolThread::beginWork should be called first
// in the derived class.
//
// If waitForWork are overridden ServerPoolThread::waitForWork
// should be called last.
//
// See examples in socket framework "proxy" example.
//

#ifndef _ServerPoolThread_h
#define _ServerPoolThread_h

#include <pcff/thread/ConnectionPool.h>
#include <pcff/thread/ConnectionPoolItem.h>
#include <pcff/thread/Thread.h>

namespace pcff
{

class ServerPool;

class ServerPoolThread : public ConnectionPoolItem
{
public:
   ServerPoolThread(ServerPool &inPool);
  virtual ~ServerPoolThread();

public:
  virtual void init();           // initializes thread
  virtual void start();          // starts up thread, then waits
  virtual void stop();          // sets flag for main loop to exit
  virtual void startServing();   // signals child to wake up
  virtual void beginWork();
  virtual void waitForWork();
  // TODO: stub in, have it sleep, and do some other work... couts...
  virtual void serverMain();

public:
	bool      exit()     { return _bExit; }

public:
  Thread*      getThread()    { return _thread;}
  int          getID()        { return _thread->getID(); }

protected:
  Thread  *_thread;
  bool     _bExit;

private:
  static void serverEntryPoint (ServerPoolThread *thd);
};


};

#endif
