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

#include <pcff/DumpBuffer.h>
#include <pcff/BasicExceptionMgr.h>

#include <pcff/services/MessageData.h>
#include <pcff/services/ServicesException.h>
#include <pcff/services/Message.h>
#include <pcff/services/MessageFactory.h>

#include <pcff/services/Connector.h>

using namespace std;
using namespace pcff;

Connector::Connector()
{
	initMembers();
}

Connector::~Connector()
{
	if (_clientSocket != NULL)
	{
		// TODO: if connected disconnect
		delete _clientSocket;
	}

	if (_msgFactory != NULL)
	{
		delete _msgFactory;
	}

	initMembers();
}

void Connector::initMembers()
{
	_clientSocket = NULL;
	_host = "";
	_port = 0;
	_addrFamily = Socket::AF_INET_IPv4;
	_msgFactory = NULL;
}

void Connector::initialize()
{
	_msgFactory = new MessageFactory(*this);
	_msgFactory->initialize();
}

void Connector::connect(const char *host, int port, int addrFamily)
{
	_host = host;
	_port = port;
	_addrFamily = addrFamily;

	BasicExceptionMgr::instance()->log(LogLevel::Debug, "Connector::connect - host: %s, port %d", host, port  );
	_clientSocket = new Socket(_addrFamily);
	_clientSocket->init();
	_clientSocket->connect(_host.c_str(), port);

	connectCB();

	// TODO: create Connect Message and send
	// wait for ACK maybe have one function called getMsg()
	// maybe use a MessageProtocolMgr class???
}

void Connector::connectCB()
{

}

void Connector::disconnect()
{
	BasicExceptionMgr::instance()->log("Connector::disconnect - _clientSocket set to NULL", LogLevel::Info );
	disconnectCB();
	delete _clientSocket;
	_clientSocket = NULL;
}

void Connector::disconnectCB()
{

}

void Connector::process(MessageData &data)
{
	Message *msg = NULL;

	try
	{
		BasicExceptionMgr::instance()->log(LogLevel::Debug, "Connector::process(MessageData &data)");

		// message data contains the messageType and dataType
		msg = _msgFactory->create(data);

		if (msg != NULL)
	    {
			msg->send();
			delete msg;
	    }
		else
		{
			throw ServicesException(ServicesException::NullMessageFromFactory, "Connector::process");
		}
	}
	catch( BasicException& ex)
	{
		BasicExceptionMgr::instance()->handle( ex );
	}
	catch(...)
	{
		BasicExceptionMgr::instance()->handle( "Connector::process Event unknown ERROR" );
	}
}

void Connector::process(int msgType, Event &evt)
{
	Message *msg = NULL;

	try
	{
		BasicExceptionMgr::instance()->log(LogLevel::Debug, "Connector::process(int msgType, Event &evt)" );

		// message data contains the messageType and dataType
		msg = _msgFactory->create(msgType, evt);

		if (msg != NULL)
	    {
			msg->send();
			delete msg;
	    }
		else
		{
			throw ServicesException(ServicesException::NullMessageFromFactory, "Connector::process");
		}
	}
	catch( BasicException& ex)
	{
		BasicExceptionMgr::instance()->handle( ex );
	}
	catch(...)
	{
		BasicExceptionMgr::instance()->handle( "Connector::process Event unknown ERROR" );
	}
}


void Connector::send(Message &msg)
{
	unsigned long bufLen = 0;
	unsigned long serialSize = 0;
	unsigned char *buf = NULL;

	try
	{
		buf = msg.serializeTo(bufLen);

		BasicExceptionMgr::instance()->log(LogLevel::Debug, "Connector::send Message msgType: %ld, msgDataType %ld, bufLen: %ld",
				msg.getMessageType(), msg.getDataType(), bufLen);


		if (_clientSocket != NULL)
		{
			if (buf != NULL)
			{
				_clientSocket->send(buf, bufLen);
			}
			else
			{
				throw ServicesException(ServicesException::BufferIsNull, "Connector::send");
			}

			if (BasicExceptionMgr::instance()->getLogLevel() >= LogLevel::Fine)
			{
				DumpBuffer::instance().init();
				DumpBuffer::instance().dump(bufLen, (char *)buf);
			}

			BasicExceptionMgr::instance()->log(LogLevel::Fine, "Connector::send calling sendCB");
			sendCB( msg );
		}
		else
		{
			throw ServicesException(ServicesException::ClientSocketIsNull, "Connector::send");
		}
	}
	catch( BasicException& ex)
	{
		BasicExceptionMgr::instance()->handle( ex );
	}
	catch(...)
	{
		BasicExceptionMgr::instance()->handle( "Connector::send unknown ERROR" );
	}
}

void Connector::recv()
{
   char  buffer[65535];
   int   numBytesRcvd = 0;
   int   numBytesSent = 0;
   bool  bExit = false;

   BasicExceptionMgr::instance()->log("Connector::recv", LogLevel::Debug);

   try
   {
	  numBytesRcvd = _clientSocket->recv(buffer, 65535);

	  // TODO: process the buffer, cache it, parse it,
	  // maybe modify the numBytes received to 0, to buffer for later
	  //
	  numBytesRcvd = processStream(numBytesRcvd, buffer);

	  // The client has exited,
	  // TODO: put yourself back into the pool.
	  if (numBytesRcvd == 0)
	  {
		 bExit = true;
		 BasicExceptionMgr::instance()->log("Connector::recv numBytes is 0", LogLevel::Info);
	  }
   }
   catch(BasicException &ex)
   {
	   BasicExceptionMgr::instance()->handle(ex);
   }
   catch(...)
   {
	   BasicExceptionMgr::instance()->log("Connector::recv caught unknown exception caught");
   }
}

// The socket is read in from base ServerThread class
// and passed on.
int Connector::processStream(int bufSize, char *buf)
{
	try
	{
		BasicExceptionMgr::instance()->log(LogLevel::Finer, "Connector::processStream bufSize: %ld", bufSize);

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
				BasicExceptionMgr::instance()->log( "Connector::processStream: messages available", LogLevel::Fine );
				processMessage();
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
		BasicExceptionMgr::instance()->handle( "Connector::processStream unknown ERROR" );
		bufSize = 0;
	}

	return bufSize;  // return to parent, if bufsize is 0, will exit
}

void Connector::processMessage()
{
	Message *msg = NULL;

	if(_msgFactory->messagesAvailable() == true)
	{
		msg = _msgFactory->getMessage();  // get message from messageQueue
		recvCB(*msg);
                delete msg;
	}
}
