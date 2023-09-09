/* 
 * File:   pcapdefaulteventprocessor.cc
 * Author: owenm
 * 
 * Created on April 17, 2009, 7:32 PM
 */

#include <pcff/BasicExceptionMgr.h>

#include <pcsec/network/PcapDefaultEventProcessor.h>

using namespace pcff;
using namespace pcsec;

PcapDefaultEventProcessor::PcapDefaultEventProcessor()
{
}

PcapDefaultEventProcessor::~PcapDefaultEventProcessor()
{
}

void PcapDefaultEventProcessor::initialize()
{

}

void PcapDefaultEventProcessor::process(Event &evt)
{
    BasicExceptionMgr::instance()->log("PcapDefaultEventProcessor::process - received event");
    BasicExceptionMgr::instance()->log( evt.getEventInfo().c_str(), LogLevel::Info );
}
