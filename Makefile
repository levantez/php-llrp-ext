CPP             = g++
RM              = rm -f
#CPP_FLAGS       = -Wall -c -I. -O2 -std=c++11
CPP_FLAGS       = -Wall -c -I. -O2 

PREFIX			= /usr
#Edit these lines to correspond with your own directories
LIBRARY_DIR		= /usr/lib/php/20190902/
PHP_CONFIG_DIR	= /etc/php/7.4/cli/conf.d/

LD              = g++
LD_FLAGS        = -Wall -shared -O2 
RESULT          = phpllrp.so

PHPINIFILE		= 30-phpllrp.ini

SOURCES			= $(wildcard *.cpp)
OBJECTS         = $(SOURCES:%.cpp=%.o)

all:	${OBJECTS} ${RESULT}

${RESULT}: ${OBJECTS}
		${LD} ${LD_FLAGS} -o $@ ${OBJECTS} -lphpcpp -L./libs -lltkcpp_x86_64 -lltkcppimpinj_x86_64 -lxml2_x86_64 -lz -lssl -lcrypto

clean:
		${RM} *.obj *~* ${OBJECTS} ${RESULT}

${OBJECTS}: 
		${CPP} ${CPP_FLAGS} -fpic -o $@ ${@:%.o=%.cpp}

install:
		cp -f ${RESULT} ${LIBRARY_DIR}
		cp -f ${PHPINIFILE}	${PHP_CONFIG_DIR}
