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

#include <string>


#include <pcff/socket/SocketExceptions.h>

#include <pcff/socket/INETAddress.h>
#include <pcff/socket/IPv4SocketAddress.h>
#include <pcff/socket/IPv6SocketAddress.h>
#include <pcff/socket/UnixSocketAddress.h>
#include <pcff/socket/BaseSocket.h>

using namespace std;
using namespace pcff;

BaseSocket::BaseSocket(int addressFamily,
		       int socketType)
        : _addrFamily(addressFamily), _socketType(socketType)
{
   _socketLocale = BaseSocket::Remote;
   _socketState = BaseSocket::Uninitialized;
}


BaseSocket::~BaseSocket()
{
    close(_sockDesc);
}

void BaseSocket::init()
{
   if ((_sockDesc = ::socket(_addrFamily, _socketType, 0)) < 0)
   {
       SocketExceptionHandler::handleError(SocketExceptionHandler::General, errno);
   }

   _socketState = BaseSocket::Initialized;

   if (_addrFamily != AF_UNIX_DOMAIN)
   {
		_pLocalAddress = new INETAddress(_addrFamily);
		_pRemoteAddress = new INETAddress(_addrFamily);
	}
}

//
// factory method to create socket address of the right family
//
SocketAddress *BaseSocket::_getSocketAddress()
{
   SocketAddress *addr = NULL;

   if (_addrFamily == BaseSocket::AF_UNIX_DOMAIN)
   {
      addr = new UnixSocketAddress();
   }
   else if (_addrFamily == BaseSocket::AF_INET_IPv4)
   {
      addr = new IPv4SocketAddress();
   }
   else if (_addrFamily == BaseSocket::AF_INET_IPv6)
   {
      addr = new IPv6SocketAddress();
   }
   else
   {
	  throw SocketException(SocketException::IncorrectAddrFamily);
   }

   return addr;
}

void BaseSocket::_resetSocketDescriptor(int newSockDesc, int addrFamily)
{
    close(_sockDesc);
    _sockDesc = newSockDesc;
    _addrFamily = addrFamily;
}

void BaseSocket::_setLocal()
{
    if (_socketState == BaseSocket::Uninitialized) throw SocketException(SocketException::NotInitialized);
    getSockName(*_pLocalAddress, _localPort);
    _socketLocale = BaseSocket::Local;
}


void BaseSocket::_setConnected()
{
    if (_socketState == BaseSocket::Uninitialized) throw SocketException(SocketException::NotInitialized);

	if (_addrFamily !=  AF_UNIX_DOMAIN)
	{
	    getPeerName(*_pRemoteAddress, _remotePort);
    }
	_socketState = BaseSocket::Connected;
}


void BaseSocket::connect(INETAddress &servAddr, int port)
{
    if (_socketState == BaseSocket::Uninitialized) throw SocketException(SocketException::NotInitialized);
    SocketAddress *serverAddr = _getSocketAddress();

    serverAddr->setINETAddress(servAddr);
    serverAddr->setPort(port);

    if (::connect(_sockDesc, serverAddr->getSockAddrStruct(), serverAddr->sizeOf()) < 0)
    {
        SocketExceptionHandler::handleError(SocketExceptionHandler::Connect, errno);
    }
    delete serverAddr;
    _socketState = BaseSocket::Connected;
}

// for Unix Domain socket
void BaseSocket::connect(const char *socketPath)
{
    if (_socketState == BaseSocket::Uninitialized) throw SocketException(SocketException::NotInitialized);
    SocketAddress *serverAddr = _getSocketAddress();

	_socketPath = socketPath;

    serverAddr->setPathAddress((char *)socketPath);

    if (::connect(_sockDesc, serverAddr->getSockAddrStruct(), serverAddr->sizeOf()) < 0)
    {
        SocketExceptionHandler::handleError(SocketExceptionHandler::Connect, errno);
    }
    delete serverAddr;
    _socketState = BaseSocket::Connected;
}

void BaseSocket::bind(INETAddress &localAddr, int port)
{
    if (_socketState == BaseSocket::Uninitialized) throw SocketException(SocketException::NotInitialized);
    SocketAddress *localSockAddr = _getSocketAddress();

    localSockAddr->setINETAddress(localAddr);
    localSockAddr->setPort(port);

    if (::bind(_sockDesc, localSockAddr->getSockAddrStruct(), localSockAddr->sizeOf()) < 0)
    {
        SocketExceptionHandler::handleError(SocketExceptionHandler::Bind, errno);
    }
    delete localSockAddr;
    _socketState = BaseSocket::Bound;
}

void BaseSocket::bind(int port)
{
    if (_socketState == BaseSocket::Uninitialized) throw SocketException(SocketException::NotInitialized);
    SocketAddress *localSockAddr = _getSocketAddress();

    localSockAddr->setINETAddressToAny();
    localSockAddr->setPort(port);

    if (::bind(_sockDesc, localSockAddr->getSockAddrStruct(), localSockAddr->sizeOf()) < 0)
    {
        SocketExceptionHandler::handleError(SocketExceptionHandler::Bind, errno);
    }
    delete localSockAddr;
    _socketState = BaseSocket::Bound;
}

// for Unix Domain socket
void BaseSocket::bind(const char *socketPath)
{
    if (_socketState == BaseSocket::Uninitialized) throw SocketException(SocketException::NotInitialized);
    SocketAddress *localSockAddr = _getSocketAddress();

	_socketPath = socketPath;

    localSockAddr->setPathAddress((char *)socketPath);

    if (::bind(_sockDesc, localSockAddr->getSockAddrStruct(), localSockAddr->sizeOf()) < 0)
    {
        SocketExceptionHandler::handleError(SocketExceptionHandler::Bind, errno);
    }
    delete localSockAddr;
    _socketState = BaseSocket::Bound;
}


void BaseSocket::listen(int backLog)
{
    if (_socketState != BaseSocket::Bound) throw SocketException(SocketException::NotBound);
    if ( ::listen(_sockDesc, backLog) < 0)
    {
        SocketExceptionHandler::handleError(SocketExceptionHandler::Listen, errno);
    }
}


void BaseSocket::accept(BaseSocket *pConnection,
		   INETAddress &remoteAddr,
		   int &port)
{
    if (_socketState == BaseSocket::Uninitialized) throw SocketException(SocketException::NotInitialized);
    int           newSockDesc;
    SocketAddress *remoteSocketAddr = _getSocketAddress();
    socklen_t     len = remoteSocketAddr->sizeOf();

    if ((newSockDesc = ::accept(_sockDesc,
				remoteSocketAddr->getSockAddrStruct(),
				&len)) < 0)
    {
        SocketExceptionHandler::handleError(SocketExceptionHandler::Accept, errno);
     }
    remoteSocketAddr->getINETAddress(remoteAddr);
    remoteSocketAddr->getPort(port);

    pConnection->_resetSocketDescriptor(newSockDesc,remoteAddr.getAddressFamily());
    delete remoteSocketAddr;
}

void BaseSocket::accept(BaseSocket *pConnection,
						UnixSocketAddress &remoteAddr)
{
    if (_socketState == BaseSocket::Uninitialized) throw SocketException(SocketException::NotInitialized);
    int           newSockDesc;
    SocketAddress *remoteSocketAddr = _getSocketAddress();
    socklen_t     len = remoteSocketAddr->sizeOf();

    if ((newSockDesc = ::accept(_sockDesc,
				remoteSocketAddr->getSockAddrStruct(),
				&len)) < 0)
    {
        SocketExceptionHandler::handleError(SocketExceptionHandler::Accept, errno);
	}

    pConnection->_resetSocketDescriptor(newSockDesc, BaseSocket::AF_UNIX_DOMAIN);

    delete remoteSocketAddr;
}

int BaseSocket::recv(void *pBuffer,
		 int bufLength,
		 unsigned int flags)
{
  // TODO: put in some exception handling
  int     bytesRead = 0;
  if (_socketState != BaseSocket::Connected)
	  throw SocketException(SocketException::NotConnected);

  bytesRead = ::recv(_sockDesc,
							pBuffer,
							bufLength,
							flags      );
	if (bytesRead > 0)
	{
		return bytesRead;
	}
	else if (bytesRead == 0)   // graceful exit
	{
		return bytesRead;
	}
	else if (bytesRead == -1)
	{
		SocketExceptionHandler::handleError(SocketExceptionHandler::Recv, errno);
	}
	else
	{
		SocketExceptionHandler::handleError(SocketExceptionHandler::Recv, errno);
	}
}

int BaseSocket::recvFrom(void *pBuffer,
		     int bufLength,
		     unsigned int flags,
                     INETAddress &sourceIP,
		     int &port)
{
    SocketAddress   *sourceSockAddr = _getSocketAddress();
    socklen_t       len = sourceSockAddr->sizeOf();
    int             bytesRead = 0;

   if (_socketState == BaseSocket::Uninitialized)
	   throw SocketException(SocketException::NotInitialized);

   bytesRead = ::recvfrom(_sockDesc,
   				pBuffer,
   				bufLength,
   				flags,
   				sourceSockAddr->getSockAddrStruct(),
   				&len);

    if (bytesRead > 0)
    {
        sourceSockAddr->getINETAddress(sourceIP);
        sourceSockAddr->getPort(port);
        delete sourceSockAddr;
        return bytesRead;
    }
	else if (bytesRead == 0)  // orderly shutdown of peer was done on socket
	{
		//	    throw RecvFromException(RecvFromException::NotConnectedGracefulExit);
		        return bytesRead;
	}
	else if (bytesRead == -1)
	{
		SocketExceptionHandler::handleError(SocketExceptionHandler::RecvFrom, errno);
	}
	else
	{
		SocketExceptionHandler::handleError(SocketExceptionHandler::RecvFrom, errno);
	}
}

int BaseSocket::recvFrom(void  *pBuffer,
		     int               bufLength,
		     unsigned int      flags,
			 UnixSocketAddress &unixAddress)
{
    SocketAddress   *sourceSockAddr = _getSocketAddress();
    socklen_t       len = sourceSockAddr->sizeOf();
    int             bytesRead = 0;

	sourceSockAddr->setPathAddress(unixAddress.getPathAddress());

   if (_socketState == BaseSocket::Uninitialized) throw SocketException(SocketException::NotInitialized);

   bytesRead = ::recvfrom(_sockDesc,
   				pBuffer,
   				bufLength,
   				flags,
   				sourceSockAddr->getSockAddrStruct(),
   				&len);

    if (bytesRead > 0)
    {
		sourceSockAddr->updateAddressPathFromStruct();  // recvfrom call updated the sockaddr struct, need to update class member
	    unixAddress.setPathAddress((UnixSocketAddress &)*sourceSockAddr);
        delete sourceSockAddr;
        return bytesRead;
    }
	else if (bytesRead == 0)  // orderly shutdown of peer was done on socket
	{
//	    throw RecvFromException(RecvFromException::NotConnectedGracefulExit);
        return bytesRead;
	}
	else if (bytesRead == -1)
	{
		SocketExceptionHandler::handleError(SocketExceptionHandler::RecvFrom, errno);
	}
	else
	{
		SocketExceptionHandler::handleError(SocketExceptionHandler::RecvFrom, errno);
	}
}


int BaseSocket::send(void *pBuffer,
		 int bufLength,
		 unsigned int flags)
{
  int             bytesSent = 0;
  if (_socketState != BaseSocket::Connected) throw SocketException(SocketException::NotConnected);

  if ((bytesSent = ::send(_sockDesc,
			  pBuffer,
			  bufLength,
			  flags)) < 0)
    {
        SocketExceptionHandler::handleError(SocketExceptionHandler::Send, errno);
    }

    return bytesSent;
}


int BaseSocket::sendTo(void *pBuffer,
		   int bufLength,
		   unsigned int flags,
                   INETAddress &destinationIP,
		   int port)
{
   if (_socketState == BaseSocket::Uninitialized) throw SocketException(SocketException::NotInitialized);
    SocketAddress   *destSockAddr = _getSocketAddress();
    int             bytesSent = 0;;

    destSockAddr->setINETAddress(destinationIP);
    destSockAddr->setPort(port);

    if ((bytesSent = ::sendto(_sockDesc,
			       pBuffer,
			       bufLength,
			       flags,
			       destSockAddr->getSockAddrStruct(),
			       destSockAddr->sizeOf())) < 0)
    {
        SocketExceptionHandler::handleError(SocketExceptionHandler::SendTo, errno);
    }

    delete destSockAddr;

    return bytesSent;
}

int BaseSocket::sendTo(void		*pBuffer,
				int				bufLength,
				unsigned int	flags,
				UnixSocketAddress &unixAddress)
{
   if (_socketState == BaseSocket::Uninitialized) throw SocketException(SocketException::NotInitialized);
    SocketAddress   *destSockAddr = _getSocketAddress();
    int             bytesSent = 0;

	destSockAddr->setPathAddress(unixAddress.getPathAddress());

    if ((bytesSent = ::sendto(_sockDesc,
			       pBuffer,
			       bufLength,
			       flags,
			       destSockAddr->getSockAddrStruct(),
			       destSockAddr->sizeOf())) < 0)
    {
        SocketExceptionHandler::handleError(SocketExceptionHandler::SendTo, errno);
    }

	delete destSockAddr;

    return bytesSent;
}


void BaseSocket::getSockName(INETAddress &localAddr,
			int &port)
{
    SocketAddress   *localSockAddr = _getSocketAddress();
    socklen_t       len = localSockAddr->sizeOf();

    if ( ::getsockname(_sockDesc,
		       localSockAddr->getSockAddrStruct(),
		       &len) < 0)
    {
        SocketExceptionHandler::handleError(SocketExceptionHandler::GetSockName, errno);
    }

    localSockAddr->getINETAddress(localAddr);
    localSockAddr->getPort(port);
}

void BaseSocket::getPeerName(INETAddress &remoteAddr,
			int &port)
{
    SocketAddress   *remoteSockAddr = _getSocketAddress();
    socklen_t       len = remoteSockAddr->sizeOf();

    if ( ::getpeername(_sockDesc,
		       remoteSockAddr->getSockAddrStruct(),
                       &len) < 0)
    {
        SocketExceptionHandler::handleError(SocketExceptionHandler::GetPeerName, errno);
    }

    remoteSockAddr->getINETAddress(remoteAddr);
    remoteSockAddr->getPort(port);
}


void BaseSocket::getSockOpt(int codeLevel,
		       int optionName,
		       void *pOptionData,
		       int &dataLen)
{
    if ( ::getsockopt(_sockDesc,
		      codeLevel,
		      optionName,
		      pOptionData,
		      (socklen_t *)&dataLen) < 0)
    {
        SocketExceptionHandler::handleError(SocketExceptionHandler::GetSockOpt, errno);
    }
}


void
BaseSocket::setSockOpt(int codeLevel,
		       int optionName,
		       const void *pOptionData,
		       int dataLen)
{
    if ( ::setsockopt(_sockDesc,
		      codeLevel,
		      optionName,
		      pOptionData,
		      dataLen) < 0)
    {
        SocketExceptionHandler::handleError(SocketExceptionHandler::SetSockOpt, errno);
    }
}

INETAddress &BaseSocket::localINETAddr()
{
   if (_socketState == BaseSocket::Uninitialized) throw SocketException(SocketException::NotInitialized);
   if ( (_socketState == BaseSocket::Remote) || (_pLocalAddress == NULL) )
   {
       throw SocketException(SocketException::NotBound);
   }
   return *_pLocalAddress;
}

int BaseSocket::localPort()
{
   if (_socketState == BaseSocket::Uninitialized) throw SocketException(SocketException::NotInitialized);
   if (_socketState == BaseSocket::Remote)
   {
       throw SocketException(SocketException::NotBound);
   }
   return _localPort;
}


INETAddress &BaseSocket::remoteINETAddr()
{
   if (_socketState == BaseSocket::Uninitialized) throw SocketException(SocketException::NotInitialized);
   if ((_socketState != BaseSocket::Connected) || (_pRemoteAddress == NULL))
   {
       throw SocketException(SocketException::NotConnected);
   }
   return *_pRemoteAddress;
}


int BaseSocket::remotePort()
{
   if (_socketState == BaseSocket::Uninitialized) throw SocketException(SocketException::NotInitialized);
   if (_socketState != BaseSocket::Connected)
   {
       throw SocketException(SocketException::NotConnected);
   }
   return _remotePort;
}

string BaseSocket::socketPath()
{
   if (_socketState == BaseSocket::Uninitialized) throw SocketException(SocketException::NotInitialized);

   if ( !((_socketState != BaseSocket::Bound) ||
        _socketState != BaseSocket::Connected)   )
   {
       throw SocketException(SocketException::NotConnected);
   }

   if (_addrFamily != BaseSocket::AF_UNIX_DOMAIN)
   {
      throw SocketException(SocketException::IncorrectAddrFamily);
   }

	return _socketPath;
}
