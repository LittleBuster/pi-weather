all: weather

weather: main.o json.o log.o
	gcc main.o log.o json.o -o weather -ljansson -lpthread -ldht_driver -lbcm2835 -lmysqlclient -O3 -Wall -std=gnu99
	strip weather

main.o: main.c
	gcc -c main.c -std=gnu99

json.o: json.c
	gcc -c json.c

log.o: log.c
	gcc -c log.c

clean:
	rm *.o

clear:
	rm weather
