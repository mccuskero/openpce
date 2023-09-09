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

#ifndef SNORTPROXYSERVICEFACTORY_H_
#define SNORTPROXYSERVICEFACTORY_H_

#include <pcff/InformationList.h>
#include <pcff/proxy/ProxyService.h>
#include <pcff/event/proxy/EventIngestor.h>
#include <pcff/event/proxy/EventProcessor.h>
#include <pcff/proxy/ProxyServiceFactory.h>

namespace pcsec
{

class SnortProxyServiceFactory : public pcff::ProxyServiceFactory
{
public:
	SnortProxyServiceFactory();
	virtual ~SnortProxyServiceFactory();

public:
	virtual pcff::ProxyService *create(pcff::InformationList &createInfo);
	virtual pcff::EventIngestor  *createIngestor(pcff::InformationList &createInfo);
	virtual pcff::EventProcessor *createProcessor(pcff::InformationList &createInfo);

private:
	bool batchMode(pcff::InformationList &createInfo);
	bool testMode(pcff::InformationList &createInfo);
	pcff::EventIngestor *createSnortUnifiedFileIngestor(pcff::InformationList &createInfo);
	pcff::EventIngestor *createSnortUnixSocketIngestor(pcff::InformationList &createInfo);
};

};

#endif /*SNORTPROXYSERVICEFACTORY_H_*/
