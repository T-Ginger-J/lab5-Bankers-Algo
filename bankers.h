#ifndef BANKER_H
#define BANKER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>

#define CUSTOMER_COUNT 5
#define RESOURCE_TYPES 3

// Resource tracking
int resources_available[RESOURCE_TYPES]; 
int demand_maximum[CUSTOMER_COUNT][RESOURCE_TYPES]; 
int resources_allocated[CUSTOMER_COUNT][RESOURCE_TYPES]; 
int resources_needed[CUSTOMER_COUNT][RESOURCE_TYPES]; 

// Mutex for resource access
pthread_mutex_t resource_access_mutex;

// Customer thread routine
void *customer_routine(void *customer_id);

// Resource management functions
int request_resources(int customer_id, int request[]);
int release_resources(int customer_id, int release[]);
bool check_safe_state();

#endif
