#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "sync_car.h"

void station_init(struct station *station) {
    pthread_mutex_init(&station->lock, NULL);
    pthread_cond_init(&station->car_arrived, NULL);
    pthread_cond_init(&station->passenger_boarded, NULL);
    pthread_cond_init(&station->car_ready_leave, NULL);

    station->car_available = 0;
    station->capacity = 0;
    station->boarded = 0;
    station->waiting = 0;
    station->onboard_pending = 0;
}

void station_load_car(struct station *station, int count) {
    pthread_mutex_lock(&station->lock);

    while (station->car_available) {
        pthread_cond_wait(&station->car_ready_leave, &station->lock);
    }

    station->car_available = 1;
    station->capacity = count;
    station->boarded = 0;
    station->onboard_pending = 0;

    pthread_cond_broadcast(&station->car_arrived);

    while (station->onboard_pending > 0 ||
           (station->boarded < station->capacity && station->waiting > 0)) {
        pthread_cond_wait(&station->passenger_boarded, &station->lock);
    }

    station->car_available = 0;
    station->capacity = 0;
    station->boarded = 0;
    station->onboard_pending = 0;

    pthread_cond_broadcast(&station->car_ready_leave);

    pthread_mutex_unlock(&station->lock);
}

void station_wait_for_car(struct station *station) {
    pthread_mutex_lock(&station->lock);

    station->waiting++;

    while (!station->car_available || station->boarded >= station->capacity) {
        pthread_cond_wait(&station->car_arrived, &station->lock);
    }

    station->waiting--;
    station->boarded++;
    station->onboard_pending++;

    pthread_mutex_unlock(&station->lock);
}

void station_on_board(struct station *station) {
    pthread_mutex_lock(&station->lock);

    station->onboard_pending--;

    pthread_cond_broadcast(&station->passenger_boarded);

    pthread_mutex_unlock(&station->lock);
}

#ifdef SYNC_CAR_MAIN

#define NUM_PASSENGERS 20
#define CAR_CAPACITY 5
#define NUM_CARS 4

static struct station st;

static void *passenger_thread(void *arg) {
    (void)arg;
    station_wait_for_car(&st);
    usleep(1000);
    station_on_board(&st);
    return NULL;
}

static void *car_thread(void *arg) {
    (void)arg;
    for (int i = 0; i < NUM_CARS; i++) {
        station_load_car(&st, CAR_CAPACITY);
        usleep(5000);
    }
    return NULL;
}

int main(void) {
    station_init(&st);

    pthread_t car;
    pthread_create(&car, NULL, car_thread, NULL);

    pthread_t passengers[NUM_PASSENGERS];
    for (int i = 0; i < NUM_PASSENGERS; i++) {
        pthread_create(&passengers[i], NULL, passenger_thread, NULL);
        usleep(200);
    }

    for (int i = 0; i < NUM_PASSENGERS; i++) {
        pthread_join(passengers[i], NULL);
    }
    pthread_join(car, NULL);

    printf("Todos os %d passageiros embarcaram em %d carros (cap=%d).\n",
           NUM_PASSENGERS, NUM_CARS, CAR_CAPACITY);

    pthread_mutex_destroy(&st.lock);
    pthread_cond_destroy(&st.car_arrived);
    pthread_cond_destroy(&st.passenger_boarded);
    pthread_cond_destroy(&st.car_ready_leave);
    return 0;
}

#endif