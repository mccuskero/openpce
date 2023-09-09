

#include <pcff/event/EventDirector.h>

#include <pcff/event/LocalEventMediator.h>

using namespace pcff;

LocalEventMediator::LocalEventMediator()
{
	_eventDirector = NULL;
}

LocalEventMediator::~LocalEventMediator()
{
	if (_eventDirector != NULL)
	{
		delete _eventDirector;
		_eventDirector = NULL;
	}
}
