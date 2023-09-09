/**
 * Copyright (c) 1994-2007 Owen McCusker <pcave@ct.metrocast.net>
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

#ifndef _EventDirector_h_
#define _EventDirector_h_

#include <map>

#include <pcff/event/Event.h>
#include <pcff/event/EventHandler.h>

namespace pcff
{
   class EventHandler;

 class EventDirector
 {
 public:
  EventDirector ();
  virtual ~EventDirector ();

  // dervived classes must set the handlers
 public:
  virtual void initialize () = 0;

 public:
  virtual void initialize (int size) = 0;
  void insert (int eventHandlerID, EventHandler *hndlr);

 public:
  virtual void handle (Event &event);

 public:
  typedef std::map<int, EventHandler *> EventHandlerMap;

 private:
  int _numHandlersAdded;
  int _numHandlers;
  EventHandlerMap _evtHandlerMap;
 };
};

#endif /*DOMAINEVENTMGR_H_*/
