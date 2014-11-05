all:
	g++ -Wall -O3 -c weather_checker.cpp -lboost_system -ldht_driver -std=c++11 -ldht_driver
	g++ -Wall -O3 -c weather.cpp -lboost_system -lboost_thread -std=c++11
	g++ -Wall -O3 weather.o weather_checker.o -o weather -lboost_system -lboost_thread -lpthread -ldht_driver -std=c++11

clean:
	rm -rf *.o
	rm -rf weather
