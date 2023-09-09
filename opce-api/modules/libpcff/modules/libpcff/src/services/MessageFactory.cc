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

// TODO: current getting serialized exceptions when
// tcpip stack combines streams. Need to separate
// the streams based on expected message size.


#include <stdlib.h>

#include <iostream>
#include <sstream>

#include <pcff/BasicExceptionMgr.h>
#include <pcff/serialize/Serializeable.h>

#include <pcff/services/ServicesException.h>
#include <pcff/services/MessageProcessor.h>
#include <pcff/services/MessageQueue.h>
#include <pcff/services/MessageFactory.h>

using namespace std;
using namespace pcff;

MessageFactory::MessageFactory(MessageProcessor &msgProcessor)
	: _msgProcessor(msgProcessor)
{
	initMembers();
}

MessageFactory::~MessageFactory()
{
	if (_msgQ != NULL)
	{
		delete _msgQ;
	}

	if (_msgBuffer != NULL)
	{
		delete _msgBuffer;
	}
}

void MessageFactory::initMembers()
{
	_currentMsgExpectedSize = 0;
	_pendingMessageSerializedSize = 0;
	_factoryStatus = MessageFactory::NoMessages;
	_pendingMessageStream = false;
	_msgQ = NULL;
	_msgBuffer = NULL;
}

void MessageFactory::initialize()
{
	_msgQ = new MessageQueue;
	_msgBuffer = new ByteBuffer();
}

// recieved messages
void MessageFactory::create(unsigned long bufSize, unsigned char *buf)
{
	Message *msg = NULL;
	unsigned long serializedSize = 0;
	unsigned char sID = 0;
	Serializeable serialBuf;

	// create a serial buffer to check size, and validity
	serialBuf.serializeFromGetMessageHeader(buf, bufSize);

	// TODO: if next buffer does not contain an ID
	// then check message pending, if not
	// pending then it is an error.
	//
	// TODO: need _messagingPending flag
	sID =serialBuf.getSerializationID();

	// if valid message
	if (sID == Serializer::SID)
	{
		// should never get a SID, if pending!!
		if (_pendingMessageStream == true)
		{
			BasicExceptionMgr::instance()->log(LogLevel::Warn,
						  "MessageFactory::create: recived SID while pending message, clearing message buffer",
						  sID);
			_msgBuffer->clear();
		}

		serializedSize = serialBuf.getSerializedSize();

		BasicExceptionMgr::instance()->log(LogLevel::Fine,
				"MessageFactory::create  (bufSize: %ld, serializedSize %ld)",
				bufSize, serializedSize);

		// if there is enough buffer for a message
		if (serializedSize == bufSize)
		{
			createMessageFromBuffer(buf, serializedSize );
		}
		// if there is enough for a buffer, and then some... process for messages
		else if (serializedSize <= bufSize)
		{
			processMessageStream(buf, bufSize, serializedSize);
		}
		// not enough buffer, wait for next send
		// need to set state messagePending
		else
		{
			appendStreamInMsgBuffer(buf, serializedSize);
		}
	}
	// messing pending or invalid message
	else
	{
		if (_pendingMessageStream == true)
		{
			processPendingMessageStream( buf, bufSize);
		}
		else
		{
			BasicExceptionMgr::instance()->log(LogLevel::Warn,
						  "MessageFactory::create: InvalidID: SID %x, should be 0xF5, dumping stream",
						  sID);
		}
	}

	BasicExceptionMgr::instance()->log(LogLevel::Finer,
			"MessageFactory::create  done (_msgQ size: %d)",
			_msgQ->size() );
}

// send messages, with no msg data or payload
Message *MessageFactory::create(int msgType)
{
	Message *msg = NULL;
	void    *data = NULL;
	long     dataSize = 0;

	msg = new Message(_msgProcessor);

	msg->setMessageType(msgType);

	return msg;
}

// send messages
Message *MessageFactory::create(int msgType, Serializeable &msgData)
{
	unsigned long bufLen = 0;
	unsigned char *buf = NULL;
	Message *msg = NULL;
	void    *data = NULL;
	long     dataSize = 0;

	msg = new Message(_msgProcessor);
	msg->setMessageType(msgType);

	if (msgType == Message::Test)
	{
		msg->setTestData();
	}

	buf = msgData.serializeTo(bufLen);

	BasicExceptionMgr::instance()->log(LogLevel::Fine, "MessageFactory::create message bufLen: %ld", bufLen);

	msg->setData(buf, bufLen);

	return msg;
}

//
Message *MessageFactory::create(MessageData &msgData)
{
        Message *msg = NULL;
        void    *data = NULL;
        long     dataSize = 0;
        int      msgType = 0;
        int      dataType = 0;

        // TODO:
        // msgType = msgData.getMsgType();
        // dataType = msgData.getMsgDataType();

        msg = new Message(_msgProcessor);

        msg->setTestData();

        // TODO: get the serialized buffer from SnortEvent
        // msgData.getData(data, dataSize)
        // msg->setData(data, dataSize)

        return msg;
}

// TODO: if there are messages in the, then messagesAvailable should be true
void MessageFactory::createMessageFromBuffer(unsigned char *buf, unsigned long serializedSize)
{
	Message *msg = NULL;

	if (_msgQ == NULL)
	{
		throw ServicesException(ServicesException::MsgQIsNull, "MessageFactory::createMessageFromBuffer");
	}

	BasicExceptionMgr::instance()->log(LogLevel::Fine,
			"MessageFactory::createMessageFromBuffer CreateMessage (serializedSize %ld)",
			serializedSize);
	msg = new Message(_msgProcessor);
	msg->serializeFrom(buf, serializedSize);
	_msgQ->put(msg);
	_pendingMessageStream = false;
}

//
// process stream until all messages are taken out, if any extra stream
// is left, then save off until next process stream, setting pendingMessage flag
// TODO: need to remember the serializedSize, for next message ??? Maybe, or check again
//
void MessageFactory::processMessageStream(unsigned char *buf, unsigned long bufSize, unsigned long initSerializedSize)
{
	Message *msg = NULL;
	Serializeable serialBuf;
	bool bMoreMessagesInStream = true;
	unsigned char *currentBuf = buf;
	unsigned long currentBufferSize = bufSize;
	unsigned long expectedSerializedSize = initSerializedSize;

	if (_msgQ == NULL)
	{
		throw ServicesException(ServicesException::MsgQIsNull, "MessageFactory::processMessageStream");
	}

	BasicExceptionMgr::instance()->log(LogLevel::Fine,
			"MessageFactory::processMessageStream CreateMessageProcessStream (bufSize: %ld, serializedSize %ld)",
			bufSize, initSerializedSize);

	// while there is room for a message in stream, create it,
	// if there is any buffer left, then put it into a byte buffer,
	// set _pendingMessageStream = true;
	while( (bMoreMessagesInStream == true) &&
			(expectedSerializedSize  <= currentBufferSize ) )
	{
		// create message from stream
		msg = new Message(_msgProcessor);
		msg->serializeFrom(currentBuf, expectedSerializedSize);
		_msgQ->put(msg);
		_factoryStatus = MessageFactory::MessagesCreated;

		// ready the next message stream
		// increment the buffer,
		currentBufferSize -= expectedSerializedSize;

		// if there is any more buffer left process it
		if (currentBufferSize > 0)
		{
			// increment buffer position
			currentBuf += expectedSerializedSize;

			// get the next serialized size
			serialBuf.serializeFromGetMessageHeader(currentBuf, currentBufferSize);
			expectedSerializedSize = serialBuf.getSerializedSize();

			// if there is enough of a stream, process,
			// if not then exit from loop storing left-over
			// stream in ByteBuffer.
			if (expectedSerializedSize > currentBufferSize )
			{
				BasicExceptionMgr::instance()->log(LogLevel::Fine,
						"MessageFactory::processMessageStream adding buf to ByteBuffer (currentBuffferSize: %ld, expected _pendingMessageSerializedSize %ld)",
						currentBufferSize, _pendingMessageSerializedSize);

				bMoreMessagesInStream = false;
				_pendingMessageStream = true;
				_pendingMessageSerializedSize = expectedSerializedSize;
				_msgBuffer->clear();
				_msgBuffer->add(buf, bufSize);
			}
		}
		// else no more messages left in buffer
		else
		{
			BasicExceptionMgr::instance()->log(LogLevel::Fine,
					"MessageFactory::processMessageStream no pending messages",
					currentBufferSize, _pendingMessageSerializedSize);
			bMoreMessagesInStream = false;
			_pendingMessageStream = false;
		}
	}

	BasicExceptionMgr::instance()->log(LogLevel::Fine,
			"MessageFactory::processMessageStream done. currentBufferSize: %ld, _pendingMessageStream: %d, _pendingMessageSerializedSize %ld",
			currentBufferSize,
			(_pendingMessageStream == true) ? 1 : 0,
			_pendingMessageSerializedSize);
}

//
//
// TODO: to test, create large data buf (e.g. 1400 bytes)
//
void MessageFactory::processPendingMessageStream(unsigned char *buf, unsigned long bufSize)
{
	unsigned long currentBufSize = 0;
	Serializeable serialBuf;
	// create a serial buffer to check size, and validity
	serialBuf.serializeFromGetMessageHeader(buf, bufSize);

	currentBufSize = _msgBuffer->getLength() + bufSize;

	BasicExceptionMgr::instance()->log(LogLevel::Fine,
			"MessageFactory::processPendingMessageStream (currentBufSize: %ld, _pendingMessageSerializedSize %ld)",
			currentBufSize, _pendingMessageSerializedSize);

	// if rest of message came in then process
	if (currentBufSize == _pendingMessageSerializedSize)
	{
		_msgBuffer->add(buf, bufSize);
		createMessageFromBuffer(_msgBuffer->getBuffer(),  _msgBuffer->getLength() );
		_msgBuffer->clear();
	}
	// if rest of message came in and then some, process it
	else if (currentBufSize > _pendingMessageSerializedSize)
	{
		_msgBuffer->add(buf, bufSize);
		processMessageStream(_msgBuffer->getBuffer(),  _msgBuffer->getLength(), _pendingMessageSerializedSize);
	}
	else
	{
		// append buf to msgBuf
		appendStreamInMsgBuffer(buf, bufSize);
	}
}

void MessageFactory::appendStreamInMsgBuffer(unsigned char *buf, unsigned long bufSize)
{
	BasicExceptionMgr::instance()->log(LogLevel::Fine,
			"MessageFactory::appendStreamInMsgBuffer adding buf to ByteBuffer (bufSize: %ld, expected _pendingMessageSerializedSize %ld)",
			bufSize, _pendingMessageSerializedSize);

	_pendingMessageStream = true;
	_pendingMessageSerializedSize = bufSize;
	_msgBuffer->add(buf, bufSize);
}

bool MessageFactory::messagesAvailable()
{
	return ( _msgQ->isEmpty() == true) ? false : true;
}

Message *MessageFactory::getMessage()
{
	Message *msg = NULL;

	BasicExceptionMgr::instance()->log(LogLevel::Finer,
			"MessageFactory::getMessage  before _msgQ->get (_msgQ size: %d)",
			_msgQ->size() );

	msg = _msgQ->get();

	BasicExceptionMgr::instance()->log(LogLevel::Finer,
			"MessageFactory::getMessage  after _msgQ->get (_msgQ size: %d)",
			_msgQ->size() );

	return msg;
}
