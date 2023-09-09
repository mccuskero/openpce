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

#ifndef __ByteBuffer_h
#define __ByteBuffer_h

namespace pcff {

class ByteBuffer
{
	// implenmentation
	public:
		ByteBuffer();
	    virtual ~ByteBuffer();

	private:
		void initMembers();
	    
   // operations
   public:
	  bool     			add(unsigned char *buf, unsigned int len);
      bool     			add(char *buf, unsigned int len);
      bool     			add(ByteBuffer* pByteBuffer);
      unsigned char*    getBuffer() { return _pByteBuffer; };
      bool     			isEmpty()   { return ( (_pByteBufferLen == 0) ? true : false ); };
      unsigned int      getLength() { return _pByteBufferLen; };
      void              clear();
      //attributes

private:
  unsigned char*    _pByteBuffer;
  unsigned int     	_pByteBufferLen;

};

};

#endif
