/* 
 * File:   pcapinterfaceingestor.cc
 * Author: owenm
 * 
 * Created on April 17, 2009, 7:34 PM
 */

#include <stdlib.h>
#include <pcap.h>

#include <pcff/BasicExceptionMgr.h>
#include <pcff/DumpBuffer.h>

#include <pcsec/network/PcapException.h>
#include <pcsec/network/PcapEvent.h>

#include <pcsec/network/PcapLiveIngestor.h>

using namespace std;
using namespace pcff;
using namespace pcsec;

PcapLiveIngestor::PcapLiveIngestor()
{
   initMembers();
}

PcapLiveIngestor::~PcapLiveIngestor()
{
   if (_pd != NULL)
   {
      pcap_close(_pd);
   }
}

void PcapLiveIngestor::initMembers()
{
   _deviceName = "";
}

void PcapLiveIngestor::initializeContinuous()
{
   char *deviceName = NULL;
   struct bpf_program fcode;
   int op = 0;

   if (geteuid() != 0)
   {
      throw PcapException(PcapException::UserMustBeRoot, "PcapLiveIngestor::initializeContinuous - User must be root to run this");
   }

   // find the pcap device if not passed in
   if (_deviceName.length() == 0)
   {
      deviceName = pcap_lookupdev(_pcapErrBuf);

      if (deviceName != NULL)
      {
         _deviceName = deviceName;
      }
      else
      {
         throw PcapException(PcapException::ErrorFindingDevice, _pcapErrBuf);
      }
   }

   if ((_pd = pcap_open_live(_deviceName.c_str(), _snapLen, 1, _timeout, _pcapErrBuf)) == NULL)
   {
      BasicExceptionMgr::instance()->log(LogLevel::Info,
                                         "PcapLiveIngestor::initializeContinuous: pcap_open_live(dev=%s,snaplen=%d,1,timeout=%d",
                                         _deviceName.c_str(), _snapLen, 1, _timeout);

      throw PcapException(PcapException::ErrorOpeningDevice, _pcapErrBuf);
   }
   else
   {
      BasicExceptionMgr::instance()->log(LogLevel::Debug,
                                         "PcapLiveIngestor::initializeContinuous: pcap_open_live(dev=%s,snaplen=%d,1,timeout=%d",
                                         _deviceName.c_str(), _snapLen, 1, _timeout);

   }

   if ((op = pcap_snapshot(_pd)) > _snapLen)
   {
      _snapLen = op;
   }

   // lookup network info
   //    if (pcap_lookupnet(_deviceName, &_localnet, &_netmask, _pcapErrBuf) < 0)
   //    {
   //        warning(0, _pcapErrBuf);
   //    }

   // TODO: take in expressions
   //
   // compile expression
   //    if (pcap_compile(_pd, &fcode, _expr.c_str(), 0, _netmask) < 0 ||
   //            pcap_setfilter(pd, &fcode) < 0)
   //    {
   //        error(0, pcap_geterr(_pd));
   //    }

   switch (pcap_datalink(_pd))
   {

   case DLT_EN10MB:
   case DLT_IEEE802:
      _dataLinkOffset = PcapEvent::EthernetHeaderSize;
      break;

   case DLT_SLIP:
      _dataLinkOffset = PcapEvent::SlipHeaderSize;
      break;

   case DLT_PPP:
      _dataLinkOffset = PcapEvent::PPPHeaderSize;
      break;

   case DLT_RAW:
      _dataLinkOffset = PcapEvent::RawHeaderSize;
      break;

   case DLT_NULL:
      _dataLinkOffset = PcapEvent::LoopHeaderSize;
      break;

   default:
      fprintf(stderr, "Error: interface is not supported\n");
      throw PcapException(PcapException::IntefaceNotSupported, "PcapLiveIngestor::initializeContinuous");
   }
}
