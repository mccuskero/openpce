
## ------------------------ ##
## includes                 ##
## ------------------------ ##
INCLUDES = -I ../include/opce \
	   -I ../../../../libpcff/modules/libpcff/include/opce \
	   $(SKINCPATH)

## -I/opt/local/include

## ------------------------ ##
## library name             ##
## ------------------------ ##
lib_LTLIBRARIES = libpcsec.la

## ------------------------ ##
## sources                  ##
## ------------------------ ##
libpcsec_la_SOURCES = \
               network/AttackConnectionData.cc \
               network/AttackData.cc \
               network/AttackDownloadData.cc \
               network/AttackPayloadData.cc \
               network/EthernetHeaderInfo.cc \
               network/ICMPInfo.cc \
               network/IPHeaderInfo.cc \
               network/NetworkPacketInfo.cc \
               network/TCPHeaderInfo.cc \
               network/UDPHeaderInfo.cc \
               network/WifiHeaderInfo.cc \
               network/honeytrap/HoneytrapDefaultEventProcessor.cc \
               network/honeytrap/HoneytrapEvent.cc \
               network/honeytrap/HoneytrapUnixSocketIngestor.cc \
               network/pcap/PcapDefaultEventProcessor.cc \
               network/pcap/PcapEvent.cc \
               network/pcap/PcapException.cc \
               network/pcap/PcapIngestor.cc \
               network/pcap/PcapLiveIngestor.cc \
               network/pcap/PcapOfflineIngestor.cc \
               network/pcap/PcapProxyServiceFactory.cc \
               network/silk/SilkDatastore.cc \
               network/silk/SilkDatastoreIngestor.cc \
               network/silk/SilkDefaultEventProcessor.cc \
               network/silk/SilkException.cc \
               network/silk/SilkFile.cc \
               network/silk/SilkFileIngestor.cc \
               network/silk/SilkInterfaceDirector.cc \
               network/silk/SilkProxyServiceFactory.cc \
               network/silk/SilkRecord.cc \
               network/silk/SilkSocketOutputEventProcessor.cc \
               network/snort/SnortProxyServiceFactory.cc \
               network/snort/SnortDefaultEventProcessor.cc \
               network/snort/SnortUnifiedFileIngestor.cc \
               network/snort/SnortUnixSocketIngestor.cc \
               network/snort/SnortEvent.cc \
               proxy/PcsecSensorProxy.cc \
               proxy/PcsecSensorProxyException.cc \
               proxy/ProxyConfig.cc \
               proxy/SensorProxyConfig.cc  \
               proxy/SensorProxyConfigCommandLine.cc  \
               proxy/SensorProxyConfigFile.cc

## ------------------------ ##
## LDFLAGS used to create   ##
## versioning suffix        ##
## defined in configure.ac  ##
## ------------------------ ##
libpcsec_la_LDFLAGS= \
-version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE) \
-release $(LT_RELEASE)

