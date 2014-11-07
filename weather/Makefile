all: weather

weather: main.o json.o
	gcc main.o json.o -o weather -ljansson -lpthread -ldht_driver -lbcm2835 -lmysqlclient -O3 -Wall -std=0x
	strip weather

main.o: main.c
	gcc -c main.c

json.o: json.c
	gcc -c json.c

clean:
	rm *.o

clear:
	rm weather
