#include <stdio.h>
#include <boost/thread.hpp>
#include "weather_checker.hpp"

int main() {
	WeatherChecker weather;
	weather.load_configs("configs.cfg");
	weather.start();

	while (true) {
		boost::this_thread::sleep( boost::posix_time::milliseconds(10000) );
	}
	return 0;
}
