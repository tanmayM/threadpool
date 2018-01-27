#g++ -I /home/tvmande/code/from-bash/trials/ThreadPool/ -I /home/tvmande/code/from-bash/trials/ThreadPool/sockt4/ server.cpp -L/home/tvmande/code/from-bash/trials/ThreadPool/ -lpthread -lthrdpool -lsockt4 -o server

INC_DIR = -I/home/tvmande/code/from-bash/trials/ThreadPool/
INC_DIR += -I/home/tvmande/code/from-bash/trials/ThreadPool/sockt4/

LIB_PATH = /home/tvmande/code/from-bash/trials/ThreadPool/

LIBS = -lpthread
LIBS += -lthrdpool
LIBS += -lsockt4


$(info INC_DIR = $(INC_DIR))
$(info LIBS = $(LIBS))

client: client.cpp
	@echo "building client"
	g++ $(INC_DIR) $^ -o $@ -L$(LIB_PATH) $(LIBS) -g
