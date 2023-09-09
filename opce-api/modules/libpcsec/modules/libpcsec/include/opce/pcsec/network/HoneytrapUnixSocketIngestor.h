#ifndef HONEYTRAPUNIXSOCKETINGESTOR_H_
#define HONEYTRAPUNIXSOCKETINGESTOR_H_

#include <string>

#include <pcff/socket/UnixDatagramSocket.h>
#include <pcff/event/proxy/EventIngestor.h>

namespace pcsec
{

#define MAX_DG_SIZE 65535

class HoneytrapUnixSocketIngestor : public pcff::EventIngestor
{
public:
	HoneytrapUnixSocketIngestor();
	virtual ~HoneytrapUnixSocketIngestor();

private:
	void initMembers();
	
public:
	virtual void initialize();
	virtual void initializeContinuous();

public:
	virtual void ingest();
	void         stopIngesting() {  _bIngest = false; }

private:
	virtual void testIngest();
	virtual void batchIngest();
	virtual void continuousIngest();


public:
	void setUnixSocketPathname(const char *pathname)         { _unixSocketpathname = pathname; }

private:
	std::string                 _unixSocketpathname;
	pcff::UnixDatagramSocket	_serverUnixDSocket;
	bool                        _bIngest;

};

};

#endif /*HONEYTRAPUNIXSOCKETINGESTOR_H_*/
