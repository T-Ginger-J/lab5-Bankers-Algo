#include "bankers.h"

int requestR(int customerN, int request[]) {
	//critical area lock mutex
	//loop through i requests
		//if requesti <= needi
		//if requesti <= availablei
			//availablei -= requesti
			//allocationNi += requesti
			//NeedNi -= requesti
		//release mutex
		//return -1
	//release mutex
	//return 0
}

int releaseR(int customerN, int release[]) {
	//critical area lock mutex
	//loop through i requests
		//availablei += requesti
		//allocationNi -= requesti
	//release mutex
	//return 0
}

//some sort of safety algorithm ???

int main( int argc, char *argv[] ) {
	int size = argc - 1;
   	int *max = (int *)malloc(size * sizeof(int));

    	if (array == NULL) {
        	printf("Memory allocation failed\n");
        	return 1;
    	}
    	
    	for (int i = 0; i < argc ; i++){
    		max[i] = atoi(argv[i+1]);
    	}
    	
    	
}
