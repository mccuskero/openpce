/**
 * Copyright (c) 1994-2007 Owen McCusker <pcave@ct.metrocast.net>
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

#include <string.h>

#include <pcff/socket/SocketExceptions.h>

using namespace pcff;

SocketException::SocketException(int exceptionNum, const char* msg)
{
    _type = ERR;
    switch (exceptionNum)
    {
    case NoProtocolSupport:
        setErrString("No Protocol Support\n");
        break;
    case KernelMemory:
        setErrString("Out of Kernel Memory\n");
        break;
    case NoDescriptors:
        setErrString("No Descriptors for Socket\n");
        break;
    case PermissionDenied:
        setErrString("Permission to create Socket denied\n");
        break;
    case Memory:
        setErrString("Memory Error\n");
        break;
    case InvalidProtocol:
        setErrString("Protocol unknown or not available\n");
        break;
    case HostNotFound:
        setErrString("Host Not Found\n");
        break;
    case NoINETAddress:
        setErrString("INET Address not available\n");
        break;
    case DNSError:
        setErrString("DNS error\n");
        break;
    case NotBound:
        setErrString("Address not bound to socket\n");
        break;
    case NotConnected:
        setErrString("Socket not connected\n");
        break;
    case NotInitialized:
        setErrString("Socket not initialized\n");
        break;
    case IncorrectAddrFamily:
        setErrString("Incorrect address family\n");
        break;
    case BadUnixPathAddress:
        setErrString("Bad Unix Socket Pathname");
        break;
    case InvalidIPv4AddrLen:
        setErrString("Invalid Length for IPv4 address");
        break;
    case InvalidIPv6AddrLen:
        setErrString("Invalid Length for IPv6 address");
        break;
    case NotSupported:
        setErrString("Feature not supported");
        break;
    case Unknown:
        setErrString("Unknown error\n");
        break;
    default:
        setErrString("Unknown error\n");
        break;
    }
}
