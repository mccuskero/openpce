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

#include <string.h>
#include <iostream>
#include <sstream>

#include <errno.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <pthread.h>

#include <pcff/socket/SocketExceptions.h>
#include <pcff/socket/INETAddress.h>

using namespace std;
using namespace pcff;

pthread_mutex_t INETAddress::_mutex = PTHREAD_MUTEX_INITIALIZER;

INETAddress::INETAddress()
{
    _addrFamily = AF_INET;
    initMembers();
}

INETAddress::INETAddress(int addrFamily)
: _addrFamily(addrFamily)
{
    initMembers();
}

INETAddress::~INETAddress()
{
    delete [] _hostname;
}

void INETAddress::initMembers()
{
    _hostname = new char[MaxHostNameLength];

    if (_addrFamily == AF_INET6)
    {
        _addrLen = IPv6AddrLength;
    }
    else
    {
        _addrLen = IPv4AddrLength;
    }

    memset(_hostname, 0, MaxHostNameLength);
    memset(&_addr, 0, INETAddress::MaxAddressLength);
    memset(&_strAddr, 0, INET6_ADDRSTRLEN);
}

void INETAddress::_setHostname(const char *name)
{
    if (name == NULL) return;
    if (strlen(name) == 0) return;

    _hostnameLen = strlen(name) + 1;

    char *temp = new char[_hostnameLen];
    delete [] _hostname;
    _hostname = temp;
    strcpy(_hostname, name);
}

void INETAddress::_setAddress(const unsigned char *address, const int len)
{
    if ((_addrFamily == AF_INET6) && (_addrLen != IPv6AddrLength))
    {
        throw SocketException(SocketException::InvalidIPv6AddrLen);
    }
    else if ((_addrFamily == AF_INET) && (_addrLen != IPv4AddrLength))
    {
        throw SocketException(SocketException::InvalidIPv4AddrLen);
    }

    _addrLen = len;
    memcpy(&_addr, address, _addrLen);
}

INETAddress &INETAddress::operator=(const INETAddress &in)
{
    set(in);

    return *this;
}

void INETAddress::set(const INETAddress &in)
{
    _addrFamily = in._addrFamily;
    _setHostname(in._hostname);
    _addrLen = in._addrLen;
    _setAddress(in._addr, in._addrLen);
    inet_ntop(_addrFamily, _addr, _strAddr, INET6_ADDRSTRLEN);
}

void INETAddress::setHostname(const char *name)
{
    struct hostent *pHostEnt;

    if (pthread_mutex_lock(&_mutex) != 0)
    {
        throw SocketException(SocketException::Unknown);
    }

    pHostEnt = gethostbyname2(name, _addrFamily);
    if (pHostEnt == NULL)
    {
        pthread_mutex_unlock(&_mutex);
        SocketExceptionHandler::handleError(SocketExceptionHandler::GetHostByName2, h_errno);
    }

    _addrFamily = pHostEnt->h_addrtype;

    _setHostname(pHostEnt->h_name);
    _setAddress((unsigned char *) pHostEnt->h_addr_list[0], pHostEnt->h_length);

    inet_ntop(_addrFamily, _addr, _strAddr, INET6_ADDRSTRLEN);

    pthread_mutex_unlock(&_mutex);
}

void INETAddress::setAddress(const unsigned char *newAddress)
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

void INETAddress::setAddress(const unsigned char *newAddress, int addrLen)
{
    struct hostent *pHostEnt;

    _setAddress(newAddress, addrLen);
    inet_ntop(_addrFamily, _addr, _strAddr, INET6_ADDRSTRLEN);

    if (pthread_mutex_lock(&_mutex) != 0)
    {
        throw SocketException(SocketException::Unknown);
    }

    _setHostname(_strAddr);

    pthread_mutex_unlock(&_mutex);
}

void INETAddress::setAddressFromIPString(std::string sIPAddr)
{
    if (pthread_mutex_lock(&_mutex) != 0)
    {
        throw SocketException(SocketException::Unknown);
    }

    inet_pton(_addrFamily, sIPAddr.c_str(), &_addr);

    if (_addrFamily == AF_INET6)
    {
        _addrLen = INETAddress::IPv6AddrLength;
    }
    else
    {
        _addrLen = INETAddress::IPv4AddrLength;
    }


    strcpy(_strAddr, sIPAddr.c_str());
    _hostnameLen = MaxHostNameLength;
    strcpy(_hostname, sIPAddr.c_str());

    pthread_mutex_unlock(&_mutex);

}

void INETAddress::setHostnameFromAddress(const unsigned char *newAddress)
{
    if (_addrFamily == AF_INET6)
    {
        setHostnameFromAddress(newAddress, INETAddress::IPv6AddrLength);
    }
    else
    {
        setHostnameFromAddress(newAddress, INETAddress::IPv4AddrLength);
    }
}

void INETAddress::setHostnameFromAddress(const unsigned char *newAddress, int addrLen)
{
    struct hostent *pHostEnt;

    _setAddress(newAddress, addrLen);
    inet_ntop(_addrFamily, _addr, _strAddr, INET6_ADDRSTRLEN);

    if (pthread_mutex_lock(&_mutex) != 0)
    {
        throw SocketException(SocketException::Unknown);
    }

    pHostEnt = gethostbyaddr(newAddress, _addrLen, _addrFamily);
    if (pHostEnt == NULL)
    {
        _setHostname(_strAddr);
    }
    else
    {
        _setHostname(pHostEnt->h_name);
    }

    pthread_mutex_unlock(&_mutex);
}

void INETAddress::serialize(pcff::Serializer &s)
{
    unsigned long addrLen = (unsigned long) INETAddress::MaxAddressLength;
    unsigned long strAddrLen = (unsigned long) INETAddress::MaxAddressStrLength;

    s.transfer(this->_addrFamily);
    s.transfer(this->_addrLen);
    s.transfer(this->_hostnameLen);
    s.transfer(this->_hostname, this->_hostnameLen);
    s.transfer(this->_addr[0], addrLen);
    s.transfer(this->_strAddr[0], strAddrLen);
}

void INETAddress::print()
{
    cout << " - Name    : " << this << endl;
    cout.flush();
    cout << " - Family  : " << ((this->getAddressFamily() == AF_INET6) ? "IPv6" : "IPv4") << endl;
    cout.flush();
    cout << " - Address : " << this->getAddressAsStr() << endl;
    cout.flush();
}

void INETAddress::getInfo(string &sInfo)
{
    ostringstream eventInfo;

    eventInfo << " - Hostname: " << _hostname << endl;
    eventInfo << " - Family  : " << ((_addrFamily == AF_INET6) ? "IPv6" : "IPv4") << endl;
    eventInfo << " - Address : " << _strAddr << endl;

    sInfo = eventInfo.str();
}

void INETAddress::setTestData()
{
    string sAddr;

    if (_addrFamily == AF_INET6)
    {
        throw SocketException(SocketException::Unknown, "setTestData not implemented for IPv6");
    }
    else
    {
        sAddr = INETAddress::createInetAddrIPv4();

        setAddressFromIPString(sAddr);
    }
}

void INETAddress::setTestData(string sAddr)
{
    if (_addrFamily == AF_INET6)
    {
        throw SocketException(SocketException::Unknown, "setTestData not implemented for IPv6");
    }
    else
    {
        setAddressFromIPString(sAddr);
    }
}

void INETAddress::seedRandom()
{
    struct timeval tvTime;

    gettimeofday(&tvTime, NULL);

    srand(tvTime.tv_usec + getpid());
}

string INETAddress::createInetAddrIPv4()
{
    ostringstream addrA, addrB, addrC, addrD;

    time_t t1;

    addrA << lrand48() % 256;
    addrB << lrand48() % 256;
    addrC << lrand48() % 256;
    addrD << lrand48() % 256;

    return addrA.str() + "." + addrB.str() + "." + addrC.str() + "." + addrD.str();
}

string INETAddress::createInetAddrIPv4(int A)
{
    ostringstream addrA, addrB, addrC, addrD;

    addrA << A;
    addrB << lrand48() % 256;
    addrC << lrand48() % 256;
    addrD << lrand48() % 256;

    return addrA.str() + "." + addrB.str() + "." + addrC.str() + "." + addrD.str();
}

string INETAddress::createInetAddrIPv4(int A, int B)
{
    ostringstream addrA, addrB, addrC, addrD;

    addrA << A;
    addrB << B;
    addrC << lrand48() % 256;
    addrD << lrand48() % 256;

    return addrA.str() + "." + addrB.str() + "." + addrC.str() + "." + addrD.str();
}

string INETAddress::createInetAddrIPv4(int A, int B, int C)
{
    ostringstream addrA, addrB, addrC, addrD;

    addrA << A;
    addrB << B;
    addrC << C;
    addrD << lrand48() % 256;

    return addrA.str() + "." + addrB.str() + "." + addrC.str() + "." + addrD.str();
}

string INETAddress::createMacAddr()
{
    ostringstream macA, macB, macC, macD, macE, macF;

    macA.fill('0');
    macA.width(2);
    macA << hex << lrand48() % 256;

    macB.fill('0');
    macB.width(2);
    macB << hex << lrand48() % 256;

    macC.fill('0');
    macC.width(2);
    macC << hex << lrand48() % 256;

    macD.fill('0');
    macD.width(2);
    macD << hex << lrand48() % 256;

    macE.fill('0');
    macE.width(2);
    macE << hex << lrand48() % 256;

    macF.fill('0');
    macF.width(2);
    macF << hex << lrand48() % 256;

    return macA.str() + ":" + macB.str() + ":" + macC.str() + ":" + macD.str() + ":" + macE.str() + ":" + macF.str();
}

string INETAddress::createNullMacAddr()
{
    ostringstream macA, macB, macC, macD, macE, macF;

    macA.fill('0');
    macA.width(2);
    macA << hex << 0;

    macB.fill('0');
    macB.width(2);
    macB << hex << 0;

    macC.fill('0');
    macC.width(2);
    macC << hex << 0;

    macD.fill('0');
    macD.width(2);
    macD << hex << 0;

    macE.fill('0');
    macE.width(2);
    macE << hex << 0;

    macF.fill('0');
    macF.width(2);
    macF << hex << 0;

    return macA.str() + ":" + macB.str() + ":" + macC.str() + ":" + macD.str() + ":" + macE.str() + ":" + macF.str();
}
