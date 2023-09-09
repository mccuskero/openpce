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

#include <sstream>
#include <iostream>

#include <pcff/DumpBuffer.h>

using namespace pcff;
using namespace std;

DumpBuffer *DumpBuffer::_instance = NULL;

DumpBuffer &DumpBuffer::instance()
{
   if (_instance == NULL)
   {
      _instance = new DumpBuffer();
   }
   
   return *_instance;
}

DumpBuffer::DumpBuffer()
{
   initPrivateMembers();
}

void DumpBuffer::initPrivateMembers()
{
   _outputType = DumpBuffer::DumpToCrt;   
   _maxColsPerRow = 60;   // default
   _maxCharPerRow = _maxColsPerRow/2;   // split between hex and char
}

int DumpBuffer::init(int maxColsPerRow, int outputType )
{
   _maxColsPerRow = maxColsPerRow;
   _outputType = outputType;
}

int DumpBuffer::dump(int bufSize, char *buf)
{
   unsigned char *ubuf = (unsigned char*)buf;
   
   int bufCount = bufSize;
   int currColPos = 0;
   int hexIdx = 0;
   int charIdx = 0;
   
   while (hexIdx < bufSize-1)
   {
      // print out hex part
      for(int i=0;i<_maxCharPerRow;i++)
      {
         if (hexIdx < bufSize-1)
         {
            cout.width(2);
            cout.fill('0');
            cout << right << hex << (int)ubuf[hexIdx++] << " ";         
         }
         else 
         {
            cout.width(2);
            cout.fill('0');
            cout << right << hex << 0 << " ";
         }
      }
      
      cout << " -- ";
      
      for(int i=0;i<_maxCharPerRow;i++)
      {
         if (charIdx < bufSize-1)
         {
            if ( ((int)buf[charIdx] >= 33) &&
                 ((int)buf[charIdx] <= 126)   )
            {
               cout << buf[charIdx++];         
            }
            else 
            {
               cout << ".";
               charIdx++;
            }
         }
         else
         {
            cout << ".";
         }
      }      
      cout << endl;   
      cout.flush();
   }
   
   return bufSize;
}
