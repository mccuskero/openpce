/** \class PcapLiveIngestor
 * \brief
 * \author Owen McCusker
 * \version current version
 * \date  04/17/2009
 *
 *  timeout - pcap_open_live
 * The timeout affects the responsiveness rather than the continuity of the
 * capture. It defines the maximum interval after which a captured packet will
 * be delivered by WinPcap to your app. A shorther interval allows better
 * responsie times, because the packets are delivered as soon as they arrive; a
 * longer interval results in a better usage of the winpcap buffers and
 * therefore allows  better efficiency
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

#ifndef _PcapLiveIngestor_h_
#define	_PcapLiveIngestor_h_

#include <pcap.h>

#include <string>

#include <pcff/socket/UnixDatagramSocket.h>
#include <pcff/event/proxy/EventIngestor.h>
#include <pcsec/network/PcapIngestor.h>

namespace pcsec
{

class PcapLiveIngestor : public PcapIngestor
{
public:
   PcapLiveIngestor();
   virtual ~PcapLiveIngestor();

private:
   void initMembers();

public:
   virtual void initializeContinuous();

public:

   void setDeviceName(std::string pathname)
   {
      _deviceName = pathname;
   }

protected:
   std::string _deviceName;
};

};

#endif	/* PcapLiveIngestor */

