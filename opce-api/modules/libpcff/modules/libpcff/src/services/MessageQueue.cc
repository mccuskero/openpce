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

#include <pcff/services/ServicesException.h>
#include <pcff/services/Message.h>
#include <pcff/services/MessageQueue.h>

using namespace std;
using namespace pcff;

MessageQueue::MessageQueue()
{
	initMembers();
}

MessageQueue::~MessageQueue()
{
	// TODO: if queue is not empty, and delete
	// is called then you must delete 
	// the contents!!!
}

void MessageQueue::initMembers()
{
	_maxQueueSize = MessageQueue::MAX_Q_SIZE;
}

void MessageQueue::put(Message *msg)
{
	if (size() < _maxQueueSize)
	{
		_mqueue.push(msg);
	}
	else
	{
		throw ServicesException(ServicesException::MsgQIsFull, " MessageQueue::put");
	}
}

Message *MessageQueue::get()
{
	Message *retMsg = NULL;
	
	if (_mqueue.empty() == false)
	{
		retMsg = _mqueue.front();
		_mqueue.pop();
	}
	else
	{
		throw ServicesException(ServicesException::MsgQIsEmpty, " MessageQueue::put");		
	}
	
	return retMsg;
}

bool MessageQueue::isEmpty()
{
	return _mqueue.empty();
}

bool MessageQueue::isFull()
{
	return (_mqueue.size() >= _maxQueueSize) ? true : false;
}

int MessageQueue::size()
{
	return _mqueue.size();
}

