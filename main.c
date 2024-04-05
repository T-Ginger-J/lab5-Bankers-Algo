#include "bankers.h"
#include <pthread.h>
#include <unistd.h>
#include <time.h>

void initialize_maximum_demand();

int main(int argc, char* argv[]) {
    // Validate command-line arguments
    if (argc != RESOURCE_TYPES + 1) {
        fprintf(stderr, "Usage: %s <R1> <R2> <R3>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Initialize available resources from command-line arguments
    for (int i = 0; i < RESOURCE_TYPES; i++) {
        resources_available[i] = atoi(argv[i + 1]);
    }

    // Initialize the mutex
    pthread_mutex_init(&resource_access_mutex, NULL);

    pthread_t customers[CUSTOMER_COUNT];
    int customer_indices[CUSTOMER_COUNT];
    srand((unsigned int)time(NULL));

    // Call initialize_maximum_demand here to set up the demand maximum matrix before calculating needs
    initialize_maximum_demand();

    // Calculate the initial need matrix based on the maximum and allocated resources
    for (int i = 0; i < CUSTOMER_COUNT; i++) {
        for (int j = 0; j < RESOURCE_TYPES; j++) {
            resources_needed[i][j] = demand_maximum[i][j] - resources_allocated[i][j];
        }
    }

    // Create customer threads
    for (int i = 0; i < CUSTOMER_COUNT; i++) {
        customer_indices[i] = i;
        if (pthread_create(&customers[i], NULL, customer_routine, &customer_indices[i]) != 0) {
            perror("Failed to create customer thread");
            exit(EXIT_FAILURE);
        }
    }

    // Wait for customer threads to finish
    for (int i = 0; i < CUSTOMER_COUNT; i++) {
        if (pthread_join(customers[i], NULL) != 0) {
            perror("Failed to join customer thread");
            exit(EXIT_FAILURE);
        }
    }

    pthread_mutex_destroy(&resource_access_mutex);
    return EXIT_SUCCESS;
}

void initialize_maximum_demand() {
    // Predefined maximum demand for each customer and resource type
    int predefined_maximum[CUSTOMER_COUNT][RESOURCE_TYPES] = {
        {7, 5, 3},
        {3, 2, 2},
        {9, 0, 2},
        {2, 2, 2},
        {4, 3, 3}
    };

    for (int i = 0; i < CUSTOMER_COUNT; i++) {
        for (int j = 0; j < RESOURCE_TYPES; j++) {
            demand_maximum[i][j] = predefined_maximum[i][j];
        }
    }
}

