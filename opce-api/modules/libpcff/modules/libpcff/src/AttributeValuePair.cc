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
#include <sstream>

#include <pcff/AttributeValuePair.h>

using namespace std;
using namespace pcff;

AttributeValuePair::AttributeValuePair() 
{
    _name      = "";
    _tupleType = UNKNOWN;
    _value     = "";  
}

AttributeValuePair::AttributeValuePair(const AttributeValuePair &in) {
    _name      = in._name;
    _tupleType = in._tupleType;
    _value     = in._value;    
}

AttributeValuePair::~AttributeValuePair() 
{
    
}

void AttributeValuePair::init( const char *name, ValueType type, const char* val ) 
{
    _name      = name;
    _tupleType = type;
    _value     = val;
}

void AttributeValuePair::init( const char *name, ValueType type, string &val ) 
{
    _name      = name;
    _tupleType = type;
    _value     = val;
}

void AttributeValuePair::init( const char *name, ValueType type, int val ) 
{
	ostringstream ss;	
	ss << val;
	
    _name      = name;
    _tupleType = type;
    _value     = ss.str();
}

void AttributeValuePair::init( const char *name, ValueType type, long val ) 
{
	ostringstream ss;	
	ss << val;
	
    _name      = name;
    _tupleType = type;
    _value     = ss.str();
}

void AttributeValuePair::init( const char *name, ValueType type, float val ) 
{
	ostringstream ss;	
	ss << val;
	
    _name      = name;
    _tupleType = type;
    _value     = ss.str();
}

AttributeValuePair& AttributeValuePair::operator=(AttributeValuePair& in) 
{
    _name      = in._name;
    _tupleType = in._tupleType;
    _value     = in._value;    
}

bool AttributeValuePair::operator==(AttributeValuePair& in) 
{
    throw "ConfigTuple: not implemented";
}

bool AttributeValuePair::operator<(AttributeValuePair& in) 
{
    throw "ConfigTuple: not implemented";    
}

