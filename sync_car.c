#include "sync_car.h"

void station_init(station_t *station) {
    pthread_mutex_init(&station->lock, NULL);
    pthread_cond_init(&station->car_arrived, NULL);
    pthread_cond_init(&station->passenger_boarded, NULL);
    pthread_cond_init(&station->car_ready_leave, NULL);

    station->car_available = 0;
    station->capacity = 0;
    station->waiting = 0;
    station->boarded = 0;
}