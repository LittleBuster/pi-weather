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

#include <pi_dht_read.h>
#include "weather_checker.hpp"
#include <boost/bind.hpp>
#include <fstream>
#include <boost/thread.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>


WeatherChecker::WeatherChecker() {
	//Init checker timer
	this->tmr = new deadline_timer(this->io);
	this->tmr->expires_from_now( boost::posix_time::seconds( this->cfg.interval ) );
	this->tmr->async_wait( boost::bind( &WeatherChecker::on_timer, this ) );
}

bool WeatherChecker::load_configs( const string &filename ) {
	//Load configs from JSON config file
	ifstream ifs( filename );
	
	cout << "Loading configs...";

    	boost::property_tree::ptree pt;
    	boost::property_tree::read_json(ifs, pt);
    	
	try {
		this->cfg.interval = pt.get<int>("Sensors.interval");
		this->cfg.DHT_Pin = pt.get<int>("Sensors.DHT_PIN");

		this->cfg.mysql_server = pt.get<string>("MariaDB.ip");
		this->cfg.mysql_user = pt.get<string>("MariaDB.user");
		this->cfg.mysql_pwd = pt.get<string>("MariaDB.pwd");
		this->cfg.mysql_base = pt.get<string>("MariaDB.base");
	}
	catch (...) {
		cout << "fail." << endl;
		return false;
	}
	cout << "ok." << endl;
	return true;
}

void WeatherChecker::on_timer( void ) {
	//Check sensors
	TempHum th = { 0.0f, 0.0f };
	
	auto get_th = [] ( int pin, TempHum * _th ) {
		while (true) {
			int res = pi_dht_read( DHT22, pin, _th );
			if (res == 0) {
				break;
			}
			boost::this_thread::sleep( boost::posix_time::milliseconds(100) );	
		}
	};
	get_th( this->cfg.DHT_Pin,  &th );
	
	cout << "Temperature: " << th.temperature << " Humidity: " << th.humidity << endl;

	//Run timer
	this->tmr->expires_from_now( boost::posix_time::seconds( this->cfg.interval ) );
	this->tmr->async_wait( boost::bind( &WeatherChecker::on_timer, this ) );
}

void WeatherChecker::start( void ) {
	//Start listen sensors
	this->io.run();
}

WeatherChecker::~WeatherChecker() {
	//Free mem
	delete this->tmr;
}
