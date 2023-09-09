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

#ifndef _Socket_h
#define _Socket_h


#include <pcff/socket/BaseSocket.h>

namespace pcff
{

class Socket : public BaseSocket
{
 public:
  Socket(); // TODO: make compiler define family...
  Socket(int family ); // TODO: make compiler define family...
  virtual     ~Socket() {}
  
 public:
  virtual void connect(const char *host, int servPort);
  virtual void connect(INETAddress &servAddr, 
                       int servPort);
  virtual int  send(void *pData, int len);
  virtual int  recv(void *pBuffer, int len); 

  //
  // server members 
 public:
  virtual Socket   *accept();
  void              bind(int port);
  void              bind(INETAddress &localAddr, 
                         int port);

  void getSockOpt(int codeLevel, 
                  int optionName, 
                  void *pOptionData, 
                  int &dataLength);
  void setSockOpt(int codeLevel, 
                  int optionName, 
                  const void *pOptionData, 
                  int dataLength);
  
// generic helper funcitons for framework
public:
     // TODO: make generic select function that takes in array of Sockets, passes back, ready socket.
     // static void select(int numInSockets, Socket *sockArray[], int *numReadSockets, Socket *readSocks[]);
     
     // TODO: make a ProxySocket, take proxySelect out...
     // The helper function is used when implementing a proxy. Becuase the 
     // BSD socket API calls are made in this class and in the BaseSocket class
     // this member was left here.
     static void proxySelect(BaseSocket *in1, 
                             BaseSocket *in2, 
                             BaseSocket **readSock,
                             BaseSocket **writeSock);
};

};

#endif
