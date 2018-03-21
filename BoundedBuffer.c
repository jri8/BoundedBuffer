//need something to tell consumers that theres nothing else to consume
//need to tell producers theres nothing else to produce
//join after these complete
//makes sure the number of threads produced are the number of threads that you join
//get it working wiht a buffer size of 1
//possible problems: cause a deadlock, initialize wrong variables first

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

pthread_mutex_t = m;
int num_consumer, num_producer, buffer_size, num_loops;

int main (int argc, char* argv[]){

	if (argc != 5){
		printf("Incorrect number of command line arguments\n");
		exit(0);
	}

	num_consumer = atoi(argv[1]);
	num_producer = atoi(argv[2]);
	buffer_size = atoi(argv[3]);
	num_loops = atoi(argv[4]);

	printf("%d, %d, %d, %d\n", num_consumer, num_producer, buffer_size, num_loops);

	return 0;
}

void *producer (void *argv){

	int i = 0;
	for (i, i < num_loops; i++){
		pthread_mutex_lock(&m);
		while (numfull == buffer_size)
	}

}
