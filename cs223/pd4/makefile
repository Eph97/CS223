CC	= gcc
CFLAGS	= -std=c99 -pedantic -Wall -g3

all:	Blotto Unit

Blotto: blotto.o smap.o smap.h
	${CC} ${CFLAGS} -o Blotto blotto.o smap.o -lm

# Private: smap.o smap_test_functions.o smap.h
# 	${CC} ${CFLAGS} -o Private smap.o smap_test_functions.o -lm

Unit: smap_unit.o smap.o smap_test_functions.o smap.h 
	${CC} ${CFLAGS} -o Unit smap_unit.o smap.o smap_test_functions.o -lm

# private.o: private.c smap.h smap_test_functions.h smap.h
# 	${CC} ${CFLAGS} -c private.c -lm

smap_unit.o: smap_unit.c smap.h smap_test_functions.h smap.h
	${CC} ${CFLAGS} -c smap_unit.c -lm
#
smap.o: smap.c smap.h 
	${CC} ${CFLAGS} -c smap.c

smap_test_functions.o: smap_test_functions.c smap_test_functions.h
	${CC} ${CFLAGS} -c smap_test_functions.c

Blotto.o: blotto.c smap.h
	${CC} ${CFLAGS} -c blotto.c

clean:
	rm -r *.o Blotto Unit
