// weather ap is used for reading temperature and humidity from dht22 sensor
// on raspberry pi B version with Pidora OS and add to database.
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

	//convert to char
	sprintf(_temp, "%d", (int)req->temp);
	sprintf(_hum, "%d", (int)req->hum);
	sprintf(_hydro, "%d", (int)req->hydro);
	
 	//creqte query
	if (index == 0) {
        	strcpy(q, "INSERT INTO weather(temperature, humidity, date, hydro) VALUES('");
	} else {
		sprintf(_ind, "%d", index);
        	strcpy(q, "INSERT INTO weather");
		strcat(q, _ind);
		strcat(q, "(temperature, humidity, date, hydro) VALUES('");
	}
	strcat(q, _temp);
	strcat(q, "','");
	strcat(q, _hum);
	strcat(q, "','");
	strcat(q, _datetime);
	strcat(q, "','");
	strcat(q, _hydro);
	strcat(q, "')");

        query_state = mysql_query(conn, q);
        if(query_state!=0)
        {
                puts("DB: Insert fail");
                return 1;
        } else {
		puts("DB: data sended.");
	}
 
         mysql_close(&mysql);
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

	//listen hydro sensor
	bcm2835_gpio_fsel(cfg->HYDRO_PIN, BCM2835_GPIO_FSEL_INPT);
	int WATER = 0;

	while (1) {
		//check temp & hum
		TempHum th, th2;	
		pi_dht_read( DHT22, cfg->DHT_PIN, &th );
		pi_dht_read( DHT22, cfg->DHT2_PIN, &th2 );

		printf("Temperature #1 is:%d Humidity is:%d\n", (int)th.temperature, (int)th.humidity );
		printf("Temperature #2 is:%d Humidity is:%d\n", (int)th2.temperature, (int)th2.humidity );

		//Read water
		int lev = bcm2835_gpio_lev(cfg->HYDRO_PIN);

		if ((lev == 0) && (WATER == 0)) {
			WATER = 1;
			send_sms_aero("Обнаружена+вода", cfg);
			send_mail("Обнаружена вода!", cfg);
			puts("Обнаружена вода!");
		}

		if ((lev == 1) && (WATER == 1)) {
			WATER = 0;
		}

		//Send to database	
		DB_Req req;
		req.temp = (int)th.temperature;
		req.hum = (int)th.humidity;
		req.hydro = WATER;

		send_to_db(&req, cfg, 0);

		req.temp = (int)th2.temperature;
		req.hum = (int)th2.humidity;
		
		send_to_db(&req, cfg, 1);

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
		sleep(10);
	}
}
