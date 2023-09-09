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


#include <pcff/BasicExceptionMgr.h>

#include <pcff/event/EventHandler.h>
#include <pcff/event/EventMediator.h>

#include "TestProcessor.h"

// event handler managed by by this domain event manager
#include "SilkDataEH.h>"
#include "SnortEventEH.h>"
#include "HoneypotEventEH.h>"

#include "TestEventDirector.h"

using namespace pcff;

TestEventDirector::TestEventDirector()
{
}

TestEventDirector::~TestEventDirector()
{
}

void TestEventDirector::initialize()
{
	TestProcessor    *processor = NULL;
	EventMediator    *eventMediator = NULL;
	TestEventHandler *evtHandler = NULL;

	BasicExceptionMgr::instance()->log(LogLevel::Debug, "TestEventDirector::init");

	eventMediator = new TestMediator;
	processor = new TestProcessor;
	evtHandler = new TestSilkDataEH();
	evtHandler->initialize(processor, eventMediator);
	insert( TestEvent::SiLKDataID, evtHandler);

	eventMediator = new TestMediator;
	processor = new TestProcessor;
	evtHandler = new TestSnortEventEH();
	evtHandler->initialize(processor, eventMediator);
	insert( TestEvent::SnortEventID, evtHandler);

	eventMediator = new TestMediator;
	processor = new TestProcessor;
	evtHandler = new TestHoneypotEventEH();
	evtHandler->initialize(processor, eventMediator);
	insert( TestEvent::HoneytrapEventID, evtHandler);
}

