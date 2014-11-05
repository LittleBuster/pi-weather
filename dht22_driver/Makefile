all: 
	g++ dht22.cpp -c -fPIC -Wall -O3 -march=armv6 -lbcm2835 -I. -std=c++11
	
	g++ -shared dht22.o -o libdht_driver.so -Wall -O3 -lbcm2835 -std=c++11

install:
	cp libdht_driver.so /usr/lib/
	cp dht22.hpp /usr/include/

clean:
	rm -rf *.o *.so

remove:
	rm -rf *.so
	rm /usr/lib/libdht_driver.so
	rm /usr/include/dht22.hpp
