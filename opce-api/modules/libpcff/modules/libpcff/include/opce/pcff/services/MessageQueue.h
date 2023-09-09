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

//
// The queue holds Message *'s to reduce memcpy overhead
//
//

#ifndef MessageQueue_h_
#define MessageQueue_h_

#include <queue>
#include <pcff/services/Message.h>

namespace pcff
{

class MessageQueue
{
public:
	MessageQueue();
	virtual ~MessageQueue();

private:
	void initMembers();

public:
	void setMaxQSize(int maxQueueSize) { _maxQueueSize = maxQueueSize; }

public:
	void     put(Message *msg);
	Message *get();
	bool     isEmpty();
	bool     isFull();
	int      size();

public:
	enum eMsgQConstants
	{
		MAX_Q_SIZE=100
	};

private:
	std::queue<Message *> _mqueue;
	int _maxQueueSize;
};

};

#endif

