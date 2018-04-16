//need something to tell consumers that theres nothing else to consume
//need to tell producers theres nothing else to produce
//join after these complete
//makes sure the number of threads produced are the number of threads that you join
//get it working wiht a buffer size of 1
//possible problems: cause a deadlock, initialize wrong variables first

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <semaphore.h>

sem_t empty;
sem_t full;
sem_t mutex;


int num_consumer, num_producer, buffer_size, num_loops;
int numfull = 0, fillptr = 0, useptr = 0;
int filled = 0, got = 0;
int cons_sleep = 0, prod_sleep = 0;
int pval = 0, cval = 0;
bool cons_done = false;
bool prod_done = false;

int *buffer;


void do_fill(int value){
	buffer[fillptr] = value;
	fillptr = (fillptr + 1) % buffer_size;
	numfull++;
}

int do_get() {
	int tmp = buffer[useptr];
	useptr = (useptr + 1) % buffer_size;
	numfull--;
	return tmp;
}

void *consumer (void *arg){
	
	while(got < num_loops - cons_sleep){
		cons_sleep++;
		sem_wait(&full);
		sem_wait(&mutex);
		got = got + 1;
		int tmp = do_get();

		printf("%d %d %d\n", tmp, (int)pthread_self(),got);
		
		sem_post(&mutex);
		sem_post(&empty);
		cons_sleep--;
	}

	printf("%d end cons\n", (int)pthread_self());

}

void *producer (void *arg){
	while(filled < num_loops-prod_sleep){
		prod_sleep++;
		sem_wait(&empty);
		sem_wait(&mutex);
		do_fill(filled);
		filled++;
		sem_post(&mutex);
		sem_post(&full);
		prod_sleep--;

	}

	printf("%d end prod\n", (int)pthread_self());


}


int main (int argc, char* argv[]){

	if (argc != 5){
		printf("Incorrect number of command line arguments\n");
		exit(0);
	}


	num_consumer = atoi(argv[1]);
	num_producer = atoi(argv[2]);
	buffer_size = atoi(argv[3]);
	num_loops = atoi(argv[4]);

	buffer = (int *) malloc(buffer_size * sizeof(int));
	
	printf("consumers: %d\n producers: %d\n buffer size: %d\n loops: %d\n", num_consumer, num_producer, buffer_size, num_loops);



	pthread_t pid[10], cid[10];
	sem_init(&empty, 0, buffer_size);
	sem_init(&full, 0, 0);
	sem_init(&mutex, 0 , 1);



	int i;

	for (i = 0; i < num_consumer; i++){
		pthread_create(&cid[i], NULL, consumer, NULL);
		printf("consumer %d = %d\n", i, (int) cid[i]);
	}
	for (i = 0; i < num_producer; i ++){
		pthread_create(&pid[i], NULL, producer, NULL);
		printf("producer %d = %d\n",i, (int)pid[i]);
	}

	for (i = 0; i < num_consumer; i++)
		pthread_join(cid[i], NULL);
	printf("consumers joined\n");

	for (i = 0; i < num_producer; i++)
		pthread_join(pid[i], NULL);
	printf("producers joined\n");
	
	printf("parent complete\n");
	return 0;
}
