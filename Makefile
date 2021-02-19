
CC=gcc
CFLAGS=
DEPS = temperature_data.h temperature_data.c
LIBRARY = temperature_data.c

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all: temperature_data.o a6_tester_all.o
	gcc ${LIBRARY} a6_tester_all.c -o build/all

basic: temperature_data.o a6_tester_basic.o
	gcc ${LIBRARY} a6_tester_basic.c -o build/basic

load_all: temperature_data.o a6_tester_load_all.o
	gcc ${LIBRARY} a6_tester_load_all.c -o build/load_all

tester_read_observations: temperature_data.o a6_tester_read_observations.o
	gcc ${LIBRARY} a6_tester_read_observations.c -o build/tester_read_observations

clean:
	rm *.o

valgrind_test_all: all
	valgrind --leak-check=full --show-leak-kinds=all ./all
