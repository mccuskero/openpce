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
SYS_LIBRARIES1 = -lstdc++ -lpcff -lpthread


LIBS1=  ${LOCAL_LIBRARIES1} ${SYS_LIBRARIES1}

SRCS1  = eventTestMain.cc 


#RCS_GNU_CPP = SRCS1

OBJ1 = ${SRCS1:.cc=.o}

ALLOBJS1 = ${OBJ1} 

EXE1 = ${prefix}/eventTest


${EXE1}: ${ALLOBJS1} ${LOCAL_LIBRARIES1}
	@echo "*****  BUILDING $@ in ${PWD_NAME} "
	${CC} -o  $@ ${LDFLAGS} ${ALLOBJS1} ${LIBS1}

libs: ${LIB1}
exes: ${EXE1} ${EXE2}
clean:
	@rm -f *.o lib*.a eventTest
doc:
	@mkdir doc
	@mkdir doc/html
	@doxygen doxy_util
