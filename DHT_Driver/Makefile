all:
	gcc -fPIC -c common_dht_read.c -c -std=gnu99 -lrt
	gcc -fPIC -c pi_dht_read.c -c -I. -std=gnu99 -lrt
	gcc -fPIC -c pi_mmio.c -c -std=gnu99 -lrt
	gcc -shared common_dht_read.o pi_dht_read.o pi_mmio.o -o libdht_driver.so -lrt

install:
	cp libdht_driver.so /usr/lib/
	cp pi_dht_read.h /usr/include/
	cp common_dht_read.h /usr/include/

remove:
	rm /usr/lib/libdht_driver.so
	rm /usr/include/dht.h

clean:
	rm -rf *.o *.so
