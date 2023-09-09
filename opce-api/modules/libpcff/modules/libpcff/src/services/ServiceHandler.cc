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

#include <string>
#include <iostream>
#include <sstream>

#include <pcff/BasicExceptionMgr.h>
#include <pcff/DumpBuffer.h>

#include <pcff/thread/Thread.h>
#include <pcff/thread/ThreadExceptionMgr.h>
#include <pcff/thread/ConnectionPoolItem.h>
#include <pcff/thread/ServerPool.h>

#include <pcff/socket/Socket.h>

#include <pcff/services/Message.h>
#include <pcff/services/ServicesException.h>
#include <pcff/services/MessageFactory.h>
#include <pcff/services/Acceptor.h>

#include <pcff/services/ServiceHandler.h>

using namespace std;
using namespace pcff;

ServiceHandler::ServiceHandler (Acceptor &server)
: ServerThread(server)
{
	initMembers();
}

ServiceHandler::~ServiceHandler()
{
	if (_msgFactory != NULL) 		delete _msgFactory;
}

void ServiceHandler::initMembers()
{
	_msgFactory = new MessageFactory(*this);
}

void ServiceHandler::init(int addrFamily)
{
	_msgFactory->initialize();
    ServerThread::init(addrFamily);
}

void ServiceHandler::startServing()
{
   ServerThread::startServing();
}

//
// start up the thread
//
void ServiceHandler::start()
{
   ThreadExceptionMgr::instance()->log("ServiceHandler::start ", LogLevel::Debug);

   ServerThread::start();
}

// The socket is read in from base ServerThread class
// and passed on.
int ServiceHandler::processStream(int bufSize, char *buf)
{
	try
	{
		BasicExceptionMgr::instance()->log(LogLevel::Finer, "ServiceHandler::processStream bufSize: %ld", bufSize);

		if (bufSize > 0)
		{
			if ( BasicExceptionMgr::instance()->getLogLevel() >= LogLevel::Debug)
			{
				DumpBuffer::instance().init();
				DumpBuffer::instance().dump(bufSize, buf);
			}

			// TODO: handle partial messages, Use ByteBuffer
			_msgFactory->create((unsigned long)bufSize,	(unsigned char *)buf);

			if (_msgFactory->messagesAvailable() )
			{
				BasicExceptionMgr::instance()->log( "ServiceHandler::processStream: messages available", LogLevel::Fine );
				processMessages();
			}
		}
	}
	catch( BasicException& ex)
	{
		BasicExceptionMgr::instance()->handle( ex );
		bufSize = 0;
	}
	catch(...)
	{
		BasicExceptionMgr::instance()->handle( "ServiceHandler::processStream unknown ERROR" );
		bufSize = 0;
	}

	return bufSize;  // return to parent, if bufsize is 0, will exit
}


void ServiceHandler::send(Message &msg)
{
	unsigned long bufLen = 0;
	unsigned long serialSize = 0;
	unsigned char *buf = NULL;

	try
	{
		buf = msg.serializeTo(bufLen);

		BasicExceptionMgr::instance()->log(LogLevel::Debug, "ServiceHandler::send Message msgType: %ld, msgDataType %ld, bufLen: %ld",
				msg.getMessageType(), msg.getDataType(), bufLen);


		if (_clientSocket != NULL)
		{
			if (buf != NULL)
			{
				_clientSocket->send(buf, bufLen);
			}
			else
			{
				throw ServicesException(ServicesException::BufferIsNull, "ServiceHandler::send");
			}

			if (BasicExceptionMgr::instance()->getLogLevel() >= LogLevel::Fine)
			{
				DumpBuffer::instance().init();
				DumpBuffer::instance().dump(bufLen, (char *)buf);
			}
		}
		else
		{
			throw ServicesException(ServicesException::ClientSocketIsNull, "ServiceHandler::send");
		}
	}
	catch( BasicException& ex)
	{
		BasicExceptionMgr::instance()->handle( ex );
	}
	catch(...)
	{
		BasicExceptionMgr::instance()->handle( "ServiceHandler::send unknown ERROR" );
	}
}

void ServiceHandler::processMessages()
{
	Message *msg = NULL;

	// TODO: maybe put a failsafe in this loop???
	// counter, with MAX count
	while(_msgFactory->messagesAvailable() == true)
	{
		msg = _msgFactory->getMessage();  // get message from messageQueue
		recv(*msg);
                delete msg;
	}
}

void ServiceHandler::recv(Message &msg)
{
	BasicExceptionMgr::instance()->log( "ServiceHandler::recv: not overridden", LogLevel::Info );
}

