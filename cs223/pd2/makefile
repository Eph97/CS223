CC	= gcc
CFLAGS	= -std=c99 -pedantic -Wall -g3

TSP:	tsp.o location.o
	${CC} ${CFLAGS} -o TSP tsp.o location.o -lm

tsp.o:	tsp.c location.h
	${CC} ${CFLAGS} -c tsp.c

location.o:	location.c location.h
	${CC} ${CFLAGS} -c location.c

clean:
	rm -r *.o TSP

