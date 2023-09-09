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

#ifndef _ProxyServer_h
#define _ProxyServer_h

#include <string>

#include <pcff/thread/ConnectionPoolItem.h>
#include <pcff/thread/ConnectionPool.h>
#include <pcff/thread/ServerPoolThread.h>
#include <pcff/thread/ServerPool.h>

#include <pcff/socket/Server.h>

namespace pcff
{
   class Socket;
   
class ProxyServer : public pcff::Server
{
public:
   static ProxyServer &instance();
   static void        shutdown();
   ProxyServer();

public:
      virtual void init(int poolSize, 
                        int addrFamily, 
                        std::string listenHost,
                        int listenPort,
                        std::string forwardHost,
                        int  forwardPort);
protected:   
   virtual ~ProxyServer();

private:
   void    initPrivateMembers();

public:
      virtual void populate();                 // allows derived class to populated 
                                               // with derived ConnectionPoolItems
protected:
   std::string _forwardHost;
   int         _forwardPort;
};

};

#endif
