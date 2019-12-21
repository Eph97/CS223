CC	= gcc
CFLAGS	= -std=c99 -pedantic -Wall -g3

all:	Heatmap Unit

Heatmap: trackpoint.o location.o Heatmap.o track.o trackpoint.h track.h location.h
	${CC} ${CFLAGS} -o Heatmap Heatmap.o trackpoint.o location.o track.o -lm

Unit: track_unit.o track.o trackpoint.o location.o track.h trackpoint.h location.h
	${CC} ${CFLAGS} -o Unit track_unit.o track.o trackpoint.o location.o -lm

track_unit.o: track_unit.c track.h trackpoint.h location.h
	${CC} ${CFLAGS} -c track_unit.c

track.o: track.c track.h location.h track.h trackpoint.h
	${CC} ${CFLAGS} -c track.c

Heatmap.o: Heatmap.c trackpoint.h
	${CC} ${CFLAGS} -c Heatmap.c

location.o:	location.c location.h
	${CC} ${CFLAGS} -c location.c

trackpoint.o: trackpoint.c trackpoint.h
	${CC} ${CFLAGS} -c trackpoint.c

clean:
	rm -r *.o Heatmap Unit
