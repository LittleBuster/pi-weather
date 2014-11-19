// weather ap is used for reading temperature and humidity from dht22 sensor
// on raspberry pi B version with Raspbian OS and add to database.
//
// Copyright © 20012-2014 by Sergey Denisov aka 'LittleBuster'
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
#include <mysql/mysql.h>
#include <bcm2835.h>

#include "configs.h"
#include <time.h>
#include <pi_dht_read.h>
#include "json.h"
#include "log.h"

#define SENSORS_COUNT 3

int send_to_db( const DB_Req *req, const Configs *cfg, int index ) {
	char _date[100];
	char _time[100];
	time_t t = time(NULL);		
	struct tm * timeinfo;
	char q[1024];
	char _datetime[100];
	char _temp[10];
	char _hum[10];
	char _hydro[10];
	char _ind[100];
	
	MYSQL mysql, *conn;
	MYSQL_RES *res;
	MYSQL_ROW rowa;
	int query_state;

	timeinfo = localtime (&t);
	strftime(_date, 100, "%F", timeinfo);
	strftime(_time, 100, "%T", timeinfo);
	strcpy(_datetime, _date);
	strcat(_datetime, " ");
	strcat(_datetime, _time);

	mysql_init(&mysql);
	conn = mysql_real_connect(&mysql, cfg->mysql_ip, cfg->mysql_user, cfg->mysql_passwd, cfg->mysql_base, 3306, NULL, 0);
	if (conn == NULL) {
		puts("Fail");
		return 1;
	}
	puts("Connected to database.");

	//convert to text
	sprintf(_temp, "%.1f", req->temp);
	sprintf(_hum, "%d", (int)req->hum);
	sprintf(_hydro, "%d", (int)req->hydro);
	
 	//creqte query
	if (index == 3) {
		strcpy(q, "INSERT INTO meteo_water0(water, date) VALUES('");
		strcat(q, _hydro);
		strcat(q, "','");
		strcat(q, _datetime);
		strcat(q, "')");

		query_state = mysql_query(conn, q);
		if (query_state != 0) {
			puts("DB Hydro: Fail insert!");
         		mysql_close(&mysql);
			return -1;
		} else {
			puts("DB Hydro: Data sended.");
			return 0;
		}
	}

	switch (index) {
		case 0: {
			strcpy(q, "INSERT INTO meteo_temp0(temp, hum, date) VALUES('");
			break;
		}

		case 1: {
			strcpy(q, "INSERT INTO meteo_temp1(temp, hum, date) VALUES('");
			break;
		}

		case 2: {
			strcpy(q, "INSERT INTO meteo_temp2(temp, hum, date) VALUES('");
			break;
		}
	}

	strcat(q, _temp);
	strcat(q, "','");
	strcat(q, _hum);
	strcat(q, "','");
	strcat(q, _datetime);
	strcat(q, "')");

	query_state = mysql_query(conn, q);
	if (query_state != 0)
	{
		printf("DB DHT%d: Fail insert!\n", index);
		mysql_close(&mysql);
		return -1;
	} else {
		printf("DB DHT%d: Data sended.\n", index);
	}

	mysql_close(&mysql);
	return 1;
}

void send_mail( const char *text, const Configs *cfg ) {
	char mail[2048];

	strcpy(mail, "curl -s --user 'api:key-");
	strcat(mail, cfg->mail_api);
	strcat(mail, "' https://api.mailgun.net/v2/sandboxfdb007e4898647fb87f6ffa9a5e511fa.mailgun.org/messages ");
    strcat(mail, "-F from='");
	strcat(mail, cfg->mail_from);
	strcat(mail, " <postmaster@sandboxfdb007e4898647fb87f6ffa9a5e511fa.mailgun.org>' ");
    strcat(mail, "-F to='");
	strcat(mail, cfg->mail_to);
	strcat(mail, "' ");

	//If to_user2 not exists, then skip
	if (strcmp(cfg->mail_to2, "")) {
		strcat(mail, "-F to='");
		strcat(mail, cfg->mail_to2);
		strcat(mail, "' ");
	}

    strcat(mail, "-F subject='");
	strcat(mail, cfg->mail_sub);
	strcat(mail, "' -F text='");
	strcat(mail, text);
	strcat(mail, "'");
	system( mail );
}

void send_sms_aero( const char *text, const Configs *cfg ) {
	char sms[2048];

	strcpy(sms, "curl http://gate.smsaero.ru/send/\\?user=");
	strcat(sms, cfg->sms_user);

	strcat(sms, "\\&password=");
	strcat(sms, cfg->sms_passwd);

	strcat(sms, "\\&to=");
	strcat(sms, cfg->sms_to);

	strcat(sms, "\\&from=");
	strcat(sms, cfg->sms_from);

	strcat(sms, "\\&text=");
	strcat(sms, text);

	system(sms);
}

void* thread_func( void *arg ) {
	Configs* cfg = (Configs*)arg;
	int WATER = 0;
	unsigned isFirst = 1;
	int res = 0;
	int count;

	//listen hydro sensor
	bcm2835_gpio_fsel(cfg->HYDRO_PIN, BCM2835_GPIO_FSEL_INPT);
	
	while (1) {
		//check temp & hum
		TempHum th;
		DB_Req req;

		//Read water			
		int lev = bcm2835_gpio_lev(cfg->HYDRO_PIN);

		if ((lev == 0) && (WATER == 0)) {
			WATER = 1;
			if (!strcmp(cfg->sms_enable, "true")) {
				send_sms_aero("Обнаружена+вода", cfg);
			}
			send_mail("Обнаружена вода!", cfg);
			puts("Обнаружена вода!");
		}

		if ((lev == 1) && (WATER == 1)) {
			WATER = 0;
		}
		req.hydro = WATER;
		send_to_db(&req, cfg, 3);	
		
		//Reading temperature and gumidity
		if (isFirst) {
			isFirst = 0;
			pi_dht_read( DHT22, cfg->DHT_PIN, &th );
			pi_dht_read( DHT22, cfg->DHT2_PIN, &th );
			pi_dht_read( DHT22, cfg->DHT3_PIN, &th );
		}

		for (unsigned i = 0; i < SENSORS_COUNT; i++) {
			//Check sensor while not return res
			count = 30;						
			
			while (1) {
				th.humidity = 0.0f;
				th.temperature = 0.0f;

				switch (i) {
					case 0: {
						res = pi_dht_read( DHT22, cfg->DHT_PIN, &th );
						break;
					}

					case 1: {
						res = pi_dht_read( DHT22, cfg->DHT2_PIN, &th );
						break;
					}

					case 2: {
						res = pi_dht_read( DHT22, cfg->DHT3_PIN, &th );
						break;
					}
				}				

				if (res == 0) {
					break;
				}

				count--;
				if (count == 0) {
					break;
				}
				bcm2835_delay(200);
			}

			if (res == 0) {
				printf("Temperature #%d is:%.1f Humidity is:%d\n", i, th.temperature, (int)th.humidity );
				req.temp = th.temperature;
				req.hum = (int)th.humidity;
				//Send to database	
				send_to_db(&req, cfg, i);			
			} else {
				printf("Sensor #%d - Not found\n", i);

				char logs[150];
				char num[10];

				sprintf(num, "%d", i);
				strcpy(logs, "DHT Sensor #");
				strcat(logs, num);
				strcat(logs, " - Not found.");
				log(logs, LOG_ERROR);
			}
		}
		//wait
		sleep(cfg->interval);
	}
}

int main( int argc, char *argv[] ) {
	pthread_t th;
	Configs cfg;

	bcm2835_init();
	json_parse_file("/home/pi/weather/weather/config.json", &cfg);

	pthread_create(&th, NULL, thread_func, (void*)&cfg);
	pthread_detach(th);
	
	while(1) {
		bcm2835_delay(60000);
	}
}
