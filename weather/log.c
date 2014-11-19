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

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "log.h"

void log( const char *message, int type ) {
	char out[1024];
	char _date[100];
	char _time[100];
	time_t t = time(NULL);		
	struct tm * timeinfo;
	char _datetime[100];

	timeinfo = localtime (&t);
	strftime(_date, 100, "%F", timeinfo);
	strftime(_time, 100, "%T", timeinfo);
	strcpy(_datetime, _date);
	strcat(_datetime, " ");
	strcat(_datetime, _time);

	FILE *f = fopen("/home/pi/weather/weather/log.txt", "a");
	strcpy(out, "[");
	strcat(out, _datetime);

	switch (type) {
		case LOG_WARNING: {
			strcat(out, "][warning]: ");
			break;
		}

		case LOG_ERROR: {
			strcat(out, "][error]: ");
			break;
		}
	}

	strcat(out, message);
	strcat(out, "\n");
	fwrite(out, strlen(out), 1, f);

	fclose(f);
}
