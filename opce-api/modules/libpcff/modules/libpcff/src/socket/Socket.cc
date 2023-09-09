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

#include <stdio.h>

#include <errno.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>

#include <pcff/socket/SocketExceptions.h>
#include <pcff/socket/INETAddress.h>
#include <pcff/socket/SocketAddress.h>
#include <pcff/socket/Socket.h>

using namespace pcff;

Socket::Socket()
        : BaseSocket(AF_INET)
{
}

Socket::Socket(int addrFamily)
        : BaseSocket(addrFamily)
{
}

void Socket::connect(const char *host, int port)
{
    INETAddress serverINETAddr(_addrFamily);
    serverINETAddr.setHostname(host);

    Socket::connect(serverINETAddr, port);
}

void Socket::connect(INETAddress &servAddr, 
			int port)
{
  BaseSocket::connect(servAddr, port);
  _setLocal();
  _setConnected();
}

//
// server socket members
//
Socket *Socket::accept()
{
  INETAddress   clientAddress;
  int           port;
  Socket        *pSock = new Socket();
  
  pSock->init();
 
  // this sets the socket descriptor for client 
  BaseSocket::accept((BaseSocket *)pSock, 
		     clientAddress, 
		     port);

  // TODO: returns remote client addr and port
  // but nothing is done???
  pSock->_setLocal();
  pSock->_setConnected();
  
  return pSock;
}

void Socket::bind(int port)
{
    BaseSocket::bind(port);
    _setLocal();
}

void Socket::bind(INETAddress &localAddr, 
		     int port)
{
    BaseSocket::bind(localAddr, port);
    _setLocal();
}

int Socket::send(void *pData, int len)
{
  if (_socketState != BaseSocket::Connected)
  {
      throw SendException(SendException::NotConnected);
  }
  return BaseSocket::send(pData, len, 0);
}

int Socket::recv(void *pBuffer, int len)
{
   if (_socketState != BaseSocket::Connected)
   {
       throw RecvException(RecvException::NotConnected);
   }
   return BaseSocket::recv(pBuffer, len, 0);
}

void Socket::getSockOpt(int codeLevel, 
		      int optionName, 
		      void *pOptionData, 
		      int &dataLength)
{
    BaseSocket::getSockOpt(codeLevel, 
			   optionName, 
			   pOptionData, 
			   dataLength);
}

void Socket::setSockOpt(int codeLevel, 
		      int optionName, 
		      const void *pOptionData, 
		      int dataLength)
{
    BaseSocket::setSockOpt(codeLevel, 
			   optionName, 
			   pOptionData, 
			   dataLength);
}

void Socket::proxySelect(BaseSocket *in1, 
                         BaseSocket *in2, 
                         BaseSocket **readSock,
                         BaseSocket **writeSock)
{
   int highestNumFD;
   fd_set fdSet;
   int in1FD = 0;
   int in2FD = 0;
   int selectRet = 0;
   
   in1FD = in1->getSockDesc();
   in2FD = in2->getSockDesc();
   
   // set the highest num file descriptor (and add one0
   highestNumFD = (in1FD > in2FD)? (in1FD+1) : (in2FD+1);
   
   FD_ZERO (&fdSet);
   FD_SET (in1FD, &fdSet);
   FD_SET (in2FD, &fdSet);
   
   // TODO: use a timeout? or block
   selectRet = ::select (highestNumFD, &fdSet, 0, 0, 0);

   assert(selectRet>=0);
   
   if (FD_ISSET(in1FD, &fdSet)) 
   {
      *readSock = in1;
      *writeSock = in2;
   }
   else
   {
      *readSock = in2;
      *writeSock = in1;
   }   
}
