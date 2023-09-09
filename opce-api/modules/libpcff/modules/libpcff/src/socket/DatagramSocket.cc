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
#include <pcff/socket/DatagramSocket.h>

using namespace pcff;

DatagramSocket::DatagramSocket(int family) 
  : BaseSocket(family, SOCK_DGRAM)
{
}

//
// for server socket
//


void DatagramSocket::init()
{
  BaseSocket::init();
}

void DatagramSocket::init(INETAddress &destAddr, 
						 int port)
{
  _destPort = port;
  _destAddr.set(destAddr);
  BaseSocket::init();
}

void DatagramSocket::bind(INETAddress &localAddr, 
				         int port,
						 bool bUseIPv6)
{
  BaseSocket::bind(localAddr, port);
  getSockName(*_pLocalAddress, _localPort);
}

void DatagramSocket::bind( int port,
		     bool bUseIPv6)
{
  BaseSocket::bind(port);
  getSockName(*_pLocalAddress, _localPort);
}

int DatagramSocket::sendDatagram(void *pBuffer, 
								 int len)
{
    return sendTo(pBuffer, len, 0, _destAddr, _destPort);
}

int DatagramSocket::sendDatagram(void *pBuffer, 
			    int len, 
			    INETAddress &destAddr,
			    int destPort)
{
    return sendTo(pBuffer, len, 0, destAddr, destPort);
}

int DatagramSocket::receiveDatagram(void *pBuffer, 
			   int len, 
			   INETAddress &sourceAddr,
                           int &sourcePort)
{
    return recvFrom(pBuffer, len, 0, sourceAddr, sourcePort);
}


void DatagramSocket::getSockOpt(int codeLevel, 
		      int optionName, 
		      void *pOptionData, 
		      int &dataLength)
{
    BaseSocket::getSockOpt(codeLevel, 
			   optionName, 
			   pOptionData, 
			   dataLength);
}


void DatagramSocket::setSockOpt(int codeLevel, 
			   int optionName, 
			   const void *pOptionData, 
			   int dataLength)
{
    BaseSocket::setSockOpt(codeLevel, 
			   optionName, 
			   pOptionData, 
			   dataLength);
}
