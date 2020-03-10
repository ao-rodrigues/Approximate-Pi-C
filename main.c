#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include<time.h>


#define DEFAULT_NUM_THREADS 1

void *calc_points_in_circle(void *thread_args);

double rand_coord(unsigned int *seed);

long g_num_points;
long g_num_threads;


typedef struct {
    long num_points;
    long points_in_circle;
} ThreadArgs;


int main(int argc, char *argv[]) {
    clock_t begin = clock();

    bool plain_output = false;
    g_num_threads = DEFAULT_NUM_THREADS;

    if (argc < 2) {
        printf("Usage: ./approxPiPar <num_points> <num_threads>(?) [* FLAGS]");
        return 0;
    } else if (argc >= 3){
        g_num_threads = (int) strtol(argv[2], (char **) NULL, 10);

        if (argc == 4) {
        // No argv comparisons just for simplicity
            plain_output = true;
        }
    }


    g_num_points = strtol(argv[1], (char **) NULL, 10);

    pthread_t threads[g_num_threads];
    ThreadArgs thread_args[g_num_threads];

    long total_points_in_circle = 0;
    double pi_estimate;

    long remainder = g_num_points % g_num_threads;
    int i;
    for (i = 0; i < g_num_threads; i++) {
        thread_args[i].points_in_circle = 0;

        if (remainder > 0) {
            thread_args[i].num_points = 1 + g_num_points / g_num_threads;
            remainder--;
        } else {
            thread_args[i].num_points = g_num_points / g_num_threads;
        }

        pthread_create(&threads[i], NULL, calc_points_in_circle, (void *) &thread_args[i]);
    }

    for (i = 0; i < g_num_threads; i++) {
        pthread_join(threads[i], NULL);
        total_points_in_circle += thread_args[i].points_in_circle;
    }

    pi_estimate = ((double) total_points_in_circle / (double) g_num_points) * 4.0;

    clock_t end = clock();

    if (plain_output) {
        double run_time_millis = (double)(end - begin) * 1000.0 / CLOCKS_PER_SEC;
        printf("%ld\t%ld\t%.2fms\n", g_num_points, g_num_threads, run_time_millis);
    } else {
        printf("Total number of points: %ld\n", g_num_points);
        printf("Points within circle: %ld\n", total_points_in_circle);
        printf("Pi estimation: %.5f\n", pi_estimate);

    }


    return 0;
}

double rand_coord(unsigned int *seed) {
    return (double) rand_r(seed) / (double) RAND_MAX;
}

void *calc_points_in_circle(void *thread_args) {
    ThreadArgs *casted_args = (ThreadArgs *) thread_args;
    long points_in_circle = 0;
    unsigned int seed = time(NULL);

    int i;
    for (i = 0; i < casted_args->num_points; i++) {
        double x = rand_coord(&seed);
        double y = rand_coord(&seed);

        double dist = x * x + y * y;

        if (dist <= 1.0) {
            points_in_circle++;
        }
    }

    casted_args->points_in_circle = points_in_circle;
    pthread_exit(NULL);
}

