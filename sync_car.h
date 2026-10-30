#ifndef SYNC_CAR_H
#define SYNC_CAR_H

#include <pthread.h>

typedef struct station {
    pthread_mutex_t lock;
    pthread_cond_t car_arrived;
    pthread_cond_t passenger_boarded;
    pthread_cond_t car_ready_leave;

    int car_available;
    int capacity;
    int waiting;
    int boarded;
} station_t;

void station_init(station_t *station);
void station_load_car(station_t *station, int count);
void station_wait_for_car(station_t *station);
void station_on_board(station_t *station);

#endif