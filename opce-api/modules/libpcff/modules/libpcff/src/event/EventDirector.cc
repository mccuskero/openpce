
#include <pcff/BasicExceptionMgr.h>

#include <pcff/event/Event.h>
#include <pcff/event/EventException.h>
#include <pcff/event/EventHandler.h>

#include <pcff/event/EventDirector.h>

using namespace std;
using namespace pcff;

EventDirector::EventDirector()
{
   _numHandlersAdded = 0;
}

EventDirector::~EventDirector()
{
   EventHandlerMap::iterator it;
   EventHandler *hdlr = NULL;

   for (it = _evtHandlerMap.begin(); it != _evtHandlerMap.end(); it++)
   {
      hdlr = (*it).second;

      if (hdlr != NULL)
      {
         delete hdlr;
      }
   }
}

void EventDirector::initialize()
{
}

void EventDirector::insert(int idx, EventHandler *hndlr)
{

   // TODO: test out to ensure boudary conditions work!!!!
   if (idx >= 0 && idx > _evtHandlerMap.size())
   {
      _evtHandlerMap.insert(EventHandlerMap::value_type(idx, hndlr));
      _numHandlersAdded++;
   }
   else
   {
      BasicExceptionMgr::instance()->log(LogLevel::Info,
                                         "EventDirector::add -Event Type Idx (%d) is out of range numHandler: %d",
                                         _evtHandlerMap.size());
      throw EventException(EventException::InvalidMapIndex, "EventDirector::insert");
   }
}

void EventDirector::handle(Event &evt)
{
   int eventID = evt.getEventID();

   BasicExceptionMgr::instance()->log(LogLevel::Debug, "EventDirector::handle - received event");
   BasicExceptionMgr::instance()->log(LogLevel::Info, "EventDirector::handle - %s", evt.getEventInfo().c_str());

   if (eventID >= 0 && eventID < _numHandlersAdded)
   {
      if (_evtHandlerMap.size() > 0)
      {
         _evtHandlerMap[eventID]->handle(evt);
      }
      else
      {
         // TODO: throw EventException "Event handler framework not intialized"
         BasicExceptionMgr::instance()->log(LogLevel::Info, "EventDirector::handle - Event handler framework not intialized");
         throw EventException(EventException::EventHandlerNotInitialized, "EventDirector::handle");
      }
   }
   else
   {
      // TODO: throw EventException "Event Type Idx is out of range"
      BasicExceptionMgr::instance()->log(LogLevel::Info,
                                         "EventDirector::handle -Event Type Idx is out of range eventID: %d numHandler: %d",
                                         eventID, _numHandlers);
      throw EventException(EventException::InvalidMapIndex, "EventDirector::handle");
   }
}
