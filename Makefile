CC = gcc
CFLAGS = -Wall -Wextra -pthread -O2

all: sync_car sync_car-runner

sync_car: sync_car.c sync_car.h
	$(CC) $(CFLAGS) -DSYNC_CAR_MAIN -o $@ sync_car.c

sync_car-runner: sync_car.c sync_car-runner.c sync_car.h
	$(CC) $(CFLAGS) -o $@ sync_car.c sync_car-runner.c

clean:
	rm -f sync_car sync_car-runner

.PHONY: all clean