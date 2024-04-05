#include "bankers.h"
#include <unistd.h> 
#include <stdlib.h> 
#include <time.h>

// Function Prototypes
void generate_request(int customer_id, int request[]);
void generate_release(int customer_id, int release[]);

void *customer_routine(void *arg) {
    int customer_id = *(int *)arg;
    int request[RESOURCE_TYPES];
    
    // Generate a random request within the customer's needs
    generate_request(customer_id, request); 
    
    pthread_mutex_lock(&resource_access_mutex);
    // Request resources
    if (request_resources(customer_id, request) == 0) {
        if (check_safe_state()) {
            printf("Customer %d's resources allocated.\n", customer_id);
            // Simulate a random work duration
            sleep(rand() % 3 + 1);
            
            // Generate a random release within the customer's allocation
            int release[RESOURCE_TYPES];
            generate_release(customer_id, release); 
            release_resources(customer_id, release);
        } else {
            release_resources(customer_id, request);
            printf("Customer %d's request denied. Unsafe state.\n", customer_id);
        }
    } else {
        printf("Customer %d's request denied. Insufficient resources.\n", customer_id);
    }
    pthread_mutex_unlock(&resource_access_mutex);

    // Rest for a while before making the next request
    sleep(rand() % 3 + 1);
    
    pthread_exit(NULL);
}

int request_resources(int customer_id, int request[]) {
    // First, check if the request is greater than the need.
    for (int i = 0; i < RESOURCE_TYPES; i++) {
        if (request[i] > resources_needed[customer_id][i]) {
            printf("Customer %d has exceeded its maximum claim.\n", customer_id);
            return -1;
        }
    }

    // Provisionally allocate resources.
    for (int i = 0; i < RESOURCE_TYPES; i++) {
        resources_available[i] -= request[i];
        resources_allocated[customer_id][i] += request[i];
        resources_needed[customer_id][i] -= request[i];
    }
    return 0;
}

int release_resources(int customer_id, int release[]) {
    // First, check if the release is within the allocation.
    for (int i = 0; i < RESOURCE_TYPES; i++) {
        if (release[i] > resources_allocated[customer_id][i]) {
            printf("Customer %d is attempting to release more resources than allocated.\n", customer_id);
            return -1;
        }
    }

    return 0;
}

bool check_safe_state() {
    int work[RESOURCE_TYPES];
    bool finish[CUSTOMER_COUNT] = {false};

    // Initialize work with the current available resources.
    for (int i = 0; i < RESOURCE_TYPES; i++) {
        work[i] = resources_available[i];
    }

    while (true) {
        bool found = false;
        for (int i = 0; i < CUSTOMER_COUNT; i++) {
            if (!finish[i]) {
                bool can_allocate = true;
                for (int j = 0; j < RESOURCE_TYPES; j++) {
                    if (resources_needed[i][j] > work[j]) {
                        can_allocate = false;
                        break;
                    }
                }
                if (can_allocate) {
                    for (int j = 0; j < RESOURCE_TYPES; j++) {
                        work[j] += resources_allocated[i][j];
                    }
                    finish[i] = true;
                    found = true;
                }
            }
        }

        if (!found) {
            for (int i = 0; i < CUSTOMER_COUNT; i++) {
                if (!finish[i]) {
                    return false; // If no unfinished process can be allocated, the system is not in a safe state.
                }
            }
            return true; // If all processes could finish, the system is in a safe state.
        }
    }
}

void generate_request(int customer_id, int request[]) {
    for (int i = 0; i < RESOURCE_TYPES; i++) {
        request[i] = rand() % (resources_needed[customer_id][i] + 1);
    }
}

void generate_release(int customer_id, int release[]) {
    for (int i = 0; i < RESOURCE_TYPES; i++) {
        release[i] = rand() % (resources_allocated[customer_id][i] + 1);
    }
}
