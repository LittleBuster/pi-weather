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


#ifndef CONFIGS_H
#define CONFIGS_H

typedef struct _db_request {
	//Insert in database
        int hydro;
        float temp;
        float hum;
} DB_Req;


typedef struct _configs {
	//Main JSON configs file
        int interval;
        int DHT_PIN;
        int DHT2_PIN;
        int HYDRO_PIN;

        char mysql_ip[100];
        char mysql_user[100];
        char mysql_passwd[100];
        char mysql_base[100];

        char sms_user[100];
        char sms_passwd[100];
        char sms_to[100];
        char sms_from[100];

        char mail_api[100];
        char mail_from[100];
        char mail_to[100];
        char mail_to2[100];
        char mail_sub[100];
} Configs;

#endif
