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

#ifndef _Connector_h_
#define _Connector_h_

#include <string>

#include <pcff/socket/Socket.h>
#include <pcff/event/Event.h>
#include <pcff/services/MessageProcessor.h>
#include <pcff/services/Message.h>
#include <pcff/services/MessageData.h>
#include <pcff/services/MessageFactory.h>

namespace pcff
{

class Connector : public MessageProcessor
{
public:
	Connector();
	virtual ~Connector();

private:
	void initMembers();

public:
	virtual void initialize();
	virtual void connect(const char *host, int port, int addrFamily);
	virtual void disconnect();
	virtual void send(Message &msg);
	virtual void recv();

private:
	int processStream(int bufSize, char *buf);
	void processMessage();

public:
	// framework can be called to process classes
	// derived from MessageData
	// MessageData class serialize themselves,
	// and can be put into Messages to be sent
	virtual void process(MessageData &data);
	virtual void process(int msgType, Event &evt);

	// derived class could override to process
public:
	virtual void connectCB()          = 0;
	virtual void disconnectCB()       = 0;
	virtual void sendCB(Message &msg) = 0;
	virtual void recvCB(Message &msg) = 0;

protected:
	MessageFactory		*_msgFactory;
    Socket              *_clientSocket;
    std::string          _host;
    int                  _port;
    int                  _addrFamily;
};

};


#endif
