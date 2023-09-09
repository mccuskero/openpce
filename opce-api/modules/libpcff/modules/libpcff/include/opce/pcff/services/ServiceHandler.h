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

#ifndef ServiceHandler_h_
#define ServiceHandler_h_

#include <pcff/thread/ConnectionPool.h>
#include <pcff/thread/ConnectionPoolItem.h>
#include <pcff/thread/Thread.h>
#include <pcff/socket/Server.h>
#include <pcff/socket/ServerThread.h>
#include <pcff/services/MessageProcessor.h>
#include <pcff/services/Acceptor.h>
#include <pcff/services/Message.h>
#include <pcff/services/MessageFactory.h>

namespace pcff
{

class ServiceHandler : public ServerThread, MessageProcessor
{
public:
	ServiceHandler(Acceptor &server);
	virtual ~ServiceHandler();
	void initMembers();
	
public:
	virtual void init(int addrFamily);
	
public:
	virtual void start();       
	virtual void startServing(); 
	virtual int  processStream(int bufSize, char *buf);
	virtual void processMessages();

// should be overridden by client
public:
	virtual void send(Message &msg);
	virtual void recv(Message &msg);
	
private:
	MessageFactory	  *_msgFactory;
};

};

#endif

