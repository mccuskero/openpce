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

#include <pcff/socket/SocketExceptions.h>
#include <pcff/socket/INETAddress.h>
#include <pcff/socket/UnixSocket.h>

using namespace pcff;

UnixSocket::UnixSocket() 
  : Socket(BaseSocket::AF_UNIX_DOMAIN)
{

}

UnixSocket::UnixSocket(int family) 
  : Socket(family)
{

}

void UnixSocket::init()
{
	BaseSocket::init();
}

void UnixSocket::init(const char *socketPath)
{
	_socketPath = socketPath;
	BaseSocket::init();
}

void UnixSocket::connect(const char *socketPath)
{
	_socketPath = socketPath;
    BaseSocket::connect(socketPath);	
}

void UnixSocket::bind(const char *socketPath)
{
    BaseSocket::bind(socketPath);	
}

//
// server socket members
//
void UnixSocket::listen(int backlog)
{
	BaseSocket::listen(backlog);
}

UnixSocket *UnixSocket::accept()
{
  UnixSocketAddress		clientAddress;
  int					port;
  UnixSocket			*pSock = new UnixSocket();
  
  pSock->init();
 
  // this sets the socket descriptor for client 
  BaseSocket::accept((BaseSocket *)pSock, 
						clientAddress);
  
  pSock->_setConnected();
  
  return pSock;
}

void UnixSocket::disConnect()
{
   _socketState = BaseSocket::Disconnected;
}

int UnixSocket::send(void *pBuffer, 
				 int len)
{
   if (_socketState != BaseSocket::Connected)
   {
       throw SocketException(SocketException::NotConnected);
   }
   return Socket::send(pBuffer, len);
}

// TODO: need to accept incoming connections, before receiving them...
int UnixSocket::recv(void *pBuffer, int len)
{
   
   return Socket::recv(pBuffer, len);
}

void UnixSocket::getSockOpt(int codeLevel, 
		      int optionName, 
		      void *pOptionData, 
		      int &dataLength)
{
    BaseSocket::getSockOpt(codeLevel, 
			   optionName, 
			   pOptionData, 
			   dataLength);
}

void UnixSocket::setSockOpt(int codeLevel, 
			   int optionName, 
			   const void *pOptionData, 
			   int dataLength)
{
    BaseSocket::setSockOpt(codeLevel, 
			   optionName, 
			   pOptionData, 
			   dataLength);
}
