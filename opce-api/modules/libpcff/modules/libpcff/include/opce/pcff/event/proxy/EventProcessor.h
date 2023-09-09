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


#ifndef __EventProcessor_h
#define __EventProcessor_h

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <pcff/event/Event.h>

namespace pcff
{

class ProxyService;

class EventProcessor 
{
public:
	virtual void initialize() = 0;
	virtual void process(Event &evt) = 0;
	
public:
	void set(ProxyService *service)	{ _proxyService = service; }
	
private:
	ProxyService *_proxyService;
};

};


#endif
