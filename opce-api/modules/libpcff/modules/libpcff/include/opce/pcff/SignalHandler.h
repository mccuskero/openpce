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


#ifndef SignalHandler_h
#define SignalHandler_h

#include <signal.h>

namespace pcff 
{

class SignalEventHandler;

class SignalHandler 
{
 public:
  static SignalHandler *instance(void);

  SignalEventHandler *registerEventHandler( int signum );
  SignalEventHandler *registerEventHandler( int signum, SignalEventHandler *eh);
  int                 removeEvHandler( int signum );

  bool                signaled(int sigNum);        // checks signal to see if set
  // TODO: to some range checking...
  SignalEventHandler *getHandler(int sigNum) { return _signalEvHandlers[sigNum]; }        // checks signal to see if set
  virtual void        processSignals()       {}     // override to change behavior
  virtual void        blockSignals();
  virtual void        unBlockSignals();
 
 protected:
  SignalHandler();

  static SignalHandler *_instance;

  // entry point for OS passed in to sigaction
  static void dispatcher (int signum);

  // table of handlers registered to by app
  static SignalEventHandler *_signalEvHandlers[NSIG];

 private:
  void initMembers();
};

};

#endif
