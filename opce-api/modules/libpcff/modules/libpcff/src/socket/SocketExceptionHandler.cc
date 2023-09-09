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

#include <stdlib.h>
#include <errno.h>
#include <sys/socket.h>
#include <netdb.h>

#include <pcff/socket/SocketExceptions.h>

using namespace pcff;

void  SocketExceptionHandler::handleError(int context, int errNum)
{
   switch(context)
   {
      case SetSockOpt:
         switch (errNum)
         {
            case EBADF:
            case ENOTSOCK:
	      throw SetSockOptException(SetSockOptException::BadDescriptor);
            case ENOPROTOOPT:
	      throw SetSockOptException(SetSockOptException::OptionNotSupported);
            case EFAULT:
	      throw SetSockOptException(SetSockOptException::IllegalPointer);
            default:
	      throw SetSockOptException(SetSockOptException::Unknown);
         }
         break;
      case GetSockOpt:
         switch (errNum)
         {
            case EBADF:
            case ENOTSOCK:
	      throw GetSockOptException(GetSockOptException::BadDescriptor);
            case ENOPROTOOPT:
	      throw GetSockOptException(GetSockOptException::OptionNotSupported);
            case EFAULT:
	      throw GetSockOptException(GetSockOptException::IllegalPointer);
            default:
	      throw GetSockOptException(GetSockOptException::Unknown);
         }
         break;
      case GetPeerName:
       switch (errNum)
         {
            case EBADF:

	      throw GetPeerNameException(GetPeerNameException::BadDescriptor);
            case ENOBUFS:
	      throw GetPeerNameException(GetPeerNameException::KernelMemory);
            case EFAULT:
	      throw GetPeerNameException(GetPeerNameException::IllegalPointer);
            default:
	      throw GetPeerNameException(GetPeerNameException::Unknown);
         }
         break;
      case GetSockName:
          switch (errNum)
          {
            case EBADF:
            case ENOTSOCK:
	      throw GetSockNameException(GetSockNameException::BadDescriptor);
            case ENOBUFS:
	      throw GetSockNameException(GetSockNameException::KernelMemory);
            case EFAULT:
	      throw GetSockNameException(GetSockNameException::IllegalPointer);
            default:
	      throw GetSockNameException(GetSockNameException::Unknown);
          }
          break;
      case SendTo:
         switch (errNum)
         {
            case EBADF:
            case ENOTSOCK:
	      throw SendToException(SendToException::BadDescriptor);
            case EAGAIN:
	      throw SendToException(SendToException::WouldBlock);
            case EINTR:
	      throw SendToException(SendToException::Interrupted);
            case EFAULT:
	      throw SendToException(SendToException::IllegalPointer);
            case EINVAL:
	      throw SendToException(SendToException::InvalidArgument);
            case EMSGSIZE:
	      throw SendToException(SendToException::MessageSizeTooBig);
            case ENOBUFS:
            case ENOMEM:
	      throw SendToException(SendToException::KernelMemory);
            case EPIPE:
	      throw SendToException(SendToException::NotConnected);
            default:
	      throw SendToException(SendToException::Unknown);
         }
      case Send:
         switch (errNum)
         {
	    case EBADF:
	    case ENOTSOCK:
	       throw SendException(SendException::BadDescriptor);
	    case EAGAIN:
	       throw SendException(SendException::WouldBlock);
	    case EINTR:
	       throw SendException(SendException::Interrupted);
	    case EFAULT:
	       throw SendException(SendException::IllegalPointer);
	    case EINVAL:
	       throw SendException(SendException::InvalidArgument);
	    case EMSGSIZE:
	       throw SendException(SendException::MessageSizeTooBig);
	    case ENOBUFS:
	    case ENOMEM:
	       throw SendException(SendException::KernelMemory);
	    case EPIPE:
	       throw SendException(SendException::NotConnected);
	    default:
	       throw SendException(SendException::Unknown);
          }
          break;
      case RecvFrom:
         switch (errNum)
         {
            case EBADF:
            case ENOTSOCK:
	      throw RecvFromException(RecvFromException::BadDescriptor);
            case ENOTCONN:
	      throw RecvFromException(RecvFromException::NotConnected);
            case EAGAIN:
	      throw RecvFromException(RecvFromException::WouldBlock);
            case EINTR:
	      throw RecvFromException(RecvFromException::Interrupted);
            case EFAULT:
	      throw RecvFromException(RecvFromException::IllegalPointer);
            case EINVAL:
	      throw RecvFromException(RecvFromException::InvalidArgument);
            default:
	      throw RecvFromException(RecvFromException::Unknown);
         }
         break;
      case Recv:
         switch (errNum)
         {
            case EBADF:
            case ENOTSOCK:
	      throw RecvException(RecvException::BadDescriptor);
            case ENOTCONN:
	      throw RecvException(RecvException::NotConnected);
            case EAGAIN:
	      throw RecvException(RecvException::WouldBlock);
            case EINTR:
	      throw RecvException(RecvException::Interrupted);
            case EFAULT:
	      throw RecvException(RecvException::IllegalPointer);
            case EINVAL:
	      throw RecvException(RecvException::InvalidArgument);
            default:
	      throw RecvException(RecvException::Unknown);
          }
          break;
      case Accept:
         switch (errNum)
         {
            case EBADF:
            case ENOTSOCK:
	      throw AcceptException(AcceptException::BadDescriptor);
            case EAFNOSUPPORT:
	      throw AcceptException(AcceptException::NotStreamSock);
            case EFAULT:
	      throw AcceptException(AcceptException::IllegalPointer);
            case EAGAIN:
	      throw AcceptException(AcceptException::NoPendingConnections);
            case EPERM:
	      throw AcceptException(AcceptException::Firewall);
            case ENOMEM:
	      throw AcceptException(AcceptException::Memory);
            default:
	      throw AcceptException(AcceptException::Unknown);
          }
          break;
      case Bind:
         switch (errNum)
         {
            case EBADF:
            case ENOTSOCK:
	      throw BindException(BindException::BadDescriptor);
            case EINVAL:
	      throw BindException(BindException::AlreadyBound);
            case EACCES:
	      throw BindException(BindException::AddressProtected);
            case EADDRINUSE:
  	      throw BindException(BindException::AddrInUse);
            case EDESTADDRREQ:
            case EISDIR:
  	      throw BindException(BindException::BaddAddress);
            case EIO:
  	      throw BindException(BindException::IOError);
            case ELOOP:
  	      throw BindException(BindException::LoopExists);
            case ENAMETOOLONG:
  	      throw BindException(BindException::AddressTooLong);
            case ENOENT:
  	      throw BindException(BindException::AddressDoesNotExist);
            case ENOTDIR:
  	      throw BindException(BindException::AddressNotADirectory);
            case EROFS:
  	      throw BindException(BindException::AddressWouldResideOnReadOnlyFS);
            default:
	      throw BindException(BindException::Unknown);
         }
         break;
      case Listen:
         switch (errNum)
         {
            case EBADF:
            case ENOTSOCK:
	      throw ListenException(ListenException::BadDescriptor);
            case EOPNOTSUPP:
	      throw ListenException(ListenException::CantListen);
            default:
	      throw ListenException(ListenException::Unknown);
         }
         break;
      case Connect:
         switch (errNum)
         {
            case EBADF:
            case ENOTSOCK:
	      throw ConnectException(ConnectException::BadDescriptor);
            case EFAULT:
	      throw ConnectException(ConnectException::IllegalPointer);
            case EISCONN:
	      throw ConnectException(ConnectException::AlreadyConnected);
            case ECONNREFUSED:
	      throw ConnectException(ConnectException::ConnectRefused);
            case ETIMEDOUT:
	      throw ConnectException(ConnectException::ConnectTimeOut);
            case ENETUNREACH:
	      throw ConnectException(ConnectException::NetUnreachable);
            case EADDRINUSE:
	      throw ConnectException(ConnectException::AddrInUse);
            case EINPROGRESS:
	      throw ConnectException(ConnectException::InProgress);
            case EALREADY:
	      throw ConnectException(ConnectException::AlreadyConnecting);
            case EAFNOSUPPORT:
	      throw ConnectException(ConnectException::IncorrectAddrFamily);
            case EACCES:
	      throw ConnectException(ConnectException::BrdCastNotEnabled);
            default:
	      throw ConnectException(ConnectException::Unknown);
          }
          break;
      case General:
         switch (errNum)
         {
            case EPROTONOSUPPORT:
	      throw SocketException(SocketException::NoProtocolSupport);
            case ENFILE:
	      throw SocketException(SocketException::KernelMemory);
            case EMFILE:
	      throw SocketException(SocketException::NoDescriptors);
            case EACCES:
	      throw SocketException(SocketException::PermissionDenied);
            case ENOMEM:
	    case ENOBUFS:
	      throw SocketException(SocketException::Memory);
            case EINVAL:
	      throw SocketException(SocketException::InvalidProtocol);
            default:
	      throw SocketException(SocketException::Unknown);
         }
         break;
      case GetHostByName2:
         switch (errNum)
	 {
	    case HOST_NOT_FOUND:
	       throw SocketException(SocketException::HostNotFound);
	     case NO_ADDRESS:
	       throw SocketException(SocketException::NoINETAddress);
	     case NO_RECOVERY:
	     case TRY_AGAIN:
	       throw SocketException(SocketException::DNSError);
	     default:
	       throw SocketException(SocketException::Unknown);
         }
         break;
      default:
	 throw SocketException(SocketException::Unknown);
         break;
   }
}
