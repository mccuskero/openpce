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

//
// One of the development goals is to contain the creation
// behavior of a NetworkPacketInfo in factory class linked
// to a sensor.
// Otherwise this class will get coupled too closely to
// sensor domains
//

//
// all packet capture times are managed in UTC/GMT time, offsets need
// to be set.
//

#include <string>
#include <sstream>

#include <netinet/in.h>
#include <arpa/inet.h>

#include <pcff/BasicExceptionMgr.h>
#include <pcff/socket/INETAddress.h>

#include <pcsec/network/EthernetHeaderInfo.h>
#include <pcsec/network/WifiHeaderInfo.h>
#include <pcsec/network/IPHeaderInfo.h>
#include <pcsec/network/ICMPInfo.h>
#include <pcsec/network/UDPHeaderInfo.h>
#include <pcsec/network/TCPHeaderInfo.h>

#include <pcsec/network/NetworkPacketInfo.h>

using namespace std;
using namespace pcff;
using namespace pcsec;

NetworkPacketInfo::NetworkPacketInfo()
{
   initMembers();
}

NetworkPacketInfo::NetworkPacketInfo(const NetworkPacketInfo &in)
{
   initMembers();

   _protocol = in._protocol;
   _packetDataMask = in._packetDataMask; // indicates what members are used
   if (in._ethernetHeaderInfo != NULL)
   {
      _ethernetHeaderInfo = new EthernetHeaderInfo(*in._ethernetHeaderInfo);
   }
   //	if (in._ethernetARPInfo != NULL) 	{	_ethernetARPInfo    = new EthernetArpInfo(*in._ethernetARPInfo); }
   //	if (in._arpHeaderInfo != NULL) 		 {	_arpHeaderInfo    = new ArpHeaderInfo(*in._arpHeaderInfo); }
   if (in._wifiHeader != NULL)
   {
      _wifiHeader = new WifiHeaderInfo(*in._wifiHeader);
   }
   if (in._ipHeaderInfo != NULL)
   {
      _ipHeaderInfo = new IPHeaderInfo(*in._ipHeaderInfo);
   }
   //	if (in._icmpHeaderInfo != NULL) 	{	_icmpHeaderInfo    = new ICMPHeaderInfo(*in._icmpHeaderInfo); }
   if (in._icmpInfo != NULL)
   {
      _icmpInfo = new ICMPInfo(*in._icmpInfo);
   }
   if (in._updHeaderInfo != NULL)
   {
      _updHeaderInfo = new UDPHeaderInfo(*in._updHeaderInfo);
   }
   if (in._tcpHeaderInfo != NULL)
   {
      _tcpHeaderInfo = new TCPHeaderInfo(*in._tcpHeaderInfo);
   }
   _fragmentFlag = in._fragmentFlag;
   _fragmentOffset = in._fragmentOffset;
   _packet_tv_sec = in._packet_tv_sec; // seconds
   _packet_tv_usec = in._packet_tv_usec; // microseconds
   _packet_gmtOffset = in._packet_gmtOffset; // gmst offset
   _dataLinkHdrOffset = in._dataLinkHdrOffset; // datalink header offset. e.g. ethernet
   _networkHdrOffset = in._networkHdrOffset; // network header offset. e.g. ip
   _transportHdrOffset = in._transportHdrOffset; // transport header offset e.g. tcp/udp/icmp
   _dataOffset = in._dataOffset; // data length, offset

   if (in._packetCaptureLen > 0 && in._packet != NULL)
   {
      _packetLen = in._packetLen;
      _packetCaptureLen = in._packetCaptureLen;
      _packet = new u_int8_t[_packetCaptureLen];
      memcpy(_packet, in._packet, _packetCaptureLen);
   }
}

NetworkPacketInfo::~NetworkPacketInfo()
{
   if (_ethernetHeaderInfo != NULL)
   {
      delete _ethernetHeaderInfo;
      _ethernetHeaderInfo = NULL;
   }
   //	if (_ethernetARPInfo != NULL) { delete _ethernetARPInfo;  _ethernetARPInfo = NULL; }
   //	if (_arpHeaderInfo != NULL) 	{	delete _arpHeaderInfo; _arpHeaderInfo = NULL; }
   if (_wifiHeader != NULL)
   {
      delete _wifiHeader;
      _wifiHeader = NULL;
   }
   if (_ipHeaderInfo != NULL)
   {
      delete _ipHeaderInfo;
      _ipHeaderInfo = NULL;
   }
   //	if (_icmpHeaderInfo != NULL) 	{	delete _icmpHeaderInfo; _icmpHeaderInfo = NULL; }
   if (_icmpInfo != NULL)
   {
      delete _icmpInfo;
      _icmpInfo = NULL;
   }
   if (_updHeaderInfo != NULL)
   {
      delete _updHeaderInfo;
      _updHeaderInfo = NULL;
   }
   if (_tcpHeaderInfo != NULL)
   {
      delete _tcpHeaderInfo;
      _tcpHeaderInfo = NULL;
   }
   if (_packet != NULL)
   {
      delete [] _packet;
      _packet = NULL;
   }

   initMembers();
}

void NetworkPacketInfo::initMembers()
{
   _protocol = NetworkPacketInfo::ProtoNotInitialized;
   _packetDataMask = 0; // indicates what members are used
   _ethernetHeaderInfo = NULL; // TCP/IP/Ethernet/ARP headers
   //_ethernetARPInfo		= NULL;
   //_arpHeaderInfo		= NULL;
   _wifiHeader = NULL; // wireless LAN header
   _ipHeaderInfo = NULL;
   //	PacketOptions 	_ipOptions[40];
   //	u_int32_t 		_ipOptionCount;
   //	_icmpHeaderInfo			= NULL;
   _icmpInfo = NULL;
   _updHeaderInfo = NULL;
   _tcpHeaderInfo = NULL;
   //	PacketOptions 	tcpOptions[40];
   //	u_int32_t 		tcpOptionCount;
   _fragmentFlag = 0;
   _fragmentOffset = 0;
   _packet_tv_sec = 0; // seconds
   _packet_tv_usec = 0; // microseconds
   _packet_gmtOffset = NetworkPacketInfo::GMTOffsetNotSet; // gmt offset
   _dataLinkHdrOffset = 0; // datalink header offset. e.g. ethernet
   _networkHdrOffset = 0; // network header offset. e.g. ip
   _transportHdrOffset = 0; // transport header offset e.g. tcp/udp/icmp
   _dataOffset = 0; // data length, offset
   _packetLen = 0;
   _packetCaptureLen = 0;
   _packet = NULL;
}

bool NetworkPacketInfo::initialize(int proto, long tv_sec, long tv_usec)
{
   NetworkPacketInfo::initialize(proto, tv_sec, tv_usec, NetworkPacketInfo::GMTOffsetNotSet);
}

bool NetworkPacketInfo::initialize(int proto, long tv_sec, long tv_usec, long gmtOffset)
{
   _packet_tv_sec = tv_sec;
   _packet_tv_usec = tv_usec;
   _packet_gmtOffset = gmtOffset;

   return setProtocol(proto);
}

//
// only serailize what is used
//

void NetworkPacketInfo::serialize(Serializer &s)
{
   s.transfer(this->_protocol);
   s.transfer(this->_packetDataMask); // used in the isSet member
   if (isSet(NetworkPacketInfo::eEthernetHeaderInfo) == true)
   {
      // if serializing from, then will need to create objs before calling serialize
      if (_ethernetHeaderInfo == NULL)
      {
         _ethernetHeaderInfo = new EthernetHeaderInfo();
      }
      _ethernetHeaderInfo->serialize(s);
   }
   //	if ( isSet(NetworkPacketInfo::eEthernetArpInfo) == true )    { _ethernetARPInfo->serialize(s); }
   //	if ( isSet(NetworkPacketInfo::eArpHeaderInfo) == true )      { _arpHeaderInfo->serialize(s); }
   if (isSet(NetworkPacketInfo::eWifiHeaderInfo) == true)
   {
      if (_wifiHeader == NULL)
      {
         _wifiHeader = new WifiHeaderInfo();
      }
      _wifiHeader->serialize(s);
   }
   if (isSet(NetworkPacketInfo::eIPHeaderInfo) == true)
   {
      if (_ipHeaderInfo == NULL)
      {
         _ipHeaderInfo = new IPHeaderInfo();
      }
      _ipHeaderInfo->serialize(s);
   }
   //	if ( isSet(NetworkPacketInfo::eICMPHeaderInfo) == true ) { _icmpHeaderInfo->serialize(s); }
   if (isSet(NetworkPacketInfo::eICMPInfo) == true)
   {
      if (_icmpInfo == NULL)
      {
         _icmpInfo = new ICMPInfo();
      }
      _icmpInfo->serialize(s);
   }
   if (isSet(NetworkPacketInfo::eUDPHeaderInfo) == true)
   {
      if (_updHeaderInfo == NULL)
      {
         _updHeaderInfo = new UDPHeaderInfo();
      }
      _updHeaderInfo->serialize(s);
   }
   if (isSet(NetworkPacketInfo::eTCPHeaderInfo) == true)
   {
      if (_tcpHeaderInfo == NULL)
      {
         _tcpHeaderInfo = new TCPHeaderInfo();
      }
      _tcpHeaderInfo->serialize(s);
   }
   s.transfer(this->_fragmentFlag);
   s.transfer(this->_fragmentOffset);
   s.transfer(this->_packet_tv_sec);
   s.transfer(this->_packet_tv_usec);
   s.transfer(this->_packet_gmtOffset);
   s.transfer(this->_dataLinkHdrOffset);
   s.transfer(this->_networkHdrOffset);
   s.transfer(this->_transportHdrOffset);
   s.transfer(this->_dataOffset);
   // TODO: max transfer size!!! may be needed
   s.transfer(this->_packetLen);
   s.transfer(this->_packetCaptureLen);

   if (_packetCaptureLen > 0)
   {
      s.transfer((unsigned char*&) this->_packet, (unsigned long &) _packetCaptureLen);
   }
}

void NetworkPacketInfo::getInfo(string &sInfo)
{
   ostringstream eventInfo;
   string info;

   eventInfo << "proto (" << _protocol << ")";
   eventInfo << " len " << _packetLen << ")";
   eventInfo << " caplen " << _packetCaptureLen << ")";
   //   eventInfo << _packetDataMask;

   if (isSet(NetworkPacketInfo::eEthernetHeaderInfo) == true)
   {
      eventInfo << endl;
      eventInfo << "    ";
      _ethernetHeaderInfo->getInfo(info);
      eventInfo << info;
   }



   //	if ( isSet(NetworkPacketInfo::eEthernetArpInfo) == true )    { _ethernetARPInfo->getInfo(info); }
   //	if ( isSet(NetworkPacketInfo::eArpHeaderInfo) == true )      { _arpHeaderInfo->getInfo(info); }

   if (isSet(NetworkPacketInfo::eWifiHeaderInfo) == true)
   {
      eventInfo << endl;
      eventInfo << "    " << "WIFI:";
      _wifiHeader->getInfo(info);
      eventInfo << info;
   }

   if (isSet(NetworkPacketInfo::eIPHeaderInfo) == true)
   {
      eventInfo << endl;
      eventInfo << "      ";
      _ipHeaderInfo->getInfo(info);
      eventInfo << info;
   }

   //	if ( isSet(NetworkPacketInfo::eICMPHeaderInfo) == true ) { _icmpHeaderInfo.serialize(s); }

   if (isSet(NetworkPacketInfo::eICMPInfo) == true)
   {
      eventInfo << endl;
      eventInfo << "         ";
      _icmpInfo->getInfo(info);
      eventInfo << info;
   }

   if (isSet(NetworkPacketInfo::eUDPHeaderInfo) == true)
   {
      eventInfo << endl;
      eventInfo << "         ";
      _updHeaderInfo->getInfo(info);
      eventInfo << info;
   }

   if (isSet(NetworkPacketInfo::eTCPHeaderInfo) == true)
   {
      eventInfo << endl;
      eventInfo << "         ";
      _tcpHeaderInfo->getInfo(info);
      eventInfo << info;
   }

   eventInfo << ":" << _fragmentFlag << ":";
   eventInfo << ":" << _fragmentOffset;

   sInfo = eventInfo.str();
}

bool NetworkPacketInfo::isSet(int mask)
{
   return (_packetDataMask & mask) ? true : false;
}

void NetworkPacketInfo::set(EthernetHeaderInfo &info)
{
   _packetDataMask |= NetworkPacketInfo::eEthernetHeaderInfo;
   _ethernetHeaderInfo = new EthernetHeaderInfo(info);
}

void NetworkPacketInfo::set(WifiHeaderInfo &info)
{
   _packetDataMask |= NetworkPacketInfo::eWifiHeaderInfo;
   _wifiHeader = new WifiHeaderInfo(info);
}

void NetworkPacketInfo::set(IPHeaderInfo &info)
{
   _packetDataMask |= NetworkPacketInfo::eIPHeaderInfo;
   _ipHeaderInfo = new IPHeaderInfo(info);

   // sets protocol of packet in NetworkPacketInfo
   setProtocol(info.getProtocol());
}

void NetworkPacketInfo::set(ICMPInfo &info)
{
   _packetDataMask |= NetworkPacketInfo::eICMPInfo;
   _icmpInfo = new ICMPInfo(info);
}

void NetworkPacketInfo::set(UDPHeaderInfo &info)
{
   _packetDataMask |= NetworkPacketInfo::eUDPHeaderInfo;
   _updHeaderInfo = new UDPHeaderInfo(info);
}

void NetworkPacketInfo::set(TCPHeaderInfo &info)
{
   _packetDataMask |= NetworkPacketInfo::eTCPHeaderInfo;
   _tcpHeaderInfo = new TCPHeaderInfo(info);
}

void NetworkPacketInfo::setPacketOffsets(long dataLnkOffset,
                                         long networkLnkOffset,
                                         long transportLnkOffset,
                                         long dataOffset)
{
   _dataLinkHdrOffset = dataLnkOffset; // datalink header offset. e.g. ethernet
   _networkHdrOffset = networkLnkOffset; // network header offset. e.g. ip
   _transportHdrOffset = transportLnkOffset; // transport header offset e.g. tcp/udp/icmp
   _dataOffset = dataOffset; // data length, offset
}

void NetworkPacketInfo::setPacket(u_int8_t *p, u_int32_t pLen, u_int32_t pCapLen)
{
   if (p != NULL && pCapLen > 0)
   {
      _packetCaptureLen = pCapLen;
      _packetLen = pLen;
      _packet = new u_int8_t[_packetCaptureLen];
      memcpy(_packet, p, _packetCaptureLen);
   }
   else
   {
      // TODO: review execpetion handling for pcsec
      throw "NetworkPacketInfo::set - packet arg is NULL ";
   }
}

bool NetworkPacketInfo::setProtocol(int protocol)
{
   bool bProtocolSupported = false;

   switch (protocol)
   {
   case IPPROTO_ICMP:
      bProtocolSupported = true;
      _protocol = protocol;
      break;
   case IPPROTO_TCP:
      bProtocolSupported = true;
      _protocol = protocol;
      break;
   case IPPROTO_UDP:
      bProtocolSupported = true;
      _protocol = protocol;
      break;
   case IPPROTO_IPV6:
      BasicExceptionMgr::instance()->log(LogLevel::Info, "NetworkPacketInfo::initialize - IPv6 protocol not supported.");
      _protocol = protocol;
      break;
   default:
      BasicExceptionMgr::instance()->log(LogLevel::Info, "NetworkPacketInfo::initialize - protocol not supported %ld.", protocol);
      _protocol = protocol;
      break;
   }

   return bProtocolSupported;
}

void NetworkPacketInfo::setPacketCaptureTime(long tv_sec, long tv_usec)
{
   _packet_tv_sec = tv_sec;
   _packet_tv_usec = tv_usec;
   _packet_gmtOffset = NetworkPacketInfo::GMTOffsetNotSet;

}

void NetworkPacketInfo::setPacketCaptureTime(long tv_sec, long tv_usec, long gmtOffset)
{
   _packet_tv_sec = tv_sec;
   _packet_tv_usec = tv_usec;
   _packet_gmtOffset = gmtOffset;
}

u_int8_t NetworkPacketInfo::getHeaderLen()
{
   if (_ipHeaderInfo == NULL) return 0;
   return _ipHeaderInfo->_ipVerHeaderLen;
}

u_int8_t NetworkPacketInfo::getIPVersion()
{
   return 0; // TODO: need to make this available
}

u_int8_t NetworkPacketInfo::getTOS()
{
   if (_ipHeaderInfo == NULL) return 0;
   return _ipHeaderInfo->_ipTOS;
}

u_int16_t NetworkPacketInfo::getLen()
{
   if (_ipHeaderInfo == NULL) return 0;
   return _ipHeaderInfo->_ipLen;
}

u_int16_t NetworkPacketInfo::getID()
{
   if (_ipHeaderInfo == NULL) return 0;
   return _ipHeaderInfo->_ipID;
}

u_int16_t NetworkPacketInfo::getIPFlagsFragOffset()
{
   if (_ipHeaderInfo == NULL) return 0;
   return _ipHeaderInfo->_ipFlagFragOff;
}

u_int8_t NetworkPacketInfo::getTTL()
{
   if (_ipHeaderInfo == NULL) return 0;
   return _ipHeaderInfo->_ipTTL;
}

u_int8_t NetworkPacketInfo::getProto()
{
   if (_ipHeaderInfo == NULL) return 0;
   return _ipHeaderInfo->_ipProto;
}

u_int16_t NetworkPacketInfo::getChecksum()
{
   if (_ipHeaderInfo == NULL) return 0;
   return _ipHeaderInfo->_ipCksum;
}

void NetworkPacketInfo::getIPSrc(string &sAddr)
{
   if (_ipHeaderInfo != NULL)
   {
      sAddr = _ipHeaderInfo->_ipSrc.getAddressAsStr();
   }
   else
   {
      sAddr = "0.0.0.0";
   }
}

void NetworkPacketInfo::getIPDst(string &sAddr)
{
   if (_ipHeaderInfo != NULL)
   {
      sAddr = _ipHeaderInfo->_ipDst.getAddressAsStr();
   }
   else
   {
      sAddr = "0.0.0.0";
   }
}

u_int16_t NetworkPacketInfo::getSrcPort()
{
   if (_updHeaderInfo != NULL)
   {
      return _updHeaderInfo->_sPort;
   }
   else if (_tcpHeaderInfo != NULL)
   {
      return _tcpHeaderInfo->_sPort;
   }
   else
   {
      return 0;
   }
}

u_int16_t NetworkPacketInfo::getDstPort()
{
   if (_updHeaderInfo != NULL)
   {
      return _updHeaderInfo->_dPort;
   }
   else if (_tcpHeaderInfo != NULL)
   {
      return _tcpHeaderInfo->_dPort;
   }
   else
   {
      return 0;
   }
}

u_int8_t NetworkPacketInfo::getTCPFlags()
{
   if (_tcpHeaderInfo != NULL)
   {
      return _tcpHeaderInfo->_flags;
   }
   else
   {
      return 0;
   }
}

u_int8_t *NetworkPacketInfo::getPacket()
{
   return _packet;
}

u_int32_t NetworkPacketInfo::getPacketCaptureLen()
{
   return _packetCaptureLen;
}

u_int32_t NetworkPacketInfo::getPacketRealLen()
{
   return _packetLen;
}

void NetworkPacketInfo::networkIPToString(struct in_addr &ipAddr, std::string &sIP)
{
   sIP = inet_ntoa(ipAddr);
}


//
//
//

void NetworkPacketInfo::setTestData()
{
   IPHeaderInfo ipInfo;
   TCPHeaderInfo tcpInfo;
   string sSrcAddr;
   string sDstAddr;

   INETAddress::seedRandom();
   sSrcAddr = INETAddress::createInetAddrIPv4();
   sDstAddr = INETAddress::createInetAddrIPv4();

   BasicExceptionMgr::instance()->log(LogLevel::Error, "NetworkPacketInfo::setTestData: srcIP: %s, dstIP: %s", sSrcAddr.c_str(), sDstAddr.c_str());

   ipInfo.initialize(1, // hdr->ip_verhl,
                     2, // hdr->ip_tos,
                     3, // hdr->ip_len,
                     4, // hdr->ip_id,
                     5, // hdr->ip_off,
                     6, // hdr->ip_ttl,
                     IPPROTO_TCP, // hdr->ip_proto,
                     7, // hdr->ip_csum,
                     sSrcAddr, // hdr->ip_src,
                     sDstAddr); //hdr->ip_dst);

   NetworkPacketInfo::set(ipInfo);

   setPacket((u_int8_t *) "TestPacket", 10, 10); // TODO: maybe an option in the future

   tcpInfo.initialize(10, // hdr->th_sport,
                      11, // hdr->th_dport,
                      12, // hdr->th_seq,
                      13, // hdr->th_ack,
                      14, // hdr->th_offx2,
                      15, // hdr->th_flags,
                      16, // hdr->th_win,
                      17, // hdr->th_sum,
                      18); // hdr->th_urp);

   set(tcpInfo);
}

