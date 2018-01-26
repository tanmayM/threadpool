#g++ sockt4.cpp -fPIC --shared -o libsockt4.soa


CFLAGS = -fPIC --shared

libsockt4.so: sockt4.cpp
	g++ $(CFLAGS) $^ -o $@
