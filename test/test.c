#include <bcm2835.h>
#include <pi_dht_read.h>

#define HYDRO_PIN 18
#define DHT_PIN 4
#define DHT_PIN2 23

int main( int argc, char **argv ) {
	TempHum th, th2;
	bcm2835_init();
	bcm2835_gpio_fsel(HYDRO_PIN, BCM2835_GPIO_FSEL_INPT);

	while (1) {
		printf("Hydro is: %d\n", bcm2835_gpio_lev(HYDRO_PIN));
		pi_dht_read( DHT22, DHT_PIN, &th );
		printf("Sensor #1; Temp: %d, Hum: %d\n", (int)th.temperature, (int)th.humidity);

		pi_dht_read( DHT22, DHT_PIN2, &th2 );
		printf("Sensor #2; Temp: %d, Hum: %d\n", (int)th2.temperature, (int)th2.humidity);
		delay(1000);
	}

	bcm2835_close();
}
