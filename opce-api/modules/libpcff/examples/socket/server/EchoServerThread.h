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

#include <string>

#ifndef _EchoServerThread_h
#define _EchoServerThread_h

#include <pcff/thread/ConnectionPool.h>
#include <pcff/thread/ConnectionPoolItem.h>
#include <pcff/thread/Thread.h>

#include <pcff/socket/ProxyServer.h>
#include <pcff/socket/ProxyServerThread.h>

class EchoServer;

class EchoServerThread : public pcff::ServerThread
{
public:
   EchoServerThread(EchoServer &proxy);
  ~EchoServerThread();

public:
  virtual void init(int addrFamily);           // initializes thread
  virtual void start();          // starts up thread, then waits
  virtual void startServing();   // signals child to wake up
  virtual int processStream(int bufSize, char *buf);  // Buffer...
};

#endif
