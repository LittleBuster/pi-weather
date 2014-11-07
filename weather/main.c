// weather ap is used for reading temperature and humidity from dht22 sensor
// on raspberry pi B version with Pidora OS and add to database.
//
// Copyright © 20012-2014 by Sergey Denisov aka 'LittleBuster', Dom and Gert
// E-Mail: DenisovS21 at gmail dor com (DenisovS21@gmail.com)
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 3
// as published by the Free Software Foundation.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>

#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <pthread.h>
//#include <mysql.h>

#include <jansson.h>
#include <time.h>
#include <pi_dht_read.h>

typedef struct _configs {
	int interval;
	int DHT_PIN;

	char mysql_ip[100];
	char mysql_user[100];
	char mysql_passwd[100];
	char mysql_base[100];
} Configs;


int json_parse_file( const char *filename, Configs *cfg ) {
	char text[1024];
	size_t i;
	json_t *root;
	json_error_t error;

	strcpy(text, "");

	FILE *f = fopen(filename, "r");
	fread(text, 255, 1, f);
	fclose(f);

	root = json_loads(text, 0, &error);
	if (!json_is_array(root)) {
		puts("Error reading json file!"); 
		json_decref(root);
		return 1;
	}

	for (i = 0; i < json_array_size(root); i++) {
		if (i == 0) {
			json_t *data, *sensors, *_interval, *_pin;

			data = json_array_get(root, i);

			sensors = json_object_get(data, "Sensors");
			_interval = json_object_get(sensors, "interval");
			_pin = json_object_get(sensors, "DHT_PIN");
			cfg->interval = json_integer_value(_interval);
			cfg->DHT_PIN = json_integer_value(_pin);
		}

		if (i == 1) {
			json_t *data, *db, *_ip, *_user, *_passwd, *_base;

			data = json_array_get(root, i);

			db = json_object_get(data, "MariaDB");
			_ip = json_object_get(db, "ip");
			_user = json_object_get(db, "user");
			_passwd = json_object_get(db, "passwd");
			_base = json_object_get(db, "base");

			strcpy(cfg->mysql_ip, json_string_value(_ip));
			strcpy(cfg->mysql_user, json_string_value(_user));
			strcpy(cfg->mysql_passwd, json_string_value(_passwd));
			strcpy(cfg->mysql_base, json_string_value(_base));
		}
	}
	json_decref(root);
	return 0;
}

void send_to_db( int temp, int hum, Configs *cfg ) {
	char _date[100];
	char _time[100];
	time_t t = time(NULL);
		
	struct tm * timeinfo;
	timeinfo = localtime (&t);
	strftime(_date, 100, "%F", timeinfo);
	strftime(_time, 100, "%T", timeinfo);


}

void* thread_func( void *arg ) {
	Configs* cfg = (Configs*)arg;

	while (1) {
		//check temp & hum
		TempHum th;	
		pi_dht_read( DHT22, cfg->DHT_PIN, &th );

		printf("Temperature is:%d Humidity is:%d\n", (int)th.temperature, (int)th.humidity );
		send_to_db( (int)th.temperature, (int)th.humidity, cfg );
		//wait
		sleep(cfg->interval);
	}
}

int main( int argc, char *argv[] ) {
	pthread_t th;
	Configs cfg;

	json_parse_file("config.json", &cfg);
	pthread_create(&th, NULL, thread_func, (void*)&cfg);
	pthread_detach(th);
	
	while(1) {
		sleep(10);
	}
}
