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

void station_load_car(station_t *station, int count) {
    pthread_mutex_lock(&station->lock);

    while (station->car_available) {
        pthread_cond_wait(&station->car_ready_leave, &station->lock);
    }

    station->car_available = 1;
    station->capacity = count;
    station->boarded = 0;

    pthread_cond_broadcast(&station->car_arrived);

    while (station->boarded < station->capacity && station->waiting > 0) {
        pthread_cond_wait(&station->passenger_boarded, &station->lock);
    }

    station->car_available = 0;
    station->capacity = 0;
    station->boarded = 0;

    pthread_cond_broadcast(&station->car_ready_leave);

    pthread_mutex_unlock(&station->lock);
}

void station_wait_for_car(station_t *station) {
    pthread_mutex_lock(&station->lock);

    station->waiting++;

    while (!station->car_available || station->boarded >= station->capacity) {
        pthread_cond_wait(&station->car_arrived, &station->lock);
    }

    station->waiting--;
    station->boarded++;

    pthread_mutex_unlock(&station->lock);
}