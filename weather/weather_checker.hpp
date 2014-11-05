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


#ifndef WEATHER_CHECKER_HPP
#define WEATHER_CHECKER_HPP


#include <iostream>
#include <boost/asio.hpp>

using namespace std;
using namespace boost::asio;

typedef struct _configs {
	std::string mysql_server;
	std::string mysql_user;
	std::string mysql_pwd;
	std::string mysql_base;
	int DHT_Pin;
	int interval;
} Configs;


class WeatherChecker
{
private:
	io_service io;
	deadline_timer *tmr;
	Configs cfg = { "", "", "", "", 0, 1 };

public:
	WeatherChecker();

	bool load_configs( const std::string &filename );

	void on_timer( void );
	
	void start( void );

	~WeatherChecker();
};


#endif
