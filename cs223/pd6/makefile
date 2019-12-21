CC = gcc
CFLAGS = -std=c99 -pedantic -Wall -g3

all: TSP Unit

Unit: lugraph.o location.o lugraph_unit.o queue.o
	${CC} -o $@ ${CFLAGS} $^ -lm


TSP: tsp.o lugraph.o location.o queue.o
	${CC} -o $@ ${CFLAGS} $^ -lm

tsp.o: tsp.c lugraph.h
lugraph_unit.o:	lugraph_unit.c lugraph.h location.h
location.o:	location.h

lugraph.o: lugraph.c queue.h

queue.o: queue.h

clean:
	rm -r *.o Unit
