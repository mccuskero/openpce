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

#include <stdio.h>
#include <string.h>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>

#include <pcff/InformationList.h>

using namespace std;
using namespace pcff;

InformationList::InformationList()  
{
   initPrivateMembers();
}

InformationList::~InformationList() 
{
}


void InformationList::initPrivateMembers() 
{
	_listSize = 10;
}

void InformationList::init( int defaultSize ) 
{
	_listSize = defaultSize;
}

void InformationList::set(const char *name, AttributeValuePair::ValueType type, const char *val) 
{
	AttributeValuePair vpair;
	
	vpair.init( name, type, val );
	_informationList[vpair.getName()] = vpair;
}

void InformationList::set(const char *name, AttributeValuePair::ValueType type, string &val) 
{
	AttributeValuePair vpair;
	
	vpair.init( name, type, val );
	_informationList[vpair.getName()] = vpair;
}

void InformationList::set(const char *name, AttributeValuePair::ValueType type, int val) 
{
	AttributeValuePair vpair;
	
	vpair.init( name, type, val );
	_informationList[vpair.getName()] = vpair;
}

void InformationList::set(const char *name, AttributeValuePair::ValueType type, long val) 
{
	AttributeValuePair vpair;
	
	vpair.init( name, type, val );
	_informationList[vpair.getName()] = vpair;
}

void InformationList::set(const char *name, AttributeValuePair::ValueType type, float val) 
{
	AttributeValuePair vpair;
	
	vpair.init( name, type, val );
	_informationList[vpair.getName()] = vpair;
}

AttributeValuePair &InformationList::get(const char *name) 
{
	// TODO: check name (exceptions!!!)
	// maybe not in list
	return _informationList[name];
}

