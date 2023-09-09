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


#include <stdlib.h>

extern "C"
{
#include <silk/dynlib.h>
#include <silk/silk.h>
}

#include <pcff/BasicExceptionMgr.h>


#include <pcsec/network/SilkFile.h>
//#include <pcsec/network/SilkDatastore.h>

#include <pcsec/network/SilkInterfaceDirector.h>

using namespace pcff;
using namespace pcsec;

SilkInterfaceDirector* SilkInterfaceDirector::_instance = NULL;

SilkInterfaceDirector& SilkInterfaceDirector::instance()
{
	if (_instance == NULL)
	{
		_instance = new SilkInterfaceDirector();
	}

	return *_instance;
}

SilkInterfaceDirector::SilkInterfaceDirector()
{
	initMembers();
}

SilkInterfaceDirector::~SilkInterfaceDirector()
{

}


void SilkInterfaceDirector::initMembers()
{
    _bInitialized = false;
}

void SilkInterfaceDirector::initialize(char *appName)
{
    // register the application
	if (_bInitialized == false)
	{
	    skAppRegister(appName);
	    _bInitialized = true;
	}
	else
	{
	    BasicExceptionMgr::instance()->log("SilkInterfaceDirectory initialize was already called");
	}

}

void SilkInterfaceDirector::shutdown()
{

}

SilkFile      &SilkInterfaceDirector::createSilkFile()
{

}

void		  SilkInterfaceDirector::destroy(SilkFile &file)
{

}

SilkDatastore &SilkInterfaceDirector::createSilkDatastore()
{

}

void		  SilkInterfaceDirector::destroy(SilkDatastore &file)
{

}
