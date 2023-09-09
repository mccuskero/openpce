/* 
 * File:   pcapofflineingestor.cc
 * Author: owenm
 * 
 * Created on April 17, 2009, 8:31 PM
 */

#include <stdlib.h>
#include <pcap.h>

#include <pcff/BasicExceptionMgr.h>
#include <pcff/DumpBuffer.h>

#include <pcsec/network/PcapException.h>
#include <pcsec/network/PcapEvent.h>


#include <pcsec/network/PcapOfflineIngestor.h>

using namespace std;
using namespace pcff;
using namespace pcsec;

PcapOfflineIngestor::PcapOfflineIngestor()
{
   initMembers();
}

PcapOfflineIngestor::~PcapOfflineIngestor()
{
   if (_pd != NULL)
   {
      pcap_close(_pd);
   }
}

void PcapOfflineIngestor::initMembers()
{
   _pcapFilename = "";
}

void PcapOfflineIngestor::initializeContinuous()
{
   // find the pcap device if not passed in
   if (_pcapFilename.length() == 0)
   {
      throw PcapException(PcapException::ErrorPcapFile, "PcapOfflineIngestor::initializeContinuous");
   }

   if ((_pd = pcap_open_offline(_pcapFilename.c_str(), _pcapErrBuf)) == NULL)
   {
      throw PcapException(PcapException::ErrorOpeningPcapFile, _pcapErrBuf);
   }

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
