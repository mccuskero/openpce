subproject=./
prefix=./
projid=int

CC=g++
PWD_NAME : sh = pwd

INCLUDE_PATH=

EXTRA_INCLUDES += -I/usr/local/pgsql/include \
        -I/usr/local/include/pcff \
        -I../include

CFLAGS = $(PROJ_CFLAGS) $(EXTRA_INCLUDES)
CXXFLAGS = $(EXTRA_INCLUDES)
CPPFLAGS = $(PROJ_CCPFLAGS) $(EXTRA_INCLUDES) -g
LDFLAGS = $(PROJ_LDFLAGS)

LIBPATH = ${subproject}/${projid}_lib

LDFLAGS = 

#########################################################
#   EXE1
#########################################################

LOCAL_LIBRARIES1 =
SYS_LIBRARIES1 = -lstdc++ -lpcff -lpcsec -lpthread

LIBS1=  ${LOCAL_LIBRARIES1} ${SYS_LIBRARIES1}

SRCS1  = \
         TestEvent.cc \
         ServicesConfig.cc \
         ExampleConnector.cc \
         testConnectorClient.cc

#RCS_GNU_CPP = SRCS1

OBJ1 = ${SRCS1:.cc=.o}

ALLOBJS1 = ${OBJ1} 

EXE1 = ${prefix}/cntr


${EXE1}: ${ALLOBJS1} ${LOCAL_LIBRARIES1}
	@echo "*****  BUILDING $@ in ${PWD_NAME} "
	${CC} -o  $@ ${LDFLAGS} ${ALLOBJS1} ${LIBS1}

####
#########################################################
#   EXE2
#########################################################

LOCAL_LIBRARIES2 =
SYS_LIBRARIES2 = -lstdc++ -lpcff -lpcsec -lpthread

LIBS2=  ${LOCAL_LIBRARIES2} ${SYS_LIBRARIES2}

SRCS2  = \
         TestEvent.cc \
         ServicesConfig.cc \
         ExampleServiceHandler.cc \
         ExampleAcceptor.cc \
         testAcceptorServer.cc

#RCS_GNU_CPP = SRCS2

OBJ2 = ${SRCS2:.cc=.o}

ALLOBJS2 = ${OBJ2} 

EXE2 = ${prefix}/acptr

${EXE2}: ${ALLOBJS2} ${LOCAL_LIBRARIES2}
	@echo "*****  BUILDING $@ in ${PWD_NAME} "
	${CC} -o  $@ ${LDFLAGS} ${ALLOBJS2} ${LIBS2}

####
libs: ${LIB1}
exes: ${EXE1} ${EXE2}
all: ${EXE1} ${EXE2}
clean:
	@rm -f *.o lib*.a cntr acptr
doc:
	@mkdir doc
	@mkdir doc/html
	@doxygen doxy_util
