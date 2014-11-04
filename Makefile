all:
	mkdir DHT_Driver/build/
	mkdir DHT_Driver/build/temp.linux-armv6l/
	mkdir DHT_Driver/build/release/
	mkdir DHT_Driver/build/temp.linux-armv6l/source/
	mkdir DHT_Driver/build/temp.linux-armv6l/source/Raspberry_Pi/

	gcc -pthread -fno-strict-aliasing -march=armv6 -mfloat-abi=hard -mfpu=vfp -O2 -pipe -fstack-protector --param=ssp-buffer-size=4 -DNDEBUG -march=armv6 -mfloat-abi=hard -mfpu=vfp -O2 -pipe -fstack-protector --param=ssp-buffer-size=4 -fPIC -c DHT_Driver/source/_Raspberry_Pi_Driver.c -o DHT_Driver/build/temp.linux-armv6l/source/_Raspberry_Pi_Driver.o -std=gnu99

	gcc -pthread -fno-strict-aliasing -march=armv6 -mfloat-abi=hard -mfpu=vfp -O2 -pipe -fstack-protector --param=ssp-buffer-size=4 -DNDEBUG -march=armv6 -mfloat-abi=hard -mfpu=vfp -O2 -pipe -fstack-protector --param=ssp-buffer-size=4 -fPIC -c DHT_Driver/source/common_dht_read.c -o DHT_Driver/build/temp.linux-armv6l/source/common_dht_read.o -std=gnu99

	gcc -pthread -fno-strict-aliasing -march=armv6 -mfloat-abi=hard -mfpu=vfp -O2 -pipe -fstack-protector --param=ssp-buffer-size=4 -DNDEBUG -march=armv6 -mfloat-abi=hard -mfpu=vfp -O2 -pipe -fstack-protector --param=ssp-buffer-size=4 -fPIC -c DHT_Driver/source/Raspberry_Pi/pi_dht_read.c -o DHT_Driver/build/temp.linux-armv6l/source/Raspberry_Pi/pi_dht_read.o -std=gnu99

	gcc -pthread -fno-strict-aliasing -march=armv6 -mfloat-abi=hard -mfpu=vfp -O2 -pipe -fstack-protector --param=ssp-buffer-size=4 -DNDEBUG -march=armv6 -mfloat-abi=hard -mfpu=vfp -O2 -pipe -fstack-protector --param=ssp-buffer-size=4 -fPIC -c DHT_Driver/source/Raspberry_Pi/pi_mmio.c -o DHT_Driver/build/temp.linux-armv6l/source/Raspberry_Pi/pi_mmio.o -std=gnu99

	gcc -pthread -shared -Wl,-O1,--sort-common,--as-needed,-z,relro DHT_Driver/build/temp.linux-armv6l/source/_Raspberry_Pi_Driver.o DHT_Driver/build/temp.linux-armv6l/source/common_dht_read.o DHT_Driver/build/temp.linux-armv6l/source/Raspberry_Pi/pi_dht_read.o DHT_Driver/build/temp.linux-armv6l/source/Raspberry_Pi/pi_mmio.o -L/usr/lib -lrt -o DHT_Driver/build/release/libdht_driver.so

	cp DHT_Driver/build/release/libdht_driver.so /usr/lib/libdht_driver.so
	cp DHT_Driver/source/dht.h /usr/include/

uninstall:
	rm -rf DHT_Driver/build/
	rm /usr/lib/libdht_driver.so
	rm /usr/include/dht.h

clean:
	rm -rf DHT_Driver/build/temp.linux-armv6l/source/*.*
	rm -rf DHT_Driver/build/temp.linux-armv6l/source/Raspberry_Pi/*.*
