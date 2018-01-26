#g++ -I sockt4/ threadpool.cpp -L.  -lpthread -lsockt4 -fPIC --shared -o libthrdpool.so

INC_DIR = -I sockt4
CFLAGS = -fPIC --shared
LDIR = -L.
LFLAGS = -lpthread -lsockt4

libthrdpool.so: threadpool.cpp threadpool.h queue.h
	g++ $(INC_DIR) $(CFLAGS) threadpool.cpp -o $@ $(LDIR) $(LFLAGS)
