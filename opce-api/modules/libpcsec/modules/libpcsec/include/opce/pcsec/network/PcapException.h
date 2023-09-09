/* 
 * File:   pcapexception.h
 * Author: owenm
 *
 * Created on April 20, 2009, 11:46 PM
 */

#ifndef _PCAPEXCEPTION_H
#define	_PCAPEXCEPTION_H

#include <pcff/BasicException.h>

namespace pcsec
{

class PcapException : public pcff::BasicException
{
public:

   enum ExceptionEnums
   {
      EthernetHeaderIsNULL = NEXT_AVAILABLE_ERROR_VAL,
      InvalidTransportHeaderSize,
      InvalidIPHeaderSize,
      UserMustBeRoot,
      ErrorFindingDevice,
      ErrorOpeningDevice,
      IntefaceNotSupported,
      ErrorOpeningPcapFile,
      ErrorPcapFile,
      ErrorPcapGetNext,
      Unknown
   };

   PcapException(int type = BasicException::UnknownError, const char* msg = NULL);
};

};


#endif	/* _PCAPEXCEPTION_H */

