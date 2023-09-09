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


#include <string.h>
#include <iostream>
#include <sstream>

#include <errno.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <pthread.h>

#include <pcff/socket/SocketExceptions.h>

#include <pcff/socket/CIDRAddress.h>

using namespace std;
using namespace pcff;

pthread_mutex_t CIDRAddress::_mutex = PTHREAD_MUTEX_INITIALIZER;

CIDRAddress::CIDRAddress()
{
    _addrFamily = AF_INET;
    initMembers();
}

CIDRAddress::CIDRAddress(int addrFamily)
: _addrFamily(addrFamily)
{
    initMembers();
}

CIDRAddress::~CIDRAddress()
{
}

void CIDRAddress::initMembers()
{
    if (_addrFamily == AF_INET6)
    {
        _addrLen = INETAddress::IPv6AddrLength;
    }
    else
    {
        _addrLen = INETAddress::INETAddress::IPv4AddrLength;
    }

    memset(&_addr, 0, INETAddress::MaxAddressLength);
    memset(&_strAddr, 0, INET6_ADDRSTRLEN);

    _cidrValue = 0;
}

void CIDRAddress::initialize(string &sIP, int cidrVal)
{
    setCIDRValue(cidrVal);
    setAddressFromIPString(sIP);
}

//
// pass in the CIDR, convert to network address
//
// see if address are equal??? if inValue == value then equal
//
// TODO: check for NULLs on the getAddressAsStr!!
// Print out statements
//

bool CIDRAddress::isIPAddrInSubnet(string &ipStr)
{
    bool retVal = false;
    INETAddress ipAddr;
    static u_int32_t ipVal = 0;
    static u_int32_t cidrVal = 0;
    u_int32_t mask = 0;

    if ((_addrFamily == AF_INET6))
    {
        throw SocketException(SocketException::NotSupported, "CIDRAddress::isIPAddrInSubnet for IPv6");
    }

    ipAddr.setAddressFromIPString(ipStr);

    mask = (0xffffffff >> (32 - _cidrValue )) << (32 - _cidrValue);

    ipVal = *(u_int32_t*)(ipAddr.getInAddr());
    Serializer::endianSwap(ipVal);
    cidrVal = *(u_int32_t*)(_addr);
    Serializer::endianSwap(cidrVal);


//    cout << "_cidrValue: " << hex << _cidrValue << endl;
//   cout << "mask: " << hex << mask << endl;
//    cout << "ipVal: " << hex << ipVal <<  endl;
//    cout << "cidrVal: " << hex << cidrVal<<  endl;
//    cout << "ipVal & mask: " << hex << (ipVal & mask) <<  endl;
//    cout << "cidrVal & mask: " << hex << (cidrVal & mask)<<  endl;

    return ((ipVal & mask) == (cidrVal & mask));
}

void CIDRAddress::_setAddress(const unsigned char *address, const int len)
{
    if ((_addrFamily == AF_INET6) && (_addrLen != INETAddress::IPv6AddrLength))
    {
        throw SocketException(SocketException::InvalidIPv6AddrLen);
    }
    else if ((_addrFamily == AF_INET) && (_addrLen != INETAddress::IPv4AddrLength))
    {
        throw SocketException(SocketException::InvalidIPv4AddrLen);
    }

    _addrLen = len;
    memcpy(&_addr, address, _addrLen);
}

CIDRAddress &CIDRAddress::operator=(const CIDRAddress &in)
{
    set(in);

    return *this;
}

void CIDRAddress::set(const CIDRAddress &in)
{
    _addrFamily = in._addrFamily;
    _addrLen = in._addrLen;
    _setAddress(in._addr, in._addrLen);
    inet_net_ntop(_addrFamily, _addr, _cidrValue, _strAddr, INET6_ADDRSTRLEN);
}

void CIDRAddress::setAddress(const unsigned char *newAddress)
{
    if (_addrFamily == AF_INET6)
    {
        setAddress(newAddress, INETAddress::IPv6AddrLength);
    }
    else
    {
        setAddress(newAddress, INETAddress::IPv4AddrLength);
    }
}

void CIDRAddress::setAddress(const unsigned char *newAddress, int addrLen)
{
    struct hostent *pHostEnt;

    _setAddress(newAddress, addrLen);
    inet_net_ntop(_addrFamily, _addr, _cidrValue, _strAddr, INET6_ADDRSTRLEN);

    if (pthread_mutex_lock(&_mutex) != 0)
    {
        throw SocketException(SocketException::Unknown);
    }

    pthread_mutex_unlock(&_mutex);
}

void CIDRAddress::setAddressFromIPString(std::string &sIPAddr)
{
    if (pthread_mutex_lock(&_mutex) != 0)
    {
        throw SocketException(SocketException::Unknown);
    }

    inet_net_pton(_addrFamily, sIPAddr.c_str(), &_addr, INETAddress::MaxAddressLength);

    if (_addrFamily == AF_INET6)
    {
        _addrLen = INETAddress::IPv6AddrLength;
    }
    else
    {
        _addrLen = INETAddress::IPv4AddrLength;
    }

    inet_ntop(_addrFamily, _addr, _strAddr, INET6_ADDRSTRLEN);

    pthread_mutex_unlock(&_mutex);

}

void CIDRAddress::serialize(pcff::Serializer &s)
{
    unsigned long addrLen = (unsigned long) INETAddress::MaxAddressLength;
    unsigned long strAddrLen = (unsigned long) INETAddress::MaxAddressStrLength;

    s.transfer(this->_addrFamily);
    s.transfer(this->_addrLen);
    s.transfer(this->_addr[0], addrLen);
    s.transfer(this->_strAddr[0], strAddrLen);
}

void CIDRAddress::print()
{
    cout << " - Name    : " << this << endl;
    cout.flush();
    cout << " - Family  : " << ((this->getAddressFamily() == AF_INET6) ? "IPv6" : "IPv4") << endl;
    cout.flush();
    cout << " - Address : " << this->getAddressAsStr() << endl;
    cout.flush();
}

void CIDRAddress::getInfo(string &sInfo)
{
    ostringstream eventInfo;

    eventInfo << " - Family  : " << ((_addrFamily == AF_INET6) ? "IPv6" : "IPv4") << endl;
    eventInfo << " - Address : " << _strAddr << endl;

    sInfo = eventInfo.str();
}
