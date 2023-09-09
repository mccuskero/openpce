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
#include <string.h>
#include <signal.h>

#include <pcff/SignalEventHandler.h>
#include <pcff/SignalHandler.h>


using namespace pcff;

SignalHandler      *SignalHandler::_instance = NULL;
SignalEventHandler *SignalHandler::_signalEvHandlers[NSIG];

SignalHandler *SignalHandler::instance() {
  if ( _instance == NULL ) {
    _instance = new SignalHandler();
  }

  return _instance;
}

SignalHandler::SignalHandler() {
  initMembers();
}

// TODO: check memory bounds
void SignalHandler::initMembers() {
  memset ( _signalEvHandlers, '\0', sizeof(_signalEvHandlers) );
}


SignalEventHandler *SignalHandler::registerEventHandler(int signum) {
   registerEventHandler(signum, new SignalEventHandler(signum) ); 
}

//
// used to create a "special" event handler. Note, the event handler
// should do a minimal amount of processing.
//
SignalEventHandler *SignalHandler::registerEventHandler(int signum, SignalEventHandler *eh) {  
  SignalEventHandler *oldEh = SignalHandler::_signalEvHandlers[signum];

  SignalHandler::_signalEvHandlers[signum] = eh;
  
  struct sigaction sa;
  sa.sa_handler = SignalHandler::dispatcher;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = 0;
  sigaction(signum, &sa, 0);

  return oldEh;
}


void SignalHandler::dispatcher( int signum ) {
  if (SignalHandler::_signalEvHandlers[signum] != NULL) {
    SignalHandler::_signalEvHandlers[signum]->handleSignal(signum);
  }
}


bool SignalHandler::signaled(int sigNum) {
   bool bSignaled = false;
   SignalEventHandler *pEvHandler = NULL;

   pEvHandler = getHandler(sigNum);
   
   if (pEvHandler != NULL) { 
      bSignaled = pEvHandler->signaled();
   }
   else {
      // TODO: throw exception
   } 


   return bSignaled;
}

void SignalHandler::blockSignals() {
   sigset_t sigs;

   sigfillset( &sigs );
   sigprocmask( SIG_BLOCK, &sigs, NULL );
}

void SignalHandler::unBlockSignals() {
   sigset_t sigs;

   sigfillset( &sigs );
   sigprocmask( SIG_UNBLOCK, &sigs, NULL );
}
