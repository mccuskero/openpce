/*
 * File:   PcapIngestor.cc
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


#include <pcsec/network/PcapIngestor.h>

using namespace std;
using namespace pcff;
using namespace pcsec;

PcapIngestor::PcapIngestor()
{
   initMembers();
}

PcapIngestor::~PcapIngestor()
{
   if (_pd != NULL)
   {
      pcap_close(_pd);
   }
}

void PcapIngestor::initMembers()
{
   _pd = NULL;
   _snapLen = 0;
   _dataLinkOffset = 0;
   _useCallback = true;
   _pd = NULL;
   _filterExpression = "";
   _bIngest = true;
   _snapLen = DefaultSnapLen;
   _timeout = DefaultTimeout;
}

void PcapIngestor::initialize()
{
   switch (_mode)
   {
   case EventIngestor::Test :
      break;
   case EventIngestor::Batch :
      break;
   case EventIngestor::Continuous :
              initializeContinuous();
      break;
   default:
      break;
   }
}

void PcapIngestor::initializeContinuous()
{
}

void PcapIngestor::ingest()
{
   _bIngest = true;

   switch (_mode)
   {
   case EventIngestor::Test :
              testIngest();
      break;
   case EventIngestor::Batch :
              batchIngest();
      break;
   case EventIngestor::Continuous :
              continuousIngest();
      break;
   default:
      break;
   }
}

void PcapIngestor::testIngest()
{
   PcapEvent *evt = NULL;
   while (true)
   {
      BasicExceptionMgr::instance()->log("PcapIngestor::testIngest: creating test event...", LogLevel::Info);
      evt = new PcapEvent();
      evt->setTestData();

      _proxyService->process(*evt);
      sleep(5); // sleep 5 secs
      delete evt;
   }
}

//
void PcapIngestor::batchIngest()
{
   PcapEvent *evt = NULL;

   while (true)
   {

      BasicExceptionMgr::instance()->log("PcapIngestor::batchIngest - sleeping...", LogLevel::Info);
      sleep(5); // sleep 5 secs
   }
}

void PcapIngestor::continuousIngest()
{
   const u_char *packet = NULL; /* Packet payload */
   struct pcap_pkthdr *header; /* The header that pcap gives us */
   int  retVal = 0;

   u_int size_ip;
   u_int size_tcp;

   try
   {
      if (_useCallback == true)
      {
         BasicExceptionMgr::instance()->log(LogLevel::Info, "PcapIngestor::continuousIngest - pcap_loop");
         while (_bIngest == true)
         {
            pcap_loop(_pd,
                      0,
                      (PACKET_HANDLER_CB) pcap_handler_cb,
                      (u_char *)this); // callback pcap_handler2 on new packet
         }
      }
      else
      {
         BasicExceptionMgr::instance()->log(LogLevel::Info, "PcapIngestor::continuousIngest - pcap_next");
         // TODO: capture signal, TERM, then tell proxy service to shutdown, top ingesting
         while (_bIngest == true)
         {
            retVal = pcap_next_ex(_pd, &header, &packet);

            if (retVal == 1)
            {
                if (packet != NULL)
                {
                   processPacket(header, packet);
                }

                BasicExceptionMgr::instance()->log(LogLevel::Info, "PcapIngestor::continuousIngest - received header");
            }
            else if (retVal ==0) // timeout expired
            {
                BasicExceptionMgr::instance()->log(LogLevel::Info, "PcapIngestor::continuousIngest - pcap_next_ex timeout expired waiting for packet.");
            }
            else if (retVal == -1) // and error occured
            {
                throw PcapException(PcapException::ErrorPcapGetNext, pcap_geterr(_pd));
            }
            else if (retVal == -2) // reading from saved file, no more packets
            {
                BasicExceptionMgr::instance()->log(LogLevel::Info, "PcapIngestor::continuousIngest - end of file.");
                _bIngest = false;
            }
            else // unknown error
            {
                throw PcapException(PcapException::UnknownError, "PcapIngestor::continuousIngest");
            }
         }
      }

      BasicExceptionMgr::instance()->log("PcapIngestor::continuousIngest - exiting...", LogLevel::Info);
   }
   catch (BasicException& ex)
   {
      BasicExceptionMgr::instance()->handle(ex);
   }
   catch (...)
   {
      BasicExceptionMgr::instance()->handle("Unknown exception");
   }
}

void PcapIngestor::processPacket(const struct pcap_pkthdr *header, const u_char *packet)
{
   PcapEvent *evt = NULL;

   BasicExceptionMgr::instance()->log("PcapIngestor::processPacket - starting...", LogLevel::Info);


   evt = new PcapEvent();
   evt->initialize(*header, packet, _dataLinkOffset);

   _proxyService->process(*evt);

   if (BasicExceptionMgr::instance()->getLogLevel() >= LogLevel::Info)
   {
      DumpBuffer::instance().init();
      DumpBuffer::instance().dump(header->caplen, (char *)packet);
   }

   delete evt;
}

void PcapIngestor::pcap_handler_cb(u_char *args, const struct pcap_pkthdr *header, const u_char *packet)
{
   PcapIngestor *ingestor = (PcapIngestor *) args;

   if (packet != NULL)
   {
       ingestor->processPacket(header, packet);
   }
   else
   {
      ingestor->stopIngesting();
   }
}
