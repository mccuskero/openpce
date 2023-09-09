/**
 * Copyright (c) 1994 Owen McCusker <pcave@myeastern.com>
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

#ifndef _CIDRAddress_H
#define	_CIDRAddress_H

#include <string>
#include <pcff/socket/INETAddress.h>

namespace pcff
{

class CIDRAddress
{
public:
    CIDRAddress();
    CIDRAddress(int addrfamily);
    virtual ~CIDRAddress();

private:
    void initMembers();

public:
    CIDRAddress & operator=(const CIDRAddress &in);

public:
    void initialize(std::string &sIP, int cidrValue);

public:
    bool isIPAddrInSubnet(std::string &ipStr);

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

    void setCIDRValue(int val)
    {
        _cidrValue = val;
    }

    int getCIDRValue()
    {
        return _cidrValue;
    }

public:
    void set(const CIDRAddress &in);
    void setAddress(const unsigned char *addr);
    void setAddress(const unsigned char *addr, int addrLen);
    void setAddressFromIPString(std::string &sIPAddr);

public:
    virtual void serialize(pcff::Serializer &s);

public:
    void getInfo(std::string &sInfo);
    void print();

protected:
    void _setAddress(const unsigned char *addr, const int len);
    //    void _extractCIDRBits(const unsigned char *addr);  // TODO

protected:
    int _addrFamily;
    int _addrLen;
    unsigned char _addr[INETAddress::MaxAddressLength]; // IPv6 has 128-bit length
    char _strAddr[INETAddress::MaxAddressStrLength]; // use largest string length
    int _cidrValue;                                  // /24 is 24

    static pthread_mutex_t _mutex;

};

};


#endif	/* _CIDRAddress_H */

