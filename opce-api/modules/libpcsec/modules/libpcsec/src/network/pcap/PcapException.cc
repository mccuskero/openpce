/* 
 * File:   pcapexception.cc
 * Author: owenm
 * 
 * Created on April 20, 2009, 11:46 PM
 */

#include <pcsec/network/PcapException.h>

using namespace pcff;
using namespace pcsec;

PcapException::PcapException(int exType, const char *msg)
{
   _type = exType;

   setErrMsg(msg);
   switch (exType)
   {
   case EthernetHeaderIsNULL:
      setErrString("Ethernet Header is NULL.");
      _type = ERR;
      break;
   case InvalidTransportHeaderSize:
      setErrString("Invalid transport header size.");
      _type = ERR;
      break;
   case InvalidIPHeaderSize:
      setErrString("Invalid header size.");
      _type = ERR;
      break;
   case UserMustBeRoot:
      setErrString("User must have root priviledges.");
      _type = ERR;
      break;
   case ErrorFindingDevice:
      setErrString("Error finding capture device.");
      _type = ERR;
      break;
   case ErrorOpeningDevice:
      setErrString("Error opening capture device.");
      _type = ERR;
      break;
   case IntefaceNotSupported:
      setErrString("Interface not supported.");
      _type = ERR;
      break;
   case ErrorOpeningPcapFile:
      setErrString("Error opening up pcap file.");
      _type = ERR;
      break;
   case ErrorPcapFile:
      setErrString("Error initializing pcap file.");
      _type = ERR;
      break;
   case ErrorPcapGetNext:
      setErrString("An error occured calling the pcap_next_ex.");
      _type = ERR;
      break;
   case BasicException::UnknownError :
              setErrString("An unknown error occured.");
      _type = ERR;
      break;
   default:
      break;
   }
}
