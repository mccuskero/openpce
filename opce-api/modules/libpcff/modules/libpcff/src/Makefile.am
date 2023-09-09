
## ------------------------ ##
## includes                 ##
## ------------------------ ##
INCLUDES = -I../include/opce

## ------------------------ ##
## library name             ##
## ------------------------ ##
lib_LTLIBRARIES = libpcff.la

## ------------------------ ##
## sources                  ##
## ------------------------ ##
libpcff_la_SOURCES = \
               AttributeValuePair.cc \
               BasicMessage.cc \
               BasicException.cc \
               BasicExceptionMgr.cc \
               Config.cc \
               ConfigException.cc \
               Daemon.cc \
               DumpBuffer.cc \
               InformationList.cc \
               Log.cc \
               LogLevel.cc \
               LogLevelTS.cc \
               SignalHandler.cc \
               SIGTERM_SignalEventHandler.cc \
               SIGINT_SignalEventHandler.cc \
               Timer.cc \
               TimeStamp.cc \
               event/Event.cc \
               event/EventDirector.cc \
               event/EventException.cc \
               event/LocalEventMediator.cc \
               event/RemoteEventMediator.cc \
               filesystem/Path.cc \
               filesystem/PathException.cc \
               filesystem/PathExceptionHandler.cc \
               proxy/ProxyService.cc \
               proxy/ProxyServiceFactoryException.cc \
               proxy/SensorProxyBase.cc \
               proxy/SensorProxyException.cc \
               serialize/FromSerializer.cc \
               serialize/GetSizeSerializer.cc \
               serialize/MessageHeaderSerializer.cc \
               serialize/SerializeException.cc \
               serialize/Serializeable.cc \
               serialize/Serializer.cc \
               serialize/ToSerializer.cc \
               services/Acceptor.cc \
               services/Connector.cc \
               services/Message.cc \
               services/MessageFactory.cc \
               services/MessageQueue.cc \
               services/ServiceHandler.cc \
               services/ServicesException.cc \
               socket/AcceptException.cc \
               socket/BaseSocket.cc \
               socket/BindException.cc \
               socket/ByteBuffer.cc \
               socket/ConnectException.cc \
               socket/CIDRAddress.cc \
               socket/DatagramSocket.cc \
               socket/GetSockNameException.cc \
               socket/GetPeerNameException.cc \
               socket/GetSockOptException.cc \
               socket/IPv4SocketAddress.cc \
               socket/IPv6SocketAddress.cc \
               socket/INETAddress.cc \
               socket/ListenException.cc \
               socket/ProxyServer.cc \
               socket/ProxyServerThread.cc \
               socket/RecvException.cc \
               socket/RecvFromException.cc \
               socket/SendException.cc \
               socket/SendToException.cc \
               socket/Server.cc \
               socket/ServerThread.cc \
               socket/SetSockOptException.cc \
               socket/Socket.cc \
               socket/SocketExceptionHandler.cc \
               socket/SocketException.cc \
               socket/UnixSocket.cc \
               socket/UnixDatagramSocket.cc \
               socket/UnixSocketAddress.cc \
               thread/Thread.cc \
               thread/ThreadDirector.cc \
               thread/Mutex.cc \
               thread/ThreadComponent.cc \
               thread/Condition.cc \
               thread/ConnectionPool.cc \
               thread/ConnectionPoolItem.cc \
               thread/ServerPool.cc \
               thread/ServerPoolThread.cc \
               thread/ThreadExceptionMgr.cc



## ------------------------ ##
## LDFLAGS used to create   ##
## versioning suffix        ##
## defined in configure.ac  ##
## ------------------------ ##
libpcff_la_LDFLAGS= \
-version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE) \
-release $(LT_RELEASE)

