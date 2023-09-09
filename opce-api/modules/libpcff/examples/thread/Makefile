subproject=./
prefix=./
projid=int

CC=g++
PWD_NAME : sh = pwd

INCLUDE_PATH=

EXTRA_INCLUDES += -I/usr/local/include/pcff \
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
SYS_LIBRARIES1 = -lstdc++ -lpthread -lpcff

LIBS1=  ${LOCAL_LIBRARIES1} ${SYS_LIBRARIES1}

SRCS1  = threadTestMain.cc \
         ThreadConfig.cc \
         ThreadTestDirector.cc \
         ThreadTask.cc \
         TestThread.cc \
			TestThreadContain.cc \
			ThreadTaskContain.cc \
			TestThreadPool.cc \
			ThreadTaskPool.cc \
			TestServerPoolThread.cc \
			TestServerPool.cc \
			ThreadTaskServerPool.cc

SRCS_KRS = 

RCS_GNU_CPP = SRCS1

OBJ1 = ${SRCS1:.cc=.o}
OBJ1KR = ${SRCS_KRS:.cc=.o}

# ALLOBJS1 = ${OBJ1} ${OBJ1KR}
ALLOBJS1 = ${OBJ1} 

EXE1 = ${prefix}/threadTest


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
