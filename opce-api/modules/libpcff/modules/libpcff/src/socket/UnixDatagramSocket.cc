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
#include <pcff/socket/UnixDatagramSocket.h>

using namespace pcff;

UnixDatagramSocket::UnixDatagramSocket() 
  : DatagramSocket(BaseSocket::AF_UNIX_DOMAIN)
{

}

UnixDatagramSocket::UnixDatagramSocket(int family) 
  : DatagramSocket(family)
{

}

void UnixDatagramSocket::init()
{
	DatagramSocket::init();
}

void UnixDatagramSocket::init(const char *socketPath)
{
	_serverAddress.setPathAddress(socketPath);
	_socketPath = socketPath;					// TODO: fix up _socketPath semantics in BaseSocket... remove?
	DatagramSocket::init();
}

void UnixDatagramSocket::bind()
{
	if (_socketPath.length() == 0 )
	{
      throw SocketException(SocketException::BadUnixPathAddress); 	
	}
	
	// call unlink() before bind() to remove the socket if it already exists
	unlink(_socketPath.c_str());
	
	BaseSocket::bind(_socketPath.c_str());	
}

void UnixDatagramSocket::bind(const char *socketPath)
{
	_localAddress.setPathAddress(socketPath);
	_socketPath = socketPath;
    BaseSocket::bind(_socketPath.c_str());	
}

int UnixDatagramSocket::sendDatagram(void *pBuffer, int len, int direction)
{
	int retVal = 0;
	UnixSocketAddress *unixAddress = NULL;
	
	// send datagrams back to client, from server
	if (direction == UnixDatagramSocket::FromServerToClient)
	{
		unixAddress = &_fromAddress;
	}
	else
	{
		unixAddress = &_serverAddress;
	}
	
	retVal = BaseSocket::sendTo(pBuffer, len, 0, *unixAddress);
	
    return retVal;
}

//
// When a server receives a datagram, the _fromAddress needs to be
// used to send to the client. This is done using the sendDatagramFrom 
// method.
//
int UnixDatagramSocket::receiveDatagram(void *pBuffer, int len)
{
	int retVal = 0;

   retVal =  BaseSocket::recvFrom(pBuffer, len, 0, _fromAddress);

   return retVal;
}

void UnixDatagramSocket::getSockOpt(int codeLevel, 
		      int optionName, 
		      void *pOptionData, 
		      int &dataLength)
{
    BaseSocket::getSockOpt(codeLevel, 
			   optionName, 
			   pOptionData, 
			   dataLength);
}

void UnixDatagramSocket::setSockOpt(int codeLevel, 
			   int optionName, 
			   const void *pOptionData, 
			   int dataLength)
{
    BaseSocket::setSockOpt(codeLevel, 
			   optionName, 
			   pOptionData, 
			   dataLength);
}

const char *UnixDatagramSocket::getFromAddressPath()
{
	return _fromAddress.getPathAddress();
}
