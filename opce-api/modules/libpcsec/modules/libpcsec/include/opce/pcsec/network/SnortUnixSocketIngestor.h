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
 /*
 * \class SnortUnixSocketIngestor 
 * \brief Ingests snort alerts from the -A unixsock option 
 */

#ifndef SNORTUNIXSOCKETINGESTOR_H_
#define SNORTUNIXSOCKETINGESTOR_H_

#include <string>

#include <pcff/socket/UnixDatagramSocket.h>
#include <pcff/event/proxy/EventIngestor.h>

namespace pcsec
{

#define MAX_DG_SIZE 65535

class SnortUnixSocketIngestor : public pcff::EventIngestor
{
public:
	SnortUnixSocketIngestor();
	virtual ~SnortUnixSocketIngestor();

private:
	void initMembers();
	
public:
	virtual void initialize();
	virtual void initializeContinuous();

public:
	virtual void ingest();
	void         stopIngesting() {  _bIngest = false; }

private:
	virtual void testIngest();
	virtual void batchIngest();
	virtual void continuousIngest();


public:
	void setUnixSocketPathname(const char *pathname)         { _unixSocketpathname = pathname; }

private:
	std::string                 _unixSocketpathname;
	pcff::UnixDatagramSocket	_serverUnixDSocket;
	bool                        _bIngest;


};

};

#endif /*SNORTUNIXSOCKETINGESTOR_H_*/
