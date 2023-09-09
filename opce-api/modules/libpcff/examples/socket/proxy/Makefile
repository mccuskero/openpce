prefix=./

CC=g++
PWD_NAME : sh = pwd

INCLUDE_PATH=

EXTRA_INCLUDES += -I/usr/local/pgsql/include \
        -I/usr/local/include/pcff \
        -I../include

CFLAGS = $(PROJ_CFLAGS) $(EXTRA_INCLUDES)
CXXFLAGS = $(EXTRA_INCLUDES) -g
CPPFLAGS = $(PROJ_CCPFLAGS) $(EXTRA_INCLUDES) -g
gLDFLAGS = $(PROJ_LDFLAGS)


LDFLAGS = 

#########################################################
#   EXE1
#########################################################

LOCAL_LIBRARIES1 = 
SYS_LIBRARIES1 = -lpcff -lstdc++ -lpthread

LIBS1=  ${LOCAL_LIBRARIES1} ${SYS_LIBRARIES1}

SRCS1  = proxyTest.cc \
         ProxyTestException.cc \
         ProxyTestConfig.cc \
			EchoProxyServer.cc \
			EchoProxyServerThread.cc
			

OBJ1 = ${SRCS1:.cc=.o}

ALLOBJS1 = ${OBJ1} 

EXE1 = ${prefix}/eproxyd


${EXE1}: ${ALLOBJS1} ${LOCAL_LIBRARIES1}
	@echo "*****  BUILDING $@ in ${PWD_NAME} "
	${CC} -o  $@ ${LDFLAGS} ${ALLOBJS1} ${LIBS1}

####
libs: ${LIB1}
exes: ${EXE1}
clean:
	@rm -f *.o lib*.a ${EXE1}
doc:
	@mkdir doc
	@mkdir doc/html
	@doxygen doxy_xxxx
