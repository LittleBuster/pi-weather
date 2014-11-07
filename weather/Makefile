all: weather

weather: main.o
	gcc main.o -o weather -ljansson -lpthread -ldht_driver

main.o: main.c
	gcc -c main.c

clean:
	rm *.o weather
