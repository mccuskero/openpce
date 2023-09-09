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
   

#ifndef _TestEventDirector_h_
#define _TestEventDirector_h_

#include <pcff/event/EventDirector.h>

namespace pcff
{

   class TestEventDirector : public EventDirector
   {
   public:
      TestEventDirector();
      virtual ~TestEventDirector();

   public:
      virtual void initialize();

   public:

      typedef enum
      {
         UnknownEvent = -1,
         SilkData = 0,
         NidsEvent,
         HoneytrapEvent,
         NumHandlerIDs
      };
   };

};

#endif /* _TestEventDirector_h_ */
