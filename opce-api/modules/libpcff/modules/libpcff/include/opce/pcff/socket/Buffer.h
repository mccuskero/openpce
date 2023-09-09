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

#ifndef _Buffer_h
#define _Buffer_h

namespace pcff
{

class Buffer
{
 public:
  Buffer (int);
  ~Buffer ();
  
 public:
  int getDatasize() {return datasize;}
  void setDatasize(int i) {datasize=i;}
  
  int getCursorsize() {return cursorsize;}

  int getBufsize() {return bufsize;}
  
  unsigned char * getData() {return data;}

 public:
  void initCursor() {cursor=data;cursorsize=datasize;}
  unsigned char * getCursor() {return cursor;}
  void set_cursorOffset(int);

 private:
  int            bufsize;
  int            datasize;
  int            cursorsize;
  unsigned char *data;
  unsigned char *cursor;
};

}; // namespace 

#endif
