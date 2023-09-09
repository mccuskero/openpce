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

/** \class INETAddress
 * \brief Manages both IPv6 and IPv4 socket addresses
 * \author Owen. E McCusker
 * \version current version
 * \date    08/15/1997
 *
 * INETAddress manages Internet address for the socket framework.
 * This class encapsulates differences between IPv4, and IPv6.
 * The class inherits from Serializeable to allow the class to easily
 * be manages by streams e.g. sockets, files, ...
 *
 * \bug list of potential bugs (one per line)
 * \warning list of warnings (one per line)
 * \todo add here any comment about possible improvements
 */

#ifndef _INETAddress_h
#define _INETAddress_h

#include <stdio.h>
#include <errno.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>

#include <string>

#include <pcff/serialize/Serializeable.h>

namespace pcff
{

class INETAddress : public pcff::Serializeable
{
public:
    INETAddress();
    INETAddress(int addrfamily);
    ~INETAddress();

private:
    void initMembers();

public:

    enum eSocketConstants
    {
        MaxHostNameLength = 256,
        IPv4AddrLength = 4,
        IPv6AddrLength = 16,
        MaxAddressLength = IPv6AddrLength, // // IPv6 has 128-bit length (8 * 16)
        MaxAddressStrLength = INET6_ADDRSTRLEN
    };

public:

    const int getAddressFamily()
    {
        return _addrFamily;
    }

    const void *getInAddr()
    {
        return (void *) _addr;
    }

    const int getInAddrLen()
    {
        return _addrLen;
    }

    const char *getAddressAsStr()
    {
        return _strAddr;
    }

    void set(const INETAddress &in);
    INETAddress & operator=(const INETAddress &in);
    void setHostname(const char *name);
    void setAddress(const unsigned char *addr);
    void setAddress(const unsigned char *addr, int addrLen);
    void setAddressFromIPString(std::string sIPAddr);
    void setHostnameFromAddress(const unsigned char *addr);
    void setHostnameFromAddress(const unsigned char *addr, int addrLen);

public:
    static void seedRandom();
    static std::string createInetAddrIPv4();
    static std::string createInetAddrIPv4(int A);
    static std::string createInetAddrIPv4(int A, int B);
    static std::string createInetAddrIPv4(int A, int B, int C);
    static std::string createMacAddr();
    static std::string createNullMacAddr();

public:
    virtual void serialize(pcff::Serializer &s);
    virtual void setTestData();
    virtual void setTestData(std::string sAddr);

public:
    void getInfo(std::string &sInfo);
    void print();

protected:
    void _setHostname(const char *name);
    void _setAddress(const unsigned char *addr, const int len);

protected:
    int _addrFamily;
    int _addrLen;
    unsigned long _hostnameLen;
    char *_hostname;
    unsigned char _addr[MaxAddressLength]; // IPv6 has 128-bit length
    char _strAddr[MaxAddressStrLength]; // use largest string length

    static pthread_mutex_t _mutex;
};

}; // namespace

#endif
