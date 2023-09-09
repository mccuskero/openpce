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

#ifndef _SocketException_h
#define _SocketException_h


#include <pcff/BasicException.h>

namespace pcff
{

class SocketExceptionHandler
{
   public:
      typedef enum {
         SetSockOpt=1,
         GetSockOpt,
         GetPeerName,
         GetSockName,
         SendTo,
         Send,
         RecvFrom,
         Recv,
         Bind,
         Listen,
         Accept,
         Connect,
         GetHostByName2,
         General
      } ErrorContext;

   public:
      static void handleError(int context, int errorNum);
};

class SocketException : public pcff::BasicException
{
    public:
        enum ExceptionEnums
	{
	  NoProtocolSupport,
	  KernelMemory,
	  NoDescriptors,
	  PermissionDenied,
	  Memory,
	  InvalidProtocol,
	  HostNotFound,
	  NoINETAddress,
	  DNSError,
	  NotBound,
	  NotConnected,
	  NotInitialized,
	  IncorrectAddrFamily,
	  BadUnixPathAddress,
	  InvalidIPv4AddrLen,
	  InvalidIPv6AddrLen,
          NotSupported,
	  Unknown
	};

	SocketException(int type = BasicException::UnknownError, const char* msg=NULL);
};


class ConnectException : public pcff::BasicException
{
    public:
        enum ExceptionEnums
	{
	  BadDescriptor,
	  IllegalPointer,
	  AlreadyConnected,
	  ConnectRefused,
	  ConnectTimeOut,
	  NetUnreachable,
	  AddrInUse,
	  InProgress,
	  AlreadyConnecting,
	  IncorrectAddrFamily,
	  BrdCastNotEnabled,
	  Unknown
	};

	ConnectException(int type = BasicException::UnknownError, char* msg=NULL);
};

class BindException : public pcff::BasicException
{
    public:
        enum ExceptionEnums
	{
		BadDescriptor,
		AlreadyBound,
		AddressProtected,
		AddrInUse,
		BaddAddress,
		IOError,
		LoopExists,
		AddressTooLong,
		AddressDoesNotExist,
		AddressNotADirectory,
		AddressWouldResideOnReadOnlyFS,
		Unknown
	};

	BindException(int type = BasicException::UnknownError, char* msg=NULL);
};

class ListenException : public pcff::BasicException
{
    public:
        enum ExceptionEnums
	{
	  BadDescriptor,
	  CantListen,
	  Unknown
	};

	ListenException(int type = BasicException::UnknownError, char* msg=NULL);
};

class AcceptException : public pcff::BasicException
{
    public:
        enum ExceptionEnums
	{
	  BadDescriptor,
	  Memory,
	  IllegalPointer,
	  NotStreamSock,
	  NoPendingConnections,
	  Firewall,
	  Unknown
	};

	AcceptException(int type = BasicException::UnknownError, char* msg=NULL);
};

class RecvException : public pcff::BasicException
{
    public:
        enum ExceptionEnums
	{
	  BadDescriptor,
	  NotConnected,
	  NotConnectedGracefulExit,
	  IllegalPointer,
	  WouldBlock,
	  Interrupted,
	  InvalidArgument,
	  Unknown
	};

	RecvException(int type = BasicException::UnknownError, char* msg=NULL);
};

class RecvFromException : public pcff::BasicException
{
    public:
        enum ExceptionEnums
	{
	  BadDescriptor,
	  NotConnectedGracefulExit,
	  NotConnected,
	  IllegalPointer,
	  WouldBlock,
	  Interrupted,
	  InvalidArgument,
	  Unknown
	};

	RecvFromException(int type = BasicException::UnknownError, char* msg=NULL);
};

class SendException : public pcff::BasicException
{
    public:
        enum ExceptionEnums
	{
	  BadDescriptor,
	  WouldBlock,
	  Interrupted,
	  NotConnected,
	  IllegalPointer,
	  InvalidArgument,
	  MessageSizeTooBig,
	  KernelMemory,
	  Unknown
	};

	SendException(int type = BasicException::UnknownError, char* msg=NULL);
};

class SendToException : public pcff::BasicException
{
    public:
        enum ExceptionEnums
	{
	  BadDescriptor,
	  WouldBlock,
	  Interrupted,
	  NotConnected,
	  IllegalPointer,
	  InvalidArgument,
	  MessageSizeTooBig,
	  KernelMemory,
	  Unknown
	};

	SendToException(int type = BasicException::UnknownError, char* msg=NULL);
};

class GetSockNameException : public pcff::BasicException
{
    public:
        enum ExceptionEnums
	{
	  BadDescriptor,
	  IllegalPointer,
	  KernelMemory,
	  Unknown
	};

	GetSockNameException(int type = BasicException::UnknownError, char* msg=NULL);
};

class GetPeerNameException : public pcff::BasicException
{
    public:
        enum ExceptionEnums
	{
	  BadDescriptor,
	  IllegalPointer,
	  KernelMemory,
	  Unknown
	};

	GetPeerNameException(int type = BasicException::UnknownError, char* msg=NULL);
};

class GetSockOptException : public pcff::BasicException
{
    public:
        enum ExceptionEnums
	{
	  BadDescriptor,
	  OptionNotSupported,
	  IllegalPointer,
	  Unknown
	};

	GetSockOptException(int type = BasicException::UnknownError, char* msg=NULL);
};

class SetSockOptException : public pcff::BasicException
{
    public:
        enum ExceptionEnums
	{
	  BadDescriptor,
	  OptionNotSupported,
	  IllegalPointer,
	  Unknown
	};

	SetSockOptException(int type = BasicException::UnknownError, char* msg=NULL);
};

}; // namespace

#endif
