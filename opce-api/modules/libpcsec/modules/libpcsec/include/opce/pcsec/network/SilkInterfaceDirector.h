#ifndef SILKINTERFACEDIRECTOR_H_
#define SILKINTERFACEDIRECTOR_H_

namespace pcsec
{

class SilkFile;
class SilkDatastore;

class SilkInterfaceDirector
{
public:
	static SilkInterfaceDirector& instance();

private:
	SilkInterfaceDirector();
	virtual ~SilkInterfaceDirector();
	void initMembers();

public:
	void initialize(char *appName);
	void shutdown();

public:
	bool isInitialized() { return _bInitialized; }

public:
	SilkFile      &createSilkFile();
	void		  destroy(SilkFile &file);
	SilkDatastore &createSilkDatastore();
	void		  destroy(SilkDatastore &file);

private:
	bool _bInitialized;

private:
	static SilkInterfaceDirector *_instance;
};

};

#endif /*SILKINTERFACEDIRECTOR_H_*/
