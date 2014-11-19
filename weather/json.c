// weather ap is used for reading temperature and humidity from dht22 sensor
// on raspberry pi B version with Raspbian OS and add to database.
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

#include <stdio.h>
#include <jansson.h>

#include "json.h"


int json_parse_file( const char *filename, Configs *cfg ) {
	char text[1024];
	size_t i;
	json_t *root;
	json_error_t error;

	memset(text, 0x00, sizeof( text ));

	FILE *f = fopen(filename, "r");
	fread(text, 1024, 1, f);
	fclose(f);

	root = json_loads(text, 0, &error);
	if (!json_is_array(root)) {
		puts("Error reading json file!"); 
		json_decref(root);
		return 1;
	}

	for (i = 0; i < json_array_size(root); i++) {
		switch (i) 
		{
		case 0: {
			json_t *data, *sensors, *_interval, *_pin, *_pin2, *_pin3, *_pin2_3;

			data = json_array_get(root, i);

			sensors = json_object_get(data, "Sensors");
			_interval = json_object_get(sensors, "interval");
			_pin = json_object_get(sensors, "DHT_PIN");
			_pin2 = json_object_get(sensors, "DHT_PIN2");
			_pin2_3 = json_object_get(sensors, "DHT_PIN3");
			_pin3 = json_object_get(sensors, "HYDRO_PIN");
			cfg->interval = json_integer_value(_interval);
			cfg->DHT_PIN = json_integer_value(_pin);
			cfg->DHT2_PIN = json_integer_value(_pin2);
			cfg->DHT3_PIN = json_integer_value(_pin2_3);
			cfg->HYDRO_PIN = json_integer_value(_pin3);
			free(data);
		} break;

		case 1: {
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
		} break;

		case 2: {
			json_t *data, *sms, *_user, *_passwd, *_to, *_from, *_en;

			data = json_array_get(root, i);

			sms = json_object_get(data, "SMS");
			_en = json_object_get(sms, "enable");
			_user = json_object_get(sms, "user");
			_passwd = json_object_get(sms, "passwd");
			_to = json_object_get(sms, "to");
			_from = json_object_get(sms, "from");
			
			strcpy(cfg->sms_enable, json_string_value(_en));
			strcpy(cfg->sms_user, json_string_value(_user));
			strcpy(cfg->sms_passwd, json_string_value(_passwd));
			strcpy(cfg->sms_to, json_string_value(_to));
			strcpy(cfg->sms_from, json_string_value(_from));
		} break;

		case 3: {
			json_t *data, *mail, *_api, *_from, *_to, *_to2, *_sub;

			data = json_array_get(root, i);

			mail = json_object_get(data, "Mail");
			_api = json_object_get(mail, "api-key");
			_from = json_object_get(mail, "from");
			_to = json_object_get(mail, "to");
			_to2 = json_object_get(mail, "to2");
			_sub = json_object_get(mail, "subject");
			
			strcpy(cfg->mail_api, json_string_value(_api));
			strcpy(cfg->mail_from, json_string_value(_from));
			strcpy(cfg->mail_to, json_string_value(_to));
			strcpy(cfg->mail_to2, json_string_value(_to2));
			strcpy(cfg->mail_sub, json_string_value(_sub));
			
		} break;
		}
	}
	json_decref(root);
	return 0;
}
