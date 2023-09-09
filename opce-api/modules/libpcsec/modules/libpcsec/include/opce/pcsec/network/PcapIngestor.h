/** \class PcapIngestor
 * \brief base class for pcap ingesting: live and offline
 * \author Owen McCusker
 * \version current version
 * \date  04/17/2009
 *
 *
 * \bug list of potential bugs (one per line)
 * \warning list of warnings (one per line)
 * \todo add here any comment about possible improvements
 *
 */

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

#ifndef _PCAPINGESTOR_H
#define	_PCAPINGESTOR_H


#include <pcap.h>

#include <string>

#include <pcff/socket/UnixDatagramSocket.h>
#include <pcff/event/proxy/EventIngestor.h>

namespace pcsec
{

class PcapIngestor : public pcff::EventIngestor
{
   typedef void (*PACKET_HANDLER_CB)(u_char *args, const struct pcap_pkthdr *header, const u_char *packet);

public:
   PcapIngestor();
   virtual ~PcapIngestor();

private:
   void initMembers();

public:

   enum eIngestorConstants
   {
      DefaultSnapLen = 96,
      DefaultTimeout = 10
   };

public:
   virtual void initialize();
   virtual void initializeContinuous();

public:

   void setUseCallback()
   {
      _useCallback = true;
   }

   void resetUseCallback()
   {
      _useCallback = false;
   }

public:
   virtual void ingest();

   void stopIngesting()
   {
      _bIngest = false;
   }

private:
   virtual void testIngest();
   virtual void batchIngest();
   virtual void continuousIngest();

public:

   void setBPF(std::string filter)
   {
      _filterExpression = filter;
   }

private:
   void processPacket(const struct pcap_pkthdr *header, const u_char *packet);

   // callback mehtod
public:
   static void pcap_handler_cb(u_char *args, const struct pcap_pkthdr *header, const u_char *packet);

protected:
   bool _useCallback;
   char _pcapErrBuf[PCAP_ERRBUF_SIZE];
   pcap_t *_pd;
   std::string _filterExpression;
   bool _bIngest;
   int _snapLen;
   int _timeout;
   int _dataLinkOffset;
};

};

#endif	/* _PCAPINGESTOR_H */

