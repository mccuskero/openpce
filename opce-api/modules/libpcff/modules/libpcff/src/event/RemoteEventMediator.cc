

#include <pcff/services/Connector.h>

#include <pcff/event/RemoteEventMediator.h>

using namespace pcff;

RemoteEventMediator::RemoteEventMediator()
{
	_connector = NULL;
}

RemoteEventMediator::~RemoteEventMediator()
{
   if (_connector != NULL)
   {
      delete _connector;
      _connector = NULL;
   }
}
