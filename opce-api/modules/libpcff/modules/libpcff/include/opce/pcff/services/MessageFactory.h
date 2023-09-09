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

#ifndef MessageFactory_h_
#define MessageFactory_h_

#include <pcff/socket/ByteBuffer.h>
#include <pcff/serialize/Serializeable.h>
#include <pcff/services/MessageQueue.h>
#include <pcff/services/Message.h>

namespace pcff
{
class MessageProcessor;  // e.g. Connector, ServiceHandler are types of MessageProcessors
class MessageData;       

class MessageFactory
{
public:
	MessageFactory(MessageProcessor &msgProcessor);
	virtual ~MessageFactory();
	
private:
	void initMembers();
	
public:
	void initialize();
	void     create(unsigned long bufSize, unsigned char *buf);  /// after rcv'd stream from server
	Message *create(int msgType);
	Message *create(int msgType, pcff::Serializeable &msgData);
	Message *create(MessageData &msgData);    // create after event created in proxy, 
											  // Event is a type of MessageData
public:
	void createMessageFromBuffer(unsigned char *buf, unsigned long bufSize);
	void processMessageStream(unsigned char *buf, unsigned long bufSize, unsigned long serializedSize);
	void appendStreamInMsgBuffer(unsigned char *buf, unsigned long bufSize);
	void processPendingMessageStream(unsigned char *buf, unsigned long bufSize);

public:
	bool	 messagesAvailable();
	Message *getMessage();
	
public:
	enum eFactoryCreateType
	{
		MessageError = 0,
		CreateMessage,
		CreateMessageProcessStream,
		PendMessageCreation
	};
	
	enum eFactoryMsgState
	{
		Unitialized = 0,
		NoMessages,
		MessagesCreated,
		MessagesPending,
		MessageCreationError
	};

private:
	// pcff::ByteBuffer *_currentMsgByteBuffer;
	int				_currentMsgExpectedSize;
	int				_factoryStatus;
	MessageProcessor  &_msgProcessor;
	MessageQueue	  *_msgQ;
	pcff::ByteBuffer  *_msgBuffer;
	bool			  _pendingMessageStream; // signals next stream should be for pending stream
	unsigned long     _pendingMessageSerializedSize; // expected size of pending message
};

};

#endif /*MESSAGEFACTORY_H_*/
