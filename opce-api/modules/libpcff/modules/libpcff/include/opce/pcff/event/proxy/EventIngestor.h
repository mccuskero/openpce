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


#ifndef _EventIngestor_h_
#define _EventIngestor_h_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <pcff/proxy/ProxyService.h>

namespace pcff
{

 class EventIngestor
 {
 public:

  EventIngestor ()
  {
   _mode = EventIngestor::Unknown;
  }

 public:
  virtual void initialize () = 0;
  virtual void ingest () = 0;

 private:
  virtual void testIngest () = 0;
  virtual void batchIngest () = 0;
  virtual void continuousIngest () = 0;

 public:

  void
  setTestMode ()
  {
   _mode = EventIngestor::Test;
  }

  void
  setBatchMode ()
  {
   _mode = EventIngestor::Batch;
  }

  void
  setContinualMode ()
  {
   _mode = EventIngestor::Continuous;
  }

  int
  getMode ()
  {
   return _mode;
  }

 public:

  void
  set (ProxyService *service)
  {
   _proxyService = service;
  }

 public:

  enum IngestorState
  {
   Unknown,
   Test,
   Batch,
   Continuous
  };

 protected:
  int _mode;
  ProxyService *_proxyService;
 };

};


#endif
