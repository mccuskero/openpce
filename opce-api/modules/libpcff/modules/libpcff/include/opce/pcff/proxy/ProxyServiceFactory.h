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

#ifndef PROXYSERVICEFACTORY_H_
#define PROXYSERVICEFACTORY_H_

#include <pcff/proxy/ProxyService.h>


// throws ProxyServiceFactoryExceptions


namespace pcff
{
class ProxyService;
class EventIngestor;
class EventProcessor;
class InformationList;

class ProxyServiceFactory
{
public:
	virtual ProxyService   *create(InformationList &createInfo) = 0;

private:
	virtual EventIngestor  *createIngestor(InformationList &createInfo) = 0;
	virtual EventProcessor *createProcessor(InformationList &createInfo) = 0;
};

};
#endif /*PROXYSERVICEFACTORY_H_*/
