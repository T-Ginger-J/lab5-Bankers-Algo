#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUMBER_OF_CUSTOMERS 5
#define NUMBER_OF_RESOURCES 3

int available[NUMBER_OF_RESOURCES];
int maximum[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];
int allocation[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];
int need[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];

pthread_mutex_t mutex;

void *customer(void *customer_num) {
    int customer_id = *(int *)customer_num;

    while (1) {
        // Generate random resource request
        int request[NUMBER_OF_RESOURCES];
        for (int i = 0; i < NUMBER_OF_RESOURCES; ++i) {
            request[i] = rand() % (need[customer_id][i] + 1);
        }

        pthread_mutex_lock(&mutex);

        // Try to request resources
        if (request_resources(customer_id, request) == 0) {
            printf("Customer %d successfully allocated resources.\n", customer_id);
        } else {
            printf("Customer %d failed to allocate resources.\n", customer_id);
        }

        pthread_mutex_unlock(&mutex);

        // Sleep to simulate release of resources
        usleep(rand() % 1000000 + 500000);

        // Release random resources
        int release[NUMBER_OF_RESOURCES];
        for (int i = 0; i < NUMBER_OF_RESOURCES; ++i) {
            release[i] = rand() % (allocation[customer_id][i] + 1);
        }

        pthread_mutex_lock(&mutex);
        
        // Release resources
        release_resources(customer_id, release);

        pthread_mutex_unlock(&mutex);

        // Sleep before next request
        usleep(rand() % 1000000 + 500000);
    }

    return NULL;
}

int request_resources(int customer_num, int request[]) {
    // Check if request is within need and available
    for (int i = 0; i < NUMBER_OF_RESOURCES; ++i) {
        if (request[i] > need[customer_num][i] || request[i] > available[i]) {
            return -1; // Request exceeds need or available resources
        }
    }

    // Try to allocate resources
    for (int i = 0; i < NUMBER_OF_RESOURCES; ++i) {
        available[i] -= request[i];
        allocation[customer_num][i] += request[i];
        need[customer_num][i] -= request[i];
    }

    // Check safety
    if (!is_safe()) {
        // Rollback allocation
        for (int i = 0; i < NUMBER_OF_RESOURCES; ++i) {
            available[i] += request[i];
            allocation[customer_num][i] -= request[i];
            need[customer_num][i] += request[i];
        }
        return -1; // Unsafe state, deny request
    }

    return 0; // Request granted
}

void release_resources(int customer_num, int release[]) {
    // Release resources
    for (int i = 0; i < NUMBER_OF_RESOURCES; ++i) {
        available[i] += release[i];
        allocation[customer_num][i] -= release[i];
        need[customer_num][i] += release[i];
    }
}

int is_safe() {
    // Check if the system is in a safe state
    int work[NUMBER_OF_RESOURCES];
    int finish[NUMBER_OF_CUSTOMERS] = {0};

    // Initialize work to available
    for (int i = 0; i < NUMBER_OF_RESOURCES; ++i) {
        work[i] = available[i];
    }

    // Check each process
    for (int i = 0; i < NUMBER_OF_CUSTOMERS; ++i) {
        if (!finish[i]) {
            int j;
            for (j = 0; j < NUMBER_OF_RESOURCES; ++j) {
                if (need[i][j] > work[j])
                    break;
            }
            if (j == NUMBER_OF_RESOURCES) {
                // Process can finish
                finish[i] = 1;
                for (int k = 0; k < NUMBER_OF_RESOURCES; ++k) {
                    work[k] += allocation[i][k];
                }
                i = -1; // Restart loop to recheck all processes
            }
        }
    }

    // Check if all processes finished
    for (int i = 0; i < NUMBER_OF_CUSTOMERS; ++i) {
        if (!finish[i])
            return 0; // Unsafe state
    }

    return 1; // Safe state
}

int main(int argc, char *argv[]) {
    // Initialize available resources
    if (argc != NUMBER_OF_RESOURCES + 1) {
        fprintf(stderr, "Usage: %s <available resources for each type>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < NUMBER_OF_RESOURCES; ++i) {
        available[i] = atoi(argv[i + 1]);
    }

    // Initialize maximum, allocation, and need arrays (you may use any method)
    // ...

    // Initialize mutex
    pthread_mutex_init(&mutex, NULL);

    // Create customer threads
    pthread_t customers[NUMBER_OF_CUSTOMERS];
    int customer_ids[NUMBER_OF_CUSTOMERS];
    for (int i = 0; i < NUMBER_OF_CUSTOMERS; ++i) {
        customer_ids[i] = i;
        pthread_create(&customers[i], NULL, customer, &customer_ids[i]);
    }

    // Wait for all customer threads to finish
    for (int i = 0; i < NUMBER_OF_CUSTOMERS; ++i) {
        pthread_join(customers[i], NULL);
    }

    // Destroy mutex
    pthread_mutex_destroy(&mutex);

    return 0;
}

