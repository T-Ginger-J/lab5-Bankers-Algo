#ifndef BANKERS_H
#define BANKERS_H

int request_resources(int customer_num, int request[]);
int release_resources(int customer_num, int release[]);
int is_safe();

#endif

